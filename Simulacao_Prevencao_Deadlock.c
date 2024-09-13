#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t primeiro_mutex, segundo_mutex; // criando os mutexes (recursos)

int rec1 = 1;
int rec2 = 1;

void* thread1() { // thread 1
    while(1) {
        
        pthread_mutex_lock(&primeiro_mutex);
        printf("Thread 1 acessou o primeiro mutex\n");
        rec1 = rec1 - 1;
        
        if(rec2 == 0) {
            pthread_mutex_unlock(&primeiro_mutex);
            printf("Thread 1 falhou ao acessar o segundo mutex, liberou o primeiro mutex e foi bloqueada!\n");
            rec1 = rec1 + 1;
            
        } else {
            pthread_mutex_lock(&segundo_mutex);
            
            rec2 = rec2 - 1;
            
            printf("Thread 1 acessou o segundo mutex\n");
            printf("Thread 1 OK!\n");
            
            pthread_mutex_unlock(&primeiro_mutex);
            pthread_mutex_unlock(&segundo_mutex);
            
            rec1 = rec1 + 1;
            rec2 = rec2 + 1;
            
            break;
        }
    }
}

void* thread2() { // thread 2
    while(1) {
        
        pthread_mutex_lock(&segundo_mutex);
        printf("Thread 2 acessou o segundo mutex\n");
        rec2 = rec2 - 1;
        
        if(rec1 == 0) {
            pthread_mutex_unlock(&segundo_mutex);
            printf("Thread 2 falhou ao acessar o primeiro mutex, liberou o segundo mutex e foi bloqueada!\n");
            rec2 = rec2 + 1;
            
        } else {
            pthread_mutex_lock(&primeiro_mutex);
            
            rec1 = rec1 - 1;
            
            printf("Thread 2 acessou o primeiro mutex\n");
            printf("Thread 2 OK!\n");
            
            pthread_mutex_unlock(&primeiro_mutex);
            pthread_mutex_unlock(&segundo_mutex);
            
            rec1 = rec1 + 1;
            rec2 = rec2 + 1;
            
            break;
        }
    }
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

    printf("Threads finalizadas!\n");

    // limpa os mutexes
    pthread_mutex_destroy(&primeiro_mutex);
    pthread_mutex_destroy(&segundo_mutex);

    return 0;
}