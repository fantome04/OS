run: init client destroy
	./init
	./client
	./destroy

init: bank.h init.cpp bank.o bank_cell.o
	g++ init.cpp bank.o bank_cell.o -g -o init -lrt

client: bank.h client.cpp bank.o bank_cell.o
	g++ client.cpp bank.o bank_cell.o -g -o client -lrt

destroy: bank.h destroy.cpp bank.o bank_cell.o
	g++ destroy.cpp bank.o bank_cell.o -g -o destroy -lrt

bank_cell.o: bank_cell.cpp bank.h
	g++ bank_cell.cpp -c -g

bank.o: bank_cell.h bank.h bank.cpp
	g++ bank.cpp -c -g

clean:
	rm -f init client destroy *.o init_d client_d destroy_d
