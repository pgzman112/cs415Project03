p3.out :Driver.o Trie.o TernarySearchTree.o
			 g++ -g -o P3.out Driver.o Trie.o TernarySearchTree.o

Driver.o : Driver.cpp Trie.h TernarySearchTree.h
			 g++ -g -c Driver.cpp

Trie.o : Trie.cpp Trie.h
			 g++ -g -c Trie.cpp

TernarySearchTree.o : TernarySearchTree.cpp TernarySearchTree.h
			 g++ -g -c TernarySearchTree.cpp

clean :
			rm *.o P3.out *.gch
