#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <functional>
#include <pthread.h>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

class parallel_scheduler{
private:
    std::size_t capacity_;
    std::vector<pthread_t> threads_;
    std::queue<std::function<void()>> tasks_;
    pthread_mutex_t queue_mutex;
    pthread_cond_t task_cond;
    bool stop_;

    static void* thread_func(void* arg);

public:
    parallel_scheduler(std::size_t n);
    ~parallel_scheduler();
    void run(std::function<void()> func);
};


#endif