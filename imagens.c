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
	fprintf(stderr, "~$ ./mosaico -i input.ppm -o output.ppm -p diretorio/\n");  
}

// Percorre o diretório e conta quantas pastilhas existem
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

// Percorre o diretório verificando o tipo e o tamanho de cada pastilha
int verifica_diretorio(char *nome_dir){
  FILE *pastilha;
  DIR *diretorio_pastilhas;
  struct dirent *dir;
  char caminho_dir[100], tipo_pastilha;
  int tam_pastilha;
  
  // Concatena caminho_dir = "tilesxx/"
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
  while(dir){
  	// Se o tipo de arquivo no diretório for regular
  	if(dir->d_type == DT_REG){
  	  // Concatena caminho_dir = "tilesxx/nome_da_imagem.ppm"
  	  strcat(caminho_dir, dir->d_name);
  	  pastilha = fopen(caminho_dir, "r");
  	  tipo_pastilha = verifica_tipo_pastilha(pastilha);
  	  if(tipo_pastilha != '3' && tipo_pastilha != '6'){
  	  	printf("Tipo de pastilha invalido! (P%c) Tente novamente\n", tipo_pastilha);
  	  	exit(1);
  	  }
  	  tam_pastilha = verifica_tam_pastilha(pastilha);
  	  // caminho_dir = "" - reseta o caminho no diretório
  	  memset(caminho_dir, 0, strlen(caminho_dir));
  	  // caminho_dir = "tilesxx/" - restaura o nome do diretório
  	  strcpy(caminho_dir, nome_dir);
  	  fclose(pastilha);
  	}
  	// Próx arquivo do diretório
  	dir = readdir(diretorio_pastilhas);
  }
  closedir(diretorio_pastilhas);
  return tam_pastilha;
}

// Realiza a cópia de dados da pastilha para o vetor de imagens
struct imagem *carrega_pastilhas(struct imagem *vetor, char *nome_dir, int tam_dir){
  FILE *pastilha;
  DIR *diretorio_pastilhas;
  struct dirent *dir;
  char caminho_dir[100], tipo_pastilha;
  int tam_pastilha, i = 0;
  
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
  while(dir && i < tam_dir){
  	if(dir->d_type == DT_REG){
  	  strcat(caminho_dir, dir->d_name);
  	  pastilha = fopen(caminho_dir, "r");
  	  tipo_pastilha = verifica_tipo_pastilha(pastilha);
  	  if(tipo_pastilha != '3' && tipo_pastilha != '6'){
  	  	printf("Tipo de pastilha invalido! (P%c) Tente novamente\n", tipo_pastilha);
  	  	exit(1);
  	  }
  	  tam_pastilha = verifica_tam_pastilha(pastilha);
  	  // Faz a cópia de dados da pastilha para struct imagem
      vetor[i].tipo = tipo_pastilha;
      vetor[i].altura = tam_pastilha;
      vetor[i].largura = tam_pastilha;
      aloca_pixel(&vetor[i]);
      if(vetor[i].tipo == '6')
      	vetor[i] = calcula_cor_P6(&vetor[i], pastilha);
      else
      	vetor[i] = calcula_cor_P3(&vetor[i], pastilha);
      // -----------------------------------------------------
  	  memset(caminho_dir, 0, strlen(caminho_dir));
  	  strcpy(caminho_dir, nome_dir);
  	  fclose(pastilha);
  	  i++;
  	}
  	dir = readdir(diretorio_pastilhas);
  }
  closedir(diretorio_pastilhas); 
  fprintf(stderr, "%d imagens carregadas na memoria\n", i); 
  fprintf(stderr, "Calculando cores...\n");
  return vetor;
}

// Para calcular a cor de PPM P3 é utilizado fscanf
struct imagem calcula_cor_P3(struct imagem *img, FILE *arq){
  unsigned char r, g, b;
  int int_r = 0, int_g = 0, int_b = 0;
  int i, j, total, numero;
  // 32x32 ou 20x20
  total = img->altura * img->altura;
  fseek(arq, 13, SEEK_SET);
  // Neste método é necessário ler 1 número por vez
  for(i = 0;i < img->altura;i++)
  	for(j = 0;j < (img->largura * 3);j++){
  		fscanf(arq, "%d", &numero);
  		img->pixel[i][j] = numero;
  	}

  for(i = 0;i < img->altura;i++)
  	for(j = 0;j < (img->largura * 3) - 2;j += 3){
  		r = img->pixel[i][j];
  		g = img->pixel[i][j + 1];
  		b = img->pixel[i][j + 2];
        // (r * r)
  		int_r += (int)r * (int)r;
  		int_g += (int)g * (int)g;
  		int_b += (int)b * (int)b;
  	}
  // Soma dos quadrados das cores dividido pelas raizes das médias
  img->cor_imagem = malloc(sizeof(cores));
  img->cor_imagem->r = sqrt(int_r/total);
  img->cor_imagem->g = sqrt(int_g/total);
  img->cor_imagem->b = sqrt(int_b/total);	
  return *img;
}

// Para calcular a cor de PPM P6 é utilizado fread
struct imagem calcula_cor_P6(struct imagem *img, FILE *arq){
  unsigned char r, g, b;
  int int_r = 0, int_g = 0, int_b = 0;
  int i, j, total;

  total = img->altura * img->altura;
  // Lê linha por linha do arquivo da pastilha (blocos de números)
  for(i = 0;i < img->altura;i++)
  	fread(img->pixel[i], 1, 3 * img->largura, arq);

  for(i = 0;i < img->altura;i++)
  	for(j = 0;j < (img->largura * 3) - 2;j += 3){
  		r = img->pixel[i][j];
  		g = img->pixel[i][j + 1];
  		b = img->pixel[i][j + 2];

  		int_r += (int)r * (int)r;
  		int_g += (int)g * (int)g;
  		int_b += (int)b * (int)b;
  	}

  img->cor_imagem = malloc(sizeof(cores));
  img->cor_imagem->r = sqrt(int_r/total);
  img->cor_imagem->g = sqrt(int_g/total);
  img->cor_imagem->b = sqrt(int_b/total);	
  return *img;
}

struct imagem *aloca_vetor(struct imagem *vetor, int tam_vetor){
  vetor = malloc(tam_vetor * sizeof(*vetor));
  if(vetor == NULL){
  	fprintf(stderr, "Erro na alocacao de vetor!\n");
  	exit(1);
  }
  fprintf(stderr, "Vetor de imagens criado\n");
  return vetor;
}

void aloca_pixel(struct imagem *img){
  int i;

  img->pixel = malloc(img->altura * sizeof(img->pixel));
  for(i = 0;i < img->altura;i++)
  	img->pixel[i] = malloc(3 * img->altura * sizeof(img->pixel));

  if(img->pixel == NULL){
  	fprintf(stderr, "Erro ao alocar matriz de pixel!\n");
  	exit(1);
  }
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