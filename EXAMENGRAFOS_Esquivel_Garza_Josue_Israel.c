//Josue Israel Esquivel Garza (281876)
//Estructura de datos 2
//03-05-20
//Programa: Grafos METRO CDMX
//Universidad Autonoma De San Luis Potosi

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h> //manejo de modo grafico
#include <string.h>   //manejo de cadenas

//defines
#define IDENX 80
#define IDENY -140
//estructuras
//estructura datos
typedef struct
{
    char estacion[25];
    int visitado;
    int x;
    int y;
    int color;
    int e;
} TDatos;
//estructura ARISTAS
typedef struct nodoa
{
    struct nodoa *sig;
    struct nodo *origen;
    struct nodo *destino;
} ARISTA;
//estructura VERTICES
typedef struct nodo
{
    TDatos info;
    struct nodo *sig;
    struct nodo *anterior;
    ARISTA *arista;
} VERTICE;
//estructura PARA MANEJO DE COLAS
typedef struct
{
    int tam;
    int ini;
    int fin;
    VERTICE **Cola;
} TCola;
//estructura PARA MANEJO DE COLAS
typedef struct
{
    VERTICE *Cabeza;
    TCola Camino;
} TCabeza;
//prototipos
void presentacion();
//lee archivos txt
int leeRutas(VERTICE **Cab);      //hecha
int leeConexiones(VERTICE **Cab); //hecha
//creas
ARISTA *creaArista(VERTICE *origen, VERTICE *destino); //hecha
VERTICE *creaVertice(TDatos dato);                     //hecha
//insertas
int insercionArista(VERTICE **Cab, char ori[], char dest[]); //hecha
int insertaFinal(VERTICE **Cab, TDatos info);                //hecha
//buscas
VERTICE *buscaNombreVertice(VERTICE *Cab);    //hecha
VERTICE *buscaVertice(VERTICE *Vr, char V[]); //hecha
//dibujas
void dibujaRutas(VERTICE *Cab);                        //hecha
void dibujaCamino(VERTICE *Cab, TCola C, void *metro); //hecha
//guarda imagen
void guardaimagen(void **metro, int *imagetam); //hecha
//caminos
int calculaCamino(VERTICE *origen, VERTICE *actual, VERTICE *destino, TCola *Camino); //hecha
void SeleccionaCamino(VERTICE *Cab, TCola *Camino);                                   //hecha
//colas
int iniciaCabeza(TCabeza *Cab);            //hecha
int ColaVacia(TCola C);                    //hecha
int Colallena(TCola C);                    //hecha
void reiniciaCola(TCola *C, VERTICE *Cab); //hecha
int enCola(TCola *Camino, VERTICE *dir);   //hecha
int ExtraerCola(TCola *C);                 //hecha

int main()
{
    int v = 0;
    void *metro;
    int imagentam;
    TCabeza Cab;
    VERTICE *Cabeza = NULL;

    presentacion();                 //presentacion en consola
    initwindow(900, 900);           //tamaño de ventana
    setbkcolor(RGB(255, 229,117)); //color principal
    setfillstyle(11, 15);           //estilos
    //inicia el programa
    guardaimagen(&metro, &imagentam); //Captura la imagen que se movera a lo largo del programa
    //inicializa y crea cabezas, rutas, aristas etc
    iniciaCabeza(&Cab);
    leeRutas(&Cab.Cabeza);
    leeConexiones(&Cab.Cabeza);
    setcolor(BLACK);
    setactivepage(1); // Para que esa pagina que abramos sea la pagina grafica, todos los graficos posteriores se redirigen a esta

    while (1)
    {
        setactivepage(v);
        cleardevice();
        dibujaRutas(Cab.Cabeza); //dibuja las rutas del metro de cdmx
        delay(16);
        setvisualpage(v);
        SeleccionaCamino(Cab.Cabeza, &Cab.Camino);   //Con el click del mouse selecciona el usuario que ruta quiere
        dibujaCamino(Cab.Cabeza, Cab.Camino, metro); //dibuja la ruta para el usuario, como es que el metro recorrera todo
        reiniciaCola(&Cab.Camino, Cab.Cabeza);       //una vez finalizado se reinicia la cola y puede volver a usar el programa
        v = !v;
    }
}

//presentacion
void presentacion()
{
    printf("Josue Israel Esquivel Garza (281876)\n");
    printf("Estructura de datos 2\n");
    printf("03-05-20\n");
    printf("Programa: Grafos METRO CDMX\n");
    printf("Universidad Autonoma De San Luis Potosi\n");
}
//LEE
//lee las rutas de un archivo de texto
int leeRutas(VERTICE **Cab)
{
    FILE *a;                        //variable tipo file
    a = fopen("Rutas.txt", "r"); //abre y lee el archivo tipo texto llamado Rutas.txt
    TDatos datos;

    while (!feof(a))
    {
        fgets(datos.estacion, 25, a);  //lee el nombre de la estacion
        fscanf(a, "%d", &datos.x);     //la posicion en x en donde estara
        fscanf(a, "%d", &datos.y);     //la posicion en x en donde estara
        fscanf(a, "%d", &datos.color); //el color

        insertaFinal(Cab, datos); //los inserta al final
    }
}
//lee las posibles conexiones entre estaciones
int leeConexiones(VERTICE **Cab)
{
    FILE *a;                       //variable tipo file
    a = fopen("Conexiones.txt", "r"); //abre y lee el archivo tipo texto llamado Conexiones.txt
    char origen[25];
    char destino[25];
    int bandera;

    while (!feof(a))
    {
        fgets(origen, 25, a);  //obtienes la estacion origen
        fgets(destino, 25, a); //obtienes la estacion origen

        bandera = insercionArista(Cab, origen, destino); // manda la cabeza, con el origen y destino obtenidos en el documento txt
        //regresa una bandera
    }
}
//CREAS
// crea las conexiones con memoria dinamica
ARISTA *creaArista(VERTICE *origen, VERTICE *destino)
{
    ARISTA *nuevo;
    nuevo = (ARISTA *)malloc(sizeof(ARISTA)); //asignacion de memoria dinamica
    if (nuevo)
    {
        nuevo->origen = origen;   //crea una arista origen
        nuevo->destino = destino; //crea una arista destino
        nuevo->sig = NULL;        //crea una liga siguiente
    }
    return (nuevo); //retorna nuevo
}
// crea las rutas con memoria dinamica
VERTICE *creaVertice(TDatos dato)
{
    VERTICE *nuevo;
    nuevo = (VERTICE *)malloc(sizeof(VERTICE)); //asignacion de memoria dinamica

    if (nuevo)
    {
        strcpy(nuevo->info.estacion, dato.estacion); //copia la informacion de la estacion
        nuevo->info.x = dato.x;                      //se le asigna la posicion en x que esta designada en el documento Vertice.txt
        nuevo->info.y = dato.y;                      //se le asigna la posicion en y que esta designada en el documento Vertice.txt
        nuevo->info.color = dato.color;              //se le asigna el color que esta designado en el documento Vertice.txt
        nuevo->info.visitado = nuevo->info.e = 0;    //asignacion de informacion si esta visitado o no, por default es 0
        nuevo->sig = NULL;                           //liga a siguiente null
        nuevo->anterior = NULL;                      //nodo anterior en null
        nuevo->arista = NULL;                        //conexion en null
    }
    return (nuevo);
}
//INSERCIONES
//Inserta conexion
int insercionArista(VERTICE **Cab, char ori[], char dest[])
{
    int bandera = 0;
    ARISTA *nuevo;
    VERTICE *origen, *destino;

    origen = buscaVertice(*Cab, ori);   //busca el vertice y lo asigna a origen
    destino = buscaVertice(*Cab, dest); //busca el vertice y lo asigna a destino
    if (origen && destino)
    {
        nuevo = creaArista(origen, destino); //crea la conexion
        if (nuevo)
        {
            nuevo->sig = origen->arista;
            origen->arista = nuevo;
        }
        bandera++;
    }
    return (bandera);
}
//inserta al final los datos
int insertaFinal(VERTICE **Cab, TDatos info)
{
    int bandera = 0;
    VERTICE *nuevo, *aux;

    nuevo = creaVertice(info);
    if (nuevo)
    {
        if (*Cab == NULL)
        {
            *Cab = nuevo;
        }
        else
        {
            for (aux = *Cab; aux->sig; aux = aux->sig)
                ;
            aux->sig = nuevo;
            nuevo->anterior = aux;
        }
        bandera++;
    }
    return bandera;
}
//BUSCAS
//busca el nombre de la ruta segun lo que clikeaste
VERTICE *buscaNombreVertice(VERTICE *Cab)
{
    VERTICE *aux, *vertice = NULL;
    int ratonx = -1, ratony = -1;

    while (vertice == NULL)
    {
        outtextxy(700 - 700 / 2, 800 - 800 / 5, "Da click en un vertice origen: ");
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, ratonx, ratony);
        }
        for (aux = Cab; aux && !vertice && (ratonx > -1 && ratony > -1); aux = aux->sig)
        {
            if ((ratonx > ((aux->info.x + IDENX) - 10) && ratony > ((aux->info.y - IDENY) - 10)) && (ratonx < ((aux->info.x + IDENX) + 10) && ratony < ((aux->info.y - IDENY) + 10)))
                vertice = aux;
        }
    }
    return (vertice);
}
//busca el nodo
VERTICE *buscaVertice(VERTICE *Vr, char V[])
{
    VERTICE *aux;
    for (aux = Vr; aux && strcmp(aux->info.estacion, V) != 0; aux = aux->sig)
        ;
    return (aux);
}
//DIBUJAS
//dibuja las rutas
void dibujaRutas(VERTICE *Cab)
{
    VERTICE *aux;
    for (aux = Cab; aux; aux = aux->sig)
    {
        setcolor(aux->info.color);
        setlinestyle(1, 0, 7); //estilo de la linea

        if (aux->anterior)
            line(aux->anterior->info.x + IDENX, aux->anterior->info.y - IDENY, aux->info.x + IDENX, aux->info.y - IDENY); //linea de las rutas del metros
    }

    setlinestyle(0, 0, 2); //circulo de alrededor de los nodos
    setcolor(BLACK);

    for (aux = Cab; aux; aux = aux->sig)
    {
        if (aux->info.color == 2963683 && strcmp(aux->info.estacion, "El Rosario") != 0)
        {
            //settextstyle(fuente,direccion,tamaño del texto);
            settextstyle(6, 1, 1); //direccion vertical de los textos
            outtextxy(aux->info.x + IDENX - 8, aux->info.y - IDENY - 14, aux->info.estacion);
        }
        else
        {
            settextstyle(6, 0, 1); //direccion horizontal de los textos
            outtextxy((aux->info.x + IDENX) - (textwidth(aux->info.estacion) + 10), aux->info.y - (IDENY + 8), aux->info.estacion);
        }
        setfillstyle(SOLID_FILL , RGB(178,255,102)); //color del circulo
        circle(aux->info.x + IDENX, aux->info.y - IDENY, 10);
        floodfill(aux->info.x + IDENX, aux->info.y - IDENY, BLACK);
    }
}
//dibuja la ruta GRAFICAMENTE para el usuario, como es que el metro recorrera todo DESPLIEGA
//en que estacion se encuantra el usuario en tiempo real
//aqui pone la imagen del metro que obtuvimos anteriormente
void dibujaCamino(VERTICE *Cab, TCola C, void *metro)
{
    int i, v = 0, y = 0;
    setactivepage(1);

    for (i = C.ini; i < C.fin; i++)
    {
        setactivepage(v);
        cleardevice();
        if (textwidth(C.Cola[i]->info.estacion) > textwidth("Estacion actual: "))
            rectangle((700 - IDENX * 2) - 5, 295, (700 - IDENX * 2) + 5 + textwidth(C.Cola[i]->info.estacion), 300 + (textheight(C.Cola[i]->info.estacion) * 5) + 5);
        else
            rectangle((700 - IDENX * 2) - 5, 295, (700 - IDENX * 2) + 5 + textwidth("Estacion actual: "), 300 + (textheight(C.Cola[i]->info.estacion) * 5) + 5);

        outtextxy(700 - IDENX * 2, 300, "Estacion actual: ");
        outtextxy(700 - IDENX * 2, 300 + textheight("Estacion actual: "), C.Cola[i]->info.estacion);
        dibujaRutas(Cab); //dibuja las rutas del metro
        setfillstyle(9, RED);
        floodfill(C.Cola[i]->info.x + IDENX, C.Cola[i]->info.y - (IDENY), BLACK);
        putimage((C.Cola[i]->info.x + IDENX) - 16, (C.Cola[i]->info.y - (IDENY)) - 32, metro, 3);
        setvisualpage(v);
        v = !v;
        delay(500);
    }
}
//GUARDA
//lee y guarda la imagen que se va a mover en el programa
void guardaimagen(void **metro, int *imagentam)
{
    readimagefile("metro.jpg", 0, 0, 60, 60); //lee la imagen metro.jpg
    *imagentam = imagesize(0, 0, 60, 60);     //le da el tamaño
    *metro = malloc(*imagentam);              //le asigna un espacio
    getimage(0, 0, 60, 60, *metro);           // la obtiene para usarla despues
}
//CAMINOS
//calcula las rutas del usuario
int calculaCamino(VERTICE *origen, VERTICE *actual, VERTICE *destino, TCola *Camino)
{
    ARISTA *aux;
    enCola(Camino, actual);
    actual->info.visitado = 1; // valor bandera asumimos que no existe
    if (origen->sig)           //si existe
    {
        if (actual == origen && origen->info.e == 0)
            actual->info.visitado = 0;
    }

    actual->info.e = 1; //asignacion valor bandera
    int bandera = 0;

    for (aux = actual->arista; aux; aux = aux->sig)
    {
        if (aux->destino == destino || bandera == 1)
        {
            return (1); //no se pudo
        }

        if (aux->destino->info.visitado != 1)
            bandera = calculaCamino(origen, aux->destino, destino, Camino); //si existe calcula el camino
    }
}
//segun el origen y destino manda a llamar a calcular el camino y encolar
void SeleccionaCamino(VERTICE *Cab, TCola *Camino)
{
    VERTICE *origen = NULL, *destino = NULL;
    int i, c = 0;

    while (!origen && !destino) //mientras no exista origen y no exista destino
    {
        setfillstyle(1, 1);
        origen = buscaNombreVertice(Cab); //busca el origen
        floodfill(origen->info.x + IDENX, origen->info.y - IDENY, BLACK);
        outtextxy((700 - 700 / 2) + textwidth("Da click en un vertice origen: "), 800 - 800 / 5, origen->info.estacion);

        setfillstyle(1, 2);
        outtextxy(700 - 700 / 2, (800 - 800 / 5) + textheight("Origen seleccionado: "), "Da click en un vertice destino: ");
        destino = buscaNombreVertice(Cab); //busca el destino
        floodfill(destino->info.x + IDENX, destino->info.y - IDENY, BLACK);
        outtextxy(700 - 700 / 2, (800 - 800 / 5) + textheight("Origen seleccionado: "), "Destino seleccionado: ");
        outtextxy((700 - 700 / 2) + textwidth("Destino seleccionado: "), (800 - 800 / 5) + textheight("Origen seleccionado: "), destino->info.estacion);
        delay(600);
    }

    calculaCamino(origen, origen, destino, Camino); //manda llamar a la funcion que calcula el camino
    enCola(Camino, destino);                        //mete datos a la cola

    for (i = 0; i < Camino->fin; i++)
    {
        if (Camino->Cola[i] == origen)
            c++;
    }
    if (c == 2)
    {
        c = 0;
        for (i = Camino->ini; c != 2; i++)
        {
            ExtraerCola(Camino);
            if (Camino->Cola[i] == origen)
                c++;
        }
    }
}
//funcioles colas
//inicializa la cabeza de la cola
int iniciaCabeza(TCabeza *Cab)
{
    int b = 0;
    Cab->Cabeza = NULL;

    Cab->Camino.Cola = (VERTICE **)malloc(sizeof(VERTICE *) * 32);
    if (Cab->Camino.Cola)
    {
        Cab->Camino.ini = Cab->Camino.fin = 0;
        Cab->Camino.tam = 32;
        b++;
    }
    return (b);
}
//Si la cola esta vacia
int ColaVacia(TCola C)
{
    if (C.ini == C.fin)
        return (1);
    return (0);
}
//si la cola esta llena
int Colallena(TCola C)
{
    if ((C.fin + 1) % C.tam == C.ini)
        return (1);
    return (0);
}
//reinicia la cola para poder obtener nuevos valores despues en el programa
void reiniciaCola(TCola *C, VERTICE *Cab)
{
    int i;
    VERTICE *aux;
    for (aux = Cab; aux; aux = aux->sig)
    {
        aux->info.e = aux->info.visitado = 0;
    }
    C->ini = C->fin = 0;
}
//mete datos a la cola
int enCola(TCola *Camino, VERTICE *dir)
{
    if (Colallena(*Camino) == 1)
        return (0);
    Camino->Cola[Camino->fin] = dir;
    Camino->fin++;
    if (Camino->fin == Camino->tam)
    {
        Camino->fin = 0;
    }
    return (1);
}
//saca datos de la cola
int ExtraerCola(TCola *C)
{
    if (ColaVacia(*C) == 1)
        return (0);
    C->ini = (C->ini + 1) % C->tam;
    return (1);
}
