#include <stdio.h>
typedef enum { false, true } bool;

int celdas_base[9][9];
int mat[9][9] = {{0,0,0, 0,0,0, 0,0,0},
				 {0,0,0, 0,0,0, 0,0,0},
				 {0,0,0, 0,0,0, 0,0,0},
				 
				 {0,0,0, 1,0,0, 0,0,0},
				 {0,0,0, 0,0,0, 0,0,0},
				 {0,0,0, 0,0,1, 0,0,0},
				 
				 {0,0,0, 0,0,0, 0,0,0},
				 {0,0,0, 0,0,0, 0,0,0},
				 {0,0,0, 0,0,0, 0,0,0}};

int i, j, k, l;


bool verificarFilas(int fila, int columna, int num){
	for(k = 0; k < 9; k++){
		if(k != columna && mat[fila][k] == num){
			printf("Fila: %d->%d-%d = %d->%d-%d\n", mat[fila][k], fila, k, num, fila, columna);
			return false;
		}
	}
	printf("Fila está bien\n");
	return true;
}

bool verificarColumnas(int fila, int columna, int num){
	for(k = 0; k < 9; k++){
		if(k != fila && mat[k][columna] == num){
			printf("Columna: %d->%d-%d = %d->%d-%d\n", mat[k][columna], k, columna, num, fila, columna);
			return false;
		}
	}
	printf("Columna está bien\n");
	return true;
}

bool verificarZona(int fila, int columna, int inicioFila, int inicioColumna, int num){
	for (int k = 0; k < 3; k++){
        for (int l = 0; l < 3; l++){
            if(k != fila && l != columna){
				if (mat[k + inicioFila][l + inicioColumna] == num){
					printf("Caja: %d->%d-%d = %d->%d-%d\n", mat[k+ inicioFila][l+ inicioColumna], k+ inicioFila, l+ inicioColumna, num, fila, columna);
					return false;
					break;
				}
			}
		}
	}
	printf("Caja está bien\n");
    return true;
}

bool verificar(){
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(celdas_base[i][j] == 1){
				return 	verificarFilas(i, j, mat[i][j]) &&
						verificarColumnas(i, j, mat[i][j]) && 
						verificarZona(i, j, (i - i%3) , (j - j%3), mat[i][j]);
			}
		}
	}
}

int main() {
	for(i = 0; i < 9; i++){				//Revisa las filas
        for(j = 0; j < 9; j++){			//Revisa las columnas
            if(mat[i][j] == 0)			//Si es cero escribe que no había celdas base
                celdas_base[i][j] = 0;	
            else{						//Si tiene cualquier otra cosa escribe un uno si había algo
				celdas_base[i][j] = 1;
			}
        }
    }

    if(verificar() == true)
		printf("ok");
    else
		printf("No solution exists");
    
	return 0;
}
