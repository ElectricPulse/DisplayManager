#include <gtk/gtk.h>
#include "main.h"
#include <sys/wait.h>
#include "pam/login.h"

static pthread_t login_thread;

struct Data {
	GtkWidget *window;
	GtkWidget *label_message;
	GtkWidget *entry_name;
	GtkWidget *entry_password;
};

static void button_exit_clicked(GtkWidget *widget, gpointer _data){
	GtkWidget* window = _data;
	gtk_window_close(GTK_WINDOW(window));
}



static void process_input(gpointer _data){
	struct Data *data = (struct Data*) _data;
	const char *username = gtk_editable_get_text(GTK_EDITABLE(data->entry_name));
	const char *password = gtk_editable_get_text(GTK_EDITABLE(data->entry_password));
	

	gtk_label_set_text(GTK_LABEL(data->label_message), "Logging in");
	pid_t child_pid;
	//clear input
	
	if(login(username,password, &child_pid)) {
		int status;
		gtk_widget_set_visible(data->window, 0);

		waitpid(child_pid, &status, 0);

		logout();
	} else {
		gtk_label_set_text(GTK_LABEL(data->label_message), "Falshes passwort oder username");
	}	
}
static void button_clicked(GtkWidget *widget, gpointer _data) {
	pthread_create(&login_thread, NULL, process_input, _data);
}

static void activate(GApplication *app, gpointer *user_data) {
	GtkWidget *entry_password, *label_password, *box_password;
	entry_password = gtk_entry_new();
	//gtk_entry_set_visibility(GTK_ENTRY(entry_password), false);
	label_password = gtk_label_new("Password:");
	box_password = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_append(GTK_BOX(box_password), label_password);
	gtk_box_append(GTK_BOX(box_password), entry_password);
	
	GtkWidget *entry_name, *label_name, *box_name, *message_label;
	message_label = gtk_label_new("BITTE LOGIN IN!!!");
	entry_name = gtk_entry_new();
	gtk_widget_set_size_request(entry_name, 300, -1);
	label_name = gtk_label_new("User: ");
	box_name = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_append(GTK_BOX(box_name), label_name);
	gtk_box_append(GTK_BOX(box_name), entry_name);
		
	GtkWidget *button, *button_exit, *box;
	button_exit = gtk_button_new_with_label("Exit");

	button = gtk_button_new_with_label("Login");
	gtk_widget_set_size_request(button, 100, -1);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
	static struct Data data;
	data.entry_name = entry_name;
	data.entry_password = entry_password;
	data.label_message = message_label;

	g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), &data);
	
	gtk_box_append(GTK_BOX(box), message_label);
	gtk_box_append(GTK_BOX(box), box_name);
	gtk_box_append(GTK_BOX(box), box_password);
	gtk_box_append(GTK_BOX(box), button);
	gtk_box_append(GTK_BOX(box), button_exit);
	gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

	GtkWidget *win;
	win = gtk_window_new();
	data.window = win;
	gtk_window_set_default_size(GTK_WINDOW(win),1920,1080);
	gtk_window_set_title(GTK_WINDOW(win), "GingerOS");
	gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));
	gtk_window_set_child(GTK_WINDOW(win), box);
	gtk_widget_show(win);
	g_signal_connect(button_exit, "clicked", G_CALLBACK(button_exit_clicked), win);
}


void gtk_initialize (int argc, char **argv)
{ 
  GtkApplication *app;

  GtkCssProvider *css_provider;
  css_provider = gtk_css_provider_new();
  GFile *css_file;
  css_file = g_file_new_for_path("main.css");
  gtk_css_provider_load_from_file(css_provider, css_file);

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  g_application_run (G_APPLICATION (app), argc, argv);
 
  g_object_unref (app);
} 


