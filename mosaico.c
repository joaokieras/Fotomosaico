#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include "imagens.h"

int main(int argc, char *argv[]){
  int flag_in = 1, flag_out = 1;
  char /**arq_in, *arq_out,*/ *caminho_dir;
  int option, tam_dir = 0;
  // Verifica opções de entrada 
  while((option = getopt(argc, argv, "i:o:p:h")) != -1)
    switch(option){
      case 'i':
        flag_in = 0;
        //arq_in = optarg;
        break;
      case 'o':
        flag_out = 0;
        //arq_out = optarg;
        break;
      case 'p':
        caminho_dir = optarg;
        break;
      case 'h':
        print_help();
        break;
    }
  // Conta pastilhas no diretorio
  tam_dir = tam_diretorio(caminho_dir);
  fprintf(stderr, "Lendo %d pastilhas do diretorio %s\n", tam_dir, caminho_dir);
  // Ler as pastilhas
  acessa_diretorio(caminho_dir);

  return 0;
}