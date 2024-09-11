#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>

/* Mutexes */
pthread_mutex_t lock1, lock2;
int deadlock_detected = 0; // Flag para indicar deadlock

void* Processo_A(void* arg) {
    while (1) {
        // Tenta adquirir lock1
        if (pthread_mutex_trylock(&lock1) == 0) {
            printf("Processo A adquiriu lock1\n");

            // Tenta adquirir lock2
            if (pthread_mutex_trylock(&lock2) == 0) {
                printf("Processo A adquiriu lock2\n");
                printf("Processo A está na Seção Crítica!\n");

                // Simulação de operação
                sleep(1);

                // Libera os locks
                pthread_mutex_unlock(&lock2);
                pthread_mutex_unlock(&lock1);
                break; // Sai do loop quando a seção crítica é completada com sucesso
            } else {
                // Não conseguiu lock2, libera lock1 e tenta novamente
                pthread_mutex_unlock(&lock1);
                printf("Processo A não adquiriu lock2, liberando lock1 e voltando para a Memória\n");
            }
        }

        // Detecção de deadlock
        if (deadlock_detected) {
            printf("Processo A detectou deadlock, saindo do processo\n");
            break;
        }

        // Pequena pausa antes de tentar novamente
        sleep(1);
    }

    return NULL;
}

void* Processo_B(void* arg) {
    while (1) {
        // Tenta adquirir lock2
        if (pthread_mutex_trylock(&lock2) == 0) {
            printf("Processo B adquiriu lock2\n");

            // Tenta adquirir lock1
            if (pthread_mutex_trylock(&lock1) == 0) {
                printf("Processo B adquiriu lock1\n");
                printf("Processo B está na Seção Crítica!\n");

                // Simulação de operação
                sleep(1);

                // Libera os locks
                pthread_mutex_unlock(&lock1);
                pthread_mutex_unlock(&lock2);
                break; // Sai do loop quando a seção crítica é completada com sucesso
            } else {
                // Não conseguiu lock1, libera lock2 e tenta novamente
                pthread_mutex_unlock(&lock2);
                printf("Processo B não adquiriu lock1, liberando lock2 e voltando para a Memória\n");
            }
        }

        // Detecção de deadlock
        if (deadlock_detected) {
            printf("Processo B detectou deadlock, saindo do processo\n");
            break;
        }

        // Pequena pausa antes de tentar novamente
        sleep(1);
    }

    return NULL;
}

void* Deadlock_Detector(void* arg) {
    while (1) {
        // Simulação de verificação contínua do deadlock
        sleep(2); // Espera um tempo para dar chance aos processos

        // Verifica se ambos os processos estão bloqueados
        if (pthread_mutex_trylock(&lock1) != 0 && pthread_mutex_trylock(&lock2) != 0) {
            printf("Deadlock detectado! Resolvendo...\n");
            deadlock_detected = 1;

            // Força o bloqueio do último processo que está requisitando os recursos
            printf("Forçando bloqueio do processo que requisitou último recurso\n");

            // Libera os recursos para resolver o deadlock
            pthread_mutex_unlock(&lock1);
            pthread_mutex_unlock(&lock2);

            printf("Recursos liberados, deadlock resolvido. Processo que requisitou último recurso será bloqueado.\n");

            break;
        }

        // Pequena pausa antes de tentar novamente
        sleep(1);
    }

    return NULL;
}

/* Simulação dos processos funcionando */
int main() {
    /* Identificadores de thread */
    pthread_t t1, t2, deadlock_detector;

    /* Inicialização dos mutexes */
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    /* Atribuindo as threads aos processos */
    pthread_create(&t1, NULL, Processo_A, NULL);
    pthread_create(&t2, NULL, Processo_B, NULL);

    /* Thread para detecção de deadlock */
    pthread_create(&deadlock_detector, NULL, Deadlock_Detector, NULL);

    /* Sincronização de threads */
    pthread_join(&t1, NULL);
    pthread_join(&t2, NULL);
    pthread_join(&deadlock_detector, NULL);

    /* Finalização dos mutexes */
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
