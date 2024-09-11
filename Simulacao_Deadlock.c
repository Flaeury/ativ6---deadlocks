
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t primeiro_mutex, segundo_mutex; // criando os mutexes (recursos)

void* thread1() { // thread 1
    printf("Thread1 esta tentando acessar o primeiro_mutex\n");
    pthread_mutex_lock(&primeiro_mutex); // thread 1 bloqueia o primeiro mutex
    printf("Thread1 acessou o primeiro_mutex\n");
    sleep(1); // tempo de espera
    printf("Thread1 esta tentando acessar o segundo_mutex\n");
    pthread_mutex_lock(&segundo_mutex); // thread 1 bloqueia o bloqueia mutex;
    printf("Thread1 acessou o segundo_mutex\n"); 

    // libera os mutexes
    pthread_mutex_unlock(&primeiro_mutex);
    pthread_mutex_unlock(&segundo_mutex);
}

void* thread2() { // thread 2
    printf("Thread2 esta tentando acessar o segundo_mutex\n");
    pthread_mutex_lock(&segundo_mutex); // thread 2 bloqueia o segundo mutex;
    printf("Thread2 acessou o segundo_mutex\n"); 
    sleep(1); // tempo de espera
    printf("Thread2 esta tentando acessar o primeiro_mutex\n");
    pthread_mutex_lock(&primeiro_mutex); // thread 2 bloqueia o primeiro mutex;
    printf("Thread2 acessou o primeiro_mutex\n"); 

    // libera os mutexes
    pthread_mutex_unlock(&primeiro_mutex);
    pthread_mutex_unlock(&segundo_mutex);
}


int main() {
    pthread_mutex_init(&primeiro_mutex, NULL); // init dos recursos que a thread vai acessar (semaforos)
    pthread_mutex_init(&segundo_mutex, NULL); // init dos recursos que a thread vai acessar (semaforos)

    pthread_t T1, T2; // declarando as threads

    pthread_create(&T1, NULL, thread1, NULL); // criando thread 1
    pthread_create(&T2, NULL, thread2, NULL); // criando thread 2
    // Obs: as duas threads trabalham em conjunto

    pthread_join(T1, NULL); // função para aguradar o termino da thread
    pthread_join(T2, NULL); // função para aguradar o termino da thread

    printf("Thread joined\n");

    // limpa os mutexes
    pthread_mutex_destroy(&primeiro_mutex);
    pthread_mutex_destroy(&segundo_mutex);

    return 0;
}
