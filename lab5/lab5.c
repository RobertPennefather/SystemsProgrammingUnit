#include <stdio.h>
#include <string.h>

#include <tcl.h>
#include <tk.h>

//  Compile on OS-X on CSSE labs with:
//              mycc -I/opt/X11/include -o popup popup.c -ltcl -ltk

//  TCL/TK CODE TO POPUP A BASIC WINDOW - NO NEED TO UNDERSTAND THIS CODE
void popup(int x, int y, int w, int h)
{
    static Tcl_Interp *tcl_interp = NULL;

    if(tcl_interp == NULL) {
        tcl_interp      = Tcl_CreateInterp();
        if(tcl_interp == NULL)
            return;
        Tcl_Init(tcl_interp);
        Tk_Init(tcl_interp);
    }

    char    command[1024], *c = command;

    sprintf(c, "wm geometry . +%i+%i\n", x, y);
    while(*c) ++c;
    sprintf(c, "frame .f -width %i -height %i -bg gold\n", w, h);
    while(*c) ++c;
    sprintf(c, "pack .f -fill both -expand yes\n");
    Tcl_Eval(tcl_interp, command);
}

//  ----------------------------------------------------------------------

int main(int argc, char *argv[])
{
//  ADD YOUR COMMAND-LINE PROCESSING HERE
//  .....



    popup(100, 200, 300, 400);

    Tk_MainLoop();
    return 0;
}












/*#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int* maximum(int *values, int n){

    int* max = values;

    while (isdigit(*values)){
        if (*max < *values){
            max = values;
        }
        values++;
    }

    return max;
}

void sortPointers(int src[], int* ptrs[], int n){

    for (int i=0; i < n; i++){

        int* min = &src[0];

        for (int j=0; j < n; j++){

            for (int k=0; k < n; k++){

                if (src[j] < *min){
                    min = &src[j];
                }
            }  
        }

        ptrs[i] = min;
    }
}

int main(int argc, char *argv[])
{

    char* intarray = calloc(10, sizeof(char));

    if(intarray == NULL){
        fprintf(stderr, "ERROROROROOROROR!\n");
        exit(EXIT_FAILURE);
    }

    //printf("%i\n",strcmp(intarray[0],'\0');

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s argument\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        //printf("%i\n",maximum(,atoi(argv[2])));
        exit(EXIT_SUCCESS);
    }
    return 0;
}*/
