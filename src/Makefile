all:
	rm base_64.o main.o
	gcc -c b64.cpp -o base_64.o
	gcc -c main.cpp -o main.o
	g++ base_64.o main.o -o extract_tool -lsqlite3
	rm base_64.o main.o