#include <stdlib.h>
#include <stdio.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include "tinyxml2.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <map>



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



std::map<std::vector<float>, float> map_t;
std::map<std::vector<float>, float> map_times_atual;

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

void buildRotMatrix(float* x, float* y, float* z, float* m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}


float length(float* v) {

	float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return res;

}

void multMatrixVector(float* m, float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}


float multVectorVector(float* v1, float* v2) {
	float res = 0;

	for (int i = 0; i < 4; i++) {
		res += (v1[i] * v2[i]);
	}

	return res;


}

float* Ponto_to_float_pointer(Ponto p) {
	float res[3];

	res[0] = p.x;
	res[1] = p.y;
	res[2] = p.z;

	return (float*)res;
}

void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };

	float A[4];
	float mt[4] = { pow(t,3), pow(t,2), t, 1 };
	float t_deriv[4] = { 3 * (pow(t,2)), 2 * t, 1, 0 };

	// Compute A = M * P
	for (int i = 0; i < 3; ++i) {
		float p[4] = { p0[i], p1[i], p2[i], p3[i] };
		multMatrixVector((float*)m, (float*)p, (float*)A);

		// Compute pos = T * A
		pos[i] = multVectorVector((float*)mt, (float*)A);

		// compute deriv = T' * A
		deriv[i] = multVectorVector((float*)t_deriv, (float*)A);

	}

	// ...
}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, std::vector<Ponto> p) {

	int POINT_COUNT = p.size();
	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, Ponto_to_float_pointer(p[indices[0]]), Ponto_to_float_pointer(p[indices[1]]), Ponto_to_float_pointer(p[indices[2]]), Ponto_to_float_pointer(p[indices[3]]), pos, deriv);
}


void renderCatmullRomCurve(std::vector<Ponto> p) {

	float t = 0.01;
	// draw curve using line segments with GL_LINE_LOOP
	float pos[3], deriv[3];
	glBegin(GL_LINE_LOOP);
	for (float i = 0; i <= 1; i += t) {
		getGlobalCatmullRomPoint(i, (float*)pos, (float*)deriv, p);
		glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();

}

std::vector<float> pointvector_to_floatvector(std::vector<Ponto> pontos) {

	std::vector<float> res;

	for (Ponto p : pontos) {
		res.push_back(p.x);
		res.push_back(p.y);
		res.push_back(p.z);
	}
	return res;
}

float y_aux[3] = { 0, 1, 0 };
float x_aux[3] = { 1, 0, 0 };
float z_aux[3] = { 0, 0, 1 };

struct Transformacao {
	float angulo;
	float x;
	float y;
	float z;
	float time;
	bool align;
	bool align_y;
	bool align_z;
	std::vector<Ponto> pontos;
	std::string flag;


	Transformacao() {
		this->angulo = 0;
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->flag = "undef";
	}


	void Translacao(float xi, float yi, float zi) {
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->flag = "t";
	}

	void CatmullRom_x(float timei, bool aligni, std::vector<Ponto> pontosi) {
		this->align = aligni;
		this->align_y = false;
		this->align_z = false;
		this->pontos = pontosi;
		this->time = timei*1000;
		map_t[pointvector_to_floatvector(pontosi)] = 0;
		map_times_atual[pointvector_to_floatvector(pontosi)] = 0;
		this->flag = "c";
	}

	void CatmullRom_y(bool aligni, float timei, std::vector<Ponto> pontosi) {
		this->align_y = aligni;
		this->align = false;
		this->align_z = false;
		this->pontos = pontosi;
		this->time = timei * 1000;
		map_t[pointvector_to_floatvector(pontosi)] = 0;
		map_times_atual[pointvector_to_floatvector(pontosi)] = 0;
		this->flag = "c";
	}

	void CatmullRom_z(float timei, std::vector<Ponto> pontosi, bool aligni) {
		this->align_z = aligni;
		this->align = false;
		this->align_y = false;
		this->pontos = pontosi;
		this->time = timei * 1000;
		map_t[pointvector_to_floatvector(pontosi)] = 0;
		map_times_atual[pointvector_to_floatvector(pontosi)] = 0;
		this->flag = "c";
	}


	void Escala(float xi, float yi, float zi) {
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->flag = "e";
	}


	void Rotacao(float alpha, float xi, float yi, float zi) {
		this->angulo = alpha;
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->flag = "r";

	}

	void RotacaoTime(float xi, float yi, float zi, float timei) {
		this->x = xi;
		this->y = yi;
		this->z = zi;
		this->time = timei * 1000;
		this->flag = "rt";

	}



	void executa_transf() {
		if (this->flag == "t") {
			glTranslatef(this->x, this->y, this->z);
		}
		else if (this->flag == "e") {
			glScalef(this->x, this->y, this->z);
		}
		else if (this->flag == "r") {
			glRotatef(this->angulo, this->x, this->y, this->z);
		}
		else if (this->flag == "rt") {
			this->angulo = (glutGet(GLUT_ELAPSED_TIME) * 360) / this->time;  //regra de 3 simples
			glRotatef(this->angulo, this->x, this->y, this->z);
		}
		else if (this->flag == "c") {

			renderCatmullRomCurve(this->pontos);

			float pos[3], deriv[3];
			getGlobalCatmullRomPoint(map_t[pointvector_to_floatvector(this->pontos)] , (float*)pos, (float*)deriv, this->pontos);

			glTranslatef(pos[0], pos[1], pos[2]);
			if (this->align == true) {

				float y[3];
				float z[3];

				normalize((float*)deriv);

				cross((float*)deriv, (float*)(y_aux), (float*)z);
				normalize((float*)z);


				cross((float*)z, (float*)deriv, (float*)y);
				normalize((float*)y);

				for (int i = 0; i < 3; i++)
					y_aux[i] = y[i];

				float rotMatrix[16];
				buildRotMatrix((float*)deriv, (float*)y, (float*)z, (float*)rotMatrix);
				glMultMatrixf((float*)rotMatrix);


			}
			else if (this->align_y == true) {

				float x[3];
				float z[3];

				normalize((float*)deriv);

				cross((float*)deriv, (float*)(z_aux), (float*)x);
				normalize((float*)x);


				cross((float*)x, (float*)deriv, (float*)z);
				normalize((float*)z);

				for (int i = 0; i < 3; i++)
					z_aux[i] = z[i];

				float rotMatrix[16];
				buildRotMatrix((float*)x, (float*)deriv, (float*)z, (float*)rotMatrix);
				glMultMatrixf((float*)rotMatrix);


			}
			else if (this->align_z == true) {

				float x[3];
				float y[3];

				normalize((float*)deriv);

				cross((float*)deriv, (float*)(x_aux), (float*)y);
				normalize((float*)y);


				cross((float*)y, (float*)deriv, (float*)x);
				normalize((float*)x);

				for (int i = 0; i < 3; i++)
					x_aux[i] = x[i];

				float rotMatrix[16];
				buildRotMatrix((float*)x, (float*)y, (float*)deriv, (float*)rotMatrix);
				glMultMatrixf((float*)rotMatrix);


			}
			
			float prox_time = glutGet(GLUT_ELAPSED_TIME);
			float ratio = prox_time - map_times_atual[pointvector_to_floatvector(this->pontos)];
			map_t[pointvector_to_floatvector(this->pontos)] += (ratio / this->time);
			map_times_atual[pointvector_to_floatvector(this->pontos)] = prox_time;

		}
		else { ; }
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
	std::map<std::string, GLuint> indices;



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


float* pointvector_to_floatarray(std::vector<Ponto> pontos) {

	float* res = new float[pontos.size() * 3];
	int i = 0;
	for (Ponto p : pontos) {
		res[i] = p.x;
		res[i + 1] = p.y;
		res[i + 2] = p.z;
		i += 3;
	}
	return res;
}




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
			while (tipo_transf) {
				std::string nome = std::string(tipo_transf->Name());

				if (nome == "translate") {

					if (tipo_transf->Attribute("time") != nullptr) {
						std::vector<Ponto> pontos;
						bool align = tipo_transf->BoolAttribute("align");
						float time = tipo_transf->FloatAttribute("time");
						tinyxml2::XMLElement* ponto = tipo_transf->FirstChildElement("point");
						while (ponto != nullptr) {
							float x = ponto->FloatAttribute("x");
							float y = ponto->FloatAttribute("y");
							float z = ponto->FloatAttribute("z");
							pontos.push_back(Ponto(x, y, z));
							ponto = ponto->NextSiblingElement("point");
						}
						if (tipo_transf->Attribute("align") == nullptr) {
							if (tipo_transf->Attribute("aligny") != nullptr) {
								Transformacao nt = Transformacao();
								nt.CatmullRom_y(true, time, pontos);
								transfs.push_back(nt);
							}
							else if (tipo_transf->Attribute("alignz") != nullptr) {
								Transformacao nt = Transformacao();
								nt.CatmullRom_z(time, pontos, true);
								transfs.push_back(nt);
							}
						}
						else {
							Transformacao nt = Transformacao();
							nt.CatmullRom_x(time, align, pontos);
							transfs.push_back(nt);
						}
					}
					else {
						float x, y, z;
						x = tipo_transf->FloatAttribute("x");
						y = tipo_transf->FloatAttribute("y");
						z = tipo_transf->FloatAttribute("z");

						Transformacao nt = Transformacao();
						nt.Translacao(x, y, z);
						transfs.push_back(nt);
					}
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
					float x = tipo_transf->FloatAttribute("x");
					float y = tipo_transf->FloatAttribute("y");
					float z = tipo_transf->FloatAttribute("z");
					if (tipo_transf->Attribute("time") != nullptr) {
						float time = tipo_transf->FloatAttribute("time");

						Transformacao nt = Transformacao();
						nt.RotacaoTime(x, y, z, time);
						transfs.push_back(nt);
					}
					else {
						float angle;
						angle = tipo_transf->FloatAttribute("angle");

						Transformacao nt = Transformacao();
						nt.Rotacao(angle, x, y, z);
						transfs.push_back(nt);
					}
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
		radius = 700.0;
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glBindBuffer(GL_ARRAY_BUFFER, figs.indices[nick]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, figs.pts_por_fig[nick].size() * 3);
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
	else if (key == 'd' || key == 'D') {
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


	glewInit();


	// put callback registry here
	glutReshapeFunc(changeSize); //chama uma funçao com 2 parametros width e height (changeSize)
	glutIdleFunc(renderScene);   //chama a funçao renderScene quando puder
	glutDisplayFunc(renderScene); //pintar a janela (argumento é o nome da funçao)
	glutKeyboardFunc(translate_camera_keyboard);
	glutSpecialFunc(processSpecialKeys);


	glEnableClientState(GL_VERTEX_ARRAY);
	GLuint* buffers = new GLuint[figs.pts_por_fig.size()];
	glGenBuffers(figs.pts_por_fig.size(), buffers);


	int i = 0;
	for (std::pair<std::string, std::vector<Ponto>> pair : figs.pts_por_fig) {
		figs.indices[pair.first] = buffers[i];
		float* points = pointvector_to_floatarray(pair.second);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, pair.second.size() * 3 * sizeof(float), points, GL_STATIC_DRAW);
		i++;
	}

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT’s main cycle
	glutMainLoop(); //ciclo do glut; enquanto a janela nao for fechada chama o renderScene e processa eventos


	return 1;
}