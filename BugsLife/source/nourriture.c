/*!
 \file nourriture.c
 \brief Module qui gère les informations relatives à la nourriture
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "error.h"
#include "utilitaire.h"
#include "graphic.h"
#include "nourriture.h"

/*---------------------------------------------------------------------
 Structure de données d'une nourriture
	indice_n : indice de la nourriture
	x , y : coordonnées de la nourriture
	suivant : pointeur de type structure Nourriture qui pointe la
			  nourriture suivante de la liste chainée
 ----------------------------------------------------------------------*/
struct Nourriture
{
    unsigned indice_n;
    double x;
    double y;
    NOURRITURE * suivant;
};

static NOURRITURE * tete_nourriture = NULL;
static int nb_nourriture;

bool nourriture_lecture(char tab[MAX_LINE], int * pn, bool * pfin_ligne)
{
    int n;
    double x, y;
    char * deb = tab, * fin = NULL;
    *pfin_ligne = false;
    
    if(*pn == -1)
        (*pn)++;
    
    for(n=*pn; n<nb_nourriture; n++)
    {
        if(sscanf(deb, " %lf %lf", &x, &y) < 2)
        {
            *pfin_ligne = true;
            *pn = n;
            return 1;
        }
        
        nourriture_ajouter();
        
        tete_nourriture->indice_n = n;
        tete_nourriture->x = x;
        tete_nourriture->y = y;
        
        int m;
        for(m = 0; m < 2; m++)
        {
            strtod(deb, &fin);
            deb = fin;
        }
        
        if(!nourriture_erreur(n, x, y))
            return 0;
    }
    
    *pn = n;
    return 1;
}

void nourriture_ajouter(void)
{
    NOURRITURE * nouveau = NULL;
    if(!(nouveau = (NOURRITURE *) malloc (sizeof(NOURRITURE))))
        printf("Probleme de mémoire dans la fonction %s\n", __func__);
    
    nouveau->suivant = tete_nourriture;
    tete_nourriture = nouveau;
}

bool nourriture_erreur(int n, double x, double y)
{
    if(x <= -DMAX || y <= -DMAX || x >= DMAX || y >= DMAX)
    {
        error_pos_domaine(ERR_NOURRITURE, n, x, y);
        return 0;
    }
    
    return 1;
}

void nourriture_dessine(void)
{
    NOURRITURE * courant = tete_nourriture;
    while(courant)
    {
        graphic_draw_circle(courant->x, courant->y, RAYON_FOOD, GRAPHIC_EMPTY, NOIR);
        courant = courant->suivant;
    }
}

void nourriture_vider(void)
{
    NOURRITURE * a_retirer = NULL;
    while(tete_nourriture)
    {
        a_retirer = tete_nourriture;
        tete_nourriture = tete_nourriture->suivant;
        free(a_retirer);
        a_retirer = NULL;
    }
    tete_nourriture = NULL;
}

void nourriture_set_nb(int set)
{
    nb_nourriture = set;
}

void nourriture_update_manual_creation(double x, double y)
{
    if(nourriture_superposition(x, y))
        printf("Impossible de créer de la nourriture ici\n");
    else
    {
        nourriture_ajouter();
        tete_nourriture->indice_n = nb_nourriture;
        tete_nourriture->x = x;
        tete_nourriture->y = y;
        nb_nourriture++;
    }
}

void nourriture_update_automatic_creation(void)
{
    if((double)rand()/RAND_MAX <= FOOD_RATE)
    {
        nourriture_ajouter();
        tete_nourriture->indice_n = nb_nourriture;
        nb_nourriture++;
        
        do
        {
            tete_nourriture->x = ((double)rand()/RAND_MAX)*(2*DMAX) - DMAX;
            tete_nourriture->y = ((double)rand()/RAND_MAX)*(2*DMAX) - DMAX;
        }
        while(nourriture_superposition(tete_nourriture->x, tete_nourriture->y));
    }
}

bool nourriture_superposition(double x, double y)
{
    if(nourriture_entre_nourriture_superposition(x, y))
        return 1;
    
    if(fourmi_nourriture_superposition(x, y))
        return 1;
    
    if(fourmiliere_nourriture_superposition(x, y))
        return 1;
    
    return 0;
}

bool nourriture_entre_nourriture_superposition(double x, double y)
{
    if(tete_nourriture)
    {
        NOURRITURE * a_tester = tete_nourriture->suivant;
        
        while(a_tester)
        {
            if(utilitaire_contact(x, a_tester->x, y, a_tester->y,
                                  RAYON_FOOD, RAYON_FOOD))
                return 1;
            
            a_tester = a_tester->suivant;
        }
    }
    return 0;
}

bool nourriture_manger(double pos_ouvriere_x, double pos_ouvriere_y)
{
    NOURRITURE * a_tester = tete_nourriture, * precedent = NULL;
    
    while(a_tester)
    {
        if(utilitaire_contact_sans_tolerance(pos_ouvriere_x, a_tester->x,
                                             pos_ouvriere_y, a_tester->y, 
                                             RAYON_FOURMI, RAYON_FOOD))
        {
            if(a_tester == tete_nourriture)
                tete_nourriture = tete_nourriture->suivant;
            else
                precedent->suivant = a_tester->suivant;
            
            if(a_tester)
				free(a_tester);
            a_tester = NULL;
            nb_nourriture--;
            return 1;
        }
        
        precedent = a_tester;
        a_tester = a_tester->suivant;
    }
    
    return 0;
}

void nourriture_update_ouvriere_dead(double posx_o, double posy_o)
{
    nourriture_ajouter();
    tete_nourriture->indice_n = nb_nourriture;
    tete_nourriture->x = posx_o;
    tete_nourriture->y = posy_o;
    nb_nourriture++;
}

bool nourriture_la_plus_proche(double ouvriere_x, double ouvriere_y,
                               double * pnourriture_x, double * pnourriture_y,
                               unsigned indice_f, unsigned indice_o)
{
    NOURRITURE * a_tester = tete_nourriture;
    double distance_min = 2*DMAX, distance;
    /*car une distance ne sera jamais plus grande que 2DMAX (notre intervalle)
    Ainsi la premiere distance evaluée sera implicitement considérée 
    comme la plus petite ce qui simplifie le code*/
    
    bool nourriture_mangeable_trouvee = false;
    
    while(a_tester)
    {
        distance = utilitaire_calcul_norme(ouvriere_x, a_tester->x, ouvriere_y, 
                                           a_tester->y);
        
        if(distance < distance_min)
        {
            if(!fourmi_nourriture_deja_prise(indice_f, indice_o, a_tester->x, 
											 a_tester->y)
               && !fourmi_etrangere_plus_proche(indice_f, a_tester->x, 
												a_tester->y, distance)
               && !fourmiliere_sur_chemin(ouvriere_x, a_tester->x, ouvriere_y, 
										  a_tester->y, indice_f))
            {
                distance_min = distance;
                *pnourriture_x = a_tester->x;
                *pnourriture_y = a_tester->y;
                nourriture_mangeable_trouvee = true;
            }
        }
        
        a_tester = a_tester->suivant;
    }
    
    if(nourriture_mangeable_trouvee)
        return 1;
    return 0;
}

void nourriture_ecriture(FILE * fsortie)
{
    unsigned compteur = 0; // Pour afficher 3 nourritures par ligne
    NOURRITURE * courant = tete_nourriture;
    
    fprintf(fsortie, "# Nb Nourriture \n");
    fprintf(fsortie, "%d\n\n", nb_nourriture);
    fprintf(fsortie, "    ");
    
    while(courant)
    {
        if(compteur == 3)
        {
            fprintf(fsortie, "\n");
            fprintf(fsortie, "    ");
            compteur = 0;
        }
        
        fprintf(fsortie, "%lf %lf ", courant->x, courant->y);
        courant = courant->suivant;
        compteur ++;
    }
    
    fprintf(fsortie, "\n");
    
    if(nb_nourriture)
        fprintf(fsortie, "FIN_LISTE \n");
}
