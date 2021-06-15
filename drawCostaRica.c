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
INTERSECCION_PROV* ALA_I;INTERSECCION_PROV* SJO_I;INTERSECCION_PROV* _ER_I;INTERSECCION_PROV* CAR_I;INTERSECCION_PROV* GUA_I;INTERSECCION_PROV* LIM_I;INTERSECCION_PROV* PUN_I;INTERSECCION_PROV* PUN2_I;
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
	clasificar_provincias(-500,0,2,2);
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
   rotar_mapa(90);
   scanline(4,255,255,0);
   scanline(5,255,0,0);
   scanline(6,255,0,255);
   scanline(0,0,0,255);
   scanline(1,0,255,0);
   scanline(2,0,255,255);
   scanline(3,0,0,0);
   scanline(7,255,0,255);
    /*dibujar_patron(5);
   dibujar_patron(4);
    dibujar_patron(6);
   dibujar_patron(0);
   dibujar_patron(1);
   dibujar_patron(2);
   dibujar_patron(3);
   dibujar_patron(7);*/
   //pintar_bordes_mapa();
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


void rotar_mapa(double angle){
   double angulo = 0;
   if(angle < 0 && angle >= -360){
      angulo = 360+angle;
   }else if(angle >= 0 && angle <= 360){
      angulo = angle;
   }
   BORDES_PINTADOS* provincias_pintadas[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
   BORDES *provincias[8] = {ALA,SJO,_ER,CAR,GUA,LIM,PUN,PUN2};
   int i;
   for(int i = 0; i<8;i++){
    //printf("\n\nNUEVA PROVINCIA");
      int iter = 0;
      int y_min = 3000;
      int y_max = -3000;
      while(provincias[i][iter].x1 != 0 && provincias[i][iter].y1 != 0 && provincias[i][iter].x2 != 0 && provincias[i][iter].y2 != 0){
         //printf("\n ORIGINAL Coord x1 = %d, Coord y1 = %d & Coord x2 = %d, Coord y2 = %d",provincias[i][iter].x1, provincias[i][iter].y1, provincias[i][iter].x2, provincias[i][iter].y2);
         int x1_shifted = provincias[i][iter].x1 - X_Origen;
         int y1_shifted = provincias[i][iter].y1 - Y_Origen;
         int x2_shifted = provincias[i][iter].x2 - X_Origen;
         int y2_shifted = provincias[i][iter].y2 - Y_Origen;
         //printf("\n SHIFTED Coord x1 = %d, Coord y1 = %d & Coord x2 = %d, Coord y2 = %d",x1_shifted,y1_shifted,x2_shifted,y2_shifted);
         int x1 = (int)X_Origen + (x1_shifted * COS(angulo) - y1_shifted * SIN(angulo));
         int y1 = (int)Y_Origen + (x1_shifted * SIN(angulo) + y1_shifted * COS(angulo));
         int x2 = (int)X_Origen + (x2_shifted * COS(angulo) - y2_shifted * SIN(angulo));
         int y2 = (int)Y_Origen + (x2_shifted * SIN(angulo) + y2_shifted * COS(angulo));
         //printf("\n TRANSFORMA Coord x1 = %d, Coord y1 = %d & Coord x2 = %d, Coord y2 = %d",x1,y1,x2,y2);
         provincias[i][iter].x1 = x1;
         provincias[i][iter].y1 = y1;
         provincias[i][iter].x2 = x2;
         provincias[i][iter].y2 = y2;
        if(y1 > y2){
          provincias[i][iter].max_y = y1;
          provincias[i][iter].min_y = y2;
          if(y1 > y_max){
            y_max = y1;
          }
          if(y2 < y_min){
            y_min = y2;
          }
        }else{
          if(y2 > y_max){
            y_max = y2;
          }
          if(y1 < y_min){
            y_min = y1;
          }
          provincias[i][iter].max_y = y2;
          provincias[i][iter].min_y = y1;
        }
         //plot_line(x1,y1,x2,y2,0,0,0);
         iter++;
      }
      BORDES_PINTADOS* nuevo_pintado = provincias_pintadas[i];
      BORDES* provincia_iterada = provincias[i];
      nuevo_pintado = ordenar_Y(provincia_iterada,nuevo_pintado);
      int j;
        /*for(j = 0;j < nuevo_pintado->cant;j++){
            printf("\n  ORDENADA: Coord x1 = %d, Coord y1 = %d, Coord x2 = %d, Coord y2 = %d, MAX Y = %d",nuevo_pintado->bordes_array[j].x1,nuevo_pintado->bordes_array[j].y1,nuevo_pintado->bordes_array[j].x2,nuevo_pintado->bordes_array[j].y2, nuevo_pintado->bordes_array[j].max_y);
        }*/
      nuevo_pintado->yMAX = y_max;
      nuevo_pintado->yMIN = y_min;
      provincias_pintadas[i] = nuevo_pintado;

  }
   ALA = provincias[0];SJO = provincias[1];_ER = provincias[2];CAR = provincias[3];GUA = provincias[4];LIM = provincias[5];PUN = provincias[6];PUN2 = provincias[7];

   ALA_P = provincias_pintadas[0];SJO_P = provincias_pintadas[1];_ER_P = provincias_pintadas[2];CAR_P = provincias_pintadas[3];GUA_P = provincias_pintadas[4];LIM_P = provincias_pintadas[5];PUN_P = provincias_pintadas[6];PUN2_P = provincias_pintadas[7];
   calcular_intersecciones_scanline();
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

int minimo_activo(){
   struct ACTIVO *temp;
   temp = primero;
   int resp = -3000;
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
void dibujar_patron(int provincia){
    BORDES_PINTADOS *provincias[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
    INTERSECCION_PROV *intersecciones_provincias[8] = {ALA_I,SJO_I,_ER_I,CAR_I,GUA_I,LIM_I,PUN_I,PUN2_I};
    int max_y = provincias[provincia]->yMAX;
    int min_y = provincias[provincia]->yMIN;
    int tamano = abs(max_y - min_y);
    int indice = 0;
    INTERSECCION_PROV *iterada = intersecciones_provincias[provincia];
    while(indice < tamano){
        int inter_cant = iterada[indice].cant;
        //printf("\nCantidad de Intersecciones = %d",inter_cant);
        int index = 0;
        int odd_parity = 1; //1 Pinta, 0 no Pinta
        while(index < inter_cant){
            if(odd_parity==1 && index+1 < inter_cant){
                plot_texture_line(iterada[indice].intersecciones[index].ix,iterada[indice].intersecciones[index].iy,iterada[indice].intersecciones[index+1].ix, iterada[indice].intersecciones[index+1].iy,provincia);
                odd_parity = 0;
            }else{
                odd_parity = 1;
            }
            index++;
        }
        indice++;
    }    
}


void plot_texture_line (int x0, int y0, int x1, int y1, int provincia) //Bresenham pero con RGB de las texturas
{
    int  LDA[TH][TV][3] = {{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},{{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},{{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}}};
    int  CSH[TH][TV][3] = {{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},{{255,255,0},{255,255,0},{255,255,0},{255,255,0},{255,255,0}},{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}},{{255,255,0},{255,255,0},{255,255,0},{255,255,0},{255,255,0}},{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}}};
    int  SAP[TH][TV][3] = {{{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255}},{{128,0,128},{128,0,128},{128,0,128},{128,0,128},{128,0,128}},{{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255}},{{128,0,128},{128,0,128},{128,0,128},{128,0,128},{128,0,128}},{{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255}}};
    int  CSC[TH][TV][3] = {{{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255}},{{0,0,255},{0,0,255},{0,0,255},{0,0,255},{0,0,255}},{{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255}},{{0,0,255},{0,0,255},{0,0,255},{0,0,255},{0,0,255}},{{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255}}};
    int  GUA[TH][TV][3] = {{{0,255,0},{255,255,255},{255,255,255},{255,255,255},{0,255,0}},{{255,255,255},{0,255,0},{255,255,255},{0,255,0},{255,255,255}},{{255,255,255},{255,255,255},{0,255,0},{255,255,255},{255,255,255}},{{255,255,255},{0,255,0},{255,255,255},{255,255,255},{0,255,0}},{{0,255,0},{255,255,255},{255,255,255},{0,255,0},{255,255,255}}};
    int  LFC[TH][TV][3] = {{{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},{{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},{{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}},{{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}},{{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}}};
    int  PFC[TH][TV][3] = {{{255,165,0},{0,0,0},{255,165,0},{0,0,0},{255,165,0}},{{255,165,0},{0,0,0},{255,165,0},{0,0,0},{255,165,0}},{{255,165,0},{0,0,0},{255,165,0},{0,0,0},{255,165,0}},{{255,165,0},{0,0,0},{255,165,0},{0,0,0},{255,165,0}},{{255,165,0},{0,0,0},{255,165,0},{0,0,0},{255,165,0}}};

    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */
    int loop = 1; 
    while(loop == 1){  /* loop */
        //printf("\n PINTA el FB Coord x = %d, Coord y = %d",x0, y0);
        int res[3];
        if(provincia == 0){
            res[0] = LDA[x0 % TH][y0 % TV][0];res[1] = LDA[x0 % TH][y0 % TV][1];res[2] = LDA[x0 % TH][y0 % TV][2];
        }else if(provincia == 1){
            res[0] = SAP[x0 % TH][y0 % TV][0];res[1] = SAP[x0 % TH][y0 % TV][1];res[2] = SAP[x0 % TH][y0 % TV][2];
        }else if(provincia == 2){
            res[0] = CSH[x0 % TH][y0 % TV][0];res[1] = CSH[x0 % TH][y0 % TV][1];res[2] = CSH[x0 % TH][y0 % TV][2];        
        }else if(provincia == 3){
            res[0] = CSC[x0 % TH][y0 % TV][0];res[1] = CSC[x0 % TH][y0 % TV][1];res[2] = CSC[x0 % TH][y0 % TV][2];     
        }else if(provincia == 4){
            res[0] = GUA[x0 % TH][y0 % TV][0];res[1] = GUA[x0 % TH][y0 % TV][1];res[2] = GUA[x0 % TH][y0 % TV][2];     
        }else if(provincia == 5){
            res[0] = LFC[x0 % TH][y0 % TV][0];res[1] = LFC[x0 % TH][y0 % TV][1];res[2] = LFC[x0 % TH][y0 % TV][2];    
        }else{
            res[0] = PFC[x0 % TH][y0 % TV][0];res[1] = PFC[x0 % TH][y0 % TV][1];res[2] = PFC[x0 % TH][y0 % TV][2];    
        }
        
        //printf("\nResultados: x modulo TH = %d y y modulo TV = %d",(x0 % TH),(y0 % TV));
        //printf("\nColores a pintar: r = %d, g = %d, b = %d", res[0],res[1],res[2]);
        plot(x0,y0,res[0],res[1],res[2]);
        if (x0 == x1 && y0 == y1) loop = 0;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void scanline(int provincia, int r, int g, int b){
        BORDES_PINTADOS *provincias[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
        INTERSECCION_PROV *intersecciones_provincias[8] = {ALA_I,SJO_I,_ER_I,CAR_I,GUA_I,LIM_I,PUN_I,PUN2_I};
        int max_y = provincias[provincia]->yMAX;
        int min_y = provincias[provincia]->yMIN;
        int tamano = max_y - min_y;
        int indice = 0;
        INTERSECCION_PROV *iterada = intersecciones_provincias[provincia];
        while(indice < tamano){
            int inter_cant = iterada[indice].cant;
            //printf("\nCantidad de Intersecciones = %d",inter_cant);
            int index = 0;
            int odd_parity = 1; //1 Pinta, 0 no Pinta
            while(index < inter_cant){
                if(odd_parity==1 && index+1 < inter_cant){
                    plot_line(iterada[indice].intersecciones[index].ix,iterada[indice].intersecciones[index].iy,iterada[indice].intersecciones[index+1].ix, iterada[indice].intersecciones[index+1].iy,r,g,b);
                    odd_parity = 0;
                }else{
                    odd_parity = 1;
                }
                index++;
            }
            indice++;
        }
}

void calcular_intersecciones_scanline(){
    int indice;
    BORDES_PINTADOS *provincias[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
    INTERSECCION_PROV *intersecciones_provincias[8] = {ALA_I,SJO_I,_ER_I,CAR_I,GUA_I,LIM_I,PUN_I,PUN2_I};
    for(indice = 0 ; indice < 8; indice++){
        primero = NULL;
        ultimo = NULL;
        bordes_activos = 0;
        BORDES_PINTADOS *provincia_iterada = provincias[indice];
      int max_y = provincia_iterada->yMAX;
      int min_y = provincia_iterada->yMIN;
      int tamano = provincia_iterada->cant;
      int tamano_total = max_y - min_y + 1;
      INTERSECCION_PROV *nuevas_intersecciones = (INTERSECCION_PROV *)malloc(tamano_total * sizeof(INTERSECCION_PROV));
      int tamano_iterado = 0;
      //printf("\nMAX Y = %d,MIN Y = %d, TAMANO = %d", max_y,min_y,tamano_total);
      while(max_y >= min_y){//ESTE ES EL CICLO PRINCIPAL DEL METODO
        //printf("\nMAX Y = %d", max_y);
        int minimo_posible = 0;
        activarBordes(tamano,provincia_iterada->bordes_array,max_y);
        //printf("\nBORDES ACTIVOS = %d", bordes_activos);
        if(bordes_activos >= 2){ //SOLO SI HAY BORDES ACTIVADOS, SE REALIZA LO SIGUIENTE
          minimo_posible = minimo_activo(); //BUSCA DE ENTRE LOS BORDES ACTIVOS AL MINIMO POSIBLE
          //printf("\nMinimo Posible = %d",minimo_posible);
          int scanline = max_y;
          while(minimo_posible <= scanline){
            activarBordes(tamano,provincia_iterada->bordes_array,scanline);
            minimo_posible = minimo_activo();
            //printf("\nMinimo Posible = %d",minimo_posible);
            //printf("\nBORDES ACTIVOS = %d", bordes_activos);
            ACTIVO *temp = primero;
            INTERSECCION* intersec = (INTERSECCION *)malloc(100 * sizeof(INTERSECCION)); //se crea un lista de objetos interseccion
            int iter_intersecciones = 0;
            while(temp != NULL){ //POR CADA BORDE, BUSCA LA INTERSECCION PARA CADA SCANLINE
              int x;
              if(temp->m != 0){
                x = (int)( (scanline - (temp->y1 - temp->m * temp->x1)) / (temp->m) ); //CALCULO DE LA INTERSECCION
              }else{
                x = temp->x1;
              }
              if(scanline != temp->min_y){
                intersec[iter_intersecciones].ix = x;
                intersec[iter_intersecciones].iy = scanline; 
                intersec[iter_intersecciones].bminy = temp->min_y;
                intersec[iter_intersecciones].bmaxy = temp->max_y;
                iter_intersecciones++;
              }
              //printf("\n INTERSECCION. COORD X = %d , COORD Y = %d, MINIMO LOCAL  = %d, MAXIMO LOCAL  = %d", intersec[iter_intersecciones].ix, intersec[iter_intersecciones].iy,intersec[iter_intersecciones].bminy, intersec[iter_intersecciones].bmaxy);  
              temp = temp->siguiente;
              
            }
            INTERSECCION temporal;
            int i;
            int j;
            for(i = 1;i < iter_intersecciones;i++){ //ORDENA LAS INTERSECCIONES DE IZQUIERDA A DERECHA
             temporal = intersec[i];
                j = i - 1;
                while((temporal.ix < intersec[j].ix)&&(j >= 0)){
                   intersec[j+1] = intersec[j];
                   j = j - 1;
                }
                intersec[j+1] = temporal;
             }
             //printf("\n\n NUEVO SCANLINE");
             /*for(i = 0;i < iter_intersecciones; i++){
                printf("\n INTERSECCION ORDENADA. COORD X = %d , COORD Y = %d, MINIMO LOCAL  = %d, MAXIMO LOCAL  = %d", intersec[i].ix, intersec[i].iy, intersec[i].bminy, intersec[i].bmaxy);
             }*/
             //printf("\n I.Originales = %d", iter_intersecciones);
             INTERSECCION* inter_depurada = (INTERSECCION *)malloc(iter_intersecciones * sizeof(INTERSECCION)); //se crea un lista de objetos interseccion
             int index_depurada = 0;
             int index = 0;
             while(index < iter_intersecciones){
                if(index+1 < iter_intersecciones){
                   if(intersec[index].iy == intersec[index+1].iy){
                        if(intersec[index].bminy == intersec[index+1].bmaxy || intersec[index].bmaxy == intersec[index+1].bminy){
                            inter_depurada[index_depurada]=intersec[index];
                            index++;
                            index_depurada++;
                        }else{
                            inter_depurada[index_depurada]=intersec[index];
                            index_depurada++;
                        }
                   }else{
                      inter_depurada[index_depurada]=intersec[index];
                      index_depurada++;
                   }
                }else{
                   inter_depurada[index_depurada]=intersec[index];
                   index_depurada++;
                }
                index++;
             }
             /*for(i = 0;i < index_depurada; i++){
                printf("\n INTERSECCION DEPURADA. COORD X = %d , COORD Y = %d, MINIMO LOCAL  = %d, MAXIMO LOCAL  = %d", inter_depurada[i].ix, inter_depurada[i].iy, inter_depurada[i].bminy, inter_depurada[i].bmaxy);
             }*/
             //printf("\nTAMANO ITERADO = %d", tamano_iterado);
             nuevas_intersecciones[tamano_iterado].intersecciones = inter_depurada;
             nuevas_intersecciones[tamano_iterado].cant = index_depurada;
             tamano_iterado++;

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
      intersecciones_provincias[indice] = nuevas_intersecciones;
      //printf("\n-----------------------SE INSERTO UNA INTERSECCION----------------------------");
      //printf("\nSe inserto nuevas intersecciones, ultimo indice fue = %d", intersecciones_provincias[indice][tamano_total-1][0].ix);        
    }
    ALA_I = intersecciones_provincias[0];SJO_I = intersecciones_provincias[1];_ER_I = intersecciones_provincias[2];CAR_I = intersecciones_provincias[3];GUA_I = intersecciones_provincias[4];LIM_I = intersecciones_provincias[5];PUN_I = intersecciones_provincias[6];PUN2_I= intersecciones_provincias[7];
}

void clasificar_provincias(int panx,int pany, float escalx, float escaly){
	rewind(COORD);
   BORDES* nuevo_aux;
	BORDES* nuevo;
	BORDES_PINTADOS* nuevo_pintado;
	BORDES* provincias[8] = {ALA,SJO,_ER,CAR,GUA,LIM,PUN,PUN2};
	BORDES_PINTADOS* provincias_pintadas[8] = {ALA_P,SJO_P,_ER_P,CAR_P,GUA_P,LIM_P,PUN_P,PUN2_P};
	int iter = 0;
  int tamano_real = 0;
	int i = 0;
  int y_max = -3000;
  int y_min = 3000;
  int x,y,x2,y2;
  fscanf(COORD, "%d,%d", &x, &y);
  x = (int)(x+panx)*escalx; y = (int)(y + pany)*escaly;
  nuevo = crear_provinvias();
  nuevo_aux = crear_provinvias();
  nuevo_pintado = crear_provinvias_pintadas();
  while(!feof(COORD) && iter < 8){
    if((int)((x / escalx) - panx)!= 0 && (int)((y / escaly) - pany) != 0){
      fscanf(COORD, "%d,%d", &x2, &y2);
      x2 = (int)(x2+panx)*escalx; y2 = (int)(y2 + pany)*escaly;
      if((int)((x2 / escalx) - panx) != 0 && (int)((y2 / escaly) - pany) != 0){
        nuevo[i].x1 = x;nuevo[i].y1 = y;nuevo[i].x2 = x2;nuevo[i].y2 = y2;
        nuevo_aux[i].x1 = x;nuevo_aux[i].y1 = y;nuevo_aux[i].x2 = x2;nuevo_aux[i].y2 = y2;
        //plot_line(x,y,x2,y2,0,0,0);
        //printf("\n PROVIN = X1 = %d, Y1 = %d, X2 = %d, Y2 = %d ", x, y, x2, y2);
        if(y > y2){
          nuevo[i].max_y = y;nuevo[i].min_y = y2;
          nuevo_aux[i].max_y = y;nuevo_aux[i].min_y = y2;
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
          nuevo[i].max_y = y2;nuevo[i].min_y = y;
          nuevo_aux[i].max_y = y2;nuevo_aux[i].min_y = y;
        }

        x = x2;y = y2;
        tamano_real++;
        i++;
      }else if((int)((x2 / escalx)- panx) == 0 && (int)((y2 / escaly) - pany) == 0){
        fscanf(COORD, "%d,%d", &x, &y);
        x = (int)(x+panx)*escalx; y = (int)(y + pany)*escaly;
        i = 0;
        provincias[iter] = nuevo;
        nuevo_pintado->cant = tamano_real;nuevo_pintado->yMAX = y_max;nuevo_pintado->yMIN = y_min;
        //printf("\nTamano = %d, yMAX = %d, yMIN = %d", nuevo_pintado->cant,nuevo_pintado->yMAX,nuevo_pintado->yMIN);
        y_max = -3000;
        y_min = 3000;
        tamano_real = 0;
        nuevo_pintado = ordenar_Y(nuevo_aux,nuevo_pintado);
        provincias_pintadas[iter] = nuevo_pintado;
        iter++;  
        nuevo = crear_provinvias();
        nuevo_aux = crear_provinvias();
        nuevo_pintado = crear_provinvias_pintadas();
      }
    }
  }	
    
    ALA = provincias[0];SJO = provincias[1];_ER = provincias[2];CAR = provincias[3];GUA = provincias[4];LIM = provincias[5];PUN = provincias[6];PUN2 = provincias[7];
    ALA_P = provincias_pintadas[0];SJO_P = provincias_pintadas[1];_ER_P = provincias_pintadas[2];CAR_P = provincias_pintadas[3];GUA_P = provincias_pintadas[4];LIM_P = provincias_pintadas[5];PUN_P = provincias_pintadas[6];PUN2_P = provincias_pintadas[7];
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
   /*for(j = 0;j < tamano;j++){
    printf("\n  ORDENADA: Coord x1 = %d, Coord y1 = %d, Coord x2 = %d, Coord y2 = %d, MAX Y = %d",b[j].x1,b[j].y1,b[j].x2,b[j].y2, b[j].max_y);
   }*/
  bP->bordes_array = b;
  return bP;

}

BORDES* crear_provinvias(){
	//printf("\nCreo una provincia");
    BORDES *b;
  	b = (BORDES *)malloc(PROV_MAX_COORDS * sizeof(BORDES));
  	int i = 0;
    for (i = 0; i < PROV_MAX_COORDS; i++){
    	b[i].x1 = 0;b[i].y1 = 0;b[i].x2 = 0;b[i].y2 = 0;b[i].max_y = 0;b[i].min_y = 0;
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
    if(x >= 0 && x < V_SIZE && y >= 0 && y < H_SIZE){
        buffer[x][y].r = r;
        buffer[x][y].g = g;
        buffer[x][y].b = b;
    }
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
