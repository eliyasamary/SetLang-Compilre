parse.exe: lex.yy.c run.c Collection.tab.c
	gcc lex.yy.c  Collection.tab.c run.c -o parse.exe

lex.yy.c: Collection.tab.c collection.l
	flex Collection.l

Collection.tab.c: Collection.y
	bison -d Collection.y

clean:
	rm -f lex.yy.c Collection.tab.c Collection.tab.h parse.exe prog.exe

test.exe: test.c
	gcc prog.c -o prog.exe