/*
 * Instituto Tecnologico de Costa Rica
 * Escuela de Ingenieria en Computacion
 * Computer Graphics
 *
 * Programa: Mesa Example
 * Archivo:  mesa_example.h
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<math.h>  

#define TH 5
#define TV 5
#define H_SIZE 2000
#define V_SIZE 2000
#define PROV_MAX_COORDS 600
#define SIN(x) sin(x * 3.141592653589 / 180)
#define COS(x) cos(x * 3.141592653589 / 180)
#define X_Origen ((V_SIZE/2)-1)
#define Y_Origen ((H_SIZE/2)-1)

typedef struct {
  double r;
  double g;
  double b;
} COLOR;

typedef struct {
  int x1; //coordenadas
  int y1;
  int x2;
  int y2;
  int max_y; //maximo y de las dos coordenadas
  int min_y; //minimo y de las dos coordenadas
} BORDES;

typedef struct {
  BORDES *bordes_array; //coordenadas que seran ingresadas en orden de Ymax hacia Ymin a partir de las coordenadas de BORDES
  int esta_pintado;
  int cant;
  int yMAX; //maximo local
  int yMIN; //minimo local
} BORDES_PINTADOS;

typedef struct ACTIVO //estructura de datos que representa a los bordes que se encuentran activos, y a los cuales se les calcula las intersecciones
{
  int x1; //coordenadas
  int y1;
  double m;
	int max_y;
  int min_y;
  int id;
	struct ACTIVO *siguiente;
	struct ACTIVO *anterior;

}ACTIVO;

typedef struct INTERSECCION //las intersecciones estan compuestas, en este caso, unicamente de 2 bordes
{
  int ix; //coordenada x de la interseccion
  int iy; //coordenada x de la interseccion
  int bminy; //minimo valor de y del borde 1
  int bmaxy; //maximo valor de y del borde 1
  int cant;
}INTERSECCION;

typedef struct INTERSECCION_PROV
{
  struct INTERSECCION* intersecciones;
  int cant;

}INTERSECCION_PROV;

void dibujar_patron(int provincia);
void plot_texture_line (int x0, int y0, int x1, int y1, int index); //Bresenham pero con RGB de las texturas
void pintar_provincia(int indice, int r, int g, int b);
void plot(int x, int y, int r, int g, int b);
void clasificar_provincias();
BORDES_PINTADOS* ordenar_Y(BORDES *b, BORDES_PINTADOS *bP);
BORDES_PINTADOS* crear_provinvias_pintadas();
BORDES * crear_provinvias();
void scanline(int index,int r, int g, int b);
void insertar_activo(int max, int min, double pendiente, int x1, int y1, int id);
void eliminar_activo(int limite);
int minimo_activo();
void plot_line (int x0, int y0, int x1, int y1, int r, int g, int b);
void pintar_bordes_mapa();
void pintar_puntos_mapa();
void crear_buffer();
void dibujar_escena();
void actualizar_buffer();
void rotar_mapa(double angulo);
void calcular_intersecciones_scanline();
