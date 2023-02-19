#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>


void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can’t make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

int i = 1;
int d = 6;

void renderScene(void)
{   
	
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa  a cor
	
	// set camera
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 15.0f, //esquerda e direita, roda para cima e para baixo; onde a camera esta
		0.0f, 0.0f, -1.0f, 
		0.0f, 1.0f, 0.0f);
		
	// put drawing instructions here
	if (i <= 5) {
		glutWireTeapot(i); //fazer a animçao da chavena
		i++;
	}
	else {
		glutWireTeapot(d);
		d--;
		if (d == 0) {
			i = 1;
			d = 6;
		}
	}
	

	
	
	//glutSolidCube(1);
	//glutWireCube(1); //so desenha arestas; parametro dimensao aresta
	//sphere; cone; torus; teapot
	
	// End of frame
	glutSwapBuffers();
	
}


int main(int argc, char** argv)
{   
	
	// put GLUT’s init here
	glutInit(&argc, argv); //iniciar o glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //definir como queremos a janela
	glutInitWindowPosition(100, 100); //posiçao da janela
	glutInitWindowSize(800, 800); //tamanho janela
	glutCreateWindow("CG@DI"); //cria a janela com o nome
	
	// put callback registry here
	glutReshapeFunc(changeSize); //chama uma funçao com 2 parametros width e height (changeSize)
	glutIdleFunc(renderScene);   //chama a funçao renderScene quando puder
	glutDisplayFunc(renderScene); //pintar a janela (argumento é o nome da funçao)
	
	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// enter GLUT’s main cycle
	glutMainLoop(); //ciclo do glut; enquanto a janela nao for fechada chama o renderScene e processa eventos
	
	return 1;
}