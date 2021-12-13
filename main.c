/*
* @file    : main.c
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

/* randum definido para encontrar numeros entre un min y max*/
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
int necesidad[NUM_DE_CLIENTES][NUM_DE_RECURSOS];

void* implements_numbers();

/*
 * Calcular la necesidad en base al arreglo maximo y el arreglo asignado
 */
void* randormizer_numbers();

/*
 * imprime todas las matrices
 */
void* print_all_matrix();

/*
 * funcion que crea las solicitudes, generando un tal que, la solicitud es menor a la necesidad pero puede ser mayor a la cantidad disponible
 * realiza un buzy wait en caso de que la solicitud sea mayor a disponibilidad y que aparte, no logre dar con el check de si dejara el sistema en estado seguro o no,
 * tambien llama a la funcion solicitar recursos para asi luego esta llame a la funcion del algoritmo del banquero
 */
void* funcion_thread(int num_cliente);

/*
 * toma la solicitud para luego operar segun esta, logrando asi, en caso de que la solicitud sea menor a la cantidad disponible, pasarla al algoritmo del banquero
 */
int solicitar_recursos(int num_cliente, int solicitud[]);

/*
 * realiza un check para evaluar si la peticion podria dejar al estado en modo seguro, o que bien, la necesidad sea menor a la cantidad disponible y que la peticion este entre estos valores,
 * una vez realizado el check procede a realizar el algoritmo del banquero
 */
int algoritmoDelBanquero(int num_cliente, int solicitud[]);

/*
 * imprime las matrices importantes para ver el funcionamiento del codigo
 */
void* print_important();

/*
 * funcion que chequea si el array de necesidad es 0, en caso de evaluarlo como true, entonces libera los recursos.
 */
bool checkPegs(int num_client, int size);

/*
 * chequea el estado de una peticion, es decir, revisa si la peticion dejaria al estado safe o bien, que la necesidad sea menor o igual a la matriz de disponibilidad
 */
bool check_state(int num_client);

/*
 * funcion que libera los recursos de un cliente
 */
void liberar_recursos(int num_cliente, int liberacion[]);

pthread_mutex_t mutex;

/*
 * main del programa
 */
int main() {

    printf("--- Welcome to the program! ---\n\n");
    printf("--- Ingresa 1 si quieres ingresar una matriz o 0 si quieres la matriz que el porgrama ya trae ---\n\n");


    int resultado_opcion = 0;
    scanf("%d",&resultado_opcion);

    if (resultado_opcion==1){
        implements_numbers();
    }

    randormizer_numbers();
    print_all_matrix();

    pthread_t tid; /* identificador del thread */
    pthread_attr_t attr; /* setea los atributos del thread */

    /* le asignamos atributos default al thread */
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex,NULL);

   for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
       /* se crea el thread y se le asigna la funcion controladora del mismo */
       pthread_create(&tid, &attr, funcion_thread, i); /*Se espera el valor del argumento en el main argv[1]*/
   }
   for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
       /* espera a la salida del thread */
       pthread_join(tid,NULL);
   }

    return 0;
}

void* funcion_thread(int num_cliente){
    //pthread_mutex_lock(&mutex);
    int solicitud[NUM_DE_RECURSOS];
    int tmp = 0;
    int turno = 0;

    /*
    * for para crear la solicitud de manera aleatoria
    */
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        solicitud[i] = (randnum(0, necesidad[num_cliente][i]));
    }

    /*
    * for para que nunca el arreglo sea completamente 0
    */
    bool result = checkPegs(num_cliente,NUM_DE_RECURSOS);
    if (result){
        //esto quiere decir que el arreglo del cliente esta vacio, o que bien, lo dejo en estado safe
        for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
            disponibles[i] = disponibles[i] + asignados[num_cliente][i];
        }
    }else{
        for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
            turno += 1;
            tmp = tmp + solicitud[i];
            if (turno == NUM_DE_RECURSOS && tmp == 0) {
                for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
                    solicitud[i] = (randnum(1, necesidad[num_cliente][i]));
                }
            }
        }
    }
    while(solicitar_recursos(num_cliente, solicitud)==-1); //ejemplo de busy wait
    //pthread_mutex_unlock(&mutex);
    return NULL;
}

int solicitar_recursos(int num_cliente, int solicitud[]){

    /*
     * descomentar para ver las peticiones de los otros threads, lo deje comentado meramente para poder ver mejor lo que se imprimia.
     */
    //pthread_mutex_lock(&mutex);
    //printf("\nCliente %d solicita recursos: \n", num_cliente);
    //printf("EL RECURSO ES: ");
    //for (int i = 0; i < NUM_DE_RECURSOS ; ++i) {
    //    printf(" %d ",solicitud[i]);
    //}
    //pthread_mutex_unlock(&mutex);

    int tmp = 0;
    for (int i = 0; i < NUM_DE_RECURSOS; ++i){
        if(solicitud[i]<=disponibles[i]){
            NULL;
        }else{
            tmp = tmp+1;
        }
    }

    if(tmp>0){
        return -1;
    }else{
        pthread_mutex_lock(&mutex);
        algoritmoDelBanquero(num_cliente, solicitud);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int algoritmoDelBanquero(int num_cliente, int solicitud[]){

    bool result = check_state(num_cliente);
    if(result==true){

        printf("\nCliente %d solicita recursos: \n", num_cliente);
        printf("EL RECURSO ES: ");
        for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
            printf(" %d ", solicitud[i]);
        }
        printf("\n");
        for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
            necesidad[num_cliente][i] = necesidad[num_cliente][i] - solicitud[i];
            asignados[num_cliente][i] = asignados[num_cliente][i] + solicitud[i];
            disponibles[i] = disponibles[i] - solicitud[i];
        }
        print_important();

    }else{
        return -1;
    }
    return 0;
}

void* implements_numbers(){
    printf("Matriz Disponible \n");
    for(int i = 0;i < NUM_DE_RECURSOS; i++){
        printf("Cliente %d: =  ",i);
        scanf("%d",&disponibles[i]);
    }

    printf("Matriz máxima \n");
    for (int i = 0; i < NUM_DE_CLIENTES; i++){
        printf("Maximos cliente #%d \n",i);
        for (int j = 0; j < NUM_DE_RECURSOS; j++){
            printf("Cliente %d: %d = ",i,j);
            scanf("%d",&maximos[i][j]);
        }
    }

    printf("Matriz Asignacion \n");
    for (int i = 0; i < NUM_DE_CLIENTES; i++){
        printf("Asignados cliente #%d \n",i);
        for (int j = 0; j < NUM_DE_RECURSOS; j++){
            printf("Cliente %d: %d = ",i,j);
            scanf("%d",&asignados[i][j]);
        }
    }

}


void* randormizer_numbers(){

    /* Need table creation with another 2 tables generation first */
    for (int i = 0; i < NUM_DE_CLIENTES; i++) {
        for (int j = 0; j < NUM_DE_RECURSOS; j++) {
            necesidad[i][j] = maximos[i][j] - asignados[i][j];
        }
    }

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

void* print_important(){
    printf("---- Asignados ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        printf("Cliente %d: ",i);
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf(" %d | ", asignados[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n---- Necesidad ----!\n");
    for (int i = 0; i < NUM_DE_CLIENTES; ++i) {
        printf("Cliente %d: ",i);
        for (int j = 0; j < NUM_DE_RECURSOS; ++j) {
            printf(" %d | ", necesidad[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("\n---- Disponibles ----!\n");
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        printf(" %d | ", disponibles[i]);
    }
    printf("\n\n");
    return NULL;
}

bool checkPegs(int num_client, int size)
{
    for (int i = 0; i < size; i++)
    {
        if(necesidad[num_client][i] != 0)
        {
            return false;
        }
    }
    liberar_recursos(num_client,necesidad[num_client]);
    return true;
}

bool check_state(int num_client)
{
    for (int i = 0; i < NUM_DE_RECURSOS; ++i) {
        bool result = necesidad[num_client][i]<=disponibles[i];
        if(result==true){
            printf("\n----- ESTADO SEGURO -----\n");
            return true;
        }else{
            return false;
        }
    }
}

void liberar_recursos(int num_cliente, int liberacion[]){
    for(int i = 0;i < NUM_DE_RECURSOS;i++){
        disponibles[i]+= asignados[num_cliente][i];
        asignados[num_cliente][i] = 0;
    }
}