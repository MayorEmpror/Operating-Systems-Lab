#!/bin/bash
echo "Enter sutdent name"
read  name

echo "Enter marks for subject 1"
read m1

echo "Enter marks for subject 2"
read m2

echo "Enter marks for subject 3"
read m3

$grade
total=$((m1 + m2 + m3))
avg=$((total / 3))

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

echo "-------------------------"
echo "Student Name : $name"
echo "Subject_1 Marks  $m1"
echo "Subject_2 Marks $m2"
echo "Subject_3 Marks $m3"
echo "--------------------------"
echo "Student Name : $name"
echo "Total Marks  : $total"
echo "Average      : $avg"
echo "Grade        : $grade"
echo "--------------------------"