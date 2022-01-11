#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void cardName(int card){

    switch(card%13){
        case 0:  {printf("A"); break;}
        case 10: {printf("J"); break;}
        case 11: {printf("Q"); break;}
        case 12: {printf("K"); break;}
        default: {printf("%i",card%13+1); break;}
    }

    switch(card/13){
        case 0: {printf("S"); break;}
        case 1: {printf("C"); break;}
        case 2: {printf("D"); break;}
        case 3: {printf("H"); break;}
    }

    printf("\n");

}

void listints(char str[]){

    char num[10];
    char *sec;
    int i = 0;

    while ((sec = strsep(&str, ",")) != NULL) {
        printf("%s\n",sec);
        if (strchr(sec, '-') != NULL){
            printf("%s\n",sec);
        }
        else{
            if (strchr(num, atoi(sec)) == NULL){
                num[i] = *sec;
            }
        }
        i++;
    }

    for (int a = 0; a < 10; a++){
        printf("%i ",num[a]);
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
        listints(argv[1]);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
