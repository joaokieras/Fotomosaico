#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include "imagens.h"

void print_help(){
	fprintf(stderr, "Este programa gera fotomosaicos a partir de uma imagem de entrada!\n");
    fprintf(stderr, "Digite -i para indicar a imagem de entrada\n");
    fprintf(stderr, "Digite -o para indicar a imagem de saida\n");
	fprintf(stderr, "Digite -p para indicar o diretorio de pastilhas\n");    
}

int tam_diretorio(char *nome_dir){
  DIR *diretorio_pastilhas;
  struct dirent *dir;
  int cont_dir = 0;
  diretorio_pastilhas = opendir(nome_dir);
  if(!diretorio_pastilhas){
  	perror("Não foi possível abrir o diretório.\n");
  	exit(1);
  }
  while((dir = readdir(diretorio_pastilhas)) != NULL)
  	cont_dir++;
  return cont_dir - 2; // tam-2 por conta dos arquivos "." e ".."
  closedir(diretorio_pastilhas);
}

void acessa_diretorio(char *nome_dir){
  FILE *pastilha;
  DIR *diretorio_pastilhas;
  struct dirent *dir;
  char caminho_dir[100], tipo_pastilha;
  int tam_pastilha;
  
  strcpy(caminho_dir, nome_dir);
  diretorio_pastilhas = opendir(nome_dir);
  if(!diretorio_pastilhas){
  	perror("Não foi possível abrir o diretório.\n");
  	exit(1);
  }
  dir = readdir(diretorio_pastilhas);
  if(!dir){
  	perror("Não foi possível acessar as pastilhas\n");
  	exit(1);
  }
  printf("Acessando diretorio %s\n", nome_dir);
  while(dir){
  	if(dir->d_type == DT_REG){
  	  strcat(caminho_dir, dir->d_name);
  	  pastilha = fopen(caminho_dir, "r");
  	  tipo_pastilha = verifica_tipo_pastilha(pastilha);
  	  tam_pastilha = verifica_tam_pastilha(pastilha);
  	  memset(caminho_dir, 0, strlen(caminho_dir));
  	  strcpy(caminho_dir, nome_dir);
  	  fclose(pastilha);
  	}
  	dir = readdir(diretorio_pastilhas);
  }
  printf("Tamanho das pastilhas: %d\n", tam_pastilha);
  printf("Pastilhas lidas com sucesso\n");
}


char verifica_tipo_pastilha(FILE *arq){
  char p_tipo[2];
  fscanf(arq, "%s\n", p_tipo);
  return p_tipo[1];	
}

int verifica_tam_pastilha(FILE *arq){
  int tam;
  fscanf(arq, "%d", &tam);
  return tam;
}