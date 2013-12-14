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
	// Declarações
	int i,j;	// Contador
	int fdi, fdo;				// Descritor do arquivo de entrada e de saida
	char name[128];				// Arquivo de entrada
	
	unsigned char temp1=0,temp2=0;
	int nlin = 0, ncol = 0;	// Numero de linha e colunas

	if (argc<2) {
		printf("Uso: %s nome_arquivo\n",argv[0]);
		exit(0);
	}
	if ((fdi=open(argv[1],O_RDONLY))==-1) {
		printf("Erro na abertura do arquivo %s\n",argv[1]);
		exit(0);
	}

	//	lê arquivo de imagem
	//	formato: 
	//		ncolunas (2bytes) - largura
	//		nlinhas (2bytes) - altura
	// 		dados: rgba (4bytes)

	if (read(fdi,&temp1,2)==-1) {
		printf("Erro na leitura do arquivo\n");		
		exit(0);
	}
	if (read(fdi,&temp2,2)==-1) {
		printf("Erro na leitura do arquivo\n");		
		exit(0);
	}
	ncol = temp1;
	ncol = ncol << 8;
	ncol = ncol & temp2;

	if (read(fdi,&nlin,2)==-1) {
		printf("Erro na leitura do arquivo\n");		
		exit(0);
	}
	printf("Tamanho da imagem: %d x %d\n",ncol,nlin);

	// ler pixels byte a byte, colocando-os em matrizes separadas
	for(i=0;i<nlin;i++) {
		for(j=0;j<ncol;j++) {
			read(fdi,&mr[i][j],1);
			read(fdi,&mg[i][j],1);
			read(fdi,&mb[i][j],1);
			read(fdi,&ma[i][j],1);
		}
	}
	close(fdi);
	printf("Terminou de ler imagem\n");

	// aplicar filtro (estêncil)

	// repetir para mr2, mg2, mb2, ma2
	#pragma omp parallel for
	for(i=2;i<nlin-2;i++) {
		for(j=2;j<ncol-2;j++) {
			// red
			mr2[i][j] = (int) (
						(	
							mr[i-2][j-2] + mr[i-2][j-1] + mr[i-2][j  ] + mr[i-2][j+1] + mr[i-2][j+2] +
							mr[i-1][j-2] + mr[i-1][j-1] + mr[i-1][j  ] + mr[i-1][j+1] + mr[i-1][j+2] +
							mr[i  ][j-2] + mr[i  ][j-1] + mr[i  ][j  ] + mr[i  ][j+1] + mr[i  ][j+2] +
							mr[i+1][j-2] + mr[i+1][j-1] + mr[i+1][j  ] + mr[i+1][j+1] + mr[i+1][j+2] +
							mr[i+2][j-2] + mr[i+2][j-1] + mr[i+2][j  ] + mr[i+2][j+1] + mr[i+2][j+2]
						) / 25);
			// green
			mg2[i][j] = (int) (
						(	
							mg[i-2][j-2] + mg[i-2][j-1] + mg[i-2][j  ] + mg[i-2][j+1] + mg[i-2][j+2] +
							mg[i-1][j-2] + mg[i-1][j-1] + mg[i-1][j  ] + mg[i-1][j+1] + mg[i-1][j+2] +
							mg[i  ][j-2] + mg[i  ][j-1] + mg[i  ][j  ] + mg[i  ][j+1] + mg[i  ][j+2] +
							mg[i+1][j-2] + mg[i+1][j-1] + mg[i+1][j  ] + mg[i+1][j+1] + mg[i+1][j+2] +
							mg[i+2][j-2] + mg[i+2][j-1] + mg[i+2][j  ] + mg[i+2][j+1] + mg[i+2][j+2]
						) / 25);
			// blue
			mb2[i][j] = (int) (
						(	
							mb[i-2][j-2] + mb[i-2][j-1] + mb[i-2][j  ] + mb[i-2][j+1] + mb[i-2][j+2] +
							mb[i-1][j-2] + mb[i-1][j-1] + mb[i-1][j  ] + mb[i-1][j+1] + mb[i-1][j+2] +
							mb[i  ][j-2] + mb[i  ][j-1] + mb[i  ][j  ] + mb[i  ][j+1] + mb[i  ][j+2] +
							mb[i+1][j-2] + mb[i+1][j-1] + mb[i+1][j  ] + mb[i+1][j+1] + mb[i+1][j+2] +
							mb[i+2][j-2] + mb[i+2][j-1] + mb[i+2][j  ] + mb[i+2][j+1] + mb[i+2][j+2]
						) / 25);
			// alpha
			ma2[i][j] = (int) (
						(	
							ma[i-2][j-2] + ma[i-2][j-1] + ma[i-2][j  ] + ma[i-2][j+1] + ma[i-2][j+2] +
							ma[i-1][j-2] + ma[i-1][j-1] + ma[i-1][j  ] + ma[i-1][j+1] + ma[i-1][j+2] +
							ma[i  ][j-2] + ma[i  ][j-1] + ma[i  ][j  ] + ma[i  ][j+1] + ma[i  ][j+2] +
							ma[i+1][j-2] + ma[i+1][j-1] + ma[i+1][j  ] + ma[i+1][j+1] + ma[i+1][j+2] +
							ma[i+2][j-2] + ma[i+2][j-1] + ma[i+2][j  ] + ma[i+2][j+1] + ma[i+2][j+2]
						) / 25);
		}
	}


	// tratar: linhas 0, 1, n, n-1; colunas 0,1,n,n-1


	// Centro Superior
	#pragma omp parallel for
	for(j=2;j<ncol-2;j++) {
		// red
		mr2[0][j] = (int) (
					(	
						mr[0][j-2] + mr[0][j-1] + mr[0][j  ] + mr[0][j+1] + mr[0][j+2] +
						mr[1][j-2] + mr[1][j-1] + mr[1][j  ] + mr[1][j+1] + mr[1][j+2] +
						mr[2][j-2] + mr[2][j-1] + mr[2][j  ] + mr[2][j+1] + mr[2][j+2]
					) / 25);
		// green
		mg2[0][j] = (int) (
					(	
						mg[0][j-2] + mg[0][j-1] + mg[0][j  ] + mg[0][j+1] + mg[0][j+2] +
						mg[1][j-2] + mg[1][j-1] + mg[1][j  ] + mg[1][j+1] + mg[1][j+2] +
						mg[2][j-2] + mg[2][j-1] + mg[2][j  ] + mg[2][j+1] + mg[2][j+2]
					) / 25);
		// blue
		mb2[0][j] = (int) (
					(	
						mb[0][j-2] + mb[0][j-1] + mb[0][j  ] + mb[0][j+1] + mb[0][j+2] +
						mb[1][j-2] + mb[1][j-1] + mb[1][j  ] + mb[1][j+1] + mb[1][j+2] +
						mb[2][j-2] + mb[2][j-1] + mb[2][j  ] + mb[2][j+1] + mb[2][j+2]
					) / 25);
		// alpha
		ma2[0][j] = (int) (
					(	
						ma[0][j-2] + ma[0][j-1] + ma[0][j  ] + ma[0][j+1] + ma[0][j+2] +
						ma[1][j-2] + ma[1][j-1] + ma[1][j  ] + ma[1][j+1] + ma[1][j+2] +
						ma[2][j-2] + ma[2][j-1] + ma[2][j  ] + ma[2][j+1] + ma[2][j+2]
					) / 25);
	}
	#pragma omp parallel for
	for (j=2;j<ncol-2;j++) {
		// red
		mr2[1][j] = (int) (
					(	
						mr[0][j-2] + mr[0][j-1] + mr[0][j  ] + mr[0][j+1] + mr[0][j+2] +
						mr[1][j-2] + mr[1][j-1] + mr[1][j  ] + mr[1][j+1] + mr[1][j+2] +
						mr[2][j-2] + mr[2][j-1] + mr[2][j  ] + mr[2][j+1] + mr[2][j+2] +
						mr[3][j-2] + mr[3][j-1] + mr[3][j  ] + mr[3][j+1] + mr[3][j+2]
					) / 25);
		// green
		mg2[1][j] = (int) (
					(	
						mg[0][j-2] + mg[0][j-1] + mg[0][j  ] + mg[0][j+1] + mg[0][j+2] +
						mg[1][j-2] + mg[1][j-1] + mg[1][j  ] + mg[1][j+1] + mg[1][j+2] +
						mg[2][j-2] + mg[2][j-1] + mg[2][j  ] + mg[2][j+1] + mg[2][j+2] +
						mg[3][j-2] + mg[3][j-1] + mg[3][j  ] + mg[3][j+1] + mg[3][j+2]
					) / 25);
		// blue
		mb2[1][j] = (int) (
					(	
						mb[0][j-2] + mb[0][j-1] + mb[0][j  ] + mb[0][j+1] + mb[0][j+2] +
						mb[1][j-2] + mb[1][j-1] + mb[1][j  ] + mb[1][j+1] + mb[1][j+2] +
						mb[2][j-2] + mb[2][j-1] + mb[2][j  ] + mb[2][j+1] + mb[2][j+2] +
						mb[3][j-2] + mb[3][j-1] + mb[3][j  ] + mb[3][j+1] + mb[3][j+2]
					) / 25);
		// alpha
		ma2[1][j] = (int) (
					(	
						ma[0][j-2] + ma[0][j-1] + ma[0][j  ] + ma[0][j+1] + ma[0][j+2] +
						ma[1][j-2] + ma[1][j-1] + ma[1][j  ] + ma[1][j+1] + ma[1][j+2] +
						ma[2][j-2] + ma[2][j-1] + ma[2][j  ] + ma[2][j+1] + ma[2][j+2] +
						ma[3][j-2] + ma[3][j-1] + ma[3][j  ] + ma[3][j+1] + ma[3][j+2]
					) / 25);
	}
	// Centro Inferior

	// Centro Esquerda

	// Centro Direita


	// gravar imagem resultante
	sprintf(name,"%s.new",argv[1]);	
	if((fdo=open(name,O_WRONLY)) < 0) {
		printf("Erro na abertura do arquivo de escrita\n");
		exit(0);
	}

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
	printf("Terminou de escrever imagem de saida\n");
	
	return 0;
}
