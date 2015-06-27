all: sgbd
	
sgbd: sintatic
	gcc *.c sql/semantic/semantic.c -o sgbd -g -lfl

clean:
	rm *.dat
	rm sgbd

sintatic:
	cd sql/sintatic && lex c.l && bison c.y