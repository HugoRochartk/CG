#include "tinyxml2.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif




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
	
	glutWireTeapot(2); //fazer a animçao da chavena
	
	

	
	
	//glutSolidCube(1);
	//glutWireCube(1); //so desenha arestas; parametro dimensao aresta
	//sphere; cone; torus; teapot
	
	// End of frame
	glutSwapBuffers();
	
}



struct WindowData {
	int height;
	int width;
};

struct PositionData {
	float x;
	float y;
	float z;
};

struct LookAtData {
	float x;
	float y;
	float z;
};

struct UpData {
	float x;
	float y;
	float z;
};

struct ProjectionData {
	float fov;
	float near;
	float far;
};

struct CameraData {
	PositionData position;
	LookAtData look_at;
	UpData up;
	ProjectionData projection;
};

struct ModelData {
	std::string nomes_ficheiros_3d;
};


struct WorldData {
	WindowData window;
	CameraData camera;
	ModelData model;
};


void parse_xml(const std::string & figura, WorldData & data) {
	std::string path = "C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\tests\\test_files_phase_1\\test_"; //alterar aqui para os testes do stor
	path += figura;
	path += ".xml";

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		std::cerr << "Falha a abrir o ficheiro xml";
		return;
	}

	tinyxml2::XMLElement* world = doc.FirstChildElement("world");
	if (!world) {
		std::cerr << "Falha a encontrar o elemento world";
		return;
	}

		// Read window data
	tinyxml2::XMLElement* window = world->FirstChildElement("window");
	if (window) {
		data.window.height = window->IntAttribute("height");
		data.window.width = window->IntAttribute("width");
	}

		// Read camera data
	tinyxml2::XMLElement* camera = world->FirstChildElement("camera");
	if (camera) {
		tinyxml2::XMLElement* position = camera->FirstChildElement("position");
		if (position) {
			data.camera.position.x = position->FloatAttribute("x");
			data.camera.position.y = position->FloatAttribute("y");
			data.camera.position.z = position->FloatAttribute("z");
		}

		tinyxml2::XMLElement* look_at = camera->FirstChildElement("lookAt");
		if (look_at) {
			data.camera.look_at.x = look_at->FloatAttribute("x");
			data.camera.look_at.y = look_at->FloatAttribute("y");
			data.camera.look_at.z = look_at->FloatAttribute("z");
		}

		tinyxml2::XMLElement* up = camera->FirstChildElement("up");
		if (up) {
			data.camera.up.x = up->FloatAttribute("x");
			data.camera.up.y = up->FloatAttribute("y");
			data.camera.up.z = up->FloatAttribute("z");
		}

		tinyxml2::XMLElement* projection = camera->FirstChildElement("projection");
		if (projection) {
			data.camera.projection.fov = projection->FloatAttribute("fov");
			data.camera.projection.near = projection->FloatAttribute("near");
			data.camera.projection.far = projection->FloatAttribute("far");
		}
	}

		// Read group data
	tinyxml2::XMLElement* group = world->FirstChildElement("group");
	if (group) {
		tinyxml2::XMLElement* models = group->FirstChildElement("models");
		if (models) {
			data.model.nomes_ficheiros_3d = "";
			auto model = models->FirstChildElement("model");
			while (model != nullptr) {
				data.model.nomes_ficheiros_3d += model->Attribute("file");
				data.model.nomes_ficheiros_3d += ",";
				//ler ficheiros split por ,
				model = model->NextSiblingElement("model");
			}

		}
	}
}
	
void imprime_xml(WorldData world) {
	std::cout << world.window.height << " " << world.window.width << "\n";
	std::cout << world.camera.position.x << " " << world.camera.position.y << " " << world.camera.position.z << "\n";
	std::cout << world.camera.look_at.x << " " << world.camera.look_at.y << " " << world.camera.look_at.z << "\n";
	std::cout << world.camera.up.x << " " << world.camera.up.y << " " << world.camera.up.z << "\n";
	std::cout << world.camera.projection.far << " " << world.camera.projection.near << " " << world.camera.projection.fov << "\n";
	std::cout << world.model.nomes_ficheiros_3d << "\n";

}



int main(int argc, char** argv)
{   
	std::ifstream fich("C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\engine\\figura.txt", std::ios::in);
	std::string figura;
	WorldData world;

	if (fich.is_open()) {
		std::getline(fich, figura);
		fich.close();
	}
	else {
		std::cout << "Não foi possível abrir o ficheiro figura.txt.";
	}

	remove("C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\engine\\figura.txt");

	parse_xml(figura, world);
	imprime_xml(world);



	
	
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