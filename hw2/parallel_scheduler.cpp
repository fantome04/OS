#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(std::size_t n)
    :capacity_(n), stop_(false)
{
    pthread_mutex_init(&queue_mutex, nullptr);
    pthread_cond_init(&task_cond, nullptr);

    threads_.resize(n);
    for (int i = 0; i < capacity_; ++i) {
        pthread_create(&threads_[i], nullptr, &parallel_scheduler::thread_func, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&queue_mutex);
    stop_ = true;
    pthread_mutex_unlock(&queue_mutex);

    pthread_cond_broadcast(&task_cond);

    for (pthread_t tid : threads_) {
        pthread_join(tid, nullptr);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&task_cond);
}

void parallel_scheduler::run(std::function<void()> func) {
    pthread_mutex_lock(&queue_mutex);
    tasks_.push(func);
    pthread_mutex_unlock(&queue_mutex);

    pthread_cond_signal(&task_cond);
}

void* parallel_scheduler::thread_func(void* arg) {
    parallel_scheduler* scheduler = static_cast<parallel_scheduler*>(arg);

    while(true){
        pthread_mutex_lock(&scheduler->queue_mutex);

        while (!scheduler->stop_ && scheduler->tasks_.empty()) {
            pthread_cond_wait(&scheduler->task_cond, &scheduler->queue_mutex);
        }

        if (scheduler->stop_ && scheduler->tasks_.empty()) {
            pthread_mutex_unlock(&scheduler->queue_mutex);
            return nullptr;
        }

        std::function<void()> task = scheduler->tasks_.front();
        scheduler->tasks_.pop();
        pthread_mutex_unlock(&scheduler->queue_mutex);
        task();
    }

    return nullptr;
}