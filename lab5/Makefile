PROJECT =  calcmarks
HEADERS =  $(PROJECT).h
OBJ     =  calcmarks.o globals.o readmarks.o correlation.o


C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror 


$(PROJECT) : $(OBJ)
       $(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm


%.o : %.c $(HEADERS)
        $(C99) $(CFLAGS) -c $<

clean:
       rm -f $(PROJECT) $(OBJ)