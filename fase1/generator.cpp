#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

std::ofstream cria_ficheiro_3d(std::string s){
    std::ofstream fich;
    fich.open("3d/"+s);
    return fich;

}

int cria_esfera(float radius, int slices, int stacks, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::string str_vertices = "";

      fich.close();
}

int cria_caixa(float length, int divisions, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::string str_vertices = "";

      fich.close();

}
 
int cria_cone(float radius, float height, int slices, int stacks, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::string str_vertices = "";

      fich.close();
}

int cria_plano(float length, int divisions, std::string filename){
      std::ofstream fich = cria_ficheiro_3d(filename);
      std::string str_vertices = "";
      str_vertices += "teste";
      fich << str_vertices;




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



	if ((argc <= 3) || (argc >= 7)){
		printf("Incorrect syntax.\n");
		printf("You must type:\n");
		printf("generator sphere [radius] [slices] [stacks] [3d filename]\n");
		printf("generator cone [radius] [height] [slices] [stacks] [3d filename]\n");
		printf("generator box [dimension] [number of divs per edge] [3d filename]\n");
		printf("generator plane [length] [divisions] [3d file name]");
	}

	menu(argv);

    //escrever em figura.txt o nome da figura para ler no engine
    std::ofstream fich;
    fich.open("engine/figura.txt");
    fich << argv[1];
    fich.close();
   

	return 0;

}