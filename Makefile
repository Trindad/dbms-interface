all: sgbd
	
sgbd: sintatic
	gcc *.c insert/semantic/semantic.c -o sgbd -g -lfl

clean:
	rm *.dat
	rm sgbd

sintatic:
	cd insert/sintatic && lex c.l && bison c.y