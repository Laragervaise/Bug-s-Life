/*!
 \file fourmiliere.c
 \brief Module qui gère les informations relatives aux fourmilieres
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
#include "fourmiliere.h"

#define NB_LINES   11
#define NB_COLUMNS 4

/*---------------------------------------------------------------------
 Structure de données d'une fourmilière
	indice_f : indice de la fourmilière
	x , y : coordonnées de la fourmilière
	nbO : nombre d'ouvrières de la fourmilière
	nbG : nombre de gardes de la fourmilière
	nbF : nombre total de fourmis de la fourmilière
	total_food : stock de nourriture de la fourmilière
	rayon : rayon de la fourmilière
	rayon recalculé : rayon en fonction de nbF et de la nourriture
                      total_food que contient la fourmilière
	suivant : pointeur de type structure Fourmiliere qui pointe sur la
              fourmiliere suivante de la liste chaînée
 ----------------------------------------------------------------------*/
struct Fourmiliere
{
    unsigned indice_f;
    double x;
    double y;
    int nbO;
    int nbG;
    int nbF;
    float total_food;
    double rayon;
    double rayon_recalcule;
    FOURMILIERE * suivant;
};

enum Choix_naissance {NAISSANCE_OUVRIERE, NAISSANCE_GARDE};

static FOURMILIERE * tete_fourmiliere = NULL;
static int nb_fourmiliere;
static int choix = NAISSANCE_GARDE;

bool fourmiliere_lecture(char tab[MAX_LINE], int f, int * pnbO, int * pnbG, 
						 double * pcentre_x, double * pcentre_y, 
						 double * prayon_fourmiliere)
{
    int nb_caract;
    float total_food;
    
    nb_caract = sscanf(tab, " %lf %lf %d %d %f %lf", pcentre_x, pcentre_y, pnbO, 
					   pnbG, &total_food, prayon_fourmiliere);
    
    fourmiliere_ajouter(f);
    
    tete_fourmiliere->x = *pcentre_x;
    tete_fourmiliere->y = *pcentre_y;
    tete_fourmiliere->nbO = *pnbO;
    tete_fourmiliere->nbG = *pnbG;
    tete_fourmiliere->total_food = total_food;
    tete_fourmiliere->rayon = *prayon_fourmiliere;
    tete_fourmiliere->nbF = *pnbO + *pnbG;
    tete_fourmiliere->rayon_recalcule = (1 + sqrt(tete_fourmiliere->nbF) 
										 + sqrt(total_food))*RAYON_FOURMI;
    
    return fourmiliere_erreur(nb_caract, f, tete_fourmiliere);
}

void fourmiliere_ajouter(int f)
{
    FOURMILIERE * nouveau = NULL;
    if(!(nouveau = (FOURMILIERE *) malloc (sizeof(FOURMILIERE))))
        printf("Problème de mémoire dans la fonction %s\n", __func__);
    
    nouveau->indice_f = f;
    nouveau->suivant = tete_fourmiliere;
    tete_fourmiliere = nouveau;
}

bool fourmiliere_erreur(int nb_caract, int f, FOURMILIERE * tete_fourmiliere)
{
    if(nb_caract < 6)
    {
        error_lecture_elements_fourmiliere(f, ERR_FOURMILIERE, ERR_PAS_ASSEZ);
        return 0;
    }
    
    if(tete_fourmiliere->x <= -DMAX || tete_fourmiliere->y <= -DMAX
       || tete_fourmiliere->x >= DMAX || tete_fourmiliere->y >= DMAX)
    {
        error_pos_domaine(ERR_FOURMILIERE, f, tete_fourmiliere->x,
                          tete_fourmiliere->y);
        return 0;
    }
    
    if(tete_fourmiliere->rayon > tete_fourmiliere->rayon_recalcule)
    {
        error_rayon_fourmiliere(f);
        return 0;
    }
    
    return 1;
}

bool fourmiliere_verification_rendu2(void)
{
    FOURMILIERE * f1 = tete_fourmiliere;
    FOURMILIERE * f2 = tete_fourmiliere->suivant;
    
    while(f1)
    {
        while(f2)
        {
            if(utilitaire_contact_sans_tolerance(f1->x, f2->x, f1->y, f2->y, 
												 f1->rayon, f2->rayon))
            {
                error_superposition_fourmiliere(f1->indice_f, f2->indice_f);
                return 0;
            }
            f2 = f2->suivant;
        }
        
        f1 = f1->suivant;
        if(!f1)
            break;
        f2 = f1->suivant;
    }
    
    if(!fourmi_verification_rendu2())
        return 0;
    return 1;
}

void fourmiliere_dessine(void) 
{								
    FOURMILIERE * courant = tete_fourmiliere;
    
    while(courant)
    {
        graphic_draw_circle(courant->x, courant->y, courant->rayon, GRAPHIC_EMPTY, 
							courant->indice_f);
        courant = courant->suivant;
    }
}

void fourmiliere_vider(void)
{
    FOURMILIERE * a_retirer = NULL;
    
    while(tete_fourmiliere)
    {
        a_retirer = tete_fourmiliere;
        tete_fourmiliere = tete_fourmiliere->suivant;
        free(a_retirer);
        a_retirer = NULL;
    }
    
    tete_fourmiliere = NULL;
}

void fourmiliere_set_nb(int set)
{
    nb_fourmiliere = set;
}

int fourmiliere_get_nb()
{
    return nb_fourmiliere;
}

void fourmiliere_update(void)
{
    FOURMILIERE * a_tester = tete_fourmiliere, * precedent = NULL;
    
    while(a_tester)
    {
        int nourriture_rapportee = 0;
        
        //Naissance de la fourmi 
        if((double)rand()/RAND_MAX <= a_tester->total_food*BIRTH_RATE)
            fourmiliere_naissance_choix_fourmi(a_tester);
        
        //Mise à jour de la quantite de nourriture dans la fourmilière
        a_tester->total_food = a_tester->total_food - a_tester->nbF*FEED_RATE;
        
        //Mise à jour du rayon théorique
        a_tester->rayon_recalcule = (1 + sqrt(a_tester->nbF) + 
									sqrt((int)a_tester->total_food))*RAYON_FOURMI;
        
        //Mise à jour du rayon qui va être dessiné seulement si il grandit
        if(a_tester->rayon_recalcule > a_tester->rayon)
            a_tester->rayon = fourmiliere_nouveau_rayon(a_tester);
        
        //Mise à jour du nombre de fourmis
        a_tester->nbO -= fourmi_ouvriere_update(a_tester->indice_f,
                                                &nourriture_rapportee, a_tester->x,
                                                a_tester->y);
        
        a_tester->nbG -= fourmi_garde_update(a_tester->indice_f, a_tester->x, 
											 a_tester->y, a_tester->rayon);
        
        a_tester->nbF = a_tester->nbO + a_tester->nbG;
        
       
        a_tester->total_food += nourriture_rapportee;
        
        //Mort fourmilière si plus de fourmis et de nourriture
        if(!a_tester->nbF && a_tester->total_food < VAL_FOOD)
        {
            fourmiliere_dead(a_tester, precedent);
            a_tester = precedent;
            break;
        }
        
        precedent = a_tester;
        a_tester = a_tester->suivant;
    }
}

void fourmiliere_naissance_choix_fourmi(FOURMILIERE * a_tester)
{
    switch(choix)
    {
        case NAISSANCE_OUVRIERE :
            fourmi_ouvriere_ajouter();
            fourmi_ouvriere_naissance(a_tester->x, a_tester->y, a_tester->indice_f);
            (a_tester->nbO)++;
            choix = NAISSANCE_GARDE;
            break;
            
        case NAISSANCE_GARDE :
            fourmi_garde_ajouter();
            fourmi_garde_naissance(a_tester->x, a_tester->y, a_tester->indice_f);
            (a_tester->nbG)++;
            choix = NAISSANCE_OUVRIERE;
            break;
    }
}

double fourmiliere_nouveau_rayon(FOURMILIERE * f1)
{
    FOURMILIERE * f2 = tete_fourmiliere;
    double distance, nouveau_rayon = f1->rayon_recalcule;
    
    while(f2)
    {
		//Si contact entre deux fourmilières différentes
        if(f2 != f1 && utilitaire_contact(f1->x, f2->x, f1->y, f2->y,
                                          f1->rayon_recalcule, f2->rayon))
        {
            distance = utilitaire_calcul_norme(f1->x, f2->x, f1->y, f2->y) - f2->rayon;
            
            if(distance < nouveau_rayon)
                nouveau_rayon = distance - EPSIL_ZERO;
        }
        
        f2 = f2->suivant;
    }
    
    return nouveau_rayon;
}

void fourmiliere_dead(FOURMILIERE * a_tester, FOURMILIERE * precedent)
{
    if(a_tester == tete_fourmiliere)
        tete_fourmiliere = tete_fourmiliere->suivant;
    else
        precedent->suivant = a_tester->suivant;
    
    if(a_tester)
		free(a_tester);
    a_tester = NULL;
    nb_fourmiliere--;
}

bool fourmiliere_sur_chemin(double posx, double butx, double posy, double buty, 
							unsigned indice_f)
{
    FOURMILIERE * a_tester = tete_fourmiliere;
    
    while(a_tester)
    {
        if(a_tester->indice_f != indice_f && 
		   utilitaire_distance_point_droite(posx, butx, posy, buty, a_tester->x, 
											a_tester->y) 
		   - (a_tester->rayon + RAYON_FOURMI) <=  EPSIL_ZERO)
            return 1;
        
        a_tester = a_tester->suivant;
    }
    
    return 0;
}

bool fourmiliere_nourriture_superposition(double nourriture_x, double nourriture_y)
{
    FOURMILIERE * a_tester = tete_fourmiliere;
    
    while(a_tester)
    {
        if(utilitaire_contact(nourriture_x, a_tester->x, nourriture_y,
                              a_tester->y, a_tester->rayon, RAYON_FOOD))
            return 1;
        
        a_tester = a_tester->suivant;
    }
    
    return 0;
}

bool fourmiliere_fourmi_inside(double ouvriere_x, double ouvriere_y, unsigned indice_f)
{
    FOURMILIERE * a_tester = tete_fourmiliere;
    
    while(a_tester)
    {
        if(a_tester->indice_f != indice_f &&
           fourmi_contact_fourmiliere(ouvriere_x, a_tester->x, ouvriere_y, 
                                      a_tester->y, a_tester->rayon))
            return 1;
        
        a_tester = a_tester->suivant;
    }
    
    return 0;
}

bool fourmiliere_la_plus_proche(double ouvriere_x, double ouvriere_y, 
								unsigned indice_f, double * fourmiliere_a_attaquer_x, 
								double * fourmiliere_a_attaquer_y)
{
    FOURMILIERE * a_tester = tete_fourmiliere;
    double distance, plus_petite_distance = 2*DMAX;
    bool fourmiliere_attaquable_trouvee = false;
    /*car une distance ne sera jamais plus grande que 2DMAX (notre intervalle)
    Ainsi la premiere distance evaluée sera implicitement 
    considérée comme la plus petite ce qui simplifie le code*/
    
    while(a_tester)
    {
        distance = utilitaire_calcul_norme(ouvriere_x, a_tester->x, 
										   ouvriere_y, a_tester->y);
        
        if(a_tester->indice_f != indice_f && 
           a_tester->total_food && distance < plus_petite_distance)
        {
            plus_petite_distance = distance;
            *fourmiliere_a_attaquer_x = a_tester->x;
            *fourmiliere_a_attaquer_y = a_tester->y;
            fourmiliere_attaquable_trouvee = true;
        }
        
        a_tester = a_tester->suivant;
    }

    if(fourmiliere_attaquable_trouvee)
        return 1;
    return 0;
}

bool fourmiliere_manger(double pos_ouvriere_x, double pos_ouvriere_y, 
						unsigned indice_f)
{
	FOURMILIERE * a_tester = tete_fourmiliere;
	
	while(a_tester)
    {
        if(a_tester->indice_f != indice_f &&(utilitaire_calcul_norme(pos_ouvriere_x, 
           a_tester->x, pos_ouvriere_y,a_tester->y) < RAYON_FOURMI)) 
        {
			a_tester->total_food--;
			return 1;
		}
		
		a_tester = a_tester->suivant;
	}
	
	return 0;
}

void fourmiliere_ecriture(FILE * fsortie)
{
    FOURMILIERE * courant = tete_fourmiliere;
    
    fprintf(fsortie, "# Nb Fourmilieres\n");
    fprintf(fsortie, "%d\n\n", nb_fourmiliere);
    
    while(courant)
    {
        fprintf(fsortie, "  # Fourmiliere[%d]\n", courant->indice_f);
        fprintf(fsortie, "    %lf %lf %d %d %f %lf\n\n",
                courant->x, courant->y,courant->nbO, courant->nbG,
                courant->total_food, courant->rayon);
        
        if(courant->nbO)
        {
            fourmi_ouvriere_ecriture(fsortie, courant->indice_f);
            fprintf(fsortie, "    FIN_LISTE \n\n");
        }
        
        if(courant->nbG)
        {
            fourmi_garde_ecriture(fsortie, courant->indice_f);
            fprintf(fsortie, "    FIN_LISTE \n\n");
        }
        
        courant = courant->suivant;
    }
    
    if(nb_fourmiliere)
        fprintf(fsortie, "FIN_LISTE \n\n");
}

void fourmiliere_recup_info(int tab_info[NB_COLUMNS][NB_LINES]) 
{
    int i, j;
    
    //Total du tableau
    int t_nbF = 0;
    int t_nbO = 0;
    int t_nbG = 0;
    int t_food = 0;
    
    FOURMILIERE * courant = tete_fourmiliere;
    
    for(j=0 ; j<NB_LINES-1 ; j++)
		for(i=0 ; i<NB_COLUMNS ; i++)
			tab_info[i][j]=0;
  
    while(courant)
    {
        t_nbF += (tab_info[0][courant->indice_f] = courant->nbF);
        t_nbO += (tab_info[1][courant->indice_f] = courant->nbO);
        t_nbG += (tab_info[2][courant->indice_f] = courant->nbG);
        t_food += (tab_info[3][courant->indice_f] = courant->total_food);
            
        courant = courant->suivant;
    }
    
    tab_info[0][NB_LINES -1] = t_nbF;
    tab_info[1][NB_LINES -1] = t_nbO;
    tab_info[2][NB_LINES -1] = t_nbG;
    tab_info[3][NB_LINES -1] = t_food;
}
			
void fourmiliere_ecriture_gnuplot(FILE * fichier, int nb_update) 
{
	int i;
    FOURMILIERE * courant = NULL;
    bool fourmiliere_trouvee;
    
    fprintf(fichier, "%d ", nb_update);
    
    for(i=0 ; i<nb_fourmiliere ; i++)
    {
		courant = tete_fourmiliere;
		fourmiliere_trouvee = false;
		while(courant)
		{
			if(courant->indice_f == i)
			{
				fprintf(fichier, "%d ", courant->nbF);
				fourmiliere_trouvee = true;
				break;
			}
			courant = courant->suivant;
		}
		
		if(!fourmiliere_trouvee)
			fprintf(fichier, "0");
	}
    
    fprintf(fichier, "\n");
}
