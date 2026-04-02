#!/bin/bash

if [ $# -eq 0 ]; then 
       echo "No args provided"
else
       echo "All arguments : $@"
       echo "First Argument $1"
    if [ $# -ge 2 ]; then 
       echo "Second Argument $2"
    else 
       echo "second argument not provided"
   fi
fi
    
echo "User : $USER "
echo "Path_curr_Loc : $HOME "
echo "Present Working Directory : $PWD"
