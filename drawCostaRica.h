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
  int x;
  int y;
} BORDES;
void clasificar_provincias();
BORDES * crear_provinvias(BORDES * b);
void plot_line (int x0, int y0, int x1, int y1);
void pintar_bordes_mapa();
void pintar_puntos_mapa();
void dibujar_escena();
void crear_buffer();
void dibujar_escena();
void actualizar_buffer();

