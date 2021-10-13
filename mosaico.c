#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "imagens.h"

int main(int argc, char *argv[]){
  int flag_in = 0, flag_out = 0;
  char *arq_in, *arq_out, *caminho_dir;
  int option, tam_dir = 0, tam_pastilha = 0;

  // Verifica opções de entrada 
  while((option = getopt(argc, argv, "i:o:p:h")) != -1)
    switch(option){
      case 'i':
        flag_in = 1;
        arq_in = optarg;
        break;
      case 'o':
        flag_out = 1;
        arq_out = optarg;
        break;
      case 'p':
        caminho_dir = optarg;
        break;
      case 'h':
        print_help();
        exit(1);
        break;
    }
  // Conta pastilhas no diretorio
  fprintf(stderr, "Reading tiles from %s\n", caminho_dir);
  tam_dir = tam_diretorio(caminho_dir);
  fprintf(stderr, "%d tiles read\n", tam_dir);

  // Verifica propriedades das pastilhas (tipo e tamanho)
  tam_pastilha = verifica_diretorio(caminho_dir);
  fprintf(stderr, "Tile size is %dx%d\n", tam_pastilha, tam_pastilha);

  // Cria vetor de imagens contendo todas as pastilhas do diretório
  imagem *vetor_pastilhas;
  vetor_pastilhas = aloca_vetor(tam_dir);
  vetor_pastilhas = carrega_pastilhas(vetor_pastilhas, caminho_dir, tam_dir);

  // Processa entradas do programa
  // Arquivo de entrada
  int padrao = 0;
  if(flag_in)
    fprintf(stderr, "Entrada por opção -i\n");
  else
    padrao = 1;
  imagem *img_in;
  img_in = aloca_imagem();
  img_in = carrega_imagem(img_in, arq_in, stdin, padrao);
  fprintf(stderr, "Input imagem is PPM P%c, %dx%d pixels\n", img_in->tipo, img_in->altura, img_in->largura);
  fprintf(stderr, "Building mosaic image\n");
  img_in = constroi_mosaico(img_in, vetor_pastilhas, tam_dir); 

  // Arquivo de saída
  FILE *img_saida;
  if(flag_out){
    fprintf(stderr, "Saída por -o\n");
    if(!(img_saida = fopen(arq_out, "wb"))){
      fprintf(stderr, "Nao foi possivel abrir o arquivo!\n");
      exit(1);
    }
  }
  else
    img_saida = stdout;

  // Escreve arquivo de saída
  fprintf(stderr, "Writing output file\n");
  gera_imagem_saida(img_in, img_saida);
  libera_memoria(vetor_pastilhas, img_in, tam_dir);
  return 0;
}