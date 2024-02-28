#include "parallel_scheduler.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

void example_function(int id) {
    std::cout << "Function " << id << " is executed by thread " << pthread_self() << std::endl;
    sleep(5);
}

int main() {
    const size_t cap = 3;
    parallel_scheduler scheduler(cap);

    for (int i = 0; i < 10; ++i) {
        scheduler.run([i] { example_function(i); });
    }

    sleep(5);

    return 0;
}