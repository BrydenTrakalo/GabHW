P2: main.o parser.o scanner.o testScanner.o testTree.o
	g++ main.o parser.o scanner.o testScanner.o testTree.o -o P2

main.o: main.cpp scanner.hpp testScanner.hpp token.hpp
	g++ main.cpp -c

parser.cpp: parser.hpp node.hpp token.hpp testTree.hpp testScanner.hpp
	g++ parser.cpp -c

scanner.o: scanner.cpp scanner.hpp token.hpp
	g++ scanner.cpp -c

testScanner.o: testScanner.cpp testScanner.hpp token.hpp
	g++ testScanner.cpp -c

testTree.o: testTree.cpp testTree.hpp node.hpp
	g++ testTree.cpp -c
