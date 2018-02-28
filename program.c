#include <stdio.h>

int verificar_fila(int mat[9][9], int row, int num){
	for (int col = 0; col < 9; col++) {
		if (mat[row][col] == num) {
			return 1;
		}
	}
	return 0;
}

int verificar_columna(int mat[9][9], int col, int num) {
	for (int row = 0; row < 9; row++) {
		if (mat[row][col] == num) {
			return 1;
		}
	}
	return 0;
}

int verificar_sector(int mat[9][9], int startRow, int startCol, int num) {
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			if (mat[row + startRow][col + startCol] == num) {
				return 1;
			} 
		}
	}
	return 0;
}

int numero_valido(int mat[9][9], int row, int col, int num) {
	return !verificar_fila(mat, row, num) 
			&& !verificar_columna(mat, col, num) 
			&& !verificar_sector(mat, row - (row % 3), col - (col %3), num);
}

int es_vacia(int mat[9][9], int *row, int *col) {
	for (*row = 0; *row < 9; (*row)++) {
		for (*col = 0; *col < 9; (*col)++) {
			if (mat[*row][*col] == 0) {
				return 1;
			}
		}
	}
	return 0;
}

int resolver(int mat[9][9]) {
	int row = 0;
	int col = 0;
	if (!es_vacia(mat, &row, &col)){
		return 1;
	}
	for (int num = 1; num <= 9; num++ ) {
		if (numero_valido(mat, row, col, num)) {
			mat[row][col] = num;
			if (resolver(mat)) {
				return 1;
			}
			mat[row][col] = 0;
		}
	}
	
	return 0; // SI LLEGA AQUI NO TIENE SOLUCION
}

void print_mat(int mat[9][9]) {
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			printf("%2d", mat[row][col]);
		}
		printf("\n");
	}
}

int main() {
	
	int mat[9][9] = {
             {0,0,0, 0,0,3, 2,9,0},
			 {0,8,6, 5,0,0, 0,0,0},
			 {0,2,0, 0,0,1, 0,0,0},
			 {0,0,3, 7,0,5, 1,0,0},
			 {9,0,0, 0,0,0, 0,0,8},
			 {0,0,2, 9,0,8, 3,0,0},
			 {0,0,0, 4,0,0, 0,8,0},
			 {0,4,7, 1,0,0, 0,0,0}};
	
	if (resolver(mat)) {
		print_mat(mat);
	} else {
		printf("SIN SOLUCION");
	}	
	return 0;
}