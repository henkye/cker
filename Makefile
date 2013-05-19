PARTS=main.o download_url.o parse_arguments.o read_file.o parse_config.o search_and_get.o find_and_replace.o get_param.o find_nearest.o

.PHONY: build
.PHONY: install
.PHONY: uninstall
.PHONY: clean

build: cker

install: build
	cp cker /usr/bin
	
uninstall:
	rm -f /usr/bin/cker

clean:
	rm -f *.o


cker: ${PARTS}
	gcc -lcurl ${PARTS} -o cker

download_url.o: download_url.c
	gcc download_url.c -c

parse_arguments: parse_arguments.c
	gcc parse_arguments.c -c

read_file: read_file.c
	gcc read_file.c -c

parse_config: parse_config.c
	gcc parse_config.c -c

search_and_get: search_and_get.c
	gcc search_and_get.c -c

find_and_replace: find_and_replace.c
	gcc find_and_replace.c -c
	
get_param: get_param.c
	gcc get_param.c -c

find_nearest: find_nearest.c
	gcc find_nearest.c -c

main.o: main.c
	gcc main.c -c