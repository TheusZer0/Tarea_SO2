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
#include <stdbool.h>

/* Su programa debe ser modular y funcionar para cualquier valor >=0 de estos dos parámetros*/
#define NUM_DE_CLIENTES 5
#define NUM_DE_RECURSOS 3

#define randnum(min, max) \
((rand() % (int)(((max) + 1) - (min))) + (min))


/* El arreglo indicando las instancias disponibles de cada recurso*/
int disponibles[NUM_DE_RECURSOS] = {3, 3, 2};

/* El arreglo indicando la máxima demanda de recursos de cada cliente*/
int maximos[NUM_DE_CLIENTES][NUM_DE_RECURSOS] = {{7, 5, 3},
                                                 {3, 2, 2},
                                                 {9, 0, 2},
                                                 {2, 2, 2},
                                                 {4, 3, 3}};

/* La cantidad de instancias de recursos actualmente asignadas a cada cliente*/
int asignados[NUM_DE_CLIENTES][NUM_DE_RECURSOS] = {{0, 1, 0},
                                                   {2, 0, 0},
                                                   {3, 0, 2},
                                                   {2, 1, 1},
                                                   {0, 0, 2}};

/* La cantidad de recursos aún necesarios por cada cliente */
int necesidad[NUM_DE_CLIENTES][NUM_DE_RECURSOS] = {
        {7 ,  4 ,  3 },
        {1 ,  2 ,  2 },
        {6 ,  0 ,  0 },
        {0 ,  1 ,  1 },
        {4 ,  3 ,  1 }
};

int necesidad_cop[NUM_DE_CLIENTES][NUM_DE_RECURSOS] = {
        {7 ,  4 ,  3 },
        {1 ,  2 ,  2 },
        {6 ,  0 ,  0 },
        {0 ,  1 ,  1 },
        {4 ,  3 ,  1 }
};

int disponibles_cop[NUM_DE_RECURSOS] = {3, 3, 2};

void* randormizer_numbers();

void* print_all_matrix();

void* funcion_thread(int num_cliente);

int solicitar_recursos(int num_cliente, int solicitud[]);

int algoritmoDelBanquero(int num_cliente, int solicitud[]);

pthread_mutex_t mutex;

int main() {

    printf("--- Welcome to the program! ---\n\n");


    randormizer_numbers();
    print_all_matrix();

    //pthread_mutex_init(&mutex,NULL);
    pthread_t tid; /* identificador del thread */
    pthread_attr_t attr; /* setea los atributos del thread */

    /* le asignamos atributos default al thread */
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex,NULL);

   for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
       /* se crea el thread y se le asigna la funcion controladora del mismo */
       pthread_create(&tid, &attr, funcion_thread, i); /*Se espera el valor del argumento en el main argv[1]*/
       /* espera a la salida del thread */
   }
   for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
       pthread_join(tid,NULL);
   }

    return 0;
}

void* funcion_thread(int num_cliente){
    int tmp = 0;
    int turno = 0;
    int solicitud[NUM_DE_RECURSOS];

    pthread_mutex_lock(&mutex);
    /*
     * for para crear la solicitud de manera aleatoria
     */
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        solicitud[i] = (randnum(0, necesidad[num_cliente][i]));
    }

    /*
     * for para que nunca el arreglo sea completamente 0
     */
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        turno +=1;
        tmp = tmp + solicitud[i];
        if (turno == NUM_DE_RECURSOS && tmp == 0){
            for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
                solicitud[i] = (randnum(1, necesidad[num_cliente][i]));
            }
        }
    }
    pthread_mutex_unlock(&mutex);
    while(solicitar_recursos(num_cliente, solicitud)==-1); //ejemplo de busy wait
    return NULL;
}

int solicitar_recursos(int num_cliente, int solicitud[]){
    pthread_mutex_lock(&mutex); //no estoy seguro
    printf("\nCliente %d solicita recursos: \n", num_cliente);

    int result;

    printf("EL RECURSO ES: ");
    for (int i = 0; i < NUM_DE_RECURSOS ; ++i) {
        printf(" %d ",solicitud[i]);
    }
    pthread_mutex_unlock(&mutex); //no estoy seguro
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {

        if(solicitud[i]<=disponibles[i]){
            result = algoritmoDelBanquero(num_cliente, solicitud);
            return 0;
        } else{
            return -1;
        }
    }
    return 0;
}

int algoritmoDelBanquero(int num_cliente, int solicitud[]){
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        necesidad_cop[num_cliente][i] = necesidad[num_cliente][i] - solicitud[i];
        disponibles_cop[i] = disponibles[i] - solicitud[i];
    }


    return 0;
}

void* randormizer_numbers(){
    srand(time(NULL)); // Initialization for the aleatory number creation it's be should only be called once.

    //for (int i = 0; i < NUM_DE_CLIENTES; i++) {
    //    for (int j = 0; j < NUM_DE_RECURSOS; j++) {
    //        int r = rand() % 8;      // Returns a pseudo-random integer between 0 and RAND_MAX.
    //        maximos[i][j] = r;
    //    }
    //}
//
    //for (int i = 0; i < NUM_DE_CLIENTES; i++) {
    //    for (int j = 0; j < NUM_DE_RECURSOS; j++) {
    //        int r = rand() % 4;
    //        if ((maximos[i][j] - r) < 0){
    //            asignados[i][j] = 0;
    //        }else{
    //            asignados[i][j] = r;
    //        }
    //    }
    //}

    /* Need table creation with another 2 tables generation first */
    //for (int i = 0; i < NUM_DE_CLIENTES; i++) {
    //    for (int j = 0; j < NUM_DE_RECURSOS; j++) {
    //        necesidad[i][j] = maximos[i][j] - asignados[i][j];
    //        necesidad_cop[i][j] = maximos[i][j] - asignados[i][j];
    //    }
    //}

    //for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
    //    disponibles[i]= (randnum(2, 5));
    //}

    return NULL;
}

void* print_all_matrix(){
    printf("---- Maxima ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        printf("Cliente %d: ",i);
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf(" %d | ", maximos[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("---- Asignados ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        printf("Cliente %d: ",i);
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf(" %d | ", asignados[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("---- Necesidad ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        printf("Cliente %d: ",i);
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf(" %d | ", necesidad[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("---- Disponibles ----!\n");
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        printf(" %d | ", disponibles[i]);
    }
    printf("\n\n");
    return NULL;
}