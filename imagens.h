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
char verifica_tipo_pastilha(FILE *arq);
int tam_diretorio(char *nome_dir);
int verifica_diretorio(char *nome_dir);
int verifica_tam_pastilha(FILE *arq);
int procura_pastilha(struct cores *cor, struct imagem *vetor, int tam_vetor);
void print_help();
void aloca_pixel(struct imagem *img);
void print_erro();
void libera_memoria(struct imagem *vetor, struct imagem *img, int tam);
void gera_imagem_saida(struct imagem *img_in, FILE *img_out);
struct cores *calcula_cor_img(struct imagem *img, int lin, int col, int tam_pastilha, struct cores *cor);
struct imagem *aloca_vetor(int tam_vetor);
struct imagem *carrega_pastilhas(struct imagem *vetor, char *nome_dir, int tam_dir);
struct imagem calcula_cor_P3(struct imagem *img, FILE *arq);
struct imagem calcula_cor_P6(struct imagem *img, FILE *arq);
struct imagem *aloca_imagem();
struct imagem *carrega_imagem(struct imagem *img, char *nome_img, FILE *arq, int padrao);
struct imagem *constroi_mosaico(struct imagem *img, struct imagem *vetor, int tam_vetor);
#endif