all: 
	g++ -std=c++11 main.cpp Parser.cpp CSE.cpp Tokens.cpp NaryTree.cpp TreeNode.cpp ELEMENT.cpp -o p1
cl:
	rm -f *.o p1
test:
	perl ./difftest.pl -1 "./rpal FILE" -2 "./p1 -r FILE" -t ./tests/


