#include <stdlib.h>
#include <stdio.h>
#include "convert_speed.h"


int main(){
    int input;
    printf("enter speed in km/h");
    scanf("%d", &input);
    int output = runlogic(input);
    printf("return value : %d", output);
    return 0;
}



