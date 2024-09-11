#include <stdio.h>
/*biblioteca de threads */
#include <pthread.h>
#include <unistd.h>

/* Mutexes utilizados */
pthread_mutex_t lock1, lock2;


/* Simulação dos processos funcionando */
int main() {
    /* Identificadores de thread */
    pthread_t t1,t2;
    
    /* Inicialização dos mutexes */
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    /* Atribuindo as threads aos processos */
    pthread_create(&t1, NULL, Processo_A, NULL);
    pthread_create(&t2, NULL, Processo_B, NULL);

    /* Sincronização de threads */
    pthread_join(&t1, NULL);
    pthread_join(&t2, NULL);

    /* Finalização dos mutexes */
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}