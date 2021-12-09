/*
* @file    : Tarea2.c
* @author  : Robert Antonio Parra
* @date    : 12/12/2021
* @brief   : Código para tarea 02 en ELO 321, semestre 2021-2
*/

/*
 * Likns usados:
 * https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c
 * https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
 * https://www.geeksforgeeks.org/generating-random-number-range-c/
 * https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/
 * https://www.youtube.com/watch?v=BgbyDnznzls
 * https://www.youtube.com/watch?v=kbXjKKaMUAI
 * https://www.youtube.com/watch?v=uT91o6IrcZo
 * https://www.geeksforgeeks.org/program-bankers-algorithm-set-1-safety-algorithm/
 * https://www.thecrazyprogrammer.com/2016/07/bankers-algorithm-in-c.html
 * https://scanftree.com/programs/c/c-program-for-bankers-algorithm/
 * https://technicalpickout.com/bankers-algorithm-code-in-c/
 * https://www.studytonight.com/operating-system/bankers-algorithm
 * https://rosettacode.org/wiki/Banker%27s_algorithm
 * https://www.geeksforgeeks.org/multithreading-in-cpp/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

/* Su programa debe ser modular y funcionar para cualquier valor >=0 de estos dos parámetros*/
#define NUM_DE_CLIENTES 6
#define NUM_DE_RECURSOS 5

#define randnum(min, max) \
((rand() % (int)(((max) + 1) - (min))) + (min))


/* El arreglo indicando las instancias disponibles de cada recurso*/
int disponibles[NUM_DE_RECURSOS];

/* El arreglo indicando la máxima demanda de recursos de cada cliente*/
int maximos[NUM_DE_CLIENTES][NUM_DE_RECURSOS];

/* La cantidad de instancias de recursos actualmente asignadas a cada cliente*/
int asignados[NUM_DE_CLIENTES][NUM_DE_RECURSOS];

/* La cantidad de recursos aún necesarios por cada cliente */
int necesidad[NUM_DE_CLIENTES][NUM_DE_RECURSOS];

void* funcion_thread(void* num_cliente);

int solicitar_recursos(int num_cliente, int solicitud[]);

int main() {

    printf("--- Welcome to the program! ---\n\n");

    srand(time(NULL)); // Initialization for the aleatory number creation it's be should only be called once.

    int Tabla_Max[NUM_DE_CLIENTES][NUM_DE_RECURSOS];
    for (int i = 0; i < NUM_DE_CLIENTES; i++) {
        for (int j = 0; j < NUM_DE_RECURSOS; j++) {
            int r = rand() % 9;      // Returns a pseudo-random integer between 0 and RAND_MAX.
            Tabla_Max[i][j] = r;
        }
    }
    printf("---- Max Matrix Result ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf("| %d | ", Tabla_Max[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int Tabla_Allocation[NUM_DE_CLIENTES][NUM_DE_RECURSOS];
    for (int i = 0; i < NUM_DE_CLIENTES; i++) {
        for (int j = 0; j < NUM_DE_RECURSOS; j++) {
            int r = rand() % 4;
            if ((Tabla_Max[i][j] - r) < 0){
                Tabla_Allocation[i][j] = 0;
            }else{
                Tabla_Allocation[i][j] = r;
            }
        }
    }
    printf("---- Allocation Matrix Result ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf("| %d | ", Tabla_Allocation[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /* Need table creation with another 2 tables generation first */
    int need[NUM_DE_CLIENTES][NUM_DE_RECURSOS];
    for (int i = 0; i < NUM_DE_CLIENTES; i++) {
        for (int j = 0; j < NUM_DE_RECURSOS; j++) {
            need[i][j] = Tabla_Max[i][j] - Tabla_Allocation[i][j];
        }
    }
    printf("---- Need Matrix Result ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf("| %d | ", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int avail[NUM_DE_RECURSOS]; // Available Resources
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        avail[i]= (randnum(2, 4));
    }
    printf("---- Available Resources ----!\n");
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        printf("| %d | ", avail[i]);
    }

//    pthread_t tid; /* identificador del thread */
//    pthread_attr_t attr; /* setea los atributos del thread */
////
//    /* le asignamos atributos default al thread */
//    pthread_attr_init(&attr);
////
//    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
//        /* se crea el thread y se le asigna la funcion controladora del mismo */
//        pthread_create(&tid, &attr, funcion_thread, /*Se espera el valor del argumento en el main argv[1]*/"4");
//        /* espera a la salida del thread */
//        pthread_join(tid,NULL);
//    }

    return 0;
}
void* funcion_thread(void* num_cliente){
    //while(solicitar_recursos(num_cliente, solicitud)==-1); //ejemplo de busy wait
    return NULL;
}