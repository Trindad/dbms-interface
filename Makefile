all: sgbd
	
sgbd: sintatic
	gcc *.c sql/semantic/semantic.c -o sgbd -g -lfl -L/usr/local/lib -I/usr/local/include -lreadline

clean:
	rm *.dat
	rm sgbd

sintatic:
	cd sql/sintatic && lex c.l && bison c.y