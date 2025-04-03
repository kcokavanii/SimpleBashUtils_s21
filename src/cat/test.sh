#!/bin/bash

FLAGS1=(-T -E -e -t -s -A -n -b -v)
FLAGS2=("--number-nonblank" "--number" "--squeeze-blank" "--show-nonprinting")
FILES=("../../data-samples/cat/a.txt" "../../data-samples/cat/b.txt" "../../data-samples/cat/c.txt")
right_answers=0
number_of_test=0

for i in "${FLAGS1[@]}"; do
  ./s21_cat "$i" "${FILES[@]}" > s21_cat_test.txt
  cat "$i" "${FILES[@]}" > cat_test.txt
  number_of_test=$((number_of_test+1))
  if [[ $(diff -q cat_test.txt s21_cat_test.txt) ]]
  then
    echo "-----------------------------"
    echo "cat $i $FILES"
    echo "-----------------------------"
  else
    right_answers=$((right_answers+1))
    echo "$(diff -s s21_cat_test.txt cat_test.txt)"
    echo
  fi
done


for i in "${FLAGS2[@]}"; do
  ./s21_cat "$i" "${FILES[@]}" > s21_cat_test.txt
  cat "$i" "${FILES[@]}" > cat_test.txt
  number_of_test=$((number_of_test+1))
  if [[ $(diff -q cat_test.txt s21_cat_test.txt) ]]
  then
    echo "-----------------------------"
    echo "cat $i $FILES"
    echo "-----------------------------"
  else
    right_answers=$((right_answers+1))
    echo "$(diff -s s21_cat_test.txt cat_test.txt)"
    echo
  fi
done

echo
echo "-----------------------------"
echo "Correct answers:"
echo $right_answers / $number_of_test

rm -rf s21_cat_test.txt cat_test.txt
