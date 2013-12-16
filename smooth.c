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
#include <omp.h>

#define WIDTH  7680+4
#define HEIGHT 4320+4
#define LAYER 4

/*
unsigned char mr [WIDTH][HEIGHT];
unsigned char mg [WIDTH][HEIGHT];
unsigned char mb [WIDTH][HEIGHT];
unsigned char ma [WIDTH][HEIGHT];
unsigned char mr2 [WIDTH][HEIGHT];
unsigned char mg2 [WIDTH][HEIGHT];
unsigned char mb2 [WIDTH][HEIGHT];
unsigned char ma2 [WIDTH][HEIGHT];*/

unsigned char _mIn [LAYER][WIDTH][HEIGHT];
unsigned char _mOut [LAYER][WIDTH][HEIGHT];

int 
main(int argc, char **argv)
{
	// Declarações
	int i,j,k;	// Contador
	int fdi, fdo;				// Descritor do arquivo de entrada e de saida
	char name[128];				// Arquivo de entrada
	int nlin = 512, ncol = 512;	// Numero de linha e colunas

	if (argc<2) {
		printf("Uso: %s nome_arquivo\n",argv[0]);
		exit(0);
	}
	if ((fdi=open(argv[1],O_RDONLY))==-1) {
		printf("Erro na abertura do arquivo %s\n",argv[1]);
		exit(0);
	}
	memset(_mIn,0,LAYER*(nlin+4)*(ncol+4)*sizeof(unsigned char));
	memset(_mOut,0,LAYER*(nlin+4)*(ncol+4)*sizeof(unsigned char));


	//	lê arquivo de imagem
	//	formato: 
	// 		dados:	rgba(4bytes)

	printf("Tamanho da imagem: %d x %d\n",ncol,nlin);

	// ler pixels byte a byte, colocando-os em matrizes separadas
	for(i=2;i<nlin+2;i++) {
		for(j=2;j<ncol+2;j++) {
			for(k=0;k<LAYER;k++) {
				read(fdi,&_mIn[k][i][j],1);
			}
		}
	}
	close(fdi);
	printf("Terminou de ler imagem\n");

	// aplicar filtro (estêncil)

	#pragma omp parallel for private(j,k)
	for(i=2;i<nlin+2;i++) {
		for(j=2;j<ncol+2;j++) {
			for(k=0;k<LAYER;k++) {
				//_mOut[k][i][j] = _mIn[k][i][j];

				_mOut[k][i][j] = (unsigned char) (
						(	
							_mIn[k][i-2][j-2] + _mIn[k][i-2][j-1] + _mIn[k][i-2][j  ] + _mIn[k][i-2][j+1] + _mIn[k][i-2][j+2] +
							_mIn[k][i-1][j-2] + _mIn[k][i-1][j-1] + _mIn[k][i-1][j  ] + _mIn[k][i-1][j+1] + _mIn[k][i-1][j+2] +
							_mIn[k][i  ][j-2] + _mIn[k][i  ][j-1] + _mIn[k][i  ][j  ] + _mIn[k][i  ][j+1] + _mIn[k][i  ][j+2] +
							_mIn[k][i+1][j-2] + _mIn[k][i+1][j-1] + _mIn[k][i+1][j  ] + _mIn[k][i+1][j+1] + _mIn[k][i+1][j+2] +
							_mIn[k][i+2][j-2] + _mIn[k][i+2][j-1] + _mIn[k][i+2][j  ] + _mIn[k][i+2][j+1] + _mIn[k][i+2][j+2]
						) / 25);
			}
		}
	}


	// gravar imagem resultante
	sprintf(name,"%s.new",argv[1]);	
	if((fdo=open(name,O_WRONLY|O_CREAT)) < 0) {
		printf("Erro na abertura do arquivo de escrita\n");
		exit(0);
	}

	for(i=2;i<nlin+2;i++) {
		for(j=2;j<ncol+2;j++) {
			for(k=0;k<LAYER;k++) {
				write(fdo,&_mOut[k][i][j],1);
			}
		}
	}
	close(fdo);
	printf("Terminou de escrever imagem de saida\n");
	
	return 0;
}
