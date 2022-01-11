#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Compile this program with:
//      cc -std=c99 -Wall -Werror -pedantic -o rot rot.c

int ROT = 13;

//  The rotate function returns the character ROT positions further along the
//  alphabetic character sequence from c, or c if c is not lower-case

char rotate(char c)
{
        // Check if c is lower-case or not
        if (islower(c))
        {
                // The ciphered character is ROT positions beyond c,
                // allowing for wrap-around
                return ('a' + (c - 'a' + ROT) % 26);
        }
        else if (isupper(c))
        {
                // The ciphered character is ROT positions beyond c,
                // allowing for wrap-around
                return ('A' + (c - 'A' + ROT) % 26);
        }
        else
        {
                return c;
        }
}

//  Execution of the whole program begins at the main function

int main(int argc, char *argv[])
{
        // Exit with an error if the number of arguments (including
        // the name of the executable) is not precisely 2
        if(argc == 1)
        { 
                fprintf(stderr, "%s: program expected 1 argument, but instead received %d\n", argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
                for (int j = 1; j < argc; j++)
                {

                        if(isalpha(argv[j][1])){

                                FILE * fp;
                                fp = fopen (argv[j],"r");
                                if (fp==NULL)
                                {
                                        int i = 1;
                                        char str;
                                        while (fgets (&str, 1, fp) != NULL)
                                        {
                                                printf("%i: %c -> %c\n", i+1, str, rotate(str));
                                                i++;
                                        }
                                        fclose (fp);
                                }

                                else
                                {
                                        int length = strlen(argv[j]);
                                        for (int i = 0; i < length; i++)
                                        {
                                                printf("%i: %c -> %c\n", i+1, argv[j][i], rotate(argv[j][i]));
                                        }
                                }
                                printf("\n");
                        }

                        else{

                                ROT = atoi(argv[j]);

                        }
                }

                exit(EXIT_SUCCESS);
        }
        return 0;
}
