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

float fov = 45.0, near = 1.0f, far = 1000.0f;


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
	gluPerspective(fov, ratio, near, far);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float pos_x = 5.0f, pos_y = 5.0f, pos_z = 5.0f,
      lookat_x = 0.0f, lookat_y = 0.0f, lookat_z = 0.0f,
      up_x = 0.0f, up_y = 1.0f, up_z = 0.0f;

float width = 800, height = 800;


struct Ponto {
	float x;
	float y;
	float z;

	//construtor:
	Ponto(float xi, float yi, float zi) {
		this->x = xi;
		this->y = yi;
		this->z = zi;
	}
};


std::vector<std::string> ficheiros_3d;
std::vector<Ponto> pontos;



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
	std::vector<std::string> nomes_ficheiros_3d;
};


struct WorldData {
	WindowData window;
	CameraData camera;
	ModelData model;
};




void parse_xml(const std::string & teste_xml, WorldData & data) {
	std::string path = "C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\tests\\test_files_phase_1\\"; 
	path += teste_xml;
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
			auto model = models->FirstChildElement("model");
			while (model != nullptr) {
				data.model.nomes_ficheiros_3d.push_back(model->Attribute("file"));
				model = model->NextSiblingElement("model");
			}

		}
	}

	pos_x = data.camera.position.x;
	pos_y = data.camera.position.y;
	pos_z = data.camera.position.z;
	lookat_x = data.camera.look_at.x;
	lookat_y = data.camera.look_at.y;
	lookat_z = data.camera.look_at.z;
	up_x = data.camera.up.x;
	up_y = data.camera.up.y;
	up_z = data.camera.up.z;
	fov = data.camera.projection.fov;
	near = data.camera.projection.near;
	far = data.camera.projection.far;
	ficheiros_3d = data.model.nomes_ficheiros_3d;



}
	
void imprime_xml(WorldData world) {
	std::cout << world.window.height << " " << world.window.width << "\n";
	std::cout << world.camera.position.x << " " << world.camera.position.y << " " << world.camera.position.z << "\n";
	std::cout << world.camera.look_at.x << " " << world.camera.look_at.y << " " << world.camera.look_at.z << "\n";
	std::cout << world.camera.up.x << " " << world.camera.up.y << " " << world.camera.up.z << "\n";
	std::cout << world.camera.projection.far << " " << world.camera.projection.near << " " << world.camera.projection.fov << "\n";
	for (std::string fich: world.model.nomes_ficheiros_3d) {
		std::cout << fich << " ";
	}

}

void le_pontos(std::vector<std::string> ficheiros_3d) {
	
	std::string path = "C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\3d\\";
	

	for (std::string s_fich : ficheiros_3d) {
		std::ifstream fich(path+=s_fich);
		path = "C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\3d\\";
		if (fich.is_open()) {
			float x, y, z;
			while (fich >> x >> y >> z) {
				pontos.push_back(Ponto(x, y, z));
			}
			fich.close();
		}
		else {
			std::cout << "Não foi possível abrir o ficheiro " << s_fich << "\n";
		}


	}
}


void imprime_pontos(std::vector<Ponto> pontos) {

	for (Ponto p : pontos) {
		std::cout << p.x << " " << p.y << " " << p.z << "\n";
	}
}


	

void renderScene(void)
{

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa  a cor

	// set camera
	glLoadIdentity();
	gluLookAt(5, 5, 5,//pos_x, pos_y, pos_z, //esquerda e direita, roda para cima e para baixo; onde a camera esta
		lookat_x, lookat_y, lookat_z,
		up_x, up_y, up_z);

	glPolygonMode(GL_FRONT, GL_LINE);

	// put drawing instructions here
	//eixos
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glEnd();

	//pontos
	glBegin(GL_TRIANGLES);
	for (Ponto p : pontos) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();

	
	//--------------------------------------
	
	




	// End of frame
	glutSwapBuffers();

}


int main(int argc, char** argv)
{   
	std::ifstream fich("C:\\Users\\Utilizador\\Desktop\\CG-Projeto\\fase1\\tests\\test_files_phase_1\\testar_xml.txt", std::ios::in);
	std::string teste_xml;
	WorldData world;

	if (fich.is_open()) {
		std::getline(fich, teste_xml);
		fich.close();
	}
	else {
		std::cout << "Não foi possível abrir o ficheiro testar_xml.txt.\n";
	}


	parse_xml(teste_xml, world);
	//imprime_xml(world);
	le_pontos(ficheiros_3d);
	//imprime_pontos(pontos);
	



	
	
	// put GLUT’s init here
	glutInit(&argc, argv); //iniciar o glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //definir como queremos a janela
	glutInitWindowPosition(100, 100); //posiçao da janela
	glutInitWindowSize(width, height); //tamanho janela
	glutCreateWindow("CG@TP"); //cria a janela com o nome
	
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