#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
   int n;
   int m;
   int **values;
} Matrix;

Matrix *init_matrix(int n, int m){
	Matrix *A = (Matrix*)malloc(sizeof(Matrix));
	A->n = n;
	A->m = m;
	A->values = (int**)malloc(n*sizeof(int*));
	for(int i=0; i< n; i++){
		(A->values)[i] = (int*)malloc(m*sizeof(int));
	}
	return A;
}

Matrix *mmult(Matrix *A, Matrix *B){
	int i=0, j=0, k=0, sum=0;
	int an = A->n, am = A->m, **av = A->values;
	int bn = B->n, bm = B->m, **bv = B->values;
	
	if(am != bn){
		printf("array dimension mismatch\n");
		return NULL;
	}
	
	
	
	Matrix *C = init_matrix(an, bm);
	int **cv = C->values;
	
	for(i=0; i < bm; i++){
		for(j=0; j<an; j++){
			sum = 0;
			for(k=0; k < am; k++){
				sum = sum + av[j][k]*bv[k][i];
			}
			cv[j][i]=sum;			
		}
	}		
	
	return C;
	
}



void destroy_matrix(Matrix *A){
	int n = A->n;
	for(int i=0; i< n; i++){
		free((A->values)[i]);
	}
	free(A->values);
	free(A);
}

void print_matrix(Matrix *A){
	int n = A->n, m = A->m, i, j;
	int ** vals = A->values;
	for(i = 0; i < n; i++) {
		for(j = 0; j < m; j++){
			printf("%d ", vals[i][j]);		
		}
		printf("\n");		
	}
	printf("\n");		
}




int main() {
	
	Matrix *A = init_matrix(3, 3);
	Matrix *B = init_matrix(3, 2);
	
	(A->values)[0][0] = 0;
	(A->values)[0][1] = 1;
	(A->values)[0][2] = 2;
	(A->values)[1][0] = 3;
	(A->values)[1][1] = 4;
	(A->values)[1][2] = 5;
	(A->values)[2][0] = 1;
	(A->values)[2][1] = 1;
	(A->values)[2][2] = 1;
	
	(B->values)[0][0] = 0;
	(B->values)[0][1] = 1;
	(B->values)[1][0] = 2;
	(B->values)[1][1] = 3;
	(B->values)[2][0] = 4;
	(B->values)[2][1] = 5;
	
	print_matrix(A);
	print_matrix(B);
	
	Matrix *C = mmult(A, B);
	print_matrix(C);
	
			
	destroy_matrix(A);
	destroy_matrix(B);
	destroy_matrix(C);
	
	
	
	
	
	
	
	return 0;
}
