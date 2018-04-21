#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "sthread.h"

void Saludar(void* input){
    int* opcion = (int*) input;

    while(1){
        if (*opcion == 1) {
            printf("[Thread Saludador]: Holaa putoooo!\n");
            *opcion = 0;
        }
        else if (*opcion == -1) {
            sthread_exit(200);
        }
    }
}

void QueHoraEs(void* input){
    int* opcion = (int*) input;

    time_t rawtime;
    struct tm * timeinfo;

    while (1) {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf ( "La fecha y hora son: %s", asctime (timeinfo) );
        sleep(5);
        if (*opcion == -1) {
            sthread_exit(200);
        }
    }
}

int main(int argc, char const *argv[]) {
    int Op = 0;
    sthread_t saludador, dar_hora;

    sthread_create(&saludador, &Saludar, &Op);
    sthread_create(&dar_hora, &QueHoraEs, &Op);

    while (1) {
        printf("Ingrese su opcion \n");
        printf("1) Saludar \n");
        printf("-1) Salir \n");

        scanf("%d\n", &Op);
        if (Op == -1){
            break;
        }
    }

    sthread_join(saludador);
    printf("[Thread Saludador]: Chao puto.\n");
    sthread_join(dar_hora);
    printf("[Thread Horario]: Byebye.\n");

    printf("[Thread Menu]: Adios amigo.\n");

    return 0;
}
