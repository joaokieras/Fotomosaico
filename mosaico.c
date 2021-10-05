#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "imagens.h"

int main(int argc, char *argv[]){

  //int flag_in = 1, flag_out = 1;
  char /**arq_in, *arq_out,*/ *caminho_dir;
  int option, tam_dir = 0, tam_pastilha = 0;

  // Verifica opções de entrada 
  while((option = getopt(argc, argv, "i:o:p:h")) != -1)
    switch(option){
      case 'i':
        //flag_in = 0;
        //arq_in = optarg;
        break;
      case 'o':
        //flag_out = 0;
        //arq_out = optarg;
        break;
      case 'p':
        caminho_dir = optarg;
        break;
      case 'h':
        print_help();
        exit(1);
        break;
    }

  fprintf(stderr, "Lendo pastilhas de %s\n", caminho_dir);
  // Conta pastilhas no diretorio
  tam_dir = tam_diretorio(caminho_dir);
  fprintf(stderr, "%d pastilhas lidas\n", tam_dir);
  // Verifica propriedades das pastilhas (tipo e tamanho)
  tam_pastilha = verifica_diretorio(caminho_dir);
  fprintf(stderr, "Tamanho das pastilhas %dx%d\n", tam_pastilha, tam_pastilha);

  imagem *vetor_pastilhas;
  vetor_pastilhas = aloca_vetor(vetor_pastilhas, tam_dir);
  vetor_pastilhas = carrega_pastilhas(vetor_pastilhas, caminho_dir, tam_dir);


  return 0;
}