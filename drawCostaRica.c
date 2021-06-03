/*
 * Instituto Tecnologico de Costa Rica
 * Escuela de Ingenieria en Computacion
 * Computer Graphics
 *
 * Programa: Mesa Example
 * Archivo:  mesa_example.c
 */
#include "drawCostaRica.h"
#include "malloc.h"

COLOR **buffer;
BORDES *ALA;
BORDES_PINTADOS* ALA_P;
BORDES *SJO;
BORDES_PINTADOS* SJO_P;
BORDES *_ER;
BORDES_PINTADOS* _ER_P;
BORDES *CAR;
BORDES_PINTADOS* CAR_P;
BORDES *GUA;
BORDES_PINTADOS* GUA_P;
BORDES *LIM;
BORDES_PINTADOS* LIM_P;
BORDES *PUN;
BORDES_PINTADOS* PUN_P;
BORDES *PUN2;
BORDES_PINTADOS* PUN2_P;

struct ACTIVO* primero = NULL;
struct ACTIVO* ultimo = NULL;
int bordes_activos = 0;

FILE *COORD;



int main(int argc, char** argv){
	COORD=fopen ("coordenadas.txt","r");
    if ( COORD == NULL )
    {
        printf("No se puede abrir archivo") ;
        return 1;
    }
	crear_buffer();
	clasificar_provincias();
	//dibujar_escena();
	//dibujar_escena();
  	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  	glutInitWindowSize(H_SIZE,V_SIZE);
  	glutCreateWindow("Draw CostaRica");
  	glClear(GL_COLOR_BUFFER_BIT);
  	gluOrtho2D(-0.5, H_SIZE +0.5, -0.5, V_SIZE + 0.5);
  	glutDisplayFunc(dibujar_escena);
  	glutMainLoop();
  	fclose(COORD);
}

void dibujar_escena(){
  pintar_provincia(0,0,0,255);
  pintar_provincia(2,0,255,0);
  //pintar_provincia(1,255,0,255);
  pintar_provincia(3,255,0,0);
  pintar_provincia(4,0,255,140);
  pintar_provincia(5,0,255,100);
  pintar_provincia(6,200,255,40);
  pintar_provincia(7,255,0,0);
  pintar_bordes_mapa();
  //plot_line(385,49,418,58,255,0,0);
  //pintar_puntos_mapa();
  //actualizar_buffer();
  int i , j;
  for (i = 0; i < H_SIZE; i++){
    for (j = 0; j < V_SIZE; j++){
      glBegin (GL_POINTS);
      glColor3f (buffer[i][j].r,buffer[i][j].g,buffer[i][j].b);  
        glVertex2i (i,j);
        glEnd();  
          
        }
  }
  glFlush();
}

int buscar_por_id(int id){
   struct ACTIVO *temp;
   temp = primero;

    while(temp != NULL)
    {
      if(temp->id == id)
      {
        return 0;
      }
      temp = temp->siguiente;
    }
    return 1;

}

void insertar_activo(int max, int min, double pendiente, int x1, int y1, int id)
{
   struct ACTIVO *nuevo;
   nuevo = malloc(sizeof(struct ACTIVO));
   nuevo->max_y = max;
   nuevo->min_y = min;
   nuevo->x1 = x1;
   nuevo->y1 = y1;
   nuevo->id = id;
   nuevo->m = pendiente;
   nuevo->siguiente=NULL;
   nuevo->anterior=NULL;
   if(primero == NULL){
      primero = nuevo;
      ultimo = nuevo;
      bordes_activos++;
      //printf("\n PRIMERO ACTIVO CREADO: X1 = %d, Y1 = %d, X2 = %d, Y2 = %d", primero->contenido.x1, primero->contenido.y1, primero->contenido.x2, primero->contenido.y2);
   }else{
      if(buscar_por_id(id) == 0){
        //printf("\n No se ingreso un activo, debido a que es igual que el ultimo");
      }else{
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        ultimo = nuevo;
        bordes_activos++;
      }

      //printf("\n ULTIMO ACTIVO CREADO: X1 = %d, Y1 = %d, X2 = %d, Y2 = %d", ultimo->contenido.x1, ultimo->contenido.y1, ultimo->contenido.x2, ultimo->contenido.y2);
   }

}

void eliminar_activo(int limite){
   struct ACTIVO *temp;
   temp = primero;

    while(temp != NULL)
    {
        if(temp->min_y == limite)
        {
            if(temp == primero)
            {
               //printf("\nSE BORRA EL PRIMERO");
               primero->siguiente->anterior = NULL;
               primero = primero->siguiente;
               bordes_activos--;
            }
            else if(temp == ultimo)
            {
               //printf("\nSE BORRA EL ULTIMO");
               ultimo->anterior->siguiente = NULL;
               ultimo = ultimo->anterior;
               bordes_activos--;
            }
            else
            {
               //printf("\nSE BORRA EL DEL MEDIO");
               temp->anterior->siguiente = temp->siguiente;
               temp->siguiente->anterior = temp->anterior;
               bordes_activos--;

            }
        }
        temp = temp->siguiente;
    }

}
void pintar_provincia(int indice, int r, int g, int b){
  primero = NULL;
  ultimo = NULL;
  bordes_activos = 0;
  scanline(indice,r,g,b);
}

int minimo_activo(){
   struct ACTIVO *temp;
   temp = primero;
   int resp = 0;
    while(temp != NULL)
    {
      if(temp->min_y > resp){
        resp = temp->min_y;
      }  
      temp = temp->siguiente;
    }
    //printf("\n Nuevo Minimo mas grande = %d", resp);
    return resp;

}
void activarBordes(int tamano,BORDES *bordes_array, int max_y){
      int i;
      for(i = 0; i < tamano; i++){ //CICLO PARA ACTIVAR BORDES
      //printf("\nBuscando = %d", i);
      //printf("\nComparando el max del borde = %d con el scanline =  %d", provincia_iterada->bordes_array[i].max_y,max_y);
        if(bordes_array[i].max_y == max_y){ //SOLO SI EL MAXIMO LOCAL DE CADA BORDE ES IGUAL AL SCANLINE SE ACTIVA
          //printf("\nEncontre un activo con ymax = %d y  con ymin = %d", max_y,bordes_array[i].min_y);
          double x1 = (double)bordes_array[i].x1;
          double y1 = (double)bordes_array[i].y1;
          double x2 = (double)bordes_array[i].x2;
          double y2 = (double)bordes_array[i].y2;
          double m;
          if(x1 != x2){
            m = (double)((y2 - y1) / (x2 - x1));
          }else{
            m = 0;
          }
          int id = (int) x1 + y1 + x2 + y2 + m;//un ID unico para poder identificarlos
          //printf("\nPENDIENTE = %f", m);
          insertar_activo(bordes_array[i].max_y,bordes_array[i].min_y, m, x1, y1, id);
        }
    }
    //printf("\nBORDES ACTIVOS ITERADOS= %d", bordes_activos);
}

void scanline(int index, int r, int g, int b){
  BORDES_PINTADOS *provincias[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
  BORDES_PINTADOS *provincia_iterada = provincias[index];
  int max_y = provincia_iterada->yMAX;
  int min_y = provincia_iterada->yMIN;
  int tamano = provincia_iterada->cant;
  //printf("\nMAX Y = %d,MIN Y = %d, TAMANO = %d", max_y,min_y,tamano);
  while(max_y >= min_y){//ESTE ES EL CICLO PRINCIPAL DEL METODO
    //printf("\nMAX Y = %d", max_y);
    int minimo_posible = 0;
    activarBordes(tamano,provincia_iterada->bordes_array,max_y);
    //printf("\nBORDES ACTIVOS = %d", bordes_activos);
    if(bordes_activos >= 2){ //SOLO SI HAY BORDES ACTIVADOS, SE REALIZA LO SIGUIENTE
      minimo_posible = minimo_activo(); //BUSCA DE ENTRE LOS BORDES ACTIVOS AL MINIMO POSIBLE
      int scanline = max_y;
      while(minimo_posible <= scanline){
        activarBordes(tamano,provincia_iterada->bordes_array,scanline);
        minimo_posible = minimo_activo();
        //printf("\nBORDES ACTIVOS = %d", bordes_activos);
        ACTIVO *temp = primero;
        int intersecciones[1000];
        int iter_intersecciones = 0;
        while(temp != NULL){ //POR CADA BORDE, BUSCA LA INTERSECCION PARA CADA SCANLINE
          int x;
          if(temp->m != 0){
            x = (int)( (scanline - (temp->y1 - temp->m * temp->x1)) / (temp->m) ); //CALCULO DE LA INTERSECCION
          }else{
            x = temp->x1;
          }      
          intersecciones[iter_intersecciones] = x;
          temp = temp->siguiente;
          iter_intersecciones++;
        }
        int temporal;
        int i;
        int j;
        for(i = 1;i < iter_intersecciones;i++){ //ORDENA LAS INTERSECCIONES DE IZQUIERDA A DERECHA
         temporal = intersecciones[i];
            j = i - 1;
            while((temporal < intersecciones[j])&&(j >= 0)){
               intersecciones[j+1] = intersecciones[j];
               j = j - 1;
            }
            intersecciones[j+1] = temporal;
        }
        int tamano_intersecciones = 0;
        int intersecciones_local[100];
        for(i = 0;i < iter_intersecciones;i++){ //ESTE CICLO SOLO IMPRIME Y PLOT-EA LAS INTERSECCIONES EN EL FB 
          if(i+1<iter_intersecciones){
            if(intersecciones[i]!=intersecciones[i+1]){
              intersecciones_local[tamano_intersecciones] = intersecciones[i];
              tamano_intersecciones++;
              //printf("\n INTERSECCION ORDENADA. COORD X = %d , COORD Y = %d, MINIMO Y POSIBLE  = %d", intersecciones[i], scanline,minimo_posible);
            }
          }else{
            intersecciones_local[tamano_intersecciones] = intersecciones[i];
            tamano_intersecciones++;
            //printf("\n INTERSECCION ORDENADA. COORD X = %d , COORD Y = %d, MINIMO Y POSIBLE  = %d", intersecciones[i], scanline,minimo_posible);
          }
        }
        //printf("\n Tamano Intersecciones = %d", tamano_intersecciones);
        if (tamano_intersecciones == 1){
          plot(intersecciones_local[0],scanline,r,g,b);
        }else{
          int odd_parity = 0; //O PAR = PINTA. 1 INPAR = NO PINTA
          int index = 0;
          while(index < tamano_intersecciones){
            if(odd_parity == 0 && index+1 < tamano_intersecciones){
              plot_line(intersecciones_local[index],scanline,intersecciones_local[index+1],scanline,r,g,b);
              odd_parity = 1;
            }else if(odd_parity == 1 && index+1 < tamano_intersecciones){
              odd_parity = 0;
            }
            index++;
          }
        }
        //printf("\n TERMINA SCANLINE");
        scanline--;
      }
      max_y = scanline+1;
      if(max_y==min_y){
        break;
      }else{
        eliminar_activo(minimo_posible);
        //printf("\nBORDES ACTIVOS DESPUES DE BORRADO = %d", bordes_activos);
      }
    }
    max_y--;
  }
    
}



void clasificar_provincias(){
	rewind(COORD);
   BORDES* nuevo_aux;
	BORDES* nuevo;
	BORDES_PINTADOS* nuevo_pintado;
	BORDES* provincias[8] = {ALA,SJO,_ER,CAR,GUA,LIM,PUN,PUN2};
	BORDES_PINTADOS* provincias_pintadas[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
	int iter = 0;
  int tamano_real = 0;
	int i = 0;
  int y_max = 0;
  int y_min = 1500;
  int x,y,x2,y2;
  fscanf(COORD, "%d,%d", &x, &y);
  nuevo = crear_provinvias();
  nuevo_aux = crear_provinvias();
  nuevo_pintado = crear_provinvias_pintadas();
  while(!feof(COORD) && iter < 8){
    if(x != 0 && y != 0){
      fscanf(COORD, "%d,%d", &x2, &y2);
      if(x2 != 0 && y2 != 0){
        nuevo[i].x1 = x;
        nuevo[i].y1 = y;
        nuevo[i].x2 = x2;
        nuevo[i].y2 = y2;
        nuevo_aux[i].x1 = x;
        nuevo_aux[i].y1 = y;
        nuevo_aux[i].x2 = x2;
        nuevo_aux[i].y2 = y2;
        //plot_line(x,y,x2,y2,0,0,0);
        //printf("\n PROVIN = X1 = %d, Y1 = %d, X2 = %d, Y2 = %d ", x, y, x2, y2);
        if(y > y2){
          nuevo[i].max_y = y;
          nuevo[i].min_y = y2;
          nuevo_aux[i].max_y = y;
          nuevo_aux[i].min_y = y2;
          if(y > y_max){
            y_max = y;
          }
          if(y2 < y_min){
            y_min = y2;
          }
        }else{
          if(y2 > y_max){
            y_max = y2;
          }
          if(y < y_min){
            y_min = y;
          }
          nuevo[i].max_y = y2;
          nuevo[i].min_y = y;
          nuevo_aux[i].max_y = y2;
          nuevo_aux[i].min_y = y;
        }

        x = x2;
        y = y2;
        tamano_real++;
        i++;
      }else if(x2 == 0 && y2 == 0){
        fscanf(COORD, "%d,%d", &x, &y);
        i = 0;
        provincias[iter] = nuevo;
        //printf("\n PROVIN before = X1 = %d, Y1 = %d, X2 = %d, Y2 = %d ", provincias[0][0].x1, provincias[0][0].y1, provincias[0][0].x2, provincias[0][0].y2);
        //printf("\n Ultimo PROVIN = X1 = %d, Y1 = %d, X2 = %d, Y2 = %d ", provincias[iter][tamano_real-1].x1, provincias[iter][tamano_real-1].y1, provincias[iter][tamano_real-1].x2, provincias[iter][tamano_real-1].y2);
        nuevo_pintado->cant = tamano_real;
        //printf("\nMAXIMO/MINIMO DE CADA PROVINCIA: YMAX = %d, YMIN = %d", y_max,y_min);
        nuevo_pintado->yMAX = y_max;
        nuevo_pintado->yMIN = y_min;
        y_max = 0;
        y_min = 1500;
        tamano_real = 0;
        nuevo_pintado = ordenar_Y(nuevo_aux,nuevo_pintado);
        provincias_pintadas[iter] = nuevo_pintado;
        iter++;
        //printf("\n NUEVA PROVINCIA");   
        nuevo = crear_provinvias();
        nuevo_aux = crear_provinvias();
        nuevo_pintado = crear_provinvias_pintadas();
      }
    }
  }	
    
    ALA = provincias[0];
    //printf("\n ALA Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[0][0].x1, provincias[0][0].y1,provincias[0][0].x2, provincias[0][0].y2);
    //printf("\n ALA Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[0][1].x1, provincias[0][1].y1,provincias[0][1].x2, provincias[0][1].y2);
    SJO = provincias[1];
    //printf("\n SJO Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[1][0].x1, provincias[1][0].y1,provincias[1][0].x2, provincias[1][0].y2);
    _ER = provincias[2];
    //printf("\n HER Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[2][0].x1, provincias[2][0].y1,provincias[2][0].x2, provincias[2][0].y2);
    CAR = provincias[3];
    //printf("\n CAR Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[3][0].x1, provincias[3][0].y1,provincias[3][0].x2, provincias[3][0].y2);
    GUA = provincias[4];
    //printf("\n GUA Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[4][0].x1, provincias[4][0].y1,provincias[4][0].x2, provincias[4][0].y2);
    LIM = provincias[5];
    //printf("\n LIM Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[5][0].x1, provincias[5][0].y1,provincias[5][0].x2, provincias[5][0].y2);
    PUN = provincias[6];
    //printf("\n PUN Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[6][0].x1, provincias[6][0].y1,provincias[6][0].x2, provincias[6][0].y2);
    PUN2 = provincias[7];
    //printf("\n PUN2 Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[7][0].x1, provincias[7][0].y1,provincias[7][0].x2, provincias[7][0].y2);
    //printf("\n PUN2 Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[7][21].x1, provincias[7][21].y1,provincias[7][21].x2, provincias[7][21].y2);
    ALA_P = provincias_pintadas[0];
    //printf("\n ALA Coord1:  x = %d, y = %d - Coord2:  x = %d, y = %d ",provincias[0][1].x1, provincias[0][1].y1,provincias[0][1].x2, provincias[0][1].y2);
    _ER_P = provincias_pintadas[2];
    CAR_P = provincias_pintadas[3];
	  GUA_P = provincias_pintadas[4];
    LIM_P = provincias_pintadas[5];
    PUN_P = provincias_pintadas[6];
    PUN2_P = provincias_pintadas[7];
}

BORDES_PINTADOS* ordenar_Y(BORDES *bordes, BORDES_PINTADOS *bP){
   BORDES *b = bordes;
  //printf("\n NUEVA PROVINCIA");
  int tamano = bP->cant;
  //printf("\nTamano: %d",tamano );
  int a;
  BORDES temp;
  int i, j;
  for(i = 1;i < tamano;i++){
      temp = b[i];
      j = i - 1;
      while((temp.max_y > b[j].max_y)&&(j >= 0)){
         b[j+1] = b[j];
         j = j - 1;
      }
      b[j+1] = temp;
   }
   for(j = 0;j < tamano;j++){
    //printf("\n  ORDENADA: Coord x1 = %d, Coord y1 = %d, Coord x2 = %d, Coord y2 = %d, MAX Y = %d",b[j].x1,b[j].y1,b[j].x2,b[j].y2, b[j].max_y);
   }
  bP->bordes_array = b;
  return bP;

}

BORDES* crear_provinvias(){
	//printf("\nCreo una provincia");
    BORDES *b;
  	b = (BORDES *)malloc(PROV_MAX_COORDS * sizeof(BORDES));
  	int i = 0;
    for (i = 0; i < PROV_MAX_COORDS; i++){
    	b[i].x1 = 0;
    	b[i].y1 = 0;
      b[i].x2 = 0;
      b[i].y2 = 0;
      b[i].max_y = 0;
      b[i].min_y = 0;
    	//printf("\n Coord y = %d",b[i].y);
    }
    return b;
}

BORDES_PINTADOS* crear_provinvias_pintadas(){
	//printf("\nCreo una provincia");
	  BORDES* nuevo = (BORDES *)malloc(PROV_MAX_COORDS * sizeof(BORDES));
    BORDES_PINTADOS* b;
  	b = (BORDES_PINTADOS *)malloc(sizeof(BORDES_PINTADOS));
  	b->bordes_array = nuevo;
  	int i;
  	for (i = 0; i < PROV_MAX_COORDS; i++){
    	b->bordes_array[i].x1 = 0;
    	//printf("\n Coord x = %d",b->bordes_array[i].x);
    	b->bordes_array[i].y1 = 0;
      b->bordes_array[i].x2 = 0;
      //printf("\n Coord x = %d",b->bordes_array[i].x);
      b->bordes_array[i].y2 = 0;
    	//printf("\n Coord y = %d",b->bordes_array[i].y);
    }
    b->esta_pintado = 0;
  	b->cant = 0;
  	b->yMAX = 0;
  	b->yMIN = 0;
    return b;
}

void crear_buffer(){

	int i, j;

  	buffer = (COLOR **)malloc(H_SIZE * sizeof(COLOR*));
  	for (i = 0; i < H_SIZE; i++){
    	buffer[i] = (COLOR *)malloc(V_SIZE * sizeof(COLOR));
    }
  	for (i = 0; i < H_SIZE; i++){
    	for (j = 0; j < V_SIZE; j++){
            buffer[i][j].r = 255;
            buffer[i][j].g = 255;
            buffer[i][j].b = 255;
        }
    }
}


void plot(int x, int y, int r, int g, int b){

	//printf("Resultado = %d , %d\n", x, y);
    buffer[x][y].r = r;
    buffer[x][y].g = g;
    buffer[x][y].b = b;
}

void plot_line (int x0, int y0, int x1, int y1, int r, int g, int b) //Bresenham
{
   //printf("\n PINTA Coord x = %d, Coord y = %d & Coord x = %d, Coord y = %d",x0, y0, x1, y1);
  	int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  	int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  	int err = dx + dy, e2; /* error value e_xy */
	int loop = 1; 
  	while(loop == 1){  /* loop */
		//printf("\n PINTA el FB Coord x = %d, Coord y = %d",x0, y0);
    	plot(x0,y0,r,g,b);
    	if (x0 == x1 && y0 == y1) loop = 0;
    	e2 = 2 * err;
    	if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    	if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  	}
}

void pintar_puntos_mapa(){
	rewind(COORD);
	int x,y;
	int n = 18;
    while(!feof(COORD)){
    	fscanf(COORD, "%d,%d", &x, &y);
    	if(n>=0){
    		plot(x,y,255,0,255);
    	}else{
    		plot(x,y,0,0,0);
    	}
    	n--;
    	//printf("%d, %d\n",x,y);
    	
    }
    
}

void pintar_bordes_mapa(){
  BORDES *provincias[8] = {ALA,SJO,_ER,CAR,GUA,LIM,PUN,PUN2};
  for(int i = 0; i<8;i++){
    int iter = 0;
    //printf("\n PINTA UNA NUEVA PROVINCIA");
    while(provincias[i][iter].x1 != 0 && provincias[i][iter].y1 != 0 && provincias[i][iter].x2 != 0 && provincias[i][iter].y2 != 0){
      //printf("\n PINTA Coord x = %d, Coord y = %d & Coord x = %d, Coord y = %d",provincias[i][iter].x1, provincias[i][iter].y1, provincias[i][iter].x2, provincias[i][iter].y2);
      plot_line(provincias[i][iter].x1,provincias[i][iter].y1,provincias[i][iter].x2,provincias[i][iter].y2,0,0,0);
      iter++;
    }
  }
    
}
   /*while(max_y >= 900){//SCANLINES EN LA FIGURA
      int i;
      for(i = 0; i < tamano; i++){ //ACTIVAR LOS BORDES
         printf("\n BORDES MAX = %d, CURRENT MAX = %d, BORDES MIN = %d, CURRENT MIN = %d", BP->bordes_array[i].max_y, max_y, BP->bordes_array[i].min_y, min_y);
         if(BP->bordes_array[i].max_y == max_y){//si el y grande de los bordes es IGUAL al scanline
            if(minimo_Y_activo < BP->bordes_array[i].min_y){
               minimo_Y_activo = BP->bordes_array[i].min_y;
            }
            //printf("\n BORDES MAX = %d, CURRENT MAX = %d, BORDES MIN = %d, CURRENT MIN = %d", BP->bordes_array[i].max_y, max_y, BP->bordes_array[i].min_y, min_y);
            cantidad_activados++;
            insertar_activo(BP->bordes_array[i]);

         }
      }
      printf("\n ACTIVADOS AHORA = %d", cantidad_activados);
      struct ACTIVO *temp = primero;
      int puntos_en_x[1000]; //se establece 1000 ya que es imposible que hayan mas de 1000 intersecciones en una sola seccion de la provincia
      int iter = 0;
      printf("\n MINIMO Y ACTIVO = %d ", minimo_Y_activo);
      while(temp != NULL && cantidad_activados > 0){ //por cada uno de los bordes activos
         double x1 = (double)temp->contenido.x1;
         double y1 = (double)temp->contenido.y1;
         double x2 = (double)temp->contenido.x2;
         double y2 = (double)temp->contenido.y2;
         double m = (double)((y2 - y1) / (x2 - x1));
         double b = (double)(y1-(m * x1));
         //printf("\n X1 = %f, Y1 = %f, X2 = %f, Y2 = %f ", x1, y1, x2, y2);
         while(max_y >= minimo_Y_activo){ //comenzando desde el x mayor (max_y) hasta que se alcance el y minimo (min_y) de algun borde
            int nuevo_x = (int)((max_y - b) / m);
            puntos_en_x[iter] = nuevo_x;
            printf("\n NUEVA INTERSECCION = %d", nuevo_x);
            iter++;          
            max_y--;
         }
         temp = temp->siguiente;
      }
      max_y++;
      /*int tem;
      int j;
      for(i = 1;i < iter;i++){ //ordena las intersecciones de izquierda a derecha (coordenada x de menor a mayor)
         tem = puntos_en_x[i];
            j = i - 1;
            while((tem < puntos_en_x[j])&&(j >= 0)){
               puntos_en_x[j+1] = puntos_en_x[j];
               j = j - 1;
            }
            puntos_en_x[j+1] = tem;
      }*/
      /*if (cantidad_activados > 0){
         int a = eliminar_activo(max_y); //desactiva o elimina los bordes que el min_y ya fue alcanzado
         if(a == 0){
            cantidad_activados--;
         }
      }else{
         max_y--;
      }
      minimo_Y_activo = 0;

      // luego se termina con el scanline. 
      // Se recuerda que para la siguiente iteracion del while, el max_y va a aser igual al minimo_Y_activo-1 (el min_y de alguno(s) de los bordes que ya fueron alcanzados)
         
      printf("\n TERMINADO UN SCANLINE %d\n", max_y);

   }*/



