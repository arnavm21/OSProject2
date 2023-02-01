all: mutexSports

mutexSports: mutexSports.c
	gcc -o mutexSports mutexSports.c -lpthread

omnisportspark: omnisportspark.c
	gcc -o omnisportspark omnisportspark.c -lpthread

clean: 
	rm -f mutexSports
	rm -f omnisportspark
