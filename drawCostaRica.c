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
BORDES *SJO;
BORDES *_ER;
BORDES *CAR;
BORDES *GUA;
BORDES *LIM;
BORDES *PUN;
BORDES *PUN2;

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


void clasificar_provincias(){
	rewind(COORD);
	int x,y;
	BORDES* nuevo;
	BORDES *provincias[8] = {ALA,SJO,_ER,CAR,GUA,LIM,PUN,PUN2};
	int iter = 0;
	nuevo = crear_provinvias(provincias[iter]);
	int i = 0;
    while(!feof(COORD)){
    	fscanf(COORD, "%d,%d", &x, &y);
    	if(x == 0 && y == 0 && iter<8){
    		fscanf(COORD, "%d,%d", &x, &y);
    		i = 0;
    		provincias[iter] = nuevo;
    		iter++;		
    		nuevo = crear_provinvias(provincias[iter]);
    	}if(iter<8){
    		nuevo[i].x = x;
    		nuevo[i].y = y;
    		//printf("\n Coord x = %d, Coord y = %d",nuevo[i].x, nuevo[i].y);
    		i++;
    	}	
    }
    ALA = provincias[0];
    //printf("\n ALA Coord x = %d, Coord y = %d",provincias[0][0].x, provincias[0][0].y);
    SJO = provincias[1];
    //printf("\n SJO Coord x = %d, Coord y = %d",provincias[1][0].x, provincias[1][0].y);
    _ER = provincias[2];
    //printf("\n HER Coord x = %d, Coord y = %d",provincias[2][0].x, provincias[2][0].y);
    CAR = provincias[3];
    //printf("\n CAR Coord x = %d, Coord y = %d",provincias[3][0].x, provincias[3][0].y);
    GUA = provincias[4];
    //printf("\n GUA Coord x = %d, Coord y = %d",provincias[4][0].x, provincias[4][0].y);
    LIM = provincias[5];
    //printf("\n LIM Coord x = %d, Coord y = %d",provincias[5][0].x, provincias[5][0].y);
    PUN = provincias[6];
    //printf("\n PUN Coord x = %d, Coord y = %d",provincias[6][0].x, provincias[6][0].y);
    PUN2 = provincias[7];
    //printf("\n PUN2 Coord x = %d, Coord y = %d",provincias[7][0].x, provincias[7][0].y);

}

BORDES* crear_provinvias(BORDES *b){
	//printf("\nCreo una provincia");
  	b = (BORDES *)malloc(PROV_MAX_COORDS * sizeof(BORDES));
  	int i = 0;
    for (i = 0; i < PROV_MAX_COORDS; i++){
    	b[i].x = 0;
    	//printf("\n Coord x = %d",b[i].x);
    	b[i].y = 0;
    	//printf("\n Coord y = %d",b[i].y);
    }
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

void dibujar_escena(){
	//plot_line(50,200,200,50);
	pintar_bordes_mapa();
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


void plot(int x, int y, int r, int g, int b){

	//printf("Resultado = %d , %d\n", x, y);
    buffer[x][y].r = r;
    buffer[x][y].g = g;
    buffer[x][y].b = b;
}

void plot_line (int x0, int y0, int x1, int y1) //Bresenham
{
	printf("\n PINTA Coord x = %d, Coord y = %d & Coord x = %d, Coord y = %d",x0, y0, x1, y1);
  	int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  	int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  	int err = dx + dy, e2; /* error value e_xy */
	int loop = 1; 
  	while(loop == 1){  /* loop */
		printf("\n PINTA el FB Coord x = %d, Coord y = %d",x0, y0);
    	plot(x0,y0,0,0,0);
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
		int x1,y1,x_old,y_old,iter;
		iter = 0;
		x_old = provincias[i][iter].x;
		printf("\n ENTRA Coord x = %d, Coord y = %d",provincias[i][iter].x, provincias[i][iter].y);
		y_old = provincias[i][iter].y;
		iter++;
		//fscanf(COORD, "%d,%d", &x_old, &y_old);
		while(provincias[i][iter].x != 0 && provincias[i][iter].y != 0){
			x1 = provincias[i][iter].x;
			y1 = provincias[i][iter].y;
			plot_line(x_old,y_old,x1,y1);
			//printf("\n INI Coord x = %d, Coord y = %d",x1, y1);
			iter++;
			if(provincias[i][iter].x != 0 && provincias[i][iter].y != 0){
				x_old = x1; y_old = y1;
				x1 = provincias[i][iter].x;
				y1 = provincias[i][iter].y;
				plot_line(x_old,y_old,x1,y1);
				iter++;
				//printf("\n INI2 Coord x = %d, Coord y = %d",x1, y1);
				if(provincias[i][iter].x != 0 && provincias[i][iter].y != 0){	
					x_old = x1; y_old = y1;	
				}else{
					plot_line(x_old,y_old,x1,y1);
				}
			}else{
				plot_line(x_old,y_old,x1,y1);
			}

	    }
	    printf("\n SALI Coord x = %d, Coord y = %d",provincias[i][iter-1].x, provincias[i][iter-1].y);
	}
    
}




