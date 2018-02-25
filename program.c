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
    int num = 0;
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
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            if(mat[i][j]==0){
                celdas_base[i][j]=0;
            }
            else{
                celdas_base[i][j]=1;
            }
        }
    }
    
    /*SOLVE*/
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            if(celdas_base[i][j]==0)
            {
                do
                {
                    terminar=0;
                    
                    while((revisar_fila==1 || revisar_columna==1 || revisar_seccion==1) && mat[i][j]<=9)
                    {
                        revisar_fila=0; revisar_columna=0; revisar_seccion=0;
                        mat[i][j]++;

                        for(k=0;k<9;k++)
                        {
                            if(k!=j && mat[i][j]==mat[i][k])
                            {
                                revisar_fila=1;
                                printf("Error en la fila  \t%d%d%d\t%d%d%d\n",i,j,mat[i][j],i,k,mat[i][k]);
                                break;
                            }
                        }

                        if(revisar_fila==0)
                        {
                            for(l=0;l<9;l++)
                            {
                                if(l!=i && mat[i][j]==mat[l][j])
                                {
                                    revisar_columna=1;
                                    printf("Error en la columna\t%d%d%d\t%d%d%d\n",i,j,mat[i][j],l,j,mat[l][j]);
                                    break;
                                }
                            }
                        }

                        if(revisar_fila==0 && revisar_columna==0)
                        {
                            for(k=0;k<9;k++)
                            {for(l=0;l<9;l++)
                                {
                                    if(k!=j && l!=i && (10*(i/3)+j/3==10*(l/3)+k/3) && mat[i][j]==mat[l][k])
                                    {
                                        revisar_seccion=1;
                                        printf("Error en la seccion  \t%d%d%d\t%d%d%d\n",i,j,mat[i][j],l,k,mat[l][k]);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    revisar_fila=1; revisar_columna=1; revisar_seccion=1;

                    if(mat[i][j]>9)
                    {
                        mat[i][j]=0;
                        do
                        {
                            j--;
                            if(j<0)
                            {
                                j=8; i--;
                            }
                        }
                        while(celdas_base[i][j]==1);
                        terminar=1;
                        printf("\n\nRegreso\t\n\n");
                    }
                }
                while(terminar==1);
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
    
    
    imprimir_matriz(mat);
    
    guardar("guardar.txt",mat);
    
    
    tFin = clock();
    
    tDecorrido = ((tFin - tInicio) / (CLOCKS_PER_SEC / 1000));
    
    printf("\nTIEMPO: %lums \n",tDecorrido);
    


    
    return 0;
}
