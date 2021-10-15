#ifndef __IMAGENS__
#define __IMAGENS__

// Estruturas utilizadas
struct cores{
  float r;
  float g;
  float b;
};
typedef struct cores cores; 

struct imagem{
  cores *cor_imagem;
  char tipo;
  int altura;
  int largura;
  unsigned char **pixel;
};
typedef struct imagem imagem;

// Funções utilizadas
char verifica_tipo_pastilha(FILE *arq);
int tam_diretorio(char *nome_dir);
int verifica_diretorio(char *nome_dir);
int verifica_tam_pastilha(FILE *arq);
int procura_pastilha(cores *cor, imagem *vetor, int tam_vetor);
void print_help();
void aloca_pixel(imagem *img);
void print_erro();
void libera_memoria(imagem *vetor, imagem *img, int tam);
void gera_imagem_saida(imagem *img_in, FILE *img_out);
cores *calcula_cor_img(imagem *img, int lin, int col, int tam_pastilha, cores *cor);
imagem *aloca_vetor(int tam_vetor);
imagem *carrega_pastilhas(imagem *vetor, char *nome_dir, int tam_dir);
imagem calcula_cor_P3(imagem *img, FILE *arq);
imagem calcula_cor_P6(imagem *img, FILE *arq);
imagem *aloca_imagem();
imagem *carrega_imagem(imagem *img, char *nome_img, FILE *arq, int padrao);
imagem *constroi_mosaico(imagem *img, imagem *vetor, int tam_vetor);
#endif