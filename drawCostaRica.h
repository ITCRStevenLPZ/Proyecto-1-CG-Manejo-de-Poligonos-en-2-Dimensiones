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

#define H_SIZE 1050
#define V_SIZE 1050
#define PROV_MAX_COORDS 600

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
  int max_y;
  int min_y;
} BORDES;

typedef struct {
  BORDES *bordes_array; //coordenadas que seran ingresadas en orden de Ymax hacia Ymin a partir de las coordenadas de BORDES
  int esta_pintado;
  int cant;
  int yMAX;
  int yMIN; 
} BORDES_PINTADOS;

typedef struct ACTIVO
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
void dibujar_escena();
void crear_buffer();
void dibujar_escena();
void actualizar_buffer();

