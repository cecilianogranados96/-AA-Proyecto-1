#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//cambio
int mat[9][9],celdas_base[9][9],prob_len,revisar_fila=1,revisar_columna=1,revisar_seccion=1,terminar=1,repeat,i,j,k,l;

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


void guardar(char archivo[1024],int mat[9][9]){
    FILE *fp;
 	char cadena[1024] = "";
 	fp = fopen ( archivo, "w+" );
    char buffer[1024];
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
         sprintf(buffer, "%d,", mat[i][j]);
         strcat(cadena, buffer);
        }
        strcat(cadena, "\n");
	}
    fwrite(cadena, sizeof(char), sizeof(cadena), fp);
    fclose ( fp ); 
}

void leer(char archivo[1024]){
    char linea[1024];
    FILE *fich;
    int i = 0;
    int j = 0; 
    //int num = 0;
    fich = fopen(archivo, "r");
    while(fgets(linea, 1024, (FILE*) fich)) {
        char * pch;
        pch = strtok (linea," ,");
        while (pch != NULL)
        {
            mat[i][j] =  atoi(pch); 
            pch = strtok (NULL, " ,"); 
            j++;
        }
        i++;
        j = 0;
    }
    fclose(fich);  
}

void resolver(int mat[9][9]){
    
    //Define cuales celdas estaban definidas desde el principio
    for(i = 0; i < 9; i++){				//Revisa las filas
        for(j = 0; j < 9; j++){			//Revisa las columnas
            if(mat[i][j] == 0)			//Si es cero escribe que no había celdas base
                celdas_base[i][j] = 0;	
            else{						//Si tiene cualquier otra cosa escribe un uno si había algo
				celdas_base[i][j] = 1;
			}
        }
    }
    
    
    //Soluciona el sudoku
    for(i = 0; i < 9; i++){					//Revisa las columnas
        for(j = 0; j < 9; j++){				//Revisa las filas
            if(celdas_base[i][j] == 0){ 	//Si la celda base sea 0, osea, que no tuviera un dato de inicio
                do{							
                    terminar = 0;			//Cambia momentaneamente la variable de terminado como completada
                    
                    //Mientras no haya terminado de cumplir todos los requisitos
                    while((revisar_fila == 1 || revisar_columna == 1 || revisar_seccion == 1) && mat[i][j] <= 9){
                        revisar_fila = 0; revisar_columna = 0; revisar_seccion = 0;		//Temporalmente los pone en completados, mientras revisa.
                        mat[i][j]++;		//Le incrementa en un valor al espacio i,j de la matriz y revisa con dicho espacio. Si empieza en cero queda en uno
						
						//Revisa todos los j de la fila, para ver si cumple los requisitos
                        for(k = 0; k < 9; k++){
                            if(k != j && mat[i][j] == mat[i][k]){	//Si uno no es igual no cumple los requisitos
                                revisar_fila = 1;					//Pone el revisar_fila en incompleto
                                printf("Error en la fila  \t%d%d%d\t%d%d%d\n", i, j, mat[i][j], i, k, mat[i][k]);	//Marca el error en la fila
                                break;
                            }
                        }

						//Si no hubo errores en la columna. Revisa todos los i de las columnas, para ver si cumple los requisitos
                        if(revisar_fila == 0){
                            for(l = 0; l < 9; l++){
                                if(l != i && mat[i][j] == mat[l][j]){	//Si uno no es igual no cumple los requisitos
                                    revisar_columna = 1;				//Pone el revisar_columna en incompleto
                                    printf("Error en la columna\t%d%d%d\t%d%d%d\n", i, j, mat[i][j], l, j, mat[l][j]);	//Marca el error en la columna
                                    break;
                                }
                            }
                        }
						
						//Si no hubo errores en la fila o en la columna. Revisa en la seccion, para ver si cumple los requisitos
                        if(revisar_fila == 0 && revisar_columna == 0){
                            for(k = 0; k < 9; k++){
								for(l = 0; l < 9; l++){
                                    if(k != j && l != i && (10*(i/3)+j/3 == 10*(l/3)+k/3) && mat[i][j] == mat[l][k]){	//Si un número de la seccion es igual no cumple los requisitos
                                        revisar_seccion = 1;																//Pone el revisar seccion como incompleto
                                        printf("Error en la seccion  \t%d%d%d\t%d%d%d\n", i, j, mat[i][j], l, k, mat[l][k]);	//Marca el error en la seccion
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    revisar_fila = revisar_columna = revisar_seccion = 1;	//Vuelve a marcar como incompletas todas las revisiones para revisar el siguiente elemento

					//Si la matriz tiene un elemento mayor a 9 lo resetea a 0
                    if(mat[i][j] > 9){
                        mat[i][j] = 0;
                        //Busca la siguiente celda vacía
                        do{
                            j--;
                            if(j < 0){
                                j = 8;
                                i--;
							}
                        } while(celdas_base[i][j] == 1);
                        //Marca como incompleto el terminar
                        terminar = 1;
                        printf("\n\nRegreso\t\n\n"); //Regresa
                    }
                } while(terminar == 1);		//Mientras no este terminado
            }
        }
    }
}


int main()
{
    leer ("test_4.txt");
           
    clock_t tInicio, tFin, tDecorrido;
    tInicio = clock();
    resolver(mat);
    tFin = clock();
    imprimir_matriz(mat);
    
    tDecorrido = ((tFin - tInicio) / (CLOCKS_PER_SEC / 1000));
    printf("\nTIEMPO: %lums \n",tDecorrido);
       
    guardar("guardar.txt",mat);   
    return 0;
}
