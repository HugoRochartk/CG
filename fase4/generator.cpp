﻿#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <math.h>
#include <map>
#include <vector>


struct Ponto {
    float x = 0;
    float y = 0;
    float z = 0;

    //construtor:
    void set_coords(float xi, float yi, float zi) {
        this->x = xi;
        this->y = yi;
        this->z = zi;
    }
};


std::ofstream cria_ficheiro_3d(std::string s) {
    std::ofstream fich;
    fich.open("3d/" + s);
    return fich;

}

void cria_esfera(float radius, int slices, int stacks, std::string filename) {
    std::ofstream fich = cria_ficheiro_3d(filename);
    std::stringstream str_vertices;



    float alpha = (2 * M_PI) / slices; //valor angulo alpha
    float beta = M_PI / stacks; //valor angulo beta, nao precisa de ser 2pi porque nao vai dar a volta a esfera
    float px1, px2, px3, px4, py1, py2, pz1, pz2, pz3, pz4; //coordenadas cartezianas

    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            //cria um quadrado composto por 2 triangulos a cada itera��o ou apenas um triangulo na primeira e ultima itera�ao
            px1 = radius * cos(M_PI_2 - j * beta) * sin(i * alpha); //ponto comum aos dois 
            px2 = radius * cos(M_PI_2 - (j + 1) * beta) * sin(i * alpha); //ponto do primeiro
            px3 = radius * cos(M_PI_2 - (j + 1) * beta) * sin((i + 1) * alpha); //ponto comum aos dois
            px4 = radius * cos(M_PI_2 - j * beta) * sin((i + 1) * alpha); //ponto do segundo

            py1 = radius * sin(M_PI_2 - j * beta); //valor superior y
            py2 = radius * sin(M_PI_2 - (j + 1) * beta); //valor inferior y

            pz1 = radius * cos(M_PI_2 - j * beta) * cos(i * alpha); //ponto comum aos dois 
            pz2 = radius * cos(M_PI_2 - (j + 1) * beta) * cos(i * alpha); //ponto do primeiro
            pz3 = radius * cos(M_PI_2 - (j + 1) * beta) * cos((i + 1) * alpha); //ponto comum aos dois 
            pz4 = radius * cos(M_PI_2 - j * beta) * cos((i + 1) * alpha); //ponto do segundo



            str_vertices << px1 << ' ' << py1 << ' ' << pz1 << '\n';
            str_vertices << px2 << ' ' << py2 << ' ' << pz2 << '\n';
            str_vertices << px3 << ' ' << py2 << ' ' << pz3 << '\n';



            str_vertices << px1 << ' ' << py1 << ' ' << pz1 << '\n';
            str_vertices << px3 << ' ' << py2 << ' ' << pz3 << '\n';
            str_vertices << px4 << ' ' << py1 << ' ' << pz4 << '\n';

        }

    }

    fich << str_vertices.str();
    fich.close();
}

void cria_caixa(float length, int divisions, std::string filename) {
    std::ofstream fich = cria_ficheiro_3d(filename);
    std::stringstream str_vertices;

    float x1, x2, y1, y2, z1, z2;
    float y = -length / 2;
    float cima[3]   = { 0,1,0 };
    float baixo[3]  = { 0,-1,0 };
    float esq[3]    = { 1,0,0 };
    float dir[3]    = { -1,0,0 };
    float frente[3] = { 0,0,1 };
    float tras[3]   = { 0,0,-1 };
    float aux = 1.0f / divisions;

    for (int i = 0; i < divisions; i++) {
        z1 = (i * (length / divisions)) - (length / 2);
        z2 = ((i + 1) * (length / divisions)) - (length / 2);
        for (int j = 0; j < divisions; j++) {
            x1 = (j * (length / divisions)) - (length / 2);
            x2 = ((j + 1) * (length / divisions)) - (length / 2);

            str_vertices << x1 << ' ' << y << ' ' << z1 << ' ';
            str_vertices << cima[0] << ' ' << cima[1] << ' ' << cima[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';

            str_vertices << x2 << ' ' << y << ' ' << z2 << ' ';
            str_vertices << cima[0] << ' ' << cima[1] << ' ' << cima[2] << ' ';
            str_vertices << (j+1) * aux << ' ' << (i+1) * aux << '\n';

            str_vertices << x1 << ' ' << y << ' ' << z2 << ' ';
            str_vertices << cima[0] << ' ' << cima[1] << ' ' << cima[2] << ' ';
            str_vertices << j * aux << ' ' << (i+1) * aux << '\n';

            
            str_vertices << x1 << ' ' << y << ' ' << z1 << ' ';
            str_vertices << cima[0] << ' ' << cima[1] << ' ' << cima[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';

            str_vertices << x2 << ' ' << y << ' ' << z1 << ' ';
            str_vertices << cima[0] << ' ' << cima[1] << ' ' << cima[2] << ' ';
            str_vertices << (j+1) * aux << ' ' << i * aux << '\n';

            str_vertices << x2 << ' ' << y << ' ' << z2 << ' ';
            str_vertices << cima[0] << ' ' << cima[1] << ' ' << cima[2] << ' ';
            str_vertices << (j+1) * aux << ' ' << (i+1) * aux << '\n';



            str_vertices << x2 << ' ' << -y << ' ' << z2 << ' ';
            str_vertices << baixo[0] << ' ' << baixo[1] << ' ' << baixo[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << (i + 1) * aux << '\n';
            
            str_vertices << x1 << ' ' << -y << ' ' << z1 << ' ';
            str_vertices << baixo[0] << ' ' << baixo[1] << ' ' << baixo[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';

            str_vertices << x1 << ' ' << -y << ' ' << z2 << ' ';
            str_vertices << baixo[0] << ' ' << baixo[1] << ' ' << baixo[2] << ' ';
            str_vertices << j * aux << ' ' << (i + 1) * aux << '\n';


            str_vertices << x2 << ' ' << -y << ' ' << z2 << ' ';
            str_vertices << baixo[0] << ' ' << baixo[1] << ' ' << baixo[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << (i + 1) * aux << '\n';

            str_vertices << x2 << ' ' << -y << ' ' << z1 << ' ';
            str_vertices << baixo[0] << ' ' << baixo[1] << ' ' << baixo[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << i * aux << '\n';
            
            str_vertices << x1 << ' ' << -y << ' ' << z1 << ' ';
            str_vertices << baixo[0] << ' ' << baixo[1] << ' ' << baixo[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';
        }
    }

    float x = -length / 2;

    for (int i = 0; i < divisions; i++) {
        y1 = (i * (length / divisions)) - (length / 2);
        y2 = ((i + 1) * (length / divisions)) - (length / 2);
        for (int j = 0; j < divisions; j++) {
            z1 = (j * (length / divisions)) - (length / 2);
            z2 = ((j + 1) * (length / divisions)) - (length / 2);

            str_vertices << x << ' ' << y2 << ' ' << z1 << ' ';
            str_vertices << esq[0] << ' ' << esq[1] << ' ' << esq[2] << ' ';
            str_vertices << (i + 1) * aux << ' ' << j * aux << '\n';

            str_vertices << x << ' ' << y1 << ' ' << z1 << ' ';
            str_vertices << esq[0] << ' ' << esq[1] << ' ' << esq[2] << ' ';
            str_vertices << i * aux << ' ' << j * aux << '\n';

            str_vertices << x << ' ' << y2 << ' ' << z2 << ' ';
            str_vertices << esq[0] << ' ' << esq[1] << ' ' << esq[2] << ' ';
            str_vertices << (i + 1) * aux << ' ' << (j + 1) * aux << '\n';


            str_vertices << x << ' ' << y1 << ' ' << z2 << ' ';
            str_vertices << esq[0] << ' ' << esq[1] << ' ' << esq[2] << ' ';
            str_vertices << i * aux << ' ' << (j + 1) * aux << '\n';

            str_vertices << x << ' ' << y2 << ' ' << z2 << ' ';
            str_vertices << esq[0] << ' ' << esq[1] << ' ' << esq[2] << ' ';
            str_vertices << (i + 1) * aux << ' ' << (j + 1) * aux << '\n';

            str_vertices << x << ' ' << y1 << ' ' << z1 << ' ';
            str_vertices << esq[0] << ' ' << esq[1] << ' ' << esq[2] << ' ';
            str_vertices << i * aux << ' ' << j * aux << '\n';



            str_vertices << -x << ' ' << y2 << ' ' << z1 << ' ';
            str_vertices << dir[0] << ' ' << dir[1] << ' ' << dir[2] << ' ';
            str_vertices << (i + 1) * aux << ' ' << j * aux << '\n';

            str_vertices << -x << ' ' << y2 << ' ' << z2 << ' ';
            str_vertices << dir[0] << ' ' << dir[1] << ' ' << dir[2] << ' ';
            str_vertices << (i + 1) * aux << ' ' << (j + 1) * aux << '\n';

            str_vertices << -x << ' ' << y1 << ' ' << z1 << ' ';
            str_vertices << dir[0] << ' ' << dir[1] << ' ' << dir[2] << ' ';
            str_vertices << i * aux << ' ' << j * aux << '\n';


            str_vertices << -x << ' ' << y1 << ' ' << z2 << ' ';
            str_vertices << dir[0] << ' ' << dir[1] << ' ' << dir[2] << ' ';
            str_vertices << i * aux << ' ' << (j + 1) * aux << '\n';

            str_vertices << -x << ' ' << y1 << ' ' << z1 << ' ';
            str_vertices << dir[0] << ' ' << dir[1] << ' ' << dir[2] << ' ';
            str_vertices << i * aux << ' ' << j * aux << '\n';

            str_vertices << -x << ' ' << y2 << ' ' << z2 << ' ';
            str_vertices << dir[0] << ' ' << dir[1] << ' ' << dir[2] << ' ';
            str_vertices << (i + 1) * aux << ' ' << (j + 1) * aux << '\n';
        }
    }


    float z = length / 2;

    for (int i = 0; i < divisions; i++) {
        y1 = (i * (length / divisions)) - (length / 2);
        y2 = ((i + 1) * (length / divisions)) - (length / 2);
        for (int j = 0; j < divisions; j++) {
            x1 = (j * (length / divisions)) - (length / 2);
            x2 = ((j + 1) * (length / divisions)) - (length / 2);

            str_vertices << x2 << ' ' << y2 << ' ' << z << ' ';
            str_vertices << frente[0] << ' ' << frente[1] << ' ' << frente[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << (i + 1) * aux << '\n';

            str_vertices << x1 << ' ' << y2 << ' ' << z << ' ';
            str_vertices << frente[0] << ' ' << frente[1] << ' ' << frente[2] << ' ';
            str_vertices << j * aux << ' ' << (i + 1) * aux << '\n';

            str_vertices << x1 << ' ' << y1 << ' ' << z << ' ';
            str_vertices << frente[0] << ' ' << frente[1] << ' ' << frente[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';


            str_vertices << x1 << ' ' << y1 << ' ' << z << ' ';
            str_vertices << frente[0] << ' ' << frente[1] << ' ' << frente[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';

            str_vertices << x2 << ' ' << y1 << ' ' << z << ' ';
            str_vertices << frente[0] << ' ' << frente[1] << ' ' << frente[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << i * aux << '\n';

            str_vertices << x2 << ' ' << y2 << ' ' << z << ' ';
            str_vertices << frente[0] << ' ' << frente[1] << ' ' << frente[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << (i + 1) * aux << '\n';



            str_vertices << x2 << ' ' << y2 << ' ' << -z << ' ';
            str_vertices << tras[0] << ' ' << tras[1] << ' ' << tras[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << (i + 1) * aux << '\n';

            str_vertices << x1 << ' ' << y1 << ' ' << -z << ' ';
            str_vertices << tras[0] << ' ' << tras[1] << ' ' << tras[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';

            str_vertices << x1 << ' ' << y2 << ' ' << -z << ' ';
            str_vertices << tras[0] << ' ' << tras[1] << ' ' << tras[2] << ' ';
            str_vertices << j * aux << ' ' << (i + 1) * aux << '\n';


            str_vertices << x1 << ' ' << y1 << ' ' << -z << ' ';
            str_vertices << tras[0] << ' ' << tras[1] << ' ' << tras[2] << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';

            str_vertices << x2 << ' ' << y2 << ' ' << -z << ' ';
            str_vertices << tras[0] << ' ' << tras[1] << ' ' << tras[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << (i + 1) * aux << '\n';

            str_vertices << x2 << ' ' << y1 << ' ' << -z << ' ';
            str_vertices << tras[0] << ' ' << tras[1] << ' ' << tras[2] << ' ';
            str_vertices << (j + 1) * aux << ' ' << i * aux << '\n';

        }
    }

    fich << str_vertices.str();
    fich.close();

}

void cria_cone(float radius, float height, int slices, int stacks, std::string filename) {
    std::ofstream fich = cria_ficheiro_3d(filename);
    std::stringstream str_vertices;

    float alpha = (2 * M_PI) / slices;
    float x1, x2, z1, z2;
    float y = height;

    for (int i = 0; i < slices; i++) {
        x2 = radius * sin(i * alpha);
        x1 = radius * sin((i + 1) * alpha);
        z2 = radius * cos(i * alpha);
        z1 = radius * cos((i + 1) * alpha);

        str_vertices << x2 << ' ' << 0 << ' ' << z2 << '\n';
        str_vertices << 0 << ' ' << 0 << ' ' << 0 << '\n';
        str_vertices << x1 << ' ' << 0 << ' ' << z1 << '\n';



    }

    float x3, z3, x4, z4, y1, y2;
    float stack_height = height / stacks; // altura de cada stack 
    float cone_ratio = height / radius; // valor da altura em funcao do raio -> h = 3.2 * raio
    float height_stack_atual, height_stack_seguinte, raio_stack_atual, raio_stack_seguinte;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            height_stack_atual = height - (i * stack_height);
            height_stack_seguinte = height - ((i + 1) * stack_height);

            raio_stack_atual = height_stack_atual / cone_ratio; //raio dos pontos que se encontram na stack = i
            raio_stack_seguinte = height_stack_seguinte / cone_ratio; // raio dos pontos que se encontram na stack = i+1

            y1 = (i * stack_height);

            x1 = raio_stack_atual * sin(j * alpha);
            z1 = raio_stack_atual * cos(j * alpha);

            x2 = raio_stack_atual * sin((j + 1) * alpha);
            z2 = raio_stack_atual * cos((j + 1) * alpha);

            y2 = (i + 1) * stack_height;

            x3 = raio_stack_seguinte * sin((j + 1) * alpha);
            z3 = raio_stack_seguinte * cos((j + 1) * alpha);

            x4 = raio_stack_seguinte * sin(j * alpha);
            z4 = raio_stack_seguinte * cos(j * alpha);



            str_vertices << x1 << ' ' << y1 << ' ' << z1 << '\n';
            str_vertices << x2 << ' ' << y1 << ' ' << z2 << '\n';
            str_vertices << x4 << ' ' << y2 << ' ' << z4 << '\n';


            str_vertices << x4 << ' ' << y2 << ' ' << z4 << '\n';
            str_vertices << x2 << ' ' << y1 << ' ' << z2 << '\n';
            str_vertices << x3 << ' ' << y2 << ' ' << z3 << '\n';

        }
    }

    fich << str_vertices.str();
    fich.close();
}

void cria_plano(float length, int divisions, std::string filename) {
    std::ofstream fich = cria_ficheiro_3d(filename);
    std::stringstream str_vertices;

    float x1, x2, z1, z2;
    float y = 0;
    float aux = 1.0f / divisions;

    for (int i = 0; i < divisions; i++) {
        z1 = (i * (length / divisions)) - (length / 2);
        z2 = ((i + 1) * (length / divisions)) - (length / 2);
        for (int j = 0; j < divisions; j++) {
            x1 = (j * (length / divisions)) - (length / 2);
            x2 = ((j + 1) * (length / divisions)) - (length / 2);

            str_vertices << x2 << ' ' << y << ' ' << z1 << ' ';
            str_vertices << 0 << ' ' << 1 << ' ' << 0 << ' ';
            str_vertices << (j+1) * aux << ' ' << i * aux << '\n';

            str_vertices << x1 << ' ' << y << ' ' << z1 << ' ';
            str_vertices << 0 << ' ' << 1 << ' ' << 0 << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';
            
            str_vertices << x2 << ' ' << y << ' ' << z2 << ' ';
            str_vertices << 0 << ' ' << 1 << ' ' << 0 << ' ';
            str_vertices << (j+1) * aux << ' ' << (i+1) * aux << '\n';

            str_vertices << x1 << ' ' << y << ' ' << z2 << ' ';
            str_vertices << 0 << ' ' << 1 << ' ' << 0 << ' ';
            str_vertices << j * aux << ' ' << (i+1) * aux << '\n';
            
            str_vertices << x2 << ' ' << y << ' ' << z2 << ' ';
            str_vertices << 0 << ' ' << 1 << ' ' << 0 << ' ';
            str_vertices << (j+1) * aux << ' ' << (i+1) * aux << '\n';

            str_vertices << x1 << ' ' << y << ' ' << z1 << ' ';
            str_vertices << 0 << ' ' << 1 << ' ' << 0 << ' ';
            str_vertices << j * aux << ' ' << i * aux << '\n';
        }
    }


    fich << str_vertices.str();



    fich.close();

}


void cria_cilindro(float radius, float height, int slices, int stacks, std::string filename) {
    std::ofstream fich = cria_ficheiro_3d(filename);
    std::stringstream str_vertices;



    float alpha = (2 * M_PI) / slices;
    float ratio = height / stacks;
    float x1, x2, z1, z2;

    for (int i = 0; i < slices; i++) {
        x1 = radius * sin(i * alpha);
        x2 = radius * sin((i + 1) * alpha);
        z1 = radius * cos(i * alpha);
        z2 = radius * cos((i + 1) * alpha);

        str_vertices << 0 << ' ' << height << ' ' << 0 << '\n';
        str_vertices << x1 << ' ' << height << ' ' << z1 << '\n';
        str_vertices << x2 << ' ' << height << ' ' << z2 << '\n';
    }


    for (int i = 0; i < slices; i++) {
        x1 = radius * sin(i * alpha);
        x2 = radius * sin((i + 1) * alpha);
        z1 = radius * cos(i * alpha);
        z2 = radius * cos((i + 1) * alpha);

        str_vertices << 0 << ' ' << 0 << ' ' << 0 << '\n';
        str_vertices << x2 << ' ' << 0 << ' ' << z2 << '\n';
        str_vertices << x1 << ' ' << 0 << ' ' << z1 << '\n';
    }


    for (int j = 0; j < stacks; j++) {
        for (int i = 0; i < slices; i++) {

            x1 = radius * sin(i * alpha);
            x2 = radius * sin((i + 1) * alpha);
            z1 = radius * cos(i * alpha);
            z2 = radius * cos((i + 1) * alpha);

            str_vertices << x1 << ' ' << ratio * (j + 1) << ' ' << z1 << '\n';
            str_vertices << x1 << ' ' << ratio * j << ' ' << z1 << '\n';
            str_vertices << x2 << ' ' << ratio * j << ' ' << z2 << '\n';

            str_vertices << x2 << ' ' << ratio * (j + 1) << ' ' << z2 << '\n';
            str_vertices << x1 << ' ' << ratio * (j + 1) << ' ' << z1 << '\n';
            str_vertices << x2 << ' ' << ratio * j << ' ' << z2 << '\n';


        }
    }

    fich << str_vertices.str();
    fich.close();
}

std::ifstream abre_ficheiro_patch(std::string filename) {
    std::string path = "patch//";
    path += filename;
    std::ifstream fich(path);
    if (!fich.is_open()) {
        printf("Não foi possível abrir o ficheiro patch.\n");
    }
    return fich;
}

void MxM(float M1[4][4], float M2[4][4], float res[4][4]) { // res = M1M2

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                res[i][j] += (M1[i][k] * M2[k][j]);
            }
        }
    }
}



float VxV(float v1[4], float v2[4]) {
    float res = 0;

    for (int i = 0; i < 4; i++) {
        res += (v1[i] * v2[i]);
    }

    return res;


}


void MxV(float m[4][4], float v[4], float res[4]) {

    for (int j = 0; j < 4; j++) {
        res[j] = VxV(m[j], v);
    }

}

void constroi_matrizes_xyz(std::vector<Ponto> pontos, float xMatrix[4][4], float yMatrix[4][4], float zMatrix[4][4]) {

    int i = 0;
    int j = 0;

    for (Ponto p : pontos) {
        xMatrix[i][j] = p.x;
        yMatrix[i][j] = p.y;
        zMatrix[i][j] = p.z;
        i++;
        if (i == 4) {
            i = 0;
            j++;
        }
    }

}


void cria_vetor(float i, float vet[4]) {

    vet[0] = powf(i, 3);
    vet[1] = powf(i, 2);
    vet[2] = i;
    vet[3] = 1;

}


float B_u_v(float u[4], float m[4][4], float v[4]) {
    float intermed[4];
    MxV(m, v, intermed);
    float res = VxV(u, intermed);
    return res;
}

void cria_curva_bezier(std::string patch_filename, int tesselation, std::string triangles_filename) {
    std::ofstream fich = cria_ficheiro_3d(triangles_filename);
    std::ifstream patch = abre_ficheiro_patch(patch_filename);
    int num_patches; int num_pontos;
    int indice;
    char virgula;
    std::map<int, std::vector<int>> patchnr_indices;
    std::map<int, Ponto> pontonr_ponto;
    std::map<int, std::vector<Ponto>> patchnr_pontos;

    patch >> num_patches;
    for (int i = 0; i < num_patches; i++) {
        std::vector<int> indices;
        for (int j = 0; j < 16; j++) {
            patch >> indice;
            if (j != 15) {
                patch >> virgula;
            }
            indices.push_back(indice);
        }
        patchnr_indices[i] = indices;
    }

    float x, y, z;
    patch >> num_pontos;
    for (int i = 0; i < num_pontos; i++) {
        patch >> x; patch >> virgula; patch >> y; patch >> virgula; patch >> z;
        Ponto p; p.set_coords(x, y, z);
        pontonr_ponto[i] = p;
    }


    for (int i = 0; i < num_patches; i++) {
        std::vector<Ponto> pontos;

        for (int ind : patchnr_indices[i]) {
            pontos.push_back(pontonr_ponto[ind]);
        }
        patchnr_pontos[i] = pontos;
    }

    float xMatrix[4][4], yMatrix[4][4], zMatrix[4][4];
    float BezierMatrix[4][4] = {
                                 { -1,  3, -3, 1},
                                 {  3, -6,  3, 0},
                                 { -3,  3,  0, 0},
                                 {  1,  0,  0, 0}
    };
    float matriz_intermedia[4][4];
    float t = 1.0 / tesselation;
    float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
    float u_i[4], v_j[4], u_i_t[4], v_j_t[4];

    for (std::pair<int, std::vector<Ponto>> par : patchnr_pontos) {
        std::stringstream str_vertices;
        constroi_matrizes_xyz(par.second, xMatrix, yMatrix, zMatrix);
        MxM(BezierMatrix, xMatrix, matriz_intermedia);
        MxM(matriz_intermedia, BezierMatrix, xMatrix); //BezierMatrixTransposta = BezierMatrix
        MxM(BezierMatrix, yMatrix, matriz_intermedia);
        MxM(matriz_intermedia, BezierMatrix, yMatrix);
        MxM(BezierMatrix, zMatrix, matriz_intermedia);
        MxM(matriz_intermedia, BezierMatrix, zMatrix);


        for (float i = 0; i < 1; i += t) {

            cria_vetor(i, u_i);
            cria_vetor(i + t, u_i_t);
            for (float j = 0; j < 1; j += t) {

                cria_vetor(j, v_j);
                cria_vetor(j + t, v_j_t);

                x1 = B_u_v(u_i, xMatrix, v_j);
                x2 = B_u_v(u_i_t, xMatrix, v_j);
                x3 = B_u_v(u_i_t, xMatrix, v_j_t);
                x4 = B_u_v(u_i, xMatrix, v_j_t);

                y1 = B_u_v(u_i, yMatrix, v_j);
                y2 = B_u_v(u_i_t, yMatrix, v_j);
                y3 = B_u_v(u_i_t, yMatrix, v_j_t);
                y4 = B_u_v(u_i, yMatrix, v_j_t);

                z1 = B_u_v(u_i, zMatrix, v_j);
                z2 = B_u_v(u_i_t, zMatrix, v_j);
                z3 = B_u_v(u_i_t, zMatrix, v_j_t);
                z4 = B_u_v(u_i, zMatrix, v_j_t);


                str_vertices << x1 << ' ' << y1 << ' ' << z1 << '\n';
                str_vertices << x2 << ' ' << y2 << ' ' << z2 << '\n';
                str_vertices << x4 << ' ' << y4 << ' ' << z4 << '\n';

                str_vertices << x2 << ' ' << y2 << ' ' << z2 << '\n';
                str_vertices << x3 << ' ' << y3 << ' ' << z3 << '\n';
                str_vertices << x4 << ' ' << y4 << ' ' << z4 << '\n';

            }
        }

        fich << str_vertices.str();

    }

    fich.close();
}

void menu(char** argv) {

    if (!strcmp(argv[1], "sphere")) {
        cria_esfera(std::stof(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]), argv[5]);
    }
    else if (!strcmp(argv[1], "box")) {
        cria_caixa(std::stof(argv[2]), std::stoi(argv[3]), argv[4]);
    }
    else if (!strcmp(argv[1], "cone")) {
        cria_cone(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), argv[6]);
    }
    else if (!strcmp(argv[1], "plane")) {
        cria_plano(std::stof(argv[2]), std::stoi(argv[3]), argv[4]);
    }
    else if (!strcmp(argv[1], "cylinder")) {
        cria_cilindro(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), argv[6]);
    }
    else if (!strcmp(argv[1], "patch")) {
        cria_curva_bezier(argv[2], std::stoi(argv[3]), argv[4]);
    }


    else {
        printf("Incorrect syntax.\n");
        printf("You must type:\n");
        printf("generator sphere [radius] [slices] [stacks] [3d filename]\n");
        printf("generator cone [radius] [height] [slices] [stacks] [3d filename]\n");
        printf("generator box [dimension] [number of divs per edge] [3d filename]\n");
        printf("generator plane [length] [divisions] [3d file name]\n");
        printf("generator cylinder [radius] [heigth] [slices] [stacks] [3d file name]\n");
        printf("generator patch [patch filename] [tesselation level] [3d file name]\n");
    }


}


int main(int argc, char** argv) {



    if ((argc <= 4) || (argc >= 8)) {
        printf("Incorrect syntax.\n");
        printf("You must type:\n");
        printf("generator sphere [radius] [slices] [stacks] [3d filename]\n");
        printf("generator cone [radius] [height] [slices] [stacks] [3d filename]\n");
        printf("generator box [dimension] [number of divs per edge] [3d filename]\n");
        printf("generator plane [length] [divisions] [3d file name]");
        printf("generator cylinder [radius] [heigth] [slices] [stacks] [3d file name]\n");
        printf("generator patch [patch filename] [tesselation level] [3d file name]\n");
    }

    menu(argv);



    return 0;

}