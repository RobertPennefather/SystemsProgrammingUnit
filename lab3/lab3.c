#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

//#define MAXSIZE 15;

int sorted[15];

int my_strlen(char string[]){

    int length = 0;

    while (string[length] != '\0'){
        ++length;
    }

    return length;
}

int numberOfVowels(char string[]){

    int length = 0;
    int vowels = 0;

    while (string[length] != '\0'){

        if (string[length] == 'a' ||
            string[length] == 'e' ||
            string[length] == 'i' ||
            string[length] == 'o' ||
            string[length] == 'u' ||
            string[length] == 'A' ||
            string[length] == 'E' ||
            string[length] == 'I' ||
            string[length] == 'O' ||
            string[length] == 'U'){

            vowels++;
        }

        ++length;
    }

    return vowels;
}

bool isSafe(char str[]){

    int length = 0;
    int upper = 0;
    int lower = 0;
    int digits = 0;

    while (str[length] != '\0'){

        if (!isalpha(str[length])){
            digits++;
        }
        else if (isupper(str[length])){
            upper++;
        }
        else if (islower(str[length])){
            lower++;
        }

        ++length;
    }

    if (upper > 1 && lower > 1 && digits > 1){
        return true;
    }

    return false;
}

int my_strcmp(char str1[], char str2[]){

    int length = 0;

    while (str1[length] != '\0' && str2[length] != '\0'){

        if (str1[length] < str2[length]){
            return -1;
        }
        if (str1[length] > str2[length]){
            return 1;
        }

        ++length;
    }

    if (str1[length] < str2[length]){
        return -1;
    }
    if (str1[length] > str2[length]){
        return 1;
    }

    return 0;
}

bool isPalindrome(char str[]){

    int length = my_strlen(str);

    for (int i = 0; i < length/2; i++){
        if (str[i] != str[length-i-1]){
            return false;
        }
    }

    return true;
}

int minimum(int num[], int size){

    int min = num[0];

    for (int a = 1; a < size; a++){
        if(min > num[a]){
            min = num[a];
        }
    }

    return min;
}

int maximum(int num[], int size){

    int max = num[0];

    for (int a = 1; a < size; a++){
        if(max < num[a]){
            max = num[a];
        }
    }

    return max;
}

double average(int num[], int size){

    double sum = num[0];

    for (int a = 1; a < size; a++){
        sum += num[a];
    }

    return sum/size;
}

int mode(int num[], int size){

    int count[size];

    for (int a = 0; a < size; a++){
        for (int b = 0; b < a; b++){
            if (num[a]==num[b]){
                count[b]++;
            }
        }
    }

    int max = 0;

    for (int a = 1; a < size; a++){
        if(max < count[a]){
            max = a;
        }
    }

    return num[max];
}

void sort(int num[], int size){

    for (int a = 0; a < size; a++){

        sorted[a] = minimum(num, size);

        for (int b = 0; b < size; b++){
            if (sorted[a]==num[b]){
                num[b] = maximum(num, size);
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s argument\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {

        int values[argc-1];

        for (int a = 1; a < argc; a++){
            values[a-1] = atoi(argv[a]);
        }

        sort(values,argc-1);
        for (int a = 1; a < 15; a++){
            if (sorted[a]==0){break;}
            printf("%i ",sorted[a]);
        }
        printf("\n");

        int min = minimum(values,argc-1);
        printf("Min: %d\n", min);

        int max = maximum(values,argc-1);
        printf("Max: %d\n", max);

        double avg = average(values,argc-1);
        printf("Avg: %f\n", avg);

        int mod = mode(values,argc-1);
        printf("Mode: %d\n", mod);

        exit(EXIT_SUCCESS);
    }
    return 0;
}
