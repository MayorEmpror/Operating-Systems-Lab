#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char* argv[]){
        
        cout<<argv<<endl;
        cout<< atoi(argv[1]) << atoi(argv[2])<< endl; 
	cout<<*argv<<endl;
        return 0;
}
