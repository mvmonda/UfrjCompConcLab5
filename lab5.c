#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_cond_t condBemVindo;
pthread_cond_t condVolteSempre;

int x = 0;

void *bemVindo() {

    pthread_mutex_lock(&mutex);
    printf("Seja bem-vindo!\n");
    x++;
    pthread_cond_signal(&condBemVindo);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *volteSempre() {
    pthread_mutex_lock(&mutex);

    if(x < 4) {
        pthread_cond_wait(&condVolteSempre, &mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *printaNoMeio(void* texto) {
    char *string = (char *)texto;

    pthread_mutex_lock(&mutex);
    if(x < 1) {
        pthread_cond_wait(&condBemVindo, &mutex);
    }
    x++;
    printf("%s", string);

    if(x == 4) {
        pthread_cond_signal(&condVolteSempre);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t threads[5];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&condBemVindo, NULL);
    pthread_cond_init (&condVolteSempre, NULL);


    pthread_create(&threads[1], NULL, bemVindo, NULL);
    pthread_create(&threads[0], NULL, volteSempre, NULL);
    pthread_create(&threads[2], NULL, printaNoMeio, (void *)"Aceita um copo d'agua?\n");
    pthread_create(&threads[3], NULL, printaNoMeio, (void *)"Sente-se por favor\n");
    pthread_create(&threads[4], NULL, printaNoMeio, (void *)"Fique a vontade.\n");

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condBemVindo);
    pthread_cond_destroy(&condVolteSempre);
}