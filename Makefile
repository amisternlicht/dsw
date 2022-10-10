DSW: main1.cpp
	g++ main1.cpp AVL_tree.h dsw.h library.h Node.h TreeTypes.h library.cpp -Wall -g -o dsw 

.PHONY: clean
clean:
	rm -f *.o
	rm -f dsw