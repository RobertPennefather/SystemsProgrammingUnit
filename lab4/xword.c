#include <stdio.h>

//  WE WILL ASSUME A SQUARE GRID (#rows == #cols)
#define	SQUARE		9

int			grid[SQUARE][SQUARE];

//  EACH ELEMENT OF grid IS BLACK, WHITE (EMPTY), OR A +ve CLUE NUMBER
#define	WHITE		0
#define	BLACK		(-1)

// --------------------------------------------------------------------

void reset(void)
{
    printf("reset()\n");
    for(int row=0 ; row<SQUARE ; ++row) {
	for(int col=0 ; col<SQUARE ; ++col) {
	    grid[row][col] = WHITE;
	}
    }
//  YOU MAY ALSO NEED TO INITIALIZE/CLEAR YOUR OWN DATA STRUCTURES
}

static void click(int row, int col)
{
    printf("click(row=%i, col=%i)\n", row, col);

    grid[row][col] = (grid[row][col] != BLACK) ? BLACK : WHITE;

//  TOGGLE BETWEEN BLACK AND WHITE, RE-NUMBER THE CLUES
}

//  We should NEVER #include C source code (but it simplies our GUI coding!)
#include "./xword-gui.c"

int main(void)
{
    reset();
    return run_GUI();
}
