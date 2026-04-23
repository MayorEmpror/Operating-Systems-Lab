#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <queue>
#include <atomic>
#include <algorithm>

using namespace std;

#define N 1000
#define TILE 100
#define NUM_SAT 4
#define THREADS 8

// ---------------- GLOBAL DATA ----------------
vector<vector<double>> globalMatrix(N, vector<double>(N, 0));
vector<vector<int>> contribution(N, vector<int>(N, 0));

double globalMin = 1e9, glkobalMax = -1e9;
double globalSum = 0;
long long globalCount = 0;

pthread_mutex_t matrixLock;
pthread_mutex_t statsLock;

// ---------------- TILE TASK QUEUE ----------------
struct Tile {
    int r, c;
};

queue<Tile> tileQueue;
pthread_mutex_t queueLock;
pthread_cond_t queueCond;
bool doneAddingTiles = false;

// ---------------- SATELLITE DATA ----------------
vector<vector<vector<double>>> satellites(NUM_SAT, vector<vector<double>>(N, vector<double>(N)));

// ---------------- UTILITY ----------------
double safeValue(double v) {
    return isnan(v) ? 0.0 : v;
}

// Simple NaN interpolation (average neighbors)
double interpolate(int i, int j, vector<vector<double>>& mat) {
    double sum = 0;
    int cnt = 0;

    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (!di && !dj) continue;
            int ni = i + di, nj = j + dj;
            if (ni >= 0 && nj >= 0 && ni < N && nj < N) {
                double v = mat[ni][nj];
                if (!isnan(v)) {
                    sum += v;
                    cnt++;
                }
            }
        }
    }
    return cnt ? sum / cnt : 0;
}

// ---------------- SATELLITE THREAD ----------------
void* satelliteWorker(void* arg) {
    int id = *(int*)arg;

    auto& mat = satellites[id];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (isnan(mat[i][j])) {
                mat[i][j] = interpolate(i, j, mat);
            }
        }
    }

    // Merge into global matrix
    pthread_mutex_lock(&matrixLock);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            globalMatrix[i][j] += mat[i][j];
            contribution[i][j]++;
        }
    }

    pthread_mutex_unlock(&matrixLock);

    return nullptr;
}

// ---------------- TILE WORKER ----------------
struct TileResult {
    double localMin, localMax, localSum;
    double mean, variance, stddev;
    int anomalies;
};

void* tileWorker(void* arg) {
    while (true) {
        Tile tile;

        pthread_mutex_lock(&queueLock);

        while (tileQueue.empty() && !doneAddingTiles) {
            pthread_cond_wait(&queueCond, &queueLock);
        }

        if (tileQueue.empty() && doneAddingTiles) {
            pthread_mutex_unlock(&queueLock);
            break;
        }

        tile = tileQueue.front();
        tileQueue.pop();
        pthread_mutex_unlock(&queueLock);

        int rEnd = min(tile.r + TILE, N);
        int cEnd = min(tile.c + TILE, N);

        double localMin = 1e9, localMax = -1e9, sum = 0;
        int count = 0;

        vector<double> values;

        for (int i = tile.r; i < rEnd; i++) {
            for (int j = tile.c; j < cEnd; j++) {
                double v = globalMatrix[i][j] / max(1, contribution[i][j]);

                localMin = min(localMin, v);
                localMax = max(localMax, v);
                sum += v;
                values.push_back(v);
                count++;
            }
        }

        double mean = sum / count;
        double var = 0;

        int anomalies = 0;
        for (double v : values) {
            var += (v - mean) * (v - mean);
            if (fabs(v - mean) > 2 * sqrt(var / count)) {
                anomalies++;
            }
        }

        pthread_mutex_lock(&statsLock);
        globalMin = min(globalMin, localMin);
        globalMax = max(globalMax, localMax);
        globalSum += sum;
        globalCount += count;
        pthread_mutex_unlock(&statsLock);
    }

    return nullptr;
}

// ---------------- HOTSPOT / COLDSPOT ----------------
vector<pair<int,int>> hotspots, coldspots;

double T_hot = 35.0;
double T_cold = -10.0;

void detectHotCold() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double v = globalMatrix[i][j] / max(1, contribution[i][j]);

            if (v > T_hot) hotspots.push_back({i, j});
            if (v < T_cold) coldspots.push_back({i, j});
        }
    }
}

// ---------------- NORMALIZATION ----------------
vector<vector<double>> normalized(N, vector<double>(N));

void normalizeMatrix() {
    double minV = globalMin;
    double maxV = globalMax;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double v = globalMatrix[i][j] / max(1, contribution[i][j]);
            normalized[i][j] = (v - minV) / (maxV - minV + 1e-9);
        }
    }
}

// ---------------- RISK SCORE ----------------
struct RiskCell {
    int r, c;
    double score;
};

vector<RiskCell> riskCells;

double proximity(int i, int j, vector<pair<int,int>>& points) {
    double best = 1e9;
    for (auto& p : points) {
        int d = abs(p.first - i) + abs(p.second - j);
        best = min(best, (double)d);
    }
    return best;
}

void computeRisk() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double hotP = proximity(i, j, hotspots);
            double coldP = proximity(i, j, coldspots);

            double score = normalized[i][j] * hotP / (coldP + 1);

            riskCells.push_back({i, j, score});
        }
    }

    sort(riskCells.begin(), riskCells.end(),
        [](RiskCell a, RiskCell b) {
            return a.score > b.score;
        });
}

// ---------------- MAIN ----------------
int main() {
    pthread_mutex_init(&matrixLock, NULL);
    pthread_mutex_init(&statsLock, NULL);
    pthread_mutex_init(&queueLock, NULL);
    pthread_cond_init(&queueCond, NULL);

    // --------- SATELLITE THREADS ----------
    pthread_t satThreads[NUM_SAT];
    int ids[NUM_SAT];

    for (int i = 0; i < NUM_SAT; i++) {
        ids[i] = i;
        pthread_create(&satThreads[i], NULL, satelliteWorker, &ids[i]);
    }

    for (int i = 0; i < NUM_SAT; i++) {
        pthread_join(satThreads[i], NULL);
    }

    // --------- CREATE TILE TASKS ----------
    for (int i = 0; i < N; i += TILE) {
        for (int j = 0; j < N; j += TILE) {
            tileQueue.push({i, j});
        }
    }

    // --------- TILE WORKERS ----------
    pthread_t workers[THREADS];

    pthread_mutex_lock(&queueLock);
    doneAddingTiles = true;
    pthread_cond_broadcast(&queueCond);
    pthread_mutex_unlock(&queueLock);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&workers[i], NULL, tileWorker, NULL);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    // --------- FINAL PIPELINE ----------
    detectHotCold();
    normalizeMatrix();
    computeRisk();

    // --------- OUTPUT ----------
    cout << "Global Min: " << globalMin << endl;
    cout << "Global Max: " << globalMax << endl;
    cout << "Global Mean: " << globalSum / globalCount << endl;

    cout << "Hotspots: " << hotspots.size() << endl;
    cout << "Coldspots: " << coldspots.size() << endl;

    cout << "\nTop 10 Risk Cells:\n";
    for (int i = 0; i < min(10, (int)riskCells.size()); i++) {
        cout << "(" << riskCells[i].r << "," << riskCells[i].c
             << ") -> " << riskCells[i].score << endl;
    }

    return 0;
}