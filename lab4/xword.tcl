#
#  Written by Chris.McDonald@uwa.edu.au
#
set	grid	0
set	nrows	0
set	ncols	0
set	sqsize	40
set	margin	10
#
proc whitesquare {r c} {
    global	grid nrows ncols

    if {$r >= 0 && $r < $nrows && $c >= 0 && $c < $ncols} {
	$grid delete num($r,$c)
	$grid itemconfigure sq($r,$c) -fill white
    }
}
#
proc blacksquare {r c} {
    global	grid nrows ncols

    if {$r >= 0 && $r < $nrows && $c >= 0 && $c < $ncols} {
	$grid delete num($r,$c)
	$grid itemconfigure sq($r,$c) -fill black
    }
}
#
proc setnumber {r c value} {
    global	grid nrows ncols sqsize margin

    if {$r >= 0 && $r < $nrows && $c >= 0 && $c < $ncols} {
	$grid delete num($r,$c)
	$grid itemconfigure sq($r,$c) -fill white
	$grid create text \
		[expr $margin + $c * $sqsize + 3] \
		[expr $margin + $r * $sqsize + 3] \
		-text "$value" \
		-fill black \
		-anchor nw \
		-tags num($r,$c)
    }
}
#
proc initgrid {r c} {
    global	grid nrows ncols sqsize margin

    set		nrows	$r
    set		ncols	$c

    set w .
    wm withdraw $w
    wm title $w "xword - $nrows x $ncols"

    set bgcolor	"gold"
    set f1 [frame $w.f1 -background $bgcolor]
    button $f1.quit \
		-highlightbackground $bgcolor \
		-text "quit" \
		-command {exit 0}
    button $f1.reset \
		-highlightbackground $bgcolor \
		-text "reset" \
		-command "hidden_reset"

    pack $f1.quit $f1.reset -side left -padx 4
    pack $f1 -fill x -expand no

    set f2 [frame $w.f2]
    set grid [canvas $f2.can \
		    -highlightthickness 0 \
		    -width  [expr 2 * $margin + $sqsize * $ncols] \
		    -height [expr 2 * $margin + $sqsize * $nrows]  \
		    -background #ccc ]
    pack $f2.can -fill both -expand yes
    pack $f2 -fill both -expand yes
    pack $f1 $f2

    for {set r 0} {$r < $nrows} {incr r} {
	set y [expr $r * $sqsize + $margin]
	for {set c 0} {$c < $ncols} {incr c} {
	    set x [expr $c * $sqsize + $margin]
	    set tmp [$grid create rect \
		    $x $y [expr $x + $sqsize] [expr $y + $sqsize] \
		    -fill white \
		    -outline #aaa \
		    -tags sq($r,$c) ]
	    $grid bind $tmp <Button> "hidden_click $r $c"
	}
    }
    wm deiconify $w
}
