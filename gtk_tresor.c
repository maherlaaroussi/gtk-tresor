/*
***********************************************************
Licence Informatique 1 - Premier projet : Chasse au trésor

Maher LAAROUSSI 11504011
Mohammed OUSRHIR 11507552

Groupe : IG2
***********************************************************

*/

/* ----------------------------- INCLUDES ----------------------------- */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>

/* ----------------------------- VARIABLES ----------------------------- */

// Widgets
GtkWidget* tFenetre;
GtkWidget* bt_coffres[3];
GtkWidget* bt_reset;
GtkWidget* bt_reset_score;
GtkWidget* tScore;
GtkWidget* tMsg;
GtkWidget* tStats;

// Les Pixbuf pour les images
GdkPixbuf* img_coffre;
GdkPixbuf* img_coffre_100;

// Variables globales
int nb_victoires = 0;
int nb_defaites = 0;
int coffre_tresor = NULL;
int nb_coffres = 3;
int stats_victoires = 0;
int stats_defaites = 0;
int coffres[3];

/* ----------------------------- PROTOTYPES ----------------------------- */

// Prototypes
void maj_score();
int nombre_hasard (int min, int max);
void chargement_image (int coffre, int etat);
void game_reset();
void score_reset();
void choix_coffre(int coffre);
void choix_coffre1();
void choix_coffre2();
void choix_coffre3();
void victoire();
void defaite();
void ouvrir_coffre(int coffre);
int coffre_vide (int coffre1, int coffre2);
void game_reset_score();
void boutons_reset();

/* ----------------------------- ENUMERATIONS ----------------------------- */

// Les coffres
enum coffres {COFFRE1, COFFRE2, COFFRE3};

// Les etats des coffres
enum etats {OUVERT, FERME, TRESOR};

// Les etats pour le label
enum partie {VICTOIRE, DEFAITE, MAJ};

// Les etats pour l'action du reset boutons
enum action_reset {INIT, TERMINE};

/* ----------------------------- DEFINES ----------------------------- */

#define N 100

/* ------------------------------------------------------------------ */


/* ----------------------------- MAIN ----------------------------- */

int main(int argc, char** argv)
{
    // Initialisation de GTK
    gtk_init (&argc, &argv);

    // Declaration des boxs
    GtkWidget* tBox;
    GtkWidget* tBox_haut;
    GtkWidget* tBox_reset;
    GtkWidget* tBox_coffres;
    GtkWidget* tBox_bas;

    // Création de la fenetre
    tFenetre = gtk_window_new (GTK_WINDOW_TOPLEVEL);


    /* ----- Les éléments de la fenetre ----- */
    // Les boxs
    tBox = gtk_vbox_new(FALSE, 0);
    tBox_haut = gtk_vbox_new(FALSE, 0);
    tBox_reset = gtk_hbox_new(FALSE, 0);
    tBox_coffres = gtk_hbox_new(FALSE, 0);
    tBox_bas = gtk_vbox_new(FALSE, 0);

    // Les boutons
    bt_coffres[COFFRE1] = gtk_button_new();
    bt_coffres[COFFRE2] = gtk_button_new();
    bt_coffres[COFFRE3] = gtk_button_new();

    bt_reset = gtk_button_new_with_mnemonic("_Recommencer la partie");
    bt_reset_score = gtk_button_new_with_mnemonic("_Réinitialiser les scores");

    gtk_widget_set_sensitive(bt_reset, FALSE);
	gtk_widget_set_sensitive(bt_reset_score, FALSE);

    // Les labels
    tScore = gtk_label_new(NULL);
    tMsg = gtk_label_new(NULL);
    tStats = gtk_label_new(NULL);


    /* ----- Propriétés de la fenetre ----- */
    gtk_window_set_title (GTK_WINDOW(tFenetre), "Le Jeu Du Trésor");
    gtk_window_set_position (GTK_WINDOW(tFenetre), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW(tFenetre), 400,250);


    /* ----- Rajout des widgets et des boxs ----- */
    gtk_container_add (GTK_CONTAINER(tFenetre), tBox);

    // Box principale
    gtk_box_pack_start (GTK_BOX(tBox), tBox_haut, FALSE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(tBox), tBox_coffres, TRUE, TRUE, 20);
    gtk_box_pack_start (GTK_BOX(tBox), tBox_bas, FALSE, FALSE, 0);

    // Box du haut
    gtk_box_pack_start (GTK_BOX(tBox_haut), tScore, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(tBox_haut), tMsg, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(tBox_bas), tBox_reset, FALSE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX(tBox_reset), bt_reset, TRUE, FALSE, 10);
    gtk_box_pack_start (GTK_BOX(tBox_reset), bt_reset_score, TRUE, FALSE, 0);

    // Box des coffres
    gtk_box_pack_start (GTK_BOX(tBox_coffres), bt_coffres[COFFRE1], TRUE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX(tBox_coffres), bt_coffres[COFFRE2], TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(tBox_coffres), bt_coffres[COFFRE3], TRUE, TRUE, 5);

    // Box du bas
    gtk_box_pack_start (GTK_BOX(tBox_bas), tStats, TRUE, FALSE, 0);


    /* ----- Initialisation du jeu ----- */
    game_reset();
    gtk_widget_set_sensitive(bt_reset_score, FALSE);


    /* ----- Les signaux ----- */
    g_signal_connect(G_OBJECT(bt_reset), "clicked", G_CALLBACK(game_reset), NULL);
    g_signal_connect(G_OBJECT(bt_reset_score), "clicked", G_CALLBACK(game_reset_score), NULL);
    g_signal_connect(G_OBJECT(bt_coffres[COFFRE1]), "clicked", G_CALLBACK(choix_coffre1), NULL);
    g_signal_connect(G_OBJECT(bt_coffres[COFFRE2]), "clicked", G_CALLBACK(choix_coffre2), NULL);
    g_signal_connect(G_OBJECT(bt_coffres[COFFRE3]), "clicked", G_CALLBACK(choix_coffre3), NULL);
    g_signal_connect(GTK_WINDOW(tFenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    /* ----- Affichage de tout les elements ----- */
    gtk_widget_show_all(tFenetre);


    /* ----- Boucle événementielle GTK ----- */
    gtk_main ();


    return EXIT_SUCCESS;
}


/* ----------------------------- FONCTIONS ----------------------------- */

// Reset du jeu
void game_reset()
{
	// On grise les boutons
	boutons_reset(INIT);

	// Les coffres
	coffres[COFFRE1] = FERME;
	coffres[COFFRE2] = FERME;
	coffres[COFFRE3] = FERME;

	// On re-tire un nombre au hasard
	coffre_tresor = nombre_hasard(0, 2);

	// On remet les images des coffres
	chargement_image(COFFRE1, FERME);
	chargement_image(COFFRE2, FERME);
	chargement_image(COFFRE3, FERME);

	// On remet le nombre de coffres fermés à 3
	nb_coffres = 3;

	// On actualise le message, le score et les stats
	maj_score(MAJ);
}

/* ------------------------------------------- */

// Reset du score
void game_reset_score()
{

	nb_victoires = 0;
	nb_defaites = 0;

	// On grise les boutons
	game_reset();
}

/* ------------------------------------------- */

// Verification du choix du coffre joueur
void choix_coffre1()
{
	choix_coffre(COFFRE1);
}
void choix_coffre2()
{
	choix_coffre(COFFRE2);
}
void choix_coffre3()
{
	choix_coffre(COFFRE3);
}


void choix_coffre(int coffre)
{
	// Variables pour le tirage d'un deuxieme coffre
	int coffre_bis = NULL;


	/* ------ S'il reste 3 coffres ------ */
	if (nb_coffres == 3) {

		// On ouvre un des deux coffres restant qu iest vide
		if (coffre == COFFRE1) {
			coffre_bis = coffre_vide(COFFRE2, COFFRE3);
		}
		else if (coffre == COFFRE2) {
			coffre_bis = coffre_vide(COFFRE1, COFFRE3);
		}
		else if (coffre == COFFRE3) {
			coffre_bis = coffre_vide(COFFRE1, COFFRE2);
		}

		// On l'ouvre
		ouvrir_coffre(coffre_bis);
	}


	/* ------ S'il ne reste que deux coffres en jeu ------ */
	else if (nb_coffres == 2)
	{
		if (coffres[coffre] != OUVERT)
		{
			// On verifie si c'est le bon coffre et que donc le joueur à gagné
			if (coffre_tresor == coffre) {
			victoire();
			chargement_image(coffre, TRESOR);
			}
			// Si c'est pas le bon il a perdu
			else
			{
				chargement_image(coffre, OUVERT);
				coffres[coffre] = OUVERT;
				defaite();
			}
		}
	}
}

/* ------------------------------------------- */

// Pour ouvrir un coffre
void ouvrir_coffre(int coffre)
{
	if (coffre_tresor == coffre) {
	chargement_image(coffre, TRESOR);
	coffres[coffre] = OUVERT;
	defaite();
	}
	else {
		chargement_image(coffre, OUVERT);
		coffres[coffre] = OUVERT;
		nb_coffres -= 1;
	}
}

/* ------------------------------------------- */

// Reinitialiser les boutons
void boutons_reset(int action)
{
	if (action == INIT)
	{
	gtk_widget_set_sensitive(bt_reset, FALSE);
	gtk_widget_set_sensitive(bt_reset_score, TRUE);
	}

	else if (action == TERMINE)
	{
	gtk_widget_set_sensitive(bt_reset, TRUE);
	gtk_widget_set_sensitive(bt_reset_score, TRUE);
	}
}

/* ------------------------------------------- */

// En cas de victoire
void victoire()
{
	// On incrémente le nb de victoire et on met à jour le score
	nb_victoires++;
	nb_coffres -= 1;
	maj_score(VICTOIRE);

	boutons_reset(TERMINE);
}

/* ------------------------------------------- */

// En cas de defaite
void defaite()
{
	// On incrémente le nb de victoire et on met à jour le score
	nb_defaites++;
	nb_coffres -= 1;
	maj_score(DEFAITE);

	boutons_reset(TERMINE);
}

/* ------------------------------------------- */

// Tirage dun nombre au hasard
int nombre_hasard (int min, int max)
{
    srand(time(NULL));
    return (rand() % (max - min + 1) + min);
}

/* ------------------------------------------- */

// Renvoi le coffre vide entre deux coffres
int coffre_vide (int coffre1, int coffre2)
{
	int coffre, coffre_bis;
	int nb_hasard = nombre_hasard(0, 1);

	if (nb_hasard == 0) { coffre = coffre1;coffre_bis = coffre2; }
	else { coffre = coffre2;coffre_bis = coffre1; }

	if (coffre == coffre_tresor) { return coffre_bis; }
	else return coffre;
}

/* ------------------------------------------- */

// Mise a jour du score du joueur
void maj_score(int partie)
{
	// Les variables contenant les chaines de caracteres
	char score[N], stats[N], msg[N];

	// Pour calculer les stats
	if (nb_victoires > 0 || nb_defaites > 0)
	{
	  stats_victoires = ( nb_victoires * 100 ) / ( nb_victoires + nb_defaites );
	  stats_defaites = ( nb_defaites * 100 ) / ( nb_defaites + nb_victoires );
	}
	else
	{
		stats_victoires = 0;
		stats_defaites = 0;
	}

	// On maj le score et les stats
	sprintf(score, "Victoires : %d   Défaites : %d", nb_victoires, nb_defaites);
	sprintf(stats, "%d %% Victoire(s) / %d %% Défaite(s)", stats_victoires, stats_defaites);

	/* ---- MAJ du message ---- */
	if (partie == MAJ) {
	sprintf(msg, "Choisissez un coffre !");
	}

	else if (partie == VICTOIRE) {
	sprintf(msg, "Bravo; vous avez gagné !");

	}

	else if (partie == DEFAITE) {
	sprintf(msg, "Pas de chance, recommencez !");
	}

	// On envoie tout ça aux labels
	gtk_label_set_text(GTK_LABEL(tScore), score);
	gtk_label_set_text(GTK_LABEL(tMsg), msg);
	gtk_label_set_text(GTK_LABEL(tStats), stats);
}

/* ------------------------------------------- */

// Chargement d'image dans les boutons
void chargement_image (int coffre, int etat)
{
	// Declaration widget qui stockera le bouton
	GtkWidget* bt_coffre;

	if (etat == FERME) { img_coffre = gdk_pixbuf_new_from_file("img_coffre_ferme.png", NULL); }
	else if (etat == OUVERT) { img_coffre = gdk_pixbuf_new_from_file("img_coffre_ouvert.png", NULL); }
	else if (etat == TRESOR) { img_coffre = gdk_pixbuf_new_from_file("img_coffre_tresor.png", NULL); }

	// Verification des images
	if (img_coffre == NULL)
		printf("\nErreur en chargeant l'image img_coffre_ferme.png\n");
	else		
		img_coffre_100 = gdk_pixbuf_scale_simple(img_coffre, 100, 100, GDK_INTERP_NEAREST);
	


	// Affectation du bouton à charger
	if (coffre == COFFRE1) { bt_coffre = bt_coffres[COFFRE1]; }
	else if (coffre == COFFRE2) { bt_coffre = bt_coffres[COFFRE2]; }
	else if (coffre == COFFRE3) { bt_coffre = bt_coffres[COFFRE3]; }

	// Chargement de l'image dans le bouton
	gtk_button_set_image(GTK_BUTTON(bt_coffre),  gtk_image_new_from_pixbuf(img_coffre));

}

/* --------------------------------------------------------- */
