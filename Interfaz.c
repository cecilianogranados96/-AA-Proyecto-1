#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

GtkBuilder  *builder; 
GtkWidget   *winPrincipal;

void on_winPrincipal_destroy()
{
	gtk_main_quit();
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
		//open_file (filename);
		g_free (filename);
	  }

	gtk_widget_destroy (dialog);
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
		//save_to_file (filename);
		g_free (filename);
	  }

	gtk_widget_destroy (dialog);
}

void on_btnResolver_clicked()
{
	g_print ("jaja no hago nada\n");
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
		}
	}

	gtk_widget_destroy (dialog);
}

int main(int argc, char **argv)
{
    GtkWidget 	*marco;
    GtkWidget 	*matriz;
    GtkWidget	*matBotones[9][9];
	
    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "Interfaz.glade", NULL);

    winPrincipal = GTK_WIDGET(gtk_builder_get_object(builder, "winPrincipal"));
    gtk_builder_connect_signals(builder, NULL);

	marco = GTK_WIDGET(gtk_builder_get_object(builder, "marco"));

	matriz = gtk_grid_new();
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 3, 11, 1);
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_VERTICAL), 3, 0, 1, 11);
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 7, 11, 1);
	gtk_grid_attach(GTK_GRID(matriz), gtk_separator_new(GTK_ORIENTATION_VERTICAL), 7, 0, 1, 11);
	
	gtk_container_add(GTK_CONTAINER(marco), matriz);
	int k = 0, l = 0;
	for (int i=1; i<12; i++){
		for (int j=1; j<12; j++){
			if(i==4 || i==8){
				k++;
			}
			else if(j==4 || j==8){
				l++;
			}
			else{
				
				matBotones[i-k][j-l] = gtk_button_new();
				g_signal_connect(matBotones[i-k][j-l], "clicked", G_CALLBACK (cambiarNumero), winPrincipal);
				gtk_grid_attach(GTK_GRID(matriz), matBotones[i-k][j-l], i-1, j-1, 1, 1);
			}
		}
		k -= 2;
	}

	gtk_grid_set_row_homogeneous (GTK_GRID(matriz), TRUE);
	gtk_grid_set_column_homogeneous (GTK_GRID(matriz), TRUE);
                 
    g_object_unref(builder);
 
	gtk_window_resize(GTK_WINDOW(winPrincipal), 500, 500);
    gtk_widget_show_all(winPrincipal);                
    gtk_main();
    
	return 0;
}
