cli_exec:  com2 run
	rm -rf Sudoku
    
run:
	./Sudoku
    
com2:
	gcc -o Sudoku Interfaz.c -Wall `pkg-config --cflags --libs gtk+-3.0`