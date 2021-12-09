## Parte A

> Su programa debe generar tantos hilos (threads) como el número de clientes a considerar y debe esperar
> que los threads terminen antes de completar su ejecución. Cada thread ejecutará la función:
> “void* función_thread(void* num_cliente)”  

Para esto, se considero un total de 6 numero de clientes maximo y fijo, debido al numero impuesto por el ```#define NUM_DE_CLIENTES 6``` por lo que se utilizo un for en el main para recurrir a la creacion de un total de 6 threads donde cada uno ejecuta la funcion que se especifico, esperando al termino de cada una.

> que recibe como argumento el número identificador del cliente y en ella debe realizar una solicitud por
> una combinación de recursos, cuya cantidad se calculará aleatoriamente usando valores válidos para su
> programa **(por ejemplo, la cantidad solicitada debe ser positiva, mayor que cero, y NO debe superar la
necesidad  del  cliente)**.

Aqui se asumio que el numero de identificador del cliente, es un numero mayor o igual a 0 y como maximo 6, ya que hay un total de 6 numero de clientes, por lo que si existe un 7, el programa no cumpliria con las bases presentadas en la demanda del cliente.
Una vez explicado esto, se procede a crear la combinacion de recursos, generando la cantidad aleatoria usando valores validos para el programa.
Para esto, se toma en consideracion especificamente lo explicito en los parentesis, la cantidad debe ser positiva, mayor que 0 y NO DEBE SUPERAR LA NECESIDAD DEL CLIENTE. Es decir, que la necesidad del cliente debe estar pre-definida, o bien este debe definirla, pero por temas de que no se especifica en el documento, se dejo la tabla de necesidad en el main, es decir, que si se requiere cambiar para posibles testeos, esta se debe cambiar desde el main.