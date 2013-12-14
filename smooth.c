/**
** @author: Thiago Neves Romero
** Universidade Federal De São Carlos - UFSCar
** Departamento de Computação
** Algoritmo base do Prof. Dr. Helio Guardia
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define WIDTH  7680
#define HEIGHT 4320

unsigned char mr [WIDTH][HEIGHT];
unsigned char mg [WIDTH][HEIGHT];
unsigned char mb [WIDTH][HEIGHT];
unsigned char ma [WIDTH][HEIGHT];
unsigned char mr2 [WIDTH][HEIGHT];
unsigned char mg2 [WIDTH][HEIGHT];
unsigned char mb2 [WIDTH][HEIGHT];
unsigned char ma2 [WIDTH][HEIGHT];

int 
main(int argc, char **argv)
{
	int i,j;				// Contador
	int fdi, fdo;			// Descritor do arquivo de entrada e de saida
	int nlin = 0, ncol = 0;	// Numero de linha e colunas
	char name[128];			// Arquivo de entrada

	if (argc<2) {
		printf("Uso: %s nome_arquivo\n",argv[0]);
		exit(0);
	}
	if ((fdi=open(argv[1],O_RDONLY))==-1) {
		printf("Erro na abertura do arquivo %s\n",argv[1]);
		exit(0);
	}

	// lê arquivo de imagem
	// formato: 
	//		ncolunas (2bytes) - largura
	//		nlinhas (2bytes) - altura
	// dados: rgba (4bytes)

	if (read(fdi,&ncol,2)==-1) {
		printf("Erro na leitura do arquivo\n");		
		exit(0);
	}
	if (read(fdi,&nlin,2)==-1) {
		printf("Erro na leitura do arquivo\n");		
		exit(0);
	}
	printf("Tamanho da imagem: %d x %d\n",ncol,nlin);

	// (ao menos) 2 abordagens: 
	// - ler pixels byte a byte, colocando-os em matrizes separadas
	//	- ler pixels (32bits) e depois usar máscaras e rotações de bits para o processamento.

	// ordem de leitura dos bytes (componentes do pixel) depende se o formato
	// é little ou big endian
	// Assumindo little endian
	for(i=0;i<nlin;i++) {
		for(j=0;j<ncol;j++) {
			read(fdi,&mr[i][j],1);
			read(fdi,&mg[i][j],1);
			read(fdi,&mb[i][j],1);
			read(fdi,&ma[i][j],1);
		}
	}
	close(fdi);

	// aplicar filtro (estêncil)
	// repetir para mr2, mg2, mb2, ma2

	// tratar: linhas 0, 1, n, n-1; colunas 0,1,n,n-1
	// for

	// gravar imagem resultante
	sprintf(name,"%s.new",argv[1]);	
	fdo=open(name,O_WRONLY);

	write(fdo,&ncol,2);
	write(fdo,&nlin,2);

	for(i=0;i<nlin;i++) {
		for(j=0;j<ncol;j++) {
			write(fdo,&mr2[i][j],1);
			write(fdo,&mg2[i][j],1);
			write(fdo,&mb2[i][j],1);
			write(fdo,&ma2[i][j],1);
		}
	}
	close(fdo);
	
	return 0;
}
