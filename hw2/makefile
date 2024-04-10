CXX = g++
LIB_DIR = $(PWD)

all: parallel_scheduler.so demo-application

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) -c -fPIC parallel_scheduler.cpp -o parallel_scheduler.o

parallel_scheduler.so: parallel_scheduler.o
	$(CXX) -shared -o libparallel_scheduler.so parallel_scheduler.o

demo-application: demo-application.cpp parallel_scheduler.h parallel_scheduler.so
	$(CXX) demo-application.cpp -o demo-application -L$(LIB_DIR) -lparallel_scheduler -Wl,-rpath,$(LIB_DIR)

clean:
	rm -f *.o *.so demo-application

