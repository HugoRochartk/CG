#include "tinyxml2.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <map>

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


float alfa = 0.0f, beta = 90.0f, radius = 20.0f;
float pos_x, pos_y, pos_z,
lookat_x = 0.0f, lookat_y = 0.0f, lookat_z = 0.0f,
up_x = 0.0f, up_y = 1.0f, up_z = 0.0f;

float width = 800, height = 800;


void spherical2Cartesian() {
	pos_x = radius * cos(beta) * sin(alfa);
	pos_y = radius * sin(beta);
	pos_z = radius * cos(beta) * cos(alfa);
}


struct Transformacao {
	float angulo;
	float x;
	float y;
	float z;
	char flag;


	Transformacao() {
		this->angulo = 0;
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->flag = 'x';
	}


	void Translacao(float xi, float yi, float zi) {
		this->angulo = 0;
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->flag = 't';
	}


	void Escala(float xi, float yi, float zi) {
		this->angulo = 0;
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->flag = 'e';
	}


	void Rotacao(float alpha, float xi, float yi, float zi) {
		this->angulo = alpha;
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->flag = 'r';
	}


	void executa_transf() {
		if (this->flag == 't') {
			glTranslatef(this->x, this->y, this->z);
		}
		else if (this->flag == 'e') {
			glScalef(this->x, this->y, this->z);
		}
		else if (this->flag == 'r') {
			glRotatef(this->angulo, this->x, this->y, this->z);
		}
		else { ; }
	}

};

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


struct Transfs_por_Fig {
	std::vector<Transformacao> transfs;
	std::string nome_ficheiro_3d;
	


	Transfs_por_Fig(std::vector<Transformacao> transfs_aux, std::string nick) {
		this->transfs = transfs_aux;
		this->nome_ficheiro_3d = nick;
	}

};


struct FiguraData {
	std::map<std::string, std::vector<Ponto>> pts_por_fig;


	void Add_to_FiguraData(std::string nick, std::vector<Ponto> pts_aux) {
		if (this->pts_por_fig.find(nick) == this->pts_por_fig.end()) {
			this->pts_por_fig[nick] = pts_aux;
		}
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


std::vector<Transfs_por_Fig> trfs_por_fig;
FiguraData figs;

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
		std::cout << "Não foi possível abrir o ficheiro " << nome_fich << ".\n";
	}

	return res;

}

void parse_group(tinyxml2::XMLElement* g, std::vector<Transformacao> transfs) {
	std::vector<Transformacao> aux = transfs;

	while (g) {
		transfs = aux;
		tinyxml2::XMLElement* transf = g->FirstChildElement("transform");

		if (transf) {
			tinyxml2::XMLElement* tipo_transf = transf->FirstChildElement();
			while(tipo_transf) {
				std::string nome = std::string(tipo_transf->Name());

				if (nome == "translate") {

					float x, y, z;
					x = tipo_transf->FloatAttribute("x");
					y = tipo_transf->FloatAttribute("y");
					z = tipo_transf->FloatAttribute("z");

					Transformacao nt = Transformacao();
					nt.Translacao(x, y, z);
					transfs.push_back(nt);
				}

				else if (nome == "scale") {
					float x, y, z;
					x = tipo_transf->FloatAttribute("x");
					y = tipo_transf->FloatAttribute("y");
					z = tipo_transf->FloatAttribute("z");

					Transformacao nt = Transformacao();
					nt.Escala(x, y, z);
					transfs.push_back(nt);
				}

				else if (nome == "rotate") {
					float x, y, z, angle;
					angle = tipo_transf->FloatAttribute("angle");
					x = tipo_transf->FloatAttribute("x");
					y = tipo_transf->FloatAttribute("y");
					z = tipo_transf->FloatAttribute("z");

					Transformacao nt = Transformacao();
					nt.Rotacao(angle, x, y, z);
					transfs.push_back(nt);
				}
				else {
					;
				}
				tipo_transf = tipo_transf->NextSiblingElement();
			}
		}

		tinyxml2::XMLElement* models = g->FirstChildElement("models");

		if (models) {
			auto model = models->FirstChildElement("model");
			while (model != nullptr) {
				trfs_por_fig.push_back(Transfs_por_Fig(transfs, model->Attribute("file")));
				std::vector<Ponto> aux;
				aux = armazena_pontos(model->Attribute("file"));
				figs.Add_to_FiguraData(model->Attribute("file"), aux);
				model = model->NextSiblingElement("model");
			}

		}
		
		parse_group(g->FirstChildElement("group"), transfs);
		g = g->NextSiblingElement("group");
	}
	
}

void parse_xml(const std::string& teste_xml, WorldData& data) {
	std::string path = "..\\..\\tests\\";
	path += teste_xml;
	path += ".xml";

	if (teste_xml == "sistema_solar") {
		radius = 100.0;
	}

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
		std::cerr << "Falha ao abrir o ficheiro " << teste_xml << ".\n";
		return;
	}

	tinyxml2::XMLElement* world = doc.FirstChildElement("world");
	
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
		std::vector<Transformacao> transfs;
		parse_group(group, transfs);
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
	
	glColor3f(1.0f, 1.0f, 1.0f);
	for (Transfs_por_Fig tpf : trfs_por_fig) {
		std::string nick = tpf.nome_ficheiro_3d;
		glPushMatrix();
		for (Transformacao t : tpf.transfs) {
			t.executa_transf();
		}
		glBegin(GL_TRIANGLES);
		for (Ponto p : figs.pts_por_fig[nick]) {
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		glPopMatrix();
	}
	


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


void translate_camera_keyboard(unsigned char key, int x, int y) {
	
		if (key == 'a' || key == 'A') {
			pos_x -= 2;
			lookat_x -= 2;
			glutPostRedisplay();
		}
		else if (key == 'd' || key == 'D'){
		    pos_x += 2;
		    lookat_x += 2;
		    glutPostRedisplay();

	    }
		else if (key == 'e' || key == 'E') {
			pos_z -= 2;
			lookat_z -= 2;
			glutPostRedisplay();

		}
		else if (key == 'q' || key == 'Q') {
			pos_z += 2;
			lookat_z += 2;
			glutPostRedisplay();

		}
		else if (key == 'w' || key == 'W') {
			pos_y += 2;
			lookat_y += 2;
			glutPostRedisplay();

		}
		else if (key == 's' || key == 'S') {
			pos_y -= 2;
			lookat_y -= 2;
			glutPostRedisplay();

		}
}


int main(int argc, char** argv)
{

	std::ifstream fich("..\\..\\tests\\testar_xml.txt", std::ios::in);
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
	glutKeyboardFunc(translate_camera_keyboard);


	glutSpecialFunc(processSpecialKeys);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT’s main cycle
	glutMainLoop(); //ciclo do glut; enquanto a janela nao for fechada chama o renderScene e processa eventos


	return 1;
}