cli_exec:
	gcc -o gladewin Interfaz.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf program