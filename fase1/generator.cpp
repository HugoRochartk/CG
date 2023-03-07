#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <math.h>

std::ofstream cria_ficheiro_3d(std::string s){
    std::ofstream fich;
    fich.open("3d/"+s);
    return fich;

}

void cria_esfera(float radius, int slices, int stacks, std::string filename){
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

void cria_caixa(float length, int divisions, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::stringstream str_vertices;

      float x1, x2, y1, y2, z1, z2;
      float y = -length/2;
      
      for (int i = 0; i < divisions; i++) {
          z1 = (i * (length / divisions)) - (length / 2);
          z2 = ((i + 1) * (length / divisions)) - (length / 2);
          for (int j = 0; j < divisions; j++) {
              x1 = (j * (length / divisions)) - (length / 2);
              x2 = ((j + 1) * (length / divisions)) - (length / 2);

              str_vertices << x1 << ' ' << y << ' ' << z1 << '\n';
              str_vertices << x2 << ' ' << y << ' ' << z2 << '\n';
              str_vertices << x1 << ' ' << y << ' ' << z2 << '\n';

              str_vertices << x1 << ' ' << y << ' ' << z1 << '\n';
              str_vertices << x2 << ' ' << y << ' ' << z1 << '\n';
              str_vertices << x2 << ' ' << y << ' ' << z2 << '\n';

              str_vertices << x2 << ' ' << -y << ' ' << z2 << '\n';
              str_vertices << x1 << ' ' << -y << ' ' << z1 << '\n';
              str_vertices << x1 << ' ' << -y << ' ' << z2 << '\n';

              str_vertices << x2 << ' ' << -y << ' ' << z2 << '\n';
              str_vertices << x2 << ' ' << -y << ' ' << z1 << '\n';
              str_vertices << x1 << ' ' << -y << ' ' << z1 << '\n';
          }
      }
      
      float x = -length / 2;

      for (int i = 0; i < divisions; i++) {
          y1 = (i * (length / divisions)) - (length / 2);
          y2 = ((i + 1) * (length / divisions)) - (length / 2);
          for (int j = 0; j < divisions; j++) {
              z1 = (j * (length / divisions)) - (length / 2);
              z2 = ((j + 1) * (length / divisions)) - (length / 2);

              str_vertices << x << ' ' << y2 << ' ' << z1 << '\n';
              str_vertices << x << ' ' << y1 << ' ' << z1 << '\n';
              str_vertices << x << ' ' << y2 << ' ' << z2 << '\n';

              str_vertices << x << ' ' << y1 << ' ' << z2 << '\n';
              str_vertices << x << ' ' << y2 << ' ' << z2 << '\n';
              str_vertices << x << ' ' << y1 << ' ' << z1 << '\n';

              str_vertices << -x << ' ' << y2 << ' ' << z1 << '\n';
              str_vertices << -x << ' ' << y2 << ' ' << z2 << '\n';
              str_vertices << -x << ' ' << y1 << ' ' << z1 << '\n';

              str_vertices << -x << ' ' << y1 << ' ' << z2 << '\n';
              str_vertices << -x << ' ' << y1 << ' ' << z1 << '\n';
              str_vertices << -x << ' ' << y2 << ' ' << z2 << '\n';
          }
      }
      

      float z = length / 2;

      for (int i = 0; i < divisions; i++) {
          y1 = (i * (length / divisions)) - (length / 2);
          y2 = ((i + 1) * (length / divisions)) - (length / 2);
          for (int j = 0; j < divisions; j++) {
              x1 = (j * (length / divisions)) - (length / 2);
              x2 = ((j + 1) * (length / divisions)) - (length / 2);

              str_vertices << x2 << ' ' << y2 << ' ' << z << '\n';
              str_vertices << x1 << ' ' << y2 << ' ' << z << '\n';
              str_vertices << x1 << ' ' << y1 << ' ' << z << '\n';

              str_vertices << x1 << ' ' << y1 << ' ' << z << '\n';
              str_vertices << x2 << ' ' << y1 << ' ' << z << '\n';
              str_vertices << x2 << ' ' << y2 << ' ' << z << '\n';

              str_vertices << x2 << ' ' << y2 << ' ' << -z << '\n';
              str_vertices << x1 << ' ' << y1 << ' ' << -z << '\n';
              str_vertices << x1 << ' ' << y2 << ' ' << -z << '\n';

              str_vertices << x1 << ' ' << y1 << ' ' << -z << '\n';
              str_vertices << x2 << ' ' << y2 << ' ' << -z << '\n';
              str_vertices << x2 << ' ' << y1 << ' ' << -z << '\n';

          }
      }
      
      fich << str_vertices.str();
      fich.close();

}
 
void cria_cone(float radius, float height, int slices, int stacks, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::stringstream str_vertices;

      float alpha = (2 * M_PI) / slices;
      float x1, x2, z1, z2;
      float y = height;

      for (int i = 0; i < slices; i++) {
          x2 = radius * sin(i * alpha);
          x1 = radius * sin((i+1) * alpha);
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

              x2 = raio_stack_atual * sin((j+1) * alpha);
              z2 = raio_stack_atual * cos((j+1) * alpha);

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

void cria_plano(float length, int divisions, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::stringstream str_vertices;
     
      float x1, x2, z1, z2;
      float y = 0;

      for (int i = 0; i < divisions; i++) {
          z1 = (i * (length/divisions)) - (length/2);
          z2 = ((i + 1) * (length/divisions)) - (length/2);
          for (int j = 0; j < divisions; j++) {
              x1 = (j * (length/divisions)) - (length/2);
              x2 = ((j + 1) * (length/divisions)) - (length/2);
           
              str_vertices << x2 << ' ' << y << ' ' << z1 << '\n'; 
              str_vertices << x1 << ' ' << y << ' ' << z1 << '\n';
              str_vertices << x2 << ' ' << y << ' ' << z2 << '\n';

              str_vertices << x1 << ' ' << y << ' ' << z2 << '\n';
              str_vertices << x2 << ' ' << y << ' ' << z2 << '\n';
              str_vertices << x1 << ' ' << y << ' ' << z1 << '\n';
          }
      }
      
      
      fich << str_vertices.str();



      fich.close();

}


void menu(char **argv){

    if (!strcmp(argv[1], "sphere")){
           cria_esfera(std::stof(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]), argv[5]);
    }
    else if (!strcmp(argv[1], "box")){
           cria_caixa(std::stof(argv[2]), std::stoi(argv[3]), argv[4]);
    }
    else if (!strcmp(argv[1], "cone")){
           cria_cone(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), argv[6]);
    }
    else if (!strcmp(argv[1], "plane")){
           cria_plano(std::stof(argv[2]), std::stoi(argv[3]), argv[4]);
    }

    else{
        printf("Incorrect syntax.\n");
        printf("You must type:\n");
        printf("generator sphere [radius] [slices] [stacks] [3d filename]\n");
        printf("generator cone [radius] [height] [slices] [stacks] [3d filename]\n");
        printf("generator box [dimension] [number of divs per edge] [3d filename]\n");
        printf("generator plane [length] [divisions] [3d file name]");
    }


}


int main(int argc, char **argv){



	if ((argc <= 4) || (argc >= 8)){
		printf("Incorrect syntax.\n");
		printf("You must type:\n");
		printf("generator sphere [radius] [slices] [stacks] [3d filename]\n");
		printf("generator cone [radius] [height] [slices] [stacks] [3d filename]\n");
		printf("generator box [dimension] [number of divs per edge] [3d filename]\n");
		printf("generator plane [length] [divisions] [3d file name]");
	}

	menu(argv);
    
   

	return 0;

}