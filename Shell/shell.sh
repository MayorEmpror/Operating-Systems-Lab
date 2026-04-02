#!/bin/bash 








num1=1 
num2=2
num3=3


if [ $num1 -ge $num2 ]; then 
    if [ $num1 -ge $num2]; then 
            echo "num1 is greatest"
    elif [ $num2 -ge $num3 ]; then 
            echo "num2 is greatest"
    fi
elif [ $num3 -ge  $num2 ]; then 
            echo "num3 is greatest"
fi 

