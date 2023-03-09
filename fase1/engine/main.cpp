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


float alfa = 0.0f, beta = 90.0f, radius = 5.0f;
float pos_x, pos_y, pos_z,
lookat_x = 0.0f, lookat_y = 0.0f, lookat_z = 0.0f,
up_x = 0.0f, up_y = 1.0f, up_z = 0.0f;

float width = 800, height = 800;


void spherical2Cartesian() {
	pos_x = radius * cos(beta) * sin(alfa);
	pos_y = radius * sin(beta);
	pos_z = radius * cos(beta) * cos(alfa);
}

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


struct Figura {
	std::string nome_ficheiro_3d;
	std::vector<Ponto> pontos;

	Figura(std::string nick, std::vector<Ponto> pontos_aux) {
		this->nome_ficheiro_3d = nick;
		this->pontos = pontos_aux;
	}

};



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


struct WorldData {
	WindowData window;
	CameraData camera;
};


std::vector<Figura> figuras;

std::vector<Ponto> armazena_pontos(std::string nome_fich) {


	std::string path = "..\\..\\3d\\";
	path += nome_fich;
	std::vector<Ponto> res;


	std::ifstream fich(path);
	if (fich.is_open()) {
		float x, y, z;
		while (fich >> x >> y >> z) {
			res.push_back(Ponto(x, y, z));
		}
		fich.close();
	}
	else {
		std::cout << "Não foi possível abrir o ficheiro " << nome_fich << "\n";
	}

	return res;

}

void parse_xml(const std::string& teste_xml, WorldData& data) {
	std::string path = "..\\..\\tests\\test_files_phase_1\\";
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
				std::vector<Ponto> aux;
				aux = armazena_pontos(model->Attribute("file"));
				figuras.push_back(Figura(model->Attribute("file"), aux));
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


}



void renderScene(void)
{

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa  a cor

	// set camera
	glLoadIdentity();
	gluLookAt(pos_x, pos_y, pos_z,
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
	glColor3f(1.0f, 1.0f, 1.0f);
	for (Figura fig : figuras) {
		for (Ponto p : fig.pontos) {
			glVertex3f(p.x, p.y, p.z);
		}
	}
	glEnd();


	//--------------------------------------






	// End of frame
	glutSwapBuffers();

}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_RIGHT:
		alfa -= 0.1; 
		break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; 
		break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP:
		radius += 0.1f;
		break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}

int main(int argc, char** argv)
{

	std::ifstream fich("..\\..\\tests\\test_files_phase_1\\testar_xml.txt", std::ios::in);
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


	glutSpecialFunc(processSpecialKeys);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT’s main cycle
	glutMainLoop(); //ciclo do glut; enquanto a janela nao for fechada chama o renderScene e processa eventos


	return 1;
}