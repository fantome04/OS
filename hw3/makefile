run1: ex1
	./ex1

ex1: ex1.cpp
	g++ ex1.cpp -o ex1

run2: barman
	./barman

barman: barman.cpp
	g++ barman.cpp -o barman -pthread
 
clean:
	rm *.o ex1 barman
