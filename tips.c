#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//global variables delaration;
#define M 3 //no of resources
#define N 5 //no of processoses
 pthread_mutex_t mutex; 
 int available[M];
 int allocation[N][M]; 
 int Max[N][M];
 int Need[N][M];

void *funcion_thread(void* num_cliente);
int liberar_recursos(int pID,int releaseVector[]);
int algoritmoDelBanquero(int pID, int request[]);
int solicitar_recursos(int pID,int request[]);

//===========================================================================================================================
//========================================= MAIN ============================================================================
//===========================================================================================================================

int main()
{           
    int i;
    int j;
    printf("Enter available VECTOR\n");
    for(i = 0; i < M; i++)
    {      
        scanf("%d",&available[i]);
    }
    printf("Enter Allocation Matrix\n");
    for(i = 0; i < N; i++)
    {                
        for(j = 0; j< M; j++)
        {        
                scanf("%d",&allocation [i][j]);
         }
    }
    printf("Enter MAX Matrix\n");
    for(i = 0; i < N; i++)
    {               
        for(j = 0; j< M; j++)
        {        
            scanf("%d",&Max [i][j]);       
        }
    }
    //NeedMatrix matrix ko initialize karna
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < M; ++j)
        {
            Need[i][j] = Max[i][j] - allocation[i][j];
        }
    }

    pthread_mutex_init(&mutex,NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t *tid = malloc(sizeof(pthread_t) * N);

    int *pid = malloc(sizeof(int) * N); // process ID 

    //initialize pid and create pthreads
    for(i = 0; i < N; i++)
    {
        *(pid + i) = i;
        pthread_create((tid+i), &attr, funcion_thread, (pid+i));
    }

    //join threads
    for(i = 0; i < N; i++)
    {
        pthread_join(*(tid+i),NULL);
    }
    return 0;
}

//===========================================================================================================================
//================================= FUNCIÓN DEL THREAD ======================================================================
//===========================================================================================================================

void *funcion_thread(void* num_cliente)
{  
    int i;
    int j;   
    int pID = *(int*)num_cliente;
    int suma = 0;
    int solicitud;
    for (i=0;i<M;i++)
    {
        suma += Need[pID][i];
    }
    while(suma != 0)
    {
        suma = 0;
        //generate random requests
        int request[M];
        for(i = 0; i < M; i++)
        {
            if(Need[pID][i] != 0)
            {
                if(Need[pID][i] == 1)
                {
                    request[i] = 1; // Como generar un randn() entre 0 y 1
                }
                else
                {
                    request[i] = rand() % Need[pID][i];
                }
            }
            else
            {
                request[i] = 0;
            }
        }
        do{
            pthread_mutex_lock(&mutex);
            printf("Solicitando Recursos...\n");
            solicitud = solicitar_recursos(pID,request);
            pthread_mutex_unlock(&mutex);
        } while( solicitud == -1);
       for (i=0;i<M;i++)
        {
            suma += Need[pID][i];
        }
    }
    
    int liberacion[M];
    pthread_mutex_lock(&mutex);
    //release vector ko initialize karna
    for(i = 0; i < M; i++)
    {
        if(allocation[pID][i] != 0)
        {
            liberacion[i] = allocation[pID][i];
        }
        else
        {
            liberacion[i] = 0;
        }
    }
    printf("\nLiberando recursos\n");
    liberar_recursos(pID,liberacion);
    printf("El vector available es:\n");
    for (i = 0; i < M; ++i)
    {
        printf("%d, ",available[i]);
    } 
    printf("\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

//===========================================================================================================================
//================================= SOLICITAR RECURSOS ======================================================================
//===========================================================================================================================

int solicitar_recursos(int pID,int request[])
{
    int i, j;
    printf("El cliente %d solicita los recursos: ", pID);
    printf("{");
    for (i = 0; i < M; ++i)
    {
        printf("%d, ",request[i]);
    } 
    printf("}\n");
    //VERIFICAR SI LA SOLICITUD NO ES MAYOR QUE LA NECESIDAD
    for (i = 0; i < M; ++i)
    {
        if (request[i] <= Need[pID][i])
        {  
            continue; 
        }
        else
        { 
            printf("Error: la solicitud supera su necesidad máxima");
            return -2; 
        }
    }
    printf("Resources are being allocated...\n");
    // VERIFICAR DISPONIBILIDAD DE RECURSOS
    for (i = 0; i < M; ++i)
    {
        if (request[i] <= available[i])
        { 
            continue; 
        }
        else
        {
            printf("Los recursos solicitados no se encuentran diponibles.\n");
            return -1; 
        }
    }
    // EJECUTAR MODO SEGURO
    printf("Checking if the state is safe...\n");   
    if (algoritmoDelBanquero(pID,request) == 1)
    {
        printf("\nx========================x\n|Safe Mode. Resources Allocated|\nx=========================x\n");
        for (i = 0; i < M; ++i)
        {
            Need[pID][i] -= request[i];
            allocation[pID][i] += request[i];
            available[i] -= request[i];
        }   
        printf("La necesidad del thread %d es:\n{", pID);
        for (j = 0; j < M; ++j)
        {
            printf("%d, ", Need[pID][j]);
        }
        printf("}\n");
        printf("El vector available es:\n");
        for (i = 0; i < M; ++i)
        {
            printf("%d, ",available[i]);
        }
        printf("\n");
        return 0;
    }
    else
    {           
        printf("\nx=====================x\n|State is not safe.          |\nx=====================x\n"); 
        return -1;
    }
}

//===========================================================================================================================
//================================= ALGORITMO DEL BANQUERO ==================================================================
//===========================================================================================================================

int algoritmoDelBanquero(int pID, int request[])
{
    int i, j;
    int virutal_Need[N][M];
    int virutal_alloc[N][M];
    int virutal_avail[M];
    for (i = 0; i < N; ++i)
    {
        for(j=0;j<M;j++)
        {
            virutal_Need[i][j] = Need[i][j];
        }
    }
    for (i = 0; i < N; ++i)
    {
        for(j=0;j<M;j++)
        {
            virutal_alloc[i][j] = allocation[i][j];
        }
    }
    for(j=0;j<M;j++)
    {
        virutal_avail[j] = available[j];
    }
    //  ASIGNACIÓN VIRTUAL DE RECURSOS
    for (i = 0; i < M; ++i)
    {
        virutal_Need[pID][i] -= request[i];
        virutal_alloc[pID][i] += request[i];
        virutal_avail[i] -= request[i];
    }
    int finish[N] = {0};
    int work[M];
    for(i = 0; i < M; i++)
    {
        work[i] = virutal_avail[i];
    }
    int k;
    for(i = 0; i < N; i++)
    {
        if (finish[i] == 0)
        {
            for(j = 0; j < M; j++)
            {
                if(virutal_Need[i][j] <= work[j])
                {
                    if(j==M-1)                                                                                                            
                    {
                        finish[i] = 1;
                        for (k = 0; k < M; ++k)
                        {
                            work[k] += virutal_alloc[i][k];
                        }
                        i = -1;
                        break;
                    }
                    else
                    { 
                        continue; 
                    }
                }
                else
                { 
                    break; 
                }
            }
        }
        else
        {
            continue; 
        }
    }
    for(i = 0; i < N; i++)
    {
        if (finish[i] == 0)
        {
            return 0;
        }
        else
        { 
            continue; 
        }
    }
    return 1;
}

//===========================================================================================================================
//========================================= LIBERAR RECURSOS ================================================================
//===========================================================================================================================

int liberar_recursos(int pID,int releaseVector[])
{    
    int i;      
    for(i = 0; i < M; i++)
    {
        allocation[pID][i] -= releaseVector[i];
        Need[pID][i] += releaseVector[i];
        available[i] += releaseVector[i];
    }
    return 0;
}