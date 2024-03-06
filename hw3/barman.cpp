/*#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>

const int SIZE = 8;

sem_t tobacco_sem;
sem_t paper_sem;
sem_t match_sem;
sem_t barman_sem;
int count;

void* smoker_t(void* arg) {
    while(true){
        sem_wait(&tobacco_sem);
        std::cout << "T" << std::endl;
        sem_post(&barman_sem);
    }
}

void* smoker_p(void* arg) {
    while(true){
        sem_wait(&paper_sem);
        std::cout << "P" << std::endl;
        sem_post(&barman_sem);
    }
}

void* smoker_m(void* arg) {
    while(true){
        sem_wait(&match_sem);
        std::cout << "M" << std::endl;
        sem_post(&barman_sem);
    }
}

void* barman(void* arg) {
    while (true) {
        sem_wait(&barman_sem);

        if(count == SIZE) {
            std::cout << "Closing!!!" << std::endl;
            kill(getpid(), SIGTERM);
            return nullptr;
        }

        char item;
        std::cin >> item;
        ++count;

        switch (item) {
            case 't':
                sem_post(&tobacco_sem);
                break;

            case 'p':
                sem_post(&paper_sem);
                break;

            case 'm':
                sem_post(&match_sem);
                break;

            default:
                std::cout << "Closing!!!" << std::endl;
                kill(getpid(), SIGTERM);
                return nullptr;
        }
    }
}

int main() {
    sem_init(&tobacco_sem, 0, 0);
    sem_init(&paper_sem, 0, 0);
    sem_init(&match_sem, 0, 0);
    sem_init(&barman_sem, 0, 1);

    pthread_t smoker_t_thread, smoker_p_thread, smoker_m_thread, barman_thread;
    pthread_create(&smoker_t_thread, nullptr, smoker_t, nullptr);
    pthread_create(&smoker_p_thread, nullptr, smoker_p, nullptr);
    pthread_create(&smoker_m_thread, nullptr, smoker_m, nullptr);
    pthread_create(&barman_thread, nullptr, barman, nullptr);

    pthread_join(smoker_t_thread, nullptr);
    pthread_join(smoker_p_thread, nullptr);
    pthread_join(smoker_m_thread, nullptr);
    pthread_join(barman_thread, nullptr);

    sem_destroy(&tobacco_sem);
    sem_destroy(&paper_sem);
    sem_destroy(&match_sem);
    sem_destroy(&barman_sem);

    return 0;
}*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

const int SIZE = 8;

sem_t* tobacco_sem;
sem_t* paper_sem;
sem_t* match_sem;
sem_t* barman_sem;
int count;

void smoker_t() {
    while(true){
        sem_wait(tobacco_sem);
        std::cout << "T" << std::endl;
        sem_post(barman_sem);
    }
}

void smoker_p() {
    while(true){
        sem_wait(paper_sem);
        std::cout << "P" << std::endl;
        sem_post(barman_sem);
    }
}

void smoker_m() {
    while(true){
        sem_wait(match_sem);
        std::cout << "M" << std::endl;
        sem_post(barman_sem);
    }
}

void barman() {
    while (true) {
        sem_wait(barman_sem);

        if(count == SIZE) {
            std::cout << "Closing!!!" << std::endl;
            return;
        }

        char item;
        std::cin >> item;
        ++count;

        switch (item) {
            case 't':
                sem_post(tobacco_sem);
                break;

            case 'p':
                sem_post(paper_sem);
                break;

            case 'm':
                sem_post(match_sem);
                break;

            default:
                std::cout << "Closing!!!" << std::endl;
                return;
        }
    }
}

int main() {
    tobacco_sem = sem_open("/tobacco_sem", O_CREAT, 0644, 0);
    paper_sem = sem_open("/paper_sem", O_CREAT, 0644, 0);
    match_sem = sem_open("/match_sem", O_CREAT, 0644, 0);
    barman_sem = sem_open("/barman_sem", O_CREAT, 0644, 1);

    pid_t pid_t_smoker, pid_p_smoker, pid_m_smoker, pid_barman;

    pid_t_smoker = fork();
    if (pid_t_smoker == 0) {
        smoker_t();
        return 0;
    }

    pid_p_smoker = fork();
    if (pid_p_smoker == 0) {
        smoker_p();
        return 0;
    }

    pid_m_smoker = fork();
    if (pid_m_smoker == 0) {
        smoker_m();
        return 0;
    }

    pid_barman = fork();
    if (pid_barman == 0) {
        barman();
        return 0;
    }

    waitpid(pid_t_smoker, NULL, 0);
    waitpid(pid_p_smoker, NULL, 0);
    waitpid(pid_m_smoker, NULL, 0);
    waitpid(pid_barman, NULL, 0);

    sem_unlink("/tobacco_sem");
    sem_unlink("/paper_sem");
    sem_unlink("/match_sem");
    sem_unlink("/barman_sem");

    return 0;
}
