#!/bin/bash

# Taking student name
echo "Enter Student Name:"
read name

# Taking marks
echo "Enter marks for Subject 1:"
read m1

echo "Enter marks for Subject 2:"
read m2

echo "Enter marks for Subject 3:"
read m3

# Calculating total and average
total=$((m1 + m2 + m3))
avg=$((total / 3))

# Determining grade
if [ $avg -ge 80 ]; then
    grade="A"
elif [ $avg -ge 70 ]; then
    grade="B"
elif [ $avg -ge 60 ]; then
    grade="C"
elif [ $avg -ge 50 ]; then
    grade="D"
else
    grade="F"
fi

# Displaying result
echo "-----------------------------"
echo "Student Name : $name"
echo "Total Marks  : $total"
echo "Average      : $avg"
echo "Grade        : $grade"
echo "-----------------------------"