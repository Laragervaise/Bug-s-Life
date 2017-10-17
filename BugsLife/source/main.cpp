/*!
 \file main.cpp
 \brief Module qui gère GLUT, GLUI et OPENGL et qui récupère le nom
  du mode et le nom du fichier
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <GL/glui.h>

extern "C"
{   
#include "modele.h"  
#include "constantes.h"    
}

#define INIT_SIZE 400
#define INIT_POSITION 500
#define NB_LINES 11
#define NB_COLUMNS 4

/************** User IDs pour les fonctions callback ********/
#define EDITTEXT1_ID 11
#define BUTTON1_ID 12
#define EDITTEXT2_ID 13
#define BUTTON2_ID 14
#define RADIOGROUP_ID 21
#define BUTTON3_ID 31
#define BUTTON4_ID 32
#define CHECKBOX_ID 33

enum RolloutColonne{FOURMIS_TOTAL, OUVRIERES, GARDES, NOURRITURES};

namespace
{ 
    PRGMMODE prgmmode;
   
    GLUI * glui; //interface glui
    int main_window;
    int width, height; //largeur et hauteur de la fenetre
    //Variables qui définissent le modèle visualisé courant avec GLOrtho
    GLfloat x_min, x_max, y_min, y_max;
    GLfloat aspect_ratio;
 
    /*Fichier pour l'affichage du graphe avec gnuplot*/
    FILE *fichier_gnuplot = NULL;
    
    /*Booléens*/
    //initialisation du jeu en stop au depart
    bool play = false;
    //initialisation de la création de nourriture en mode automatic
    bool create_nourriture = true;
    //initialisation de record non activé
    bool record = false;
    
    /*tableau de float qui contient les informations pour le rollout*/
    int tab_info[NB_COLUMNS][NB_LINES];
    
    /*tableau de char qui remplit le rollout*/
    GLUI_StaticText * tab_info_text[NB_COLUMNS][NB_LINES];
    
    /*pointeurs pour créer les objets de GLUI*/
    GLUI_Checkbox *checkbox;
    GLUI_EditText *edittext1;
    GLUI_EditText *edittext2;
    GLUI_EditText *edittext3;
    GLUI_RadioGroup *radio;
    GLUI_Button *button_open;
    GLUI_Button *button_save;
    GLUI_Button *button_start_stop;
    GLUI_Button *button_step;
    GLUI_Rollout *rollout;
}

// Prototypes des fonctions

//---------------------------------------------------------------------
// définit le mode et renvoie un PRGMMODE
PRGMMODE def_mode(char * nom_mode, char * nom_fichier, int argc);

void display_cb(void);

//---------------------------------------------------------------------
// redessine dans la fenêtre OPEN GL
void redraw_all(void);

//---------------------------------------------------------------------
// adapte la taille de la fenêtre si on la déforme
void reshape_cb(int x, int y);

void idle(void);

//---------------------------------------------------------------------
// positionne le point cliqué par la souris
void set_point_cb(int button, int button_state, int x, int y);

//---------------------------------------------------------------------
// crée les boutons du pannel gauche
void creation_panels_gauche(char * nom_fichier);

//---------------------------------------------------------------------
// crée le tableau rollout du pannel droit
void creation_panels_droite(GLUI * glui);

//---------------------------------------------------------------------
// gère l'ensemble des callbacks
void control_cb(int control);

//---------------------------------------------------------------------
// gère la callback pour le radiogroup "manual" ou "automatic" pour le 
// mode de création de nourriture
void control_cb_create_nourriture(void);

//---------------------------------------------------------------------
// gère la callback start/stop
void control_cb_start(void);

//---------------------------------------------------------------------
// gère la callback step
void control_cb_step(void);

//---------------------------------------------------------------------
// gère le checkbox record
void control_cb_checkbox_record(void);

//---------------------------------------------------------------------
// met à jour le tableau de rollout
void update_window(void);


/****************************** main() *******************************/
int main(int argc, char *argv[])
{
    fichier_gnuplot = fopen ((char*) "out.dat", "w");
    
    switch(prgmmode = def_mode(argv[1], argv[2], argc))
    {    
        case ERROR :
            modele_lecture(argv[2], prgmmode);
            return EXIT_SUCCESS;
            break;
        case VERIFICATION :
            if(!modele_lecture(argv[2], prgmmode))
                return EXIT_FAILURE;
            else
            {
                modele_verification_rendu2();
                return EXIT_SUCCESS;
            }
            break;
        case GRAPHIC :
            if(!modele_lecture(argv[2], prgmmode))
                modele_nettoyer();
            else if(!modele_verification_rendu2())
                modele_nettoyer();
            break;
        case FINAL :
            if(!modele_lecture(argv[2], prgmmode))
                modele_nettoyer();
            else if (!modele_verification_rendu2())
                modele_nettoyer();
        case RIEN :
            break;
        case INCORRECT :
            return EXIT_FAILURE;
            break;
        default :
            break;
    }
    
    int i, j; //Initialisation du tableau tab_float à zéro
    for(i=0 ; i< NB_COLUMNS ; i++)
        for(j=0; j< NB_LINES ; j++)
            tab_info[i][j] = 0;
    
    //Création d'une fenetre GLUT (pour dessin OPENGL)
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( INIT_POSITION, INIT_POSITION );
    glutInitWindowSize( INIT_SIZE, INIT_SIZE );
    
    main_window = glutCreateWindow("Bug's Life");
    glutDisplayFunc(display_cb);
    glutReshapeFunc(reshape_cb);
    glutIdleFunc(idle);
    glutMouseFunc(set_point_cb);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glui = GLUI_Master.create_glui((char*)"Bug's Life");
    creation_panels_gauche(argv[2]);
    creation_panels_droite(glui);
    update_window();
    glui->set_main_gfx_window(main_window);
    
    glutPostRedisplay();
    glutMainLoop();
}

PRGMMODE def_mode(char* nom_mode, char* nom_fichier, int argc)
{
    if (argc == 1)
        return RIEN;
    else if (argc != 3)//Vérification de la cohérence de l'appel de l'exécutable
    {
        printf("erreur : usage : '././main.x mode_test nom_fichier'\n");
        return INCORRECT;
    }
    if(!strcmp(nom_mode, (char*) "Verification"))
        return VERIFICATION;
    else if(!strcmp(nom_mode, (char*) "Error"))
        return ERROR;
    else if(!strcmp(nom_mode, (char*) "Graphic"))
        return GRAPHIC;
    else if(!strcmp(nom_mode, (char*) "Final"))
        return FINAL;
    else
    {
        printf("erreur : usage : '././main.x mode_test nom_fichier'\n");
        return INCORRECT;
    }
    return RIEN;
}

void display_cb (void)
{
    redraw_all();
    update_window();
}

void redraw_all(void)
{
    glutSetWindow(main_window);
    /* Efface le contenu de la fenetre. */
    glClear(GL_COLOR_BUFFER_BIT);
    
    reshape_cb(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    modele_dessine();
    
    /* Affiche l'image a l'ecran. */
    glutSwapBuffers();
}

void reshape_cb (int x, int y)
{
    glViewport( 0, 0, x, y);
    //Mémorisation pour la conversion des coordonnees souris vers OPENGL
    width=x;
    height=y;
    
    aspect_ratio = (float)width/height;
    
    /* Definit le domaine. */
    x_min=-DMAX, x_max=DMAX, y_min=-DMAX, y_max=DMAX;
    glLoadIdentity();
    
    if(aspect_ratio <=1.)
        glOrtho(x_min, x_max, y_min/aspect_ratio, y_max/aspect_ratio, -1.0, 1.0);
    else
        glOrtho(x_min*aspect_ratio, x_max*aspect_ratio, y_min, y_max, -1.0, 1.0);
}    

void idle(void)
{
    //on transmet à modele_update un nouveau booléen
    bool automatic = create_nourriture;
    bool record_active = record;
    
    if(glutGetWindow() != main_window)
        glutSetWindow(main_window);
    if(play)
    {
        if(prgmmode == GRAPHIC)
            printf("modele update\n");
        else
        {
            modele_update(automatic, record_active, fichier_gnuplot);
            glutPostRedisplay();
        }
    }
}

void set_point_cb(int button, int button_state, int x, int y)
{
    double souris_x, souris_y;
    souris_x = (x_max - x_min)*((double)x/width) + x_min;
    souris_y = y_max - (y_max - y_min)*((double)y/height);
    
    //la nourriture est affichée seulement si l'utilisateur relache le bouton
    // si c'est en mode manuel
    if(!create_nourriture && button == GLUT_RIGHT_BUTTON &&
       button_state == GLUT_DOWN )
    {
        modele_nourriture_manual_creation(souris_x, souris_y);
        glutPostRedisplay();
    }
}

void creation_panels_gauche(char * nom_fichier)
{
    //panel1
    GLUI_Panel *file_panel = glui->add_panel((char*)"File",
                                             GLUI_PANEL_EMBOSSED);
                                             
    edittext1 = glui->add_edittext_to_panel(file_panel,(char*)"FileName:");
    edittext1->set_text(nom_fichier);
    button_open = glui->add_button_to_panel(file_panel, (char*)"Open",
                                            BUTTON1_ID, control_cb);
    edittext2 = glui->add_edittext_to_panel(file_panel,(char*)"FileName:");
    edittext2->set_text((char*)"save.txt");
    button_save = glui->add_button_to_panel(file_panel, (char*)"Save",
                                            BUTTON2_ID, control_cb);
    //panel2
    GLUI_Panel *food_panel = glui->add_panel((char*)"Food creation",
                                             GLUI_PANEL_EMBOSSED);
                                             
    radio = glui->add_radiogroup_to_panel(food_panel, NULL,
                                          RADIOGROUP_ID, control_cb);
    glui->add_radiobutton_to_group(radio, (char*)"Automatic");
    glui->add_radiobutton_to_group(radio, (char*)"Manual");
    
    //panel3
    GLUI_Panel *simulation_panel = glui->add_panel((char*)"Simulation",
                                                   GLUI_PANEL_EMBOSSED);
                                                   
    button_start_stop = glui->add_button_to_panel(simulation_panel,
                        (char*)"Start !", BUTTON3_ID, control_cb);
    button_step = glui->add_button_to_panel(simulation_panel,(char*)"Step",
                                            BUTTON4_ID, control_cb);
    checkbox= glui->add_checkbox_to_panel(simulation_panel,
              (char*)"Record", NULL, CHECKBOX_ID, control_cb);
    
    //Exit button
    glui->add_button((char*) "Exit", 0, (GLUI_Update_CB)exit);
}

void creation_panels_droite(GLUI * glui)
{
    glui->add_column(true); //Décale le tableau sur la droite de la fenetre
    GLUI_Rollout * array = glui->add_rollout((char*) "Information", true,
                                             GLUI_PANEL_RAISED);    
    
    glui->add_statictext_to_panel(array, (char*) "Couleur");
    glui->add_statictext_to_panel(array, (char*) "Rouge");
    glui->add_statictext_to_panel(array, (char*) "Vert");
    glui->add_statictext_to_panel(array, (char*) "Bleu");
    glui->add_statictext_to_panel(array, (char*) "Magenta");
    glui->add_statictext_to_panel(array, (char*) "Cyan");
    glui->add_statictext_to_panel(array, (char*) "Jaune");
    glui->add_statictext_to_panel(array, (char*) "Marron");
    glui->add_statictext_to_panel(array, (char*) "Orange");
    glui->add_statictext_to_panel(array, (char*) "Gris");
    glui->add_statictext_to_panel(array, (char*) "Rose");
    glui->add_statictext_to_panel(array, (char*) "Total");
    
    int i, j;
    for(i=0; i<NB_COLUMNS; i++)
    {
        glui->add_column_to_panel(array, true);
        
        switch(i)
        {
            case FOURMIS_TOTAL :
                glui->add_statictext_to_panel(array, (char*) "Fourmis total");
                break;
            case OUVRIERES :
                glui->add_statictext_to_panel(array, (char*) "Ouvrieres");
                break;
            case GARDES :
                glui->add_statictext_to_panel(array, (char*) "Gardes");
                break;
            case NOURRITURES : 
                glui->add_statictext_to_panel(array, (char*) "Nourriture");
                break;
        }
        
        for(j=0; j<NB_LINES; j++)
            tab_info_text[i][j] = glui->add_statictext_to_panel(array, (char*)"0");
    }
}

void control_cb(int control) 
{
    switch(control)
    {
        case (EDITTEXT1_ID) :
            printf("edit text 1: %s\n", edittext1->get_text());
            break;
        case (BUTTON1_ID) :
            printf("button open clicked !\n");
            if(!play)
            {
                modele_nettoyer();
                if (!modele_lecture((char *)edittext1->get_text(), prgmmode))
                    modele_nettoyer();
                else if (!modele_verification_rendu2())
                    modele_nettoyer();
                glutPostRedisplay();
            }
            break;
        case (EDITTEXT2_ID) :
            printf("edit text 2: %s\n", edittext1->get_text()); 
            break;
        case (BUTTON2_ID) :
            printf("button save clicked!\n"); 
            modele_ecriture((char*)edittext2->get_text());
            break;
        case (RADIOGROUP_ID) : 
            control_cb_create_nourriture();
            break;
        case (BUTTON3_ID) :
            control_cb_start();
            break;
        case (BUTTON4_ID) :
            control_cb_step();
            break;
        case (CHECKBOX_ID) :
            control_cb_checkbox_record();
            break;
        default : 
            printf("\n Unknown command\n");
            break;
    }
}

void control_cb_create_nourriture(void)
{
    if(!(radio->get_int_val()))
    {
        printf("Automatic food creation\n");
        create_nourriture = true;
    }
    else 
    {
        printf("Manual food creation\n");
        create_nourriture = false;
    }
}

void control_cb_start(void)
{
    printf("button start clicked!\n");
    if(play) 
    {
        button_start_stop->set_name((char*) "Start");
        play = false; // Mode pause
        if(checkbox->get_int_val()) // si record activé
        {
            record = false;
            checkbox->set_int_val(0); //désactive la checkbox
            fclose(fichier_gnuplot);
        }	
    }
    else
    {
        button_start_stop->set_name((char*) "Stop");
        play = true;
    }
}

void control_cb_step(void)
{
	printf("one step\n");
    play = false;
    bool automatic = create_nourriture;
    bool record_active = record;
    
    if(glutGetWindow() != main_window) 
        glutSetWindow(main_window);
    
    modele_update(automatic, record_active, fichier_gnuplot);
    glutPostRedisplay();
    button_start_stop->set_name((char*) "Start");
}

void control_cb_checkbox_record(void)
{
    bool automatic = create_nourriture;
    bool record_active = record;
    
    if(checkbox->get_int_val())
    {
        record = true;
        printf("Record activé!\n"); 
        modele_update(automatic, record_active, fichier_gnuplot);
    }
    else
    {
		printf("Record desactivé!\n"); 
        record = false;
    }
}

void update_window(void)
{
    char buffer[15];
    int m, n;
    
    modele_recup_info(tab_info);
    
    for(m=0; m< NB_COLUMNS; m++)
        for(n=0; n<NB_LINES; n++)
        {
            if(tab_info[m][n] <=0)
                tab_info_text[m][n]->set_text("0");
            else
            {
                sprintf(buffer, "%d", tab_info[m][n]);
                tab_info_text[m][n]->set_text(buffer);
            }
        }
}
