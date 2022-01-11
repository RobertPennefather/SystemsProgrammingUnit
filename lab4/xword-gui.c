#include <stdlib.h>
#include <tcl.h>
#include <tk.h>

//  Written by Chris.McDonald@uwa.edu.au

#define	TCL_FILE	"./xword.tcl"

#define	GUI		TCLTK

#define	TCLTK_COMMAND(func)	\
	int func(ClientData data, Tcl_Interp *interp, int argc, char *argv[])

#define TCLTK_createcommand(str, func) \
	    Tcl_CreateCommand(tcl_interp, str, (Tcl_CmdProc *)func, \
			    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL)

static	Tcl_Interp	*tcl_interp	= NULL;

static void TCLTK(const char *fmt, ...)
{
    va_list	ap;
    char	command[BUFSIZ];

    if(tcl_interp) {
	va_start(ap,fmt);
	vsprintf(command,fmt,ap);
	va_end(ap);

	if(Tcl_Eval(tcl_interp, command) != TCL_OK) {
	    if(*tcl_interp->result)
		fprintf(stderr, "%s\n", tcl_interp->result);
	    exit(EXIT_FAILURE);
	}
    }
}

static void draw_grid(void)
{
    for(int row=0 ; row<SQUARE ; ++row) {
	for(int col=0 ; col<SQUARE ; ++col) {
	    switch (grid[row][col]) {
	    case WHITE:
			TCLTK("whitesquare %i %i", row,col);
			break;
	    case BLACK:
			TCLTK("blacksquare %i %i", row,col);
			break;
	    default:
			TCLTK("setnumber %i %i %i", row,col,grid[row][col]);
			break;
	    }
	}
    }
    while(Tcl_DoOneEvent(TCL_DONT_WAIT) != 0)
	;
}

static TCLTK_COMMAND(hidden_reset)
{
    reset();
    draw_grid();
    return TCL_OK;
}

static TCLTK_COMMAND(hidden_click)
{
    click( atoi(argv[1]), atoi(argv[2]) );
    draw_grid();
    return TCL_OK;
}

//  ------------------------------------------------------------------------

static int init_TCLTK(const char *file_tcltk)
{
#if	defined(_linux_)
    char	*ev	= getenv("DISPLAY");

    if(ev == NULL) {
	fprintf(stderr, "$DISPLAY not set\n");
	exit(EXIT_FAILURE);
    }
#endif

    tcl_interp      = Tcl_CreateInterp();
    if(tcl_interp == NULL) {
	fprintf(stderr, "cannot create Tcl/Tk interpreter\n");
	exit(EXIT_FAILURE);
    }

    if(Tcl_Init(tcl_interp) != TCL_OK || Tk_Init(tcl_interp) != TCL_OK) {
	if(*tcl_interp->result)
	    fprintf(stderr, "%s\n", tcl_interp->result);
	exit(EXIT_FAILURE);
    }

    if(file_tcltk) {
	if(Tcl_EvalFile(tcl_interp, file_tcltk) != TCL_OK) {
	    if(*tcl_interp->result)
		fprintf(stderr, "%s\n", tcl_interp->result);
	    exit(EXIT_FAILURE);
	}
    }
    return 0;
}

static int run_GUI(void)
{
    init_TCLTK(TCL_FILE);
    TCLTK_createcommand("hidden_click", hidden_click);
    TCLTK_createcommand("hidden_reset", hidden_reset);
    TCLTK("initgrid %d %d", SQUARE, SQUARE);

    draw_grid();
    Tk_MainLoop();
    return 0;
}
