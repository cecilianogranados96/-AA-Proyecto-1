
#include<stdio.h>
#include <stdbool.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>

int mat[9][9];




void imprimir_matriz(int mat[9][9])
{
    int row;
    int col;
    printf("\n------------------------------------------------------\n");
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            printf("  %d  |", mat[row][col]);
        }
        printf("\n------------------------------------------------------\n");
    }
}

bool siguiente_casilla_vacia(int mat[9][9], int *row, int *col)
{
    int i;
    int j;
    
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if(mat[i][j] == 0) {
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    return false;
}

bool verificar_fila(int mat[9][9], int row, int num)
{
    int i;
    int j;
    
    for(i = 0; i < 9; i++) {
        if(mat[row][i] != 0) {
            if(mat[row][i] == 0) {
                continue;
            }
            if(mat[row][i] == num) {
                //printf("Conflict along row: %d\n", row);
                return false;
            } 
        }
    }
    return true;
}

bool verificar_columna(int mat[9][9], int col, int num)
{
    int i;
    int j;
    int found[9];
    memset(found, 0, sizeof(found));
    
    for(i = 0; i < 9; i++) {
        if(mat[i][col] != 0) {
            if(mat[i][col] == 0) {
                continue;
            }
            if(mat[i][col] == num) {
                //printf("Conflict along col: %d\n", col);
                return false;
            }
        }
    }
    return true;
}

bool verificar_bloque(int mat[9][9], int row, int col, int num)
{
    int i;
    int j;
    int box_row_index;
    int box_col_index;
    int icount = 0;
    int jcount = 0;
    
    box_row_index = (row / 3) * 3;
    box_col_index = (col / 3) * 3;
    
    for(i = box_row_index, icount = 0; icount < 3; i++, icount++) {
        for(j = box_col_index, jcount = 0; jcount < 3; j++, jcount++) {
            if(mat[i][j] == 0) {
                continue;
            }
            if(mat[i][j] == num) {
                return false;
            }
        }
    }
    return true;
}

bool verificar_conflictos(int mat[9][9], int row, int col, int num)
{
    return (verificar_fila(mat, row, num) &&
            verificar_columna(mat, col, num) &&
            verificar_bloque(mat, row, col, num));
}

bool verificar_soduku(int mat[9][9])
{
    int i;
    int j;
    static bool rows[9][9];
    static bool cols[9][9];
    static bool blocks[9][9];
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (mat[i][j] == 0) {
                continue;
            }
            int pos = mat[i][j] - 1;
            if (rows[i][pos] || cols[j][pos] || blocks[i - i % 3 + j / 3][pos]) {
                return false;
            }
            rows[i][pos] = cols[j][pos] = blocks[i - i % 3 + j / 3][pos] = true;
        }
    }
    return true;
}

bool resolver(int mat[9][9])
{
    int row;
    int col;
    int num;
    if(!siguiente_casilla_vacia(mat, &row, &col)) {
        imprimir_matriz(mat);
        return true;
    }
    for(num = 1; num <= 9; num++) {
        if(verificar_conflictos(mat, row, col, num)) {
            mat[row][col] = num;
            if(resolver(mat)) {
                return true;
            }
            mat[row][col] = 0;
        } else {
            printf("Celda[%d][%d] = %d CONFLICTO\n", row, col, num);
        }
    }
    return false;
}



void leer(char archivo[1024]){
    char linea[1024];
    FILE *fich;
    int i = 0;
    int j = 0; 
    int num = 0;
    fich = fopen(archivo, "r");
    while(fgets(linea, 1024, (FILE*) fich)) {
        char * pch;
        pch = strtok (linea," ,");
        while (pch != NULL)
        {
            //printf ("[%d,%d] %s \t",i,j,pch); 
            mat[i][j] =  atoi(pch); 
            pch = strtok (NULL, " ,"); 
            j++;
        }
        i++;
        j = 0;
    }
    fclose(fich);  
}



void guardar(char archivo[1024]){
    FILE *fp;
 	char cadena[1024] = "";
 	fp = fopen ( archivo, "w+" );
    char buffer[1024];
    for (int i = 0; i <= 8; i++) {
		for (int j = 0; j <= 8; j++){
             sprintf(buffer, "%d,", mat[j][i]);
			 strcat(cadena, buffer);
        }
        strcat(cadena, "\n");
	}
    fwrite(cadena, sizeof(char), sizeof(cadena), fp);
    fclose ( fp ); 
}



int main(int argc, char **argv)
{
    
    
    leer ("test.txt");
    
    if(verificar_soduku(mat)) {
        resolver(mat);
    } else {
        printf("INVALIDO\n");
    }
    
    
      guardar("guardar.txt");
    
    
    
    
    
    
}