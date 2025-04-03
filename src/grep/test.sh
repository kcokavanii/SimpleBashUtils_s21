#!/bin/bash

FLAGS=(-i -v -c -l -n -h -s)
FILES="../../data-samples/grep/a.txt ../../data-samples/grep/b.txt ../../data-samples/grep/c.txt"
number_of_test=0
right_answers=0

compare_with_e(){
    number_of_test=$((number_of_test+1))
    ./s21_grep -e s$ -e ^S $flag $FILES > s21_grep_test.txt
    grep $flag -e s$ -e ^S $FILES > grep_test.txt
    echo "Test with number "$number_of_test" flag $flag"
    if [[ $(diff -q grep_test.txt s21_grep_test.txt) ]]
    then
        echo "-----------------------------"
        echo "grep -e s$ -e ^S $flag $FILES"
        echo "-----------------------------"
    else
        right_answers=$((right_answers+1))
        echo "$(diff -s grep_test.txt s21_grep_test.txt)"
        echo
    fi
}

compare(){
    number_of_test=$((number_of_test+1))
    ./s21_grep $flag ss $FILES > s21_grep_test.txt
    grep $flag ss $FILES > grep_test.txt
    echo "Test with number "$number_of_test" flag $flag"
    if [[ $(diff -q grep_test.txt s21_grep_test.txt) ]]
    then
        echo "-----------------------------"
        echo "grep $flag ss $FILES"
        echo "-----------------------------"
    else
        right_answers=$((right_answers+1))
        echo "$(diff -s grep_test.txt s21_grep_test.txt)"
        echo
    fi
}

for flag1 in ${FLAGS[@]}
do
    for flag2 in ${FLAGS[@]}
    do  
        for flag3 in ${FLAGS[@]}
        do
            if [ $flag1 != $flag2 ] && [ $flag1 != $flag3 ] && [ $flag2 != $flag3 ]
            then
                flag="$flag1 $flag2 $flag3 "
                compare_with_e
            fi
        done
    done
done


for flag1 in ${FLAGS[@]}
do
    for flag2 in ${FLAGS[@]}
    do  
        for flag3 in ${FLAGS[@]}
        do
            if [ $flag1 != $flag2 ] && [ $flag1 != $flag3 ] && [ $flag2 != $flag3 ]
            then
                flag="$flag1 $flag2 $flag3 "
                compare
            fi
        done
    done
done

./s21_grep k $FILES > s21_grep_test.txt
grep k $FILES > grep_test.txt
number_of_test=$((number_of_test+1))
echo "Extra test"
if [[ $(diff -q grep_test.txt s21_grep_test.txt) ]]
then
    echo "-----------------------------"
    echo "Failed extra test"
    echo "-----------------------------"
else
    right_answers=$((right_answers+1))
    echo "$(diff -s grep_test.txt s21_grep_test.txt)"
    echo
fi

echo
echo "-----------------------------"
echo "Correct answers:"
echo $right_answers / $number_of_test

rm -rf grep_test.txt s21_grep_test.txt
