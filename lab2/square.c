#include <stdio.h>
#include <stdlib.h>

//  Compile with:  cc -std=c99 -Wall -Werror -pedantic -o square square.c

void square(int number)
{
    printf("The square of %i is %i\n", number, number*number);
}

int max(int max, int next)
{
    if(next <= max){return max;}
    else {return next;}
}

void leapYear(int year)
{
    if((year%400 == 0)||((year%4 == 0)&&(year%100 != 0))){
        printf("%i is a leap year\n", year);
    }
    else {
        printf("%i is not a leap year\n", year);
    }
}

void asterisks()
{
    for(int a = 1 ; a < 6 ; a ++)
    {
        for(int b = 0; b < 6-a ; b++){
            printf(" ");
        }
        for(int b = 0; b < 2*a-1; b++){
            printf("*");
        }
        printf("\n");
    }
}

void placing(int place)
{
    switch(place%10){
        case 1:  {
            if(place%100 == 11) {printf("%ith\n",place);}
            else                {printf("%ist\n",place);}
            break;
        }
        case 2:  {
            if(place%100 == 12) {printf("%ith\n",place);}
            else                {printf("%ind\n",place);}
            break;
        }
        case 3:  {
            if(place%100 == 13) {printf("%ith\n",place);}
            else                {printf("%ird\n",place);}
            break;
        }
        default: {printf("%ith\n",place);}

    }
}

void romanNumeral(int number)
{
    while(&free){

        for(int i = 0; i < number/1000; i++){
                printf("M");
        }

        number = number%1000;
        for(int i = 0; i < number/500; i++){
                printf("D");
        }

        number = number%500;
        for(int i = 0; i < number/100; i++){
                printf("C");
        }

        number = number%100;
        for(int i = 0; i < number/50; i++){
                printf("L");
        }

        number = number%50;
        if (number == 9){printf("IX\n"); break;}
        else{
            for(int i = 0; i < number/10; i++){
                printf("X");
            }
        }

        number = number%10;
        if (number == 4){printf("IV\n"); break;}
        else{
            for(int i = 0; i < number/5; i++){
                printf("V");
            }
        }
        
        number = number%5;
        for(int i = 0; i < number; i++){
                printf("I");
        }

        printf("\n");
        break;

    }
}

int main(int argc, char *argv[])
{
    // Check the number of command-line arguments
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s value1 [value2 ...]\n", argv[0]);
        asterisks();
        exit(EXIT_FAILURE);             // Exit indicating failure
    }
    else
    {
        //int currentMax = 0;

        // Loop over each argument passed to the program
        for(int a = 1 ; a <= atoi(argv[1]) ; a++)
        {
            romanNumeral(a);
            //int n = atoi(argv[a]);
            //placing(n);
            //leapYear(n);
            //currentMax = max(currentMax,n);
        }

        //printf("Maximum: %i\n",currentMax);
        exit(EXIT_SUCCESS);             // Exit indicating success
    }
    return 0;
}
