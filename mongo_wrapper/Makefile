all:
	gcc -o json_msg.o -c json_msg.c -g -std=c99 `pkg-config --cflags json-glib-1.0`
	gcc -o db_controller.o -c db_controller.c -g `pkg-config --cflags json-glib-1.0 libmongoc-1.0`
	gcc -o db_controller *.o `pkg-config --libs libmongoc-1.0 json-glib-1.0`

clean:
	rm *.o
