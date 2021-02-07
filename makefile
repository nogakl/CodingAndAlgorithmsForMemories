all: findScs


findScs: findScs.cpp
	gcc -o findScs findScs.cpp -I.


.PHONY: clean

clean: 
	rm -f *.o findScs
