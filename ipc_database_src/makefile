all:
	gcc -c -g `mysql_config --cflags` database_dblinker.c
	gcc -o database_dblinker database_dblinker.o `mysql_config --libs`

clean:
	rm *.o database_dblinker
