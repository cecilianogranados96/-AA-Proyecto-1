cli_exec:
	gcc -o gladewin Intefaz.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -rf program