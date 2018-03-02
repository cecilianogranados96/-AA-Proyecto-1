#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>


/// VARIABLES GLOBALES
GtkWidget	*g_matBotones[9][9];	//Botones que forman la matriz
GtkWidget   *g_winPrincipal;		//Ventana principal de la interfaz
GtkWidget	*g_labelTiempo;			//Campo de edición del tiempo
clock_t 	g_tiempoInicio;			//Variables de tiempo
char 		*g_error;				//Mensaje de error
int 		g_celdasBase[9][9];						//Variable con las celdas que están llenas de serie
int 		g_matriz[9][9] = {{0,0,0,0,0,0,0,0},	//Variable con las entradas de la matriz
					  {0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0}};



/// RELACIONADO CON INTERFAZ
void on_winPrincipal_destroy () {
	gtk_main_quit();
}

void ventana_error () {
	GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(g_winPrincipal), 
												GTK_DIALOG_DESTROY_WITH_PARENT, 
												GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
												"La matriz no pudo ser resuelta:\n\n%s", g_error);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

void actualizar () {
    if((int)g_tiempoInicio != 0) {
		int tiempoTotal = ((int)clock() - (int)g_tiempoInicio) / (CLOCKS_PER_SEC / 1000);     
		
		int largo = snprintf(NULL, 0, "Tiempo: %dms", tiempoTotal);
		char *string = malloc(largo++);
		snprintf(string, largo+1, "Tiempo: %dms", tiempoTotal);
		
		gtk_label_set_text (GTK_LABEL(g_labelTiempo), string);
		free(string);
	}
    while (g_main_context_pending(NULL)) g_main_context_iteration(NULL, FALSE);
}

void actualizar_num (int row, int col) {
    g_usleep(3000);
    
    if(g_matriz[row][col] != 0){	
        int largo = snprintf( NULL, 0, "%d", g_matriz[row][col]);
        char *string = malloc(largo++);
        snprintf(string, largo + 1, "%d", g_matriz[row][col]);
        
        gtk_button_set_label(GTK_BUTTON(g_matBotones[row][col]), string);
        free(string);
    }
    
    else gtk_button_set_label(GTK_BUTTON(g_matBotones[row][col]), "");
    actualizar();
}

void actualizar_matriz () {
	int row, col;
	
	for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
			if(g_matriz[row][col] != 0) {
				int largo = snprintf( NULL, 0, "%d", g_matriz[row][col]);
				char *string = malloc(largo++);
				snprintf(string, largo + 1, "%d", g_matriz[row][col]);
				gtk_button_set_label(GTK_BUTTON(g_matBotones[row][col]), string);
				free(string);
			}
			else gtk_button_set_label(GTK_BUTTON(g_matBotones[row][col]), "");
        }
    }
}

void cambiarNumero (GtkWidget *widget, gpointer data) {
	
	int row, col, largo, respuesta, valor;
	GtkWidget *spin, *caja, *dialog;
	
	dialog = gtk_dialog_new_with_buttons ("Cambiar Número", GTK_WINDOW(g_winPrincipal) , 
										  GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT, 
										  "_Aceptar" , GTK_RESPONSE_ACCEPT, 
										  "_Cancelar", GTK_RESPONSE_REJECT, NULL);
	
	caja = GTK_WIDGET(gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	spin = gtk_spin_button_new_with_range (0, 9, 1);
	
	gtk_container_add(GTK_CONTAINER(caja), spin);
	gtk_widget_show_all(dialog);
	
	respuesta = gtk_dialog_run (GTK_DIALOG (dialog));
	if (respuesta == GTK_RESPONSE_ACCEPT) {
		valor = gtk_spin_button_get_value (GTK_SPIN_BUTTON(spin));
		
		if (valor != 0) {
			largo = snprintf( NULL, 0, "%d", valor);
			char *string = malloc(largo++);
			snprintf(string, largo + 1, "%d", valor);
			for(row = 0; row < 9; row++) {
				for(col = 0; col < 9; col++) {
					if(widget == g_matBotones[row][col]) {
						gtk_button_set_label(GTK_BUTTON(widget), string);
						g_matriz[row][col] = g_celdasBase[row][col] = valor;
						break;
					}
				}
			}
			free(string);
		}
		else{
			gtk_button_set_label(GTK_BUTTON(widget), "");
			for(row = 0; row < 9; row++) {
				for(col = 0; col < 9; col++) {
					if(widget == g_matBotones[row][col]) {
						g_matriz[row][col] = g_celdasBase[row][col] = 0;
						break;
					}
				}
			}
		}
	}

	gtk_widget_destroy (dialog);
}

void crearInterfaz () {
	GtkBuilder  *builder; 
	GtkWidget 	*marcoTiempo, *marco, *gridMatriz;
	
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Interfaz.glade", NULL);
    
    g_winPrincipal 	= GTK_WIDGET(gtk_builder_get_object(builder, "winPrincipal"));
    marcoTiempo 	= GTK_WIDGET(gtk_builder_get_object(builder, "marcoTiempo"));
	marco 			= GTK_WIDGET(gtk_builder_get_object(builder, "marco"));
	g_labelTiempo 	= GTK_WIDGET(gtk_label_new("Tiempo: 0"));
	gridMatriz 		= gtk_grid_new();
	
	gtk_grid_attach(GTK_GRID(gridMatriz), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 3, 11, 1);
	gtk_grid_attach(GTK_GRID(gridMatriz), gtk_separator_new(GTK_ORIENTATION_VERTICAL), 3, 0, 1, 11);
	gtk_grid_attach(GTK_GRID(gridMatriz), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 7, 11, 1);
	gtk_grid_attach(GTK_GRID(gridMatriz), gtk_separator_new(GTK_ORIENTATION_VERTICAL), 7, 0, 1, 11);
	
	gtk_container_add(GTK_CONTAINER(marco), gridMatriz);
	gtk_container_add(GTK_CONTAINER(marcoTiempo), g_labelTiempo);
	
	int i, j, k = 0, l = 0;
	for (i = 0; i < 11; i++){
		for (j = 0; j < 11; j++){
			if(i == 3 || i == 7){
				k++;
				break;
			}
			else if(j == 3 || j == 7) l++;
			else{
				g_matBotones[j-l][i-k] = gtk_button_new();
				g_signal_connect(g_matBotones[j-l][i-k], "clicked", G_CALLBACK (cambiarNumero), g_winPrincipal);
				gtk_grid_attach(GTK_GRID(gridMatriz), g_matBotones[j-l][i-k], i, j, 1, 1);
			}		
		} l = 0;
	}

	gtk_grid_set_row_homogeneous 	(GTK_GRID(gridMatriz), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID(gridMatriz), TRUE);
    
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
	gtk_window_resize(GTK_WINDOW(g_winPrincipal), 500, 500);
    gtk_widget_show_all(g_winPrincipal);                
    gtk_main();
}



/// BOTÓN LEER
void leer (char nombre[1024]) {
	int i = 0, j = 0;
    char linea[1024];
    FILE *archivo = fopen(nombre, "r");
    
    while(fgets(linea, 1024, archivo)) {
        char * pch = strtok (linea," ,");
        while (pch != NULL){
            g_matriz[i][j] =  atoi(pch);
            pch = strtok (NULL, " ,"); 
            j++;
        } i++; j = 0;
    }
    fclose(archivo);
    actualizar_matriz();
}

void on_btnLeer_clicked () {
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Leer Archivo", GTK_WINDOW(g_winPrincipal), 
													GTK_FILE_CHOOSER_ACTION_OPEN, 
													"_Cancelar", GTK_RESPONSE_CANCEL, 
													"_Abrir"   , GTK_RESPONSE_ACCEPT, NULL);
	int respuesta = gtk_dialog_run(GTK_DIALOG (dialog));
	if (respuesta == GTK_RESPONSE_ACCEPT){
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		char *filename = gtk_file_chooser_get_filename (chooser);
		leer(filename);
	}
	gtk_widget_destroy (dialog);
}



/// BOTÓN GUARDAR
void guardar (char nombre[1024],int g_matriz[9][9]) {
 	int  i, j;
 	char buffer[1024], cadena[1024];
 	FILE *archivo = fopen (nombre, "w+");
    
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
         sprintf(buffer, "%d,", g_matriz[i][j]);
         strcat(cadena, buffer);
        }
        strcat(cadena, "\n");
	}
    fwrite(cadena, sizeof(char), sizeof(cadena), archivo);
    fclose (archivo); 
}

void on_btnGrabar_clicked () {
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Grabar Archivo", GTK_WINDOW(g_winPrincipal), 
													 GTK_FILE_CHOOSER_ACTION_SAVE, 
													 "_Cancelar", GTK_RESPONSE_CANCEL, 
													 "_Grabar"  , GTK_RESPONSE_ACCEPT, NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
	int respuesta = gtk_dialog_run (GTK_DIALOG (dialog));
	if (respuesta == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename (chooser);
		guardar(filename, g_matriz);
	}
	gtk_widget_destroy (dialog);
}



/// BOTÓN RESOLVER
bool verificarFilas (int fila, int columna, int num) {
	int largo, k;
	for(k = 0; k < 9; k++) {
		if(k != columna && g_matriz[fila][k] == num) {
			//printf("Repetido en la fila %d:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, fila, columna, fila, k);
			largo = snprintf( NULL, 0, "Repetido en la fila %d:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, fila, columna, fila, k);
			g_error = malloc(largo++);
			snprintf(g_error, largo + 1, "Repetido en la fila %d:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, fila, columna, fila, k);
			return false;
			break;
		}
	}
	//printf("Fila está bien\n");
	return true;
}

bool verificarColumnas (int fila, int columna, int num) {
	int largo, k;
	for(k = 0; k < 9; k++){
		if(k != fila && g_matriz[k][columna] == num) {
			//printf("Repetido en la columna %d:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, fila, columna, k, columna);
			largo = snprintf( NULL, 0, "Repetido en la columna %d:\n\t Elementos en (%d, %d) y en (%d, %d)\n", columna, fila, columna, k, columna);
			g_error = malloc(largo++);
			snprintf(g_error, largo + 1, "Repetido en la columna %d:\n\t Elementos en (%d, %d) y en (%d, %d)\n", columna, fila, columna, k, columna);
			return false;
			break;
		}
	}
	//printf("Columna está bien\n");
	return true;
}

bool verificarZona (int fila, int columna, int inicioFila, int inicioColumna, int num) {
	int largo, k, l;
	for (k = 0; k < 3; k++){
        for (l = 0; l < 3; l++){
            if(k + inicioFila != fila && l + inicioColumna != columna) {
				if (g_matriz[k + inicioFila][l + inicioColumna] == num) {
					//printf("Repetido en la caja:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, columna, k + inicioFila, l + inicioColumna);
					largo = snprintf( NULL, 0, "Repetido en una caja:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, columna, k + inicioFila, l + inicioColumna);
					g_error = malloc(largo++);
					snprintf(g_error, largo + 1, "Repetido en una caja:\n\t Elementos en (%d, %d) y en (%d, %d)\n", fila, columna, k + inicioFila, l + inicioColumna);
					return false;
					break;
				}
			}
		}
	}
	//printf("Caja está bien\n");
    return true;
}

bool verificar () {
	int i, j;
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            if(g_matriz[i][j] == 0)	g_celdasBase[i][j] = 0;
            else g_celdasBase[i][j] = 1;
        }
    }
	
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(g_celdasBase[i][j] == 1) return 	verificarFilas(i, j, g_matriz[i][j]) 	&&
												verificarColumnas(i, j, g_matriz[i][j]) && 
												verificarZona(i, j, i - i%3 , j - j%3, g_matriz[i][j]);
		}
	}
	printf("Todo bien\n\n");
	return true;
}

void resolver () {
	int i, j, k, l;
	int terminar = 1, revisar_fila = 1, revisar_columna = 1, revisar_seccion = 1;
    //Soluciona el sudoku
    for(i = 0; i < 9; i++){					//Revisa las columnas
        for(j = 0; j < 9; j++){				//Revisa las filas
            if(g_celdasBase[i][j] == 0){ 	//Si la celda base sea 0, osea, que no tuviera un dato de inicio
                do{			
                        
                    terminar = 0;			//Cambia momentaneamente la variable de terminado como completada
					//printf("\n");
                    //Mientras no haya terminado de cumplir todos los requisitos
                    while((revisar_fila == 1 || revisar_columna == 1 || revisar_seccion == 1) && g_matriz[i][j] <= 9){
                        revisar_fila = 0; revisar_columna = 0; revisar_seccion = 0;		//Temporalmente los pone en completados, mientras revisa.
                        g_matriz[i][j]++;		//Le incrementa en un valor al espacio i,j de la matriz y revisa con dicho espacio. Si empieza en cero queda en uno
                        
						actualizar_num(i,j);
						
						//Revisa todos los j de la fila, para ver si cumple los requisitos
                        for(k = 0; k < 9; k++){
                            if(k != j && g_matriz[i][j] == g_matriz[i][k]){	//Si uno no es igual no cumple los requisitos
                                revisar_fila = 1;					//Pone el revisar_fila en incompleto
                                //printf("Error en la fila  \t%d%d%d\t%d%d%d\n", i, j, mat[i][j], i, k, mat[i][k]);	//Marca el error en la fila
                                break;
                            }
                        }

						//Si no hubo errores en la columna. Revisa todos los i de las columnas, para ver si cumple los requisitos
                        if(revisar_fila == 0){
                            for(l = 0; l < 9; l++){
                                if(l != i && g_matriz[i][j] == g_matriz[l][j]){	//Si uno no es igual no cumple los requisitos
                                    revisar_columna = 1;				//Pone el revisar_columna en incompleto
                                    //printf("Error en la columna\t%d%d%d\t%d%d%d\n", i, j, mat[i][j], l, j, mat[l][j]);	//Marca el error en la columna
                                    break;
                                }
                            }
                        }
						
						//Si no hubo errores en la fila o en la columna. Revisa en la seccion, para ver si cumple los requisitos
                        if(revisar_fila == 0 && revisar_columna == 0){
                            for(k = 0; k < 9; k++){
								for(l = 0; l < 9; l++){
                                    if(k != j && l != i && (10*(i/3)+j/3 == 10*(l/3)+k/3) && g_matriz[i][j] == g_matriz[l][k]){	//Si un número de la seccion es igual no cumple los requisitos
                                        revisar_seccion = 1;																//Pone el revisar seccion como incompleto
                                        //printf("Error en la seccion  \t%d%d%d\t%d%d%d\n", i, j, mat[i][j], l, k, mat[l][k]);	//Marca el error en la seccion
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    revisar_fila = revisar_columna = revisar_seccion = 1;	//Vuelve a marcar como incompletas todas las revisiones para revisar el siguiente elemento

					//Si la matriz tiene un elemento mayor a 9 lo resetea a 0
                    if(g_matriz[i][j] > 9){
                        g_matriz[i][j] = 0;
                        actualizar_num(i,j);
                       
                          //printf("i = %d j=%d \n",j,i);
                            
                        //Busca la siguiente celda vacía
                        do{
                            j--;
                            if(j < 0){
                                j = 8;
                                i--;
							}
                            if (j < 0 || i < 0){
                                terminar = 1;
                            }
                          
                        } while(g_celdasBase[i][j] == 1);
                        //Marca como incompleto el terminar
                        terminar = 1;
                        //printf("\n\nRegreso\t\n\n"); //Regresa
                    }
                } while(terminar == 1);		//Mientras no este terminado
            }
        }
    }
}

void on_btnResolver_clicked () {
	if(verificar()) {
		printf("Todo bien\n\n");
		g_tiempoInicio = clock();    
		resolver();
	}
    else {
		ventana_error();
    }
}



/// MAIN
int main (int argc, char **argv) {
	gtk_init(&argc, &argv);
	crearInterfaz();

	return 0;
}
