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
  cores cor_imagem;
  char tipo;
  int altura;
  int largura;
  unsigned char **pixel;
};
typedef struct imagem imagem;

// Funções utilizadas
int tam_diretorio(char *nome_dir);
void print_help();
void acessa_diretorio(char *nome_dir);
char verifica_tipo_pastilha(FILE *arq);
int verifica_tam_pastilha(FILE *arq);
#endif