#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Mutexes */
pthread_mutex_t primeiro_mutex, segundo_mutex;

int thread1_ok = 0; // Flag para indicar que a thread 1 terminou
int deadlock_detectado = 0; // Flag para indicar que o deadlock foi detectado

void* Deadlock_Resolution() { // thread de resolução do deadlock
    while (1) {
        if (deadlock_detectado) {
            printf("Deadlock detectado! Resolvendo...\n");

            // Libera os recursos da Thread 2
            pthread_mutex_unlock(&segundo_mutex);
            pthread_mutex_unlock(&primeiro_mutex);

            printf("Recursos liberados, Thread 1 vai continuar.\n");

            deadlock_detectado = 0;
        }
    }
    return NULL;
}

void* Thread_1() {
    printf("Thread 1 acessou o primeiro_mutex\n");
    pthread_mutex_lock(&primeiro_mutex); // Thread 1 bloqueia o primeiro mutex

    sleep(1); // Simula atraso para gerar o deadlock

    printf("Thread 1 tentando acessar o segundo_mutex\n");
    pthread_mutex_lock(&segundo_mutex); // Thread 1 bloqueia o segundo mutex

    printf("Thread 1 executando com ambos os mutexes.\n");

    // Simula o trabalho da Thread 1
    sleep(2);

    // Libera os mutexes
    pthread_mutex_unlock(&segundo_mutex);
    pthread_mutex_unlock(&primeiro_mutex);

    thread1_ok = 1; // Indica que a Thread 1 completou sua execução

    printf("Thread 1 OK!\n");
    return NULL;
}

void* Thread_2() {
    printf("Thread 2 acessou o segundo_mutex\n");
    pthread_mutex_lock(&segundo_mutex); // Thread 2 bloqueia o segundo mutex

    sleep(1); // Simula atraso para gerar o deadlock

    printf("Thread 2 tentando acessar o primeiro_mutex\n");
    if (pthread_mutex_trylock(&primeiro_mutex) != 0) { // Tenta bloquear o primeiro mutex
        deadlock_detectado = 1; // Marca que o deadlock foi detectado
        while (thread1_ok == 0) { // Espera a Thread 1 terminar
            // Espera pela resolução do deadlock
        }
    } else {
        pthread_mutex_lock(&primeiro_mutex); // Thread 2 bloqueia o primeiro mutex
    }

    printf("Thread 2 executando com ambos os mutexes.\n");

    // Simula o trabalho da Thread 2
    sleep(2);

    // Libera os mutexes
    pthread_mutex_unlock(&primeiro_mutex);
    pthread_mutex_unlock(&segundo_mutex);

    printf("Thread 2 OK!\n");
    return NULL;
}

/* Simulação dos processos funcionando */
int main() {
    /* Identificadores de thread */
    pthread_t t1, t2, deadlock_detected;

    /* Inicialização dos mutexes */
    pthread_mutex_init(&primeiro_mutex, NULL);
    pthread_mutex_init(&segundo_mutex, NULL);

    /* Atribuindo as threads aos processos */
    pthread_create(&t1, NULL, Thread_1, NULL);
    pthread_create(&t2, NULL, Thread_2, NULL);

    /* Thread para resolver deadlock */
    pthread_create(&deadlock_detected, NULL, Deadlock_Resolution, NULL);

    /* Sincronização de threads */
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    /* Finalização dos mutexes */
    pthread_mutex_destroy(&primeiro_mutex);
    pthread_mutex_destroy(&segundo_mutex);
    
    printf("Threads finalizadas!\n");

    return 0;
}
