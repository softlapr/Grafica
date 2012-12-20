#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>

using namespace std;

#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)

#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

typedef	GLdouble Vertice[3];
typedef	GLdouble Vector[4];


typedef struct Camera{
	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertice center;

}Camera;

typedef struct Estado{
	Camera		camera;
	int			xMouse,yMouse;
	GLboolean	light;
	GLboolean	apresentaNormais;
	GLint		lightViewer;
	GLint		eixoTranslaccao;
	GLdouble	eixo[3];
}Estado;


typedef struct Modelo {
	GLfloat g_pos_luz1[4];
	GLfloat g_pos_luz2[4];

	GLfloat escala;
	GLUquadric *quad;
}Modelo;

Estado estado;
Modelo modelo;


void initEstado(){
	estado.camera.dir_lat=M_PI/4;
	estado.camera.dir_long=-M_PI/4;
	estado.camera.fov=60;
	estado.camera.dist=100;
	estado.eixo[0]=0;
	estado.eixo[1]=0;
	estado.eixo[2]=0;
	estado.camera.center[0]=0;
	estado.camera.center[1]=0;
	estado.camera.center[2]=0;
	estado.light=GL_FALSE;
	estado.apresentaNormais=GL_FALSE;
	estado.lightViewer=1;
}

void initModelo(){
	modelo.escala=0.2;

	//modelo.cor_cubo = brass;
	modelo.g_pos_luz1[0]=-5.0;
	modelo.g_pos_luz1[1]= 5.0;
	modelo.g_pos_luz1[2]= 5.0;
	modelo.g_pos_luz1[3]= 0.0;
	modelo.g_pos_luz2[0]= 5.0;
	modelo.g_pos_luz2[1]= -15.0;
	modelo.g_pos_luz2[2]= 5.0;
	modelo.g_pos_luz2[3]= 0.0;
}


void myInit()
{

	GLfloat LuzAmbiente[]={0.5,0.5,0.5, 0.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente); 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer); 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 

	initModelo();
	initEstado();
	modelo.quad=gluNewQuadric();
	gluQuadricDrawStyle(modelo.quad, GLU_FILL);
	gluQuadricNormals(modelo.quad, GLU_OUTSIDE);

	//leGrafo();
}


void desenhaSolo(){
#define STEP 10
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		for(int i=-300;i<300;i+=STEP)
			for(int j=-300;j<300;j+=STEP){
				glVertex2f(i,j);
				glVertex2f(i+STEP,j);
				glVertex2f(i+STEP,j+STEP);
				glVertex2f(i,j+STEP);
			}
	glEnd();
}

void desenhaEixo(){
	gluCylinder(modelo.quad,0.5,0.5,20,16,15);
	glPushMatrix();
		glTranslatef(0,0,20);
		glPushMatrix();
			glRotatef(180,0,1,0);
			gluDisk(modelo.quad,0.5,2,16,6);
		glPopMatrix();
		gluCylinder(modelo.quad,2,0,5,16,15);
	glPopMatrix();
}


void desenhaEixos(){

	glPushMatrix();
		glTranslated(estado.eixo[0],estado.eixo[1],estado.eixo[2]);
		//material(emerald);
		glPushName(EIXO_Z);
			desenhaEixo();
		glPopName();
		glPushName(EIXO_Y);
			glPushMatrix();
				glRotatef(-90,1,0,0);
				//material(red_plastic);
				desenhaEixo();
			glPopMatrix();
		glPopName();
		glPushName(EIXO_X);
			glPushMatrix();
				glRotatef(90,0,1,0);
				//material(azul);
				desenhaEixo();
			glPopMatrix();
		glPopName();
	glPopMatrix();
}

void setCamera(){
	Vertice eye;
	eye[0]=estado.camera.center[0]+estado.camera.dist*cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
	eye[1]=estado.camera.center[1]+estado.camera.dist*sin(estado.camera.dir_long)*cos(estado.camera.dir_lat);
	eye[2]=estado.camera.center[2]+estado.camera.dist*sin(estado.camera.dir_lat);

	if(estado.light){
		gluLookAt(eye[0],eye[1],eye[2],estado.camera.center[0],estado.camera.center[1],estado.camera.center[2],0,0,1);
		//putLights((GLfloat*)white_light);
	}else{
		//putLights((GLfloat*)white_light);
		gluLookAt(eye[0],eye[1],eye[2],estado.camera.center[0],estado.camera.center[1],estado.camera.center[2],0,0,1);
	}
}


void keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {
		case 27 :
				exit(0);
			break;
		case 'h':
		case 'H':
				//imprime_ajuda();
			break;
		case 'l':
		case 'L':
				if(estado.lightViewer)
					estado.lightViewer=0;
				else
					estado.lightViewer=1;
				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer);
				glutPostRedisplay();
			break;
		case 'k':
		case 'K':
				estado.light=!estado.light;
				glutPostRedisplay();
			break;
		case 'w':
		case 'W':
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glutPostRedisplay();
			break;
		case 'p':
		case 'P':
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glutPostRedisplay();
			break;
		case 's':
		case 'S':
				glEnable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glutPostRedisplay();
			break;
		case 'c':
		case 'C':
				if(glIsEnabled(GL_CULL_FACE))
					glDisable(GL_CULL_FACE);
				else
					glEnable(GL_CULL_FACE);
				glutPostRedisplay();
			break;    
		case 'n':
		case 'N':
				estado.apresentaNormais=!estado.apresentaNormais;
				glutPostRedisplay();
			break;    		
		case 'i':
		case 'I':
				initEstado();
				initModelo();
				glutPostRedisplay();
			break;    
	}
}

void Special(int key, int x, int y){

	switch(key){
		case GLUT_KEY_F1 :
				//gravaGrafo();
			break;
		case GLUT_KEY_F2 :
				//leGrafo();
				glutPostRedisplay();
			break;	

		/*case GLUT_KEY_F6 :
				numNos=numArcos=0;
				addNo(criaNo( 0, 10,0));  // 0
				addNo(criaNo( 0,  5,0));  // 1
				addNo(criaNo(-5,  5,0));  // 2
				addNo(criaNo( 5,  5,0));  // 3
				addNo(criaNo(-5,  0,0));  // 4
				addNo(criaNo( 5,  0,0));  // 5
				addNo(criaNo(-5, -5,0));  // 6
				addArco(criaArco(0,1,1,1));  // 0 - 1
				addArco(criaArco(1,2,1,1));  // 1 - 2
				addArco(criaArco(1,3,1,1));  // 1 - 3
				addArco(criaArco(2,4,1,1));  // 2 - 4
				addArco(criaArco(3,5,1,1));  // 3 - 5
				addArco(criaArco(4,5,1,1));  // 4 - 5
				addArco(criaArco(4,6,1,1));  // 4 - 6
				glutPostRedisplay();
			break;	*/
		case GLUT_KEY_UP:
				estado.camera.dist-=1;
				glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
				estado.camera.dist+=1;
				glutPostRedisplay();
			break;	
	}
}

void setProjection(int x, int y, GLboolean picking){
    glLoadIdentity();
	if (picking) { // se está no modo picking, lê viewport e define zona de picking
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT)  - y, 4, 4, vport); // Inverte o y do rato para corresponder à jana
	}
	    
	gluPerspective(estado.camera.fov,(GLfloat)glutGet(GLUT_WINDOW_WIDTH) /glutGet(GLUT_WINDOW_HEIGHT) ,1,500);

}


void motionRotate(int x, int y){
#define DRAG_SCALE	0.01
	double lim=M_PI/2-0.1;
	estado.camera.dir_long+=(estado.xMouse-x)*DRAG_SCALE;
	estado.camera.dir_lat-=(estado.yMouse-y)*DRAG_SCALE*0.5;
	if(estado.camera.dir_lat>lim)
		estado.camera.dir_lat=lim;
	else 
		if(estado.camera.dir_lat<-lim)
			estado.camera.dir_lat=-lim;
	estado.xMouse=x;
	estado.yMouse=y;
	glutPostRedisplay();
}

void motionZoom(int x, int y){
#define ZOOM_SCALE	0.5
	estado.camera.dist-=(estado.yMouse-y)*ZOOM_SCALE;
	if(estado.camera.dist<5)
		estado.camera.dist=5;
	else 
		if(estado.camera.dist>200)
			estado.camera.dist=200;
	estado.yMouse=y;
	glutPostRedisplay();
}

int picking(int x, int y){
	int i, n, objid=0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
		glLoadIdentity();
		setProjection(x,y,GL_TRUE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaEixos();
	
	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin > (double) ptr[1] / UINT_MAX) {
				zmin = (double) ptr[1] / UINT_MAX;
				objid = ptr[3];
			}
			ptr += 3 + ptr[0]; // ptr[0] contem o número de nomes (normalmente 1); 3 corresponde a numnomes, zmin e zmax
		}
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return objid;
}


void motionDrag(int x, int y){
	GLuint buffer[100];
	GLint vp[4];
	GLdouble proj[16], mv[16];
	int n;
	GLdouble newx, newy, newz;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
		glLoadIdentity();
		setProjection(x,y,GL_TRUE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	//desenhaPlanoDrag(estado.eixoTranslaccao);
	
	n = glRenderMode(GL_RENDER);
	if (n > 0) {
		glGetIntegerv(GL_VIEWPORT, vp);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, (double) buffer[2] / UINT_MAX, mv, proj, vp, &newx, &newy, &newz);
		printf("Novo x:%lf, y:%lf, z:%lf\n\n", newx, newy, newz);
		switch (estado.eixoTranslaccao) {
			case EIXO_X :
					estado.eixo[0]=newx;
					//estado.eixo[1]=newy;
				break;
			case EIXO_Y :
					estado.eixo[1]=newy;
					//estado.eixo[2]=newz;
				break;
			case EIXO_Z :
					//estado.eixo[0]=newx;
					estado.eixo[2]=newz;
				break;		
		}
		glutPostRedisplay();
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y){
	switch(btn) {
		case GLUT_RIGHT_BUTTON :
					if(state == GLUT_DOWN){
						estado.xMouse=x;
						estado.yMouse=y;
						if(glutGetModifiers() & GLUT_ACTIVE_CTRL)
							glutMotionFunc(motionZoom);
						else
							glutMotionFunc(motionRotate);
						cout << "Left down\n";
					}
					else{
						glutMotionFunc(NULL);
						cout << "Left up\n";
					}
				break;
		case GLUT_LEFT_BUTTON :
					if(state == GLUT_DOWN){
						estado.eixoTranslaccao=picking(x,y);
						if(estado.eixoTranslaccao)
							glutMotionFunc(motionDrag);
						cout << "Right down - objecto:" << estado.eixoTranslaccao << endl;
					}
					else{
						if(estado.eixoTranslaccao!=0) {
							estado.camera.center[0]=estado.eixo[0];
							estado.camera.center[1]=estado.eixo[1];
							estado.camera.center[2]=estado.eixo[2];
							glutMotionFunc(NULL);
							estado.eixoTranslaccao=0;
							glutPostRedisplay();
						}
						cout << "Right up\n";
					}
				break;
	}
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL");
    //glutReshapeFunc(myReshape);
    //glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(mouse);

	myInit();

	//imprime_ajuda();

	// callbacks timer/idle
	glutTimerFunc(estado.lightViewer, Timer, 0);

    glutMainLoop();
	
	return 0;
	
}