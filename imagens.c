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
  fprintf(stderr, "Digite -p para indicar o diretório de pastilhas\n");  
  fprintf(stderr, "~$ ./mosaico -i input.ppm -o output.ppm -p diretorio/\n");  
}

void print_erro(){
  fprintf(stderr, "Erro na alocação de memória!\n");
  exit(1);
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
  closedir(diretorio_pastilhas);
  return cont_dir - 2; // tam-2 por conta dos arquivos "." e ".."
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
  	  	fprintf(stderr, "Tipo de pastilha invalido! (P%c) Tente novamente\n", tipo_pastilha);
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
  char caminho_dir[100], tipo_pastilha[2];
  int tam_pastilha1, tam_pastilha2, max_brilho, i = 0;
  
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
  	  fscanf(pastilha, "%s", tipo_pastilha);
  	  if(tipo_pastilha[1] != '3' && tipo_pastilha[1] != '6'){
  	  	fprintf(stderr, "Tipo de pastilha invalido! (P%c) Tente novamente\n", tipo_pastilha[1]);
  	  	exit(1);
  	  }
  	  fscanf(pastilha, "%d", &tam_pastilha1);
  	  fscanf(pastilha, "%d", &tam_pastilha2);
  	  fscanf(pastilha, "%d", &max_brilho);
  	  getc(pastilha); // \n
  	  // Faz a cópia de dados da pastilha para struct imagem
  	  vetor[i].tipo = tipo_pastilha[1];
  	  vetor[i].altura = tam_pastilha1;
  	  vetor[i].largura = tam_pastilha2;
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
  //fprintf(stderr, "%d imagens carregadas na memoria\n", i); 
  fprintf(stderr, "Calculating tiles' average colors\n");
  return vetor;
}

// Para calcular a cor de PPM P3 é utilizado fscanf
struct imagem calcula_cor_P3(struct imagem *img, FILE *arq){
  float r = 0, g = 0, b = 0;
  int i, j, numero, total;
  
  total = img->altura * img->largura;
  // Neste método é necessário ler 1 número por vez
  // Copia números da imagem PPM para matriz pixel[i][j]
  for(i = 0;i < img->altura;i++)
  	for(j = 0;j < (img->largura * 3);j++){
  		fscanf(arq, "%d", &numero);
  		img->pixel[i][j] = numero;
  	}

  for(i = 0;i < img->altura;i++)
  	for(j = 0;j < (img->largura * 3) - 2;j += 3){
  		r += (int)img->pixel[i][j] * (int)img->pixel[i][j];
  		g += (int)img->pixel[i][j + 1] * (int)img->pixel[i][j + 1];
  		b += (int)img->pixel[i][j + 2] * (int)img->pixel[i][j + 2];
  	}
  // Soma dos quadrados das cores dividido pelas raizes das médias
  img->cor_imagem = malloc(sizeof(cores));
  if(img->cor_imagem == NULL)
  	print_erro();
  img->cor_imagem->r = sqrt(r/total);
  img->cor_imagem->g = sqrt(g/total);
  img->cor_imagem->b = sqrt(b/total);	
  return *img;
}

// Para calcular a cor de PPM P6 é utilizado fread
struct imagem calcula_cor_P6(struct imagem *img, FILE *arq){
  float r = 0, g = 0, b = 0;
  int i, j, total = 0;

  total = img->altura * img->largura;
  // Lê linha por linha do arquivo da pastilha (blocos de números)
  for(i = 0;i < img->altura;i++)
  	fread(img->pixel[i], 1, 3 * img->largura, arq);

  for(i = 0;i < img->altura;i++)
  	for(j = 0;j < (img->largura * 3) - 2;j += 3){
  		r += (int)img->pixel[i][j] * (int)img->pixel[i][j];
  		g += (int)img->pixel[i][j + 1] * (int)img->pixel[i][j + 1];
  		b += (int)img->pixel[i][j + 2] * (int)img->pixel[i][j + 2];
  	}

  img->cor_imagem = malloc(sizeof(cores));
  if(img->cor_imagem == NULL)
  	print_erro();
  img->cor_imagem->r = sqrt(r/total);
  img->cor_imagem->g = sqrt(g/total);
  img->cor_imagem->b = sqrt(b/total);	
  return *img;
}

struct imagem *carrega_imagem(struct imagem *img, char *nome_img, FILE *arq, int padrao){
  int tam_img1, tam_img2, max_brilho;
  char img_tipo[2];
  FILE *aux;

  if(padrao)
  	aux = arq;
  else{
  	aux = fopen(nome_img, "r");
    if(!aux){
  	  perror("Erro ao abrir imagem\n");
  	  exit(1);
    }
  }
  fscanf(aux, "%s", img_tipo);
  fscanf(aux, "%d", &tam_img1);
  fscanf(aux, "%d", &tam_img2);
  fscanf(aux, "%d", &max_brilho);
  getc(aux);
  aloca_pixel(img);
  img->tipo = img_tipo[1];
  img->altura = tam_img2;
  img->largura = tam_img1;
  if(img->tipo == '6')
  	*img = calcula_cor_P6(img, aux);
  else
    *img = calcula_cor_P3(img, aux);
  fclose(aux);
  return img;
}

struct imagem *constroi_mosaico(struct imagem *img, struct imagem *vetor, int tam_vetor){
  int tam_pastilha, i, j, m, n, menor_index = 0, lim_altura, lim_largura;
  struct cores *cor;

  cor = malloc(sizeof(cor));
  if(cor == NULL)
    print_erro();

  tam_pastilha = vetor[0].altura;
  lim_altura = (img->altura % tam_pastilha);
  lim_largura = ((img->largura * 3) % (3 * tam_pastilha));

  for(i = 0;i < img->altura - lim_altura;i += tam_pastilha)
    for(j = 0;j < (img->largura * 3) - lim_largura;j += 3 * tam_pastilha){
      // Calcula cor media de blocos Nx3N dentro da img principal
      cor = calcula_cor_img(img, i, j, tam_pastilha, cor);
      menor_index = procura_pastilha(cor, vetor, tam_vetor);
      // Substitui pastilhas na img principal
      for(m = 0;m < tam_pastilha;m++)
        for(n = 0;n < 3*tam_pastilha;n += 3){
          img->pixel[i + m][j + n] = vetor[menor_index].pixel[m][n];
          img->pixel[i + m][j + n + 1] = vetor[menor_index].pixel[m][n + 1];
          img->pixel[i + m][j + n + 2] = vetor[menor_index].pixel[m][n + 2];
        }
    }
  return img;
}

void gera_imagem_saida(struct imagem *img_in, FILE *img_out){
  int i, j;

  fprintf(img_out, "P%c\n", img_in->tipo);
  fprintf(img_out, "%d %d\n", img_in->largura, img_in->altura);
  fprintf(img_out, "%d\n", 255);

  if(img_in->tipo == '6'){
    for(i = 0;i < img_in->altura;i++)
      fwrite(img_in->pixel[i], 1, 3*img_in->largura, img_out);
  }
  else{
    fwrite("\n", sizeof(char), 1, img_out);
    for(i = 0;i < img_in->altura;i++)
      for(j = 0;j < 3*img_in->largura;j++)
        fprintf(img_out, "%d ", img_in->pixel[i][j]);
  }
  fclose(img_out);
}

int procura_pastilha(struct cores *cor, struct imagem *vetor, int tam_vetor){
  int k, menor_index = 0;
  float distancia = 0, menor = 9999999;
  float r = 0, g = 0, b = 0;

  for(k = 0;k < tam_vetor;k++){
    r = (cor->r - vetor[k].cor_imagem->r) * (cor->r - vetor[k].cor_imagem->r);
    g = (cor->g - vetor[k].cor_imagem->g) * (cor->g - vetor[k].cor_imagem->g);
    b = (cor->b - vetor[k].cor_imagem->b) * (cor->b - vetor[k].cor_imagem->b);

    distancia = r + g + b;
    distancia = sqrt(distancia);

    if(distancia < menor){
      menor = distancia;
      menor_index = k;
    }
  }
  return menor_index;
}

struct cores *calcula_cor_img(struct imagem *img, int lin, int col, int tam_pastilha, struct cores *cor){
  long int i, j;
  float r = 0, g = 0, b = 0;
  int lim_lin, lim_col, total = 0;

  total = tam_pastilha * tam_pastilha;
  lim_lin = lin + tam_pastilha;
  lim_col = col + 3*tam_pastilha;
  cor->r = 0;
  cor->g = 0;
  cor->b = 0;
  for(i = lin;i < lim_lin;i++){
    for(j = col;j < lim_col - 2;j += 3){
      r += (int)(img->pixel[i][j]) * (int)(img->pixel[i][j]);
      g += (int)(img->pixel[i][j + 1]) * (int)(img->pixel[i][j + 1]);
      b += (int)(img->pixel[i][j + 2]) * (int)(img->pixel[i][j + 2]);
    }
  }
  cor->r = sqrt(r/total);
  cor->g = sqrt(g/total);
  cor->b = sqrt(b/total);
  // Trata imagens mais escuras
  if(cor->r < 75 && cor->g < 75 && cor->b < 75){
  	cor->r = cor->r * 1.5;
  	cor->g = cor->g * 1.5;
  	cor->b = cor->b * 1.5;
  }
  return cor;
}

struct imagem *aloca_vetor(int tam_vetor){
  struct imagem *vetor;
  vetor = malloc(tam_vetor * sizeof(*vetor));
  if(vetor == NULL)
  	print_erro();
  //fprintf(stderr, "Vetor de imagens criado\n");
  return vetor;
}

void aloca_pixel(struct imagem *img){
  int i;

  img->pixel = malloc(img->altura * sizeof(img->pixel));
  for(i = 0;i < img->altura;i++)
  	img->pixel[i] = malloc(3 * img->altura * sizeof(img->pixel));

  if(img->pixel == NULL)
  	print_erro();
}

struct imagem *aloca_imagem(){
  struct imagem *img;
  img = malloc(sizeof(*img));
  if(img == NULL)
  	print_erro();
  return img;
}

void libera_memoria(struct imagem *vetor, struct imagem *img, int tam){
  int i, j;

  for(i = 0;i < tam;i++){
    for(j = 0;j < vetor[i].altura;j++)
      free(vetor[i].pixel[j]);
    free(vetor[i].pixel);
  }
  free(vetor);

  for(i = 0;i < img->altura;i++)
    free(img->pixel[i]);
  free(img->pixel);
  free(img);
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