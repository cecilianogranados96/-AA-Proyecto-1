#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int mat[9][9];

int f_vacia(int A, int B)
{
    return 0;
}

void imprimir(void)
{
	for (int i = 0; i <= 8; i++) {
		for (int j = 0; j <= 8; j++){
             printf ("[%d,%d] %d \t",i,j,mat[i][j]); 
        }
        putchar('\n');
	}
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



int main(void)
{   
     
    leer ("test.txt"); //LEE ARCHIVO 
         
    printf("\n ");
    printf("\n ");
    
    imprimir(); //ESCRIBE LA MATRIZ DE 8 X 8 
    
    printf("\n ");
    printf("\n ");
    
    guardar("guardar.txt");
    
	return 0;
}