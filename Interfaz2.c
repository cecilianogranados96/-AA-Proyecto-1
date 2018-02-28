#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <pthread.h>

GtkBuilder  *builder; 
GtkWidget   *winPrincipal;
GtkWidget	*labelTiempo;
GtkWidget	*matBotones[9][9];

pthread_t hiloTiempo;
pthread_t hiloActualizar;

clock_t tInicio, tFin;
int tDecorrido;

int prob_len, repeat, i, j, k, l;
int mat[9][9] = {{0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0},
				 {0,0,0,0,0,0,0,0}};
int celdas_base[9][9];
int terminar = 1, revisar_fila = 1, revisar_columna = 1, revisar_seccion = 1;

/// RELACIONADO CON INTERFAZ
void on_winPrincipal_destroy()
{
	gtk_main_quit();
}

void actualizar_matriz()
{
	for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
			if(mat[row][col] != 0){
				//g_print("R:%d C:%d matriz:%d\n", row, col, mat[row][col]);
				int length = snprintf( NULL, 0, "%d", mat[row][col]);
				char* str = malloc(length++);
				snprintf(str, length + 1, "%d", mat[row][col]);
				gtk_button_set_label(GTK_BUTTON(matBotones[row][col]), str);
				free(str);
				gtk_widget_queue_draw(matBotones[row][col]);
				while (g_main_context_pending(NULL))
					g_main_context_iteration(NULL, FALSE);
			}
			else{
				gtk_button_set_label(GTK_BUTTON(matBotones[row][col]), "");
				gtk_widget_queue_draw(matBotones[row][col]);
				while (g_main_context_pending(NULL))
					g_main_context_iteration(NULL, FALSE);
				}
        }
    }
}

void cambiarNumero (GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	GtkWidget *spin;
	GtkWidget *caja;
	
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	dialog = gtk_dialog_new_with_buttons ("Cambiar Número", GTK_WINDOW(winPrincipal), flags, "_Aceptar", GTK_RESPONSE_ACCEPT, "_Cancelar", GTK_RESPONSE_REJECT, NULL);
	
	caja = GTK_WIDGET(gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
	spin = gtk_spin_button_new_with_range (0, 9, 1);
	gtk_container_add(GTK_CONTAINER(caja), spin);
	gtk_widget_show_all(dialog);
	
	int respuesta = gtk_dialog_run (GTK_DIALOG (dialog));
	if (respuesta == GTK_RESPONSE_ACCEPT)
	{
		int valor = gtk_spin_button_get_value (GTK_SPIN_BUTTON(spin));
		if (valor != 0){
			int length = snprintf( NULL, 0, "%d", valor);
			char* str = malloc(length++);
			snprintf(str, length + 1, "%d", valor);
			gtk_button_set_label(GTK_BUTTON(widget), str);
			free(str);
			
			int row;
			int col;
			for(row = 0; row < 9; row++) {
				for(col = 0; col < 9; col++) {
					if(widget == matBotones[row][col]){
						mat[row][col] = valor;
						celdas_base[row][col] = valor;
						break;
					}
				}
			}
		}
		else{
			int length = snprintf( NULL, 0, "%d", valor);
			char* str = malloc(length++);
			snprintf(str, length + 1, "%d", valor);
			gtk_button_set_label(GTK_BUTTON(widget), "");
			free(str);
			
			int row;
			int col;
			for(row = 0; row < 9; row++) {
				for(col = 0; col < 9; col++) {
					if(widget == matBotones[row][col]){
						mat[row][col] = 0;
						celdas_base[row][col] = 0;
						break;
					}
				}
			}
		}
	}

	gtk_widget_destroy (dialog);
}

void crearInterfaz()
{
	GtkWidget 	*marco;
    GtkWidget 	*matriz;
    GtkWidget 	*marcoTiempo;
    
    
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Interfaz.glade", NULL);
    winPrincipal = GTK_WIDGET(gtk_builder_get_object(builder, "winPrincipal"));
    
	marcoTiempo = GTK_WIDGET(gtk_builder_get_object(builder, "marcoTiempo"));
	labelTiempo = GTK_WIDGET(gtk_label_new("Tiempo: 0"));
	gtk_container_add(GTK_CONTAINER(marcoTiempo), labelTiempo);


	
	marco = GTK_WIDGET(gtk_builder_get_object(builder, "marco"));
	matriz = gtk_grid_new();
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 3, 11, 1);
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_VERTICAL), 3, 0, 1, 11);
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 7, 11, 1);
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_VERTICAL), 7, 0, 1, 11);
	
	gtk_container_add(GTK_CONTAINER(marco), matriz);
	int k = 0, l = 0;
	for (int i=0; i<11; i++){
		for (int j=0; j<11; j++){
			if(i==3 || i==7){
				k++;
				break;
			}
			else if(j==3 || j==7){
				l++;
			}
			else{
				matBotones[j-l][i-k] = gtk_button_new();
				g_signal_connect(matBotones[j-l][i-k], "clicked", G_CALLBACK (cambiarNumero), winPrincipal);
				gtk_grid_attach(GTK_GRID(matriz), matBotones[j-l][i-k], i, j, 1, 1);
			}
			
		}
		l = 0;
	}

	gtk_grid_set_row_homogeneous (GTK_GRID(matriz), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID(matriz), TRUE);
    
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
	gtk_window_resize(GTK_WINDOW(winPrincipal), 500, 500);
    gtk_widget_show_all(winPrincipal);                
    gtk_main();
}

/// BOTÓN LEER
void leer(char archivo[1024])
{
	char linea[1024];
    FILE *fich;
    int i = 0, j = 0;
    fich = fopen(archivo, "r");
    while(fgets(linea, 1024, (FILE*) fich)) {
        char * pch;
        pch = strtok (linea," ,");
        while (pch != NULL){
            mat[i][j] =  atoi(pch);
            pch = strtok (NULL, " ,"); 
            j++;
        }
        i++;
        j = 0;
    }
    fclose(fich);
    actualizar_matriz();
}

void on_btnLeer_clicked()
{
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	int respuesta;

	dialog = gtk_file_chooser_dialog_new("Leer Archivo", GTK_WINDOW(winPrincipal), action, "_Cancelar", GTK_RESPONSE_CANCEL, "_Abrir", GTK_RESPONSE_ACCEPT, NULL);

	respuesta = gtk_dialog_run(GTK_DIALOG (dialog));
	
	if (respuesta == GTK_RESPONSE_ACCEPT)
	  {
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		leer(filename);
		g_free (filename);
	  }

	gtk_widget_destroy (dialog);
}

/// BOTÓN GUARDAR
void guardar(char archivo[1024],int mat[9][9])
{
    FILE *fp;
 	char cadena[1024] = "";
 	fp = fopen ( archivo, "w+" );
    char buffer[1024];
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
         sprintf(buffer, "%d,", mat[i][j]);
         strcat(cadena, buffer);
        }
        strcat(cadena, "\n");
	}
    fwrite(cadena, sizeof(char), sizeof(cadena), fp);
    fclose ( fp ); 
}

void on_btnGrabar_clicked()
{
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	int respuesta;

	dialog = gtk_file_chooser_dialog_new ("Grabar Archivo", GTK_WINDOW(winPrincipal), action, "_Cancelar", GTK_RESPONSE_CANCEL, "_Grabar", GTK_RESPONSE_ACCEPT, NULL);
	chooser = GTK_FILE_CHOOSER (dialog);

	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	respuesta = gtk_dialog_run (GTK_DIALOG (dialog));
	if (respuesta == GTK_RESPONSE_ACCEPT)
	  {
		char *filename;
		filename = gtk_file_chooser_get_filename (chooser);
		
		guardar(filename, mat);
		
		g_free (filename);
	  }

	gtk_widget_destroy (dialog);
}

/// BOTÓN RESOLVER
void resolver(int mat[9][9])
{
    
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
                     printf("\n");
                    //Mientras no haya terminado de cumplir todos los requisitos
                    while((revisar_fila == 1 || revisar_columna == 1 || revisar_seccion == 1) && mat[i][j] <= 9){
                        revisar_fila = 0; revisar_columna = 0; revisar_seccion = 0;		//Temporalmente los pone en completados, mientras revisa.
                        mat[i][j]++;		//Le incrementa en un valor al espacio i,j de la matriz y revisa con dicho espacio. Si empieza en cero queda en uno
						actualizar_matriz();
						
						//Revisa todos los j de la fila, para ver si cumple los requisitos
                        for(k = 0; k < 9; k++){
                            if(k != j && mat[i][j] == mat[i][k]){	//Si uno no es igual no cumple los requisitos
                                revisar_fila = 1;					//Pone el revisar_fila en incompleto
                                //printf("Error en la fila  \t%d%d%d\t%d%d%d\n", i, j, mat[i][j], i, k, mat[i][k]);	//Marca el error en la fila
                                break;
                            }
                        }

						//Si no hubo errores en la columna. Revisa todos los i de las columnas, para ver si cumple los requisitos
                        if(revisar_fila == 0){
                            for(l = 0; l < 9; l++){
                                if(l != i && mat[i][j] == mat[l][j]){	//Si uno no es igual no cumple los requisitos
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
                                    if(k != j && l != i && (10*(i/3)+j/3 == 10*(l/3)+k/3) && mat[i][j] == mat[l][k]){	//Si un número de la seccion es igual no cumple los requisitos
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
                    if(mat[i][j] > 9){
                        mat[i][j] = 0;
                        actualizar_matriz();
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
                        //printf("\n\nRegreso\t\n\n"); //Regresa
                    }
                } while(terminar == 1);		//Mientras no este terminado
            }
        }
    }
}

void on_btnResolver_clicked()
{

    tInicio = clock();
    resolver(mat);
    tFin = clock();
    actualizar_matriz();
    tDecorrido = (((int)tFin - (int)tInicio) / (CLOCKS_PER_SEC / 1000));     
    int length = snprintf( NULL, 0, "Tiempo: %dms", tDecorrido);
    char* str = malloc(length++);
    snprintf(str, length + 1, "Tiempo: %dms", tDecorrido);
    gtk_label_set_text (labelTiempo,str);
    free(str);
}


/// MAIN
int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
	crearInterfaz();

	return 0;
}
