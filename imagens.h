#ifndef __IMAGENS__
#define __IMAGENS__

// Estruturas utilizadas
struct cores{
  int r;
  int g;
  int b;
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
int tam_diretorio(char *nome_dir);
int verifica_diretorio(char *nome_dir);
int verifica_tam_pastilha(FILE *arq);
char verifica_tipo_pastilha(FILE *arq);
void print_help();
struct imagem *aloca_vetor(struct imagem *vetor, int tam_vetor);
void aloca_pixel(struct imagem *img);
struct imagem *carrega_pastilhas(struct imagem *vetor, char *nome_dir, int tam_dir);

#endif