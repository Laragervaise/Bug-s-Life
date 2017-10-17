/*!
 \file fourmi.c
 \brief Module qui gère les informations relatives aux fourmis
 ouvrières et gardes
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
#include "fourmi.h"

/*---------------------------------------------------------------------
 Structure de données d'une fourmi ouvrière
	indice_o : indice de l'ouvrière
	indice_f : indice de la fourmilière à laquelle appartient l'ouvrière
	age : age de l'ouvrière
	posx , posy : coordonnées de l'ouvrière
	butx, buty : coordonnées du but de l'ouvrière
	bool_nourriture : booléen qui indique si l'ouvrière possède une
                      nourriture ou non
	bool_contact : booléen qui indique si l'ouvrière est en collision
                   avec une autre fourmi ou pas
	suivant : pointeur de type structure Ouvriere qui pointe sur
              l'ouvrière suivante de la liste chainée
 ----------------------------------------------------------------------*/
struct Ouvriere
{
    unsigned indice_o;
    unsigned indice_f;
    unsigned age;
    double posx;
    double posy;
    double butx;
    double buty;
    int bool_nourriture;
    int bool_contact;
    OUVRIERE * suivant;
};

/*---------------------------------------------------------------------
 Structure de données d'une fourmi garde
	indice_g : indice de la garde
	indice_f : indice de la fourmilière à laquelle appartient la garde
	age : age de la garde
	x , y : coordonnées de la garde
	butx, buty : coordonnées du but de la garde
	bool_contact : booléen qui indique si la garde est en collision
                   avec une autre ouvrière ou pas
	suivant : pointeur de type structure Garde qui pointe sur la
              garde suivante de la liste chainée
 ----------------------------------------------------------------------*/
struct Garde
{
    unsigned indice_g;
    unsigned indice_f;
    unsigned age;
    double x;
    double y;
    double butx;
    double buty;
    int bool_contact;
    GARDE * suivant;
};

static OUVRIERE * tete_ouvriere = NULL;
static GARDE * tete_garde = NULL;

static int last_o, last_g; // dernier indice attribué

static bool paix = true; 
/* paix = true : etat lorsque l'ouvriere peut aller chercher des nourritures
 * paix = false : etat lorsque l'ouvriere n'a pas en vue de nourritures qui 
 remplissent les conditions indiquées dans la donnée
 * elle attaque dans la fourmiliere la plus proche qui a de la nourriture
*/ 

bool fourmi_ouvriere_lecture(char tab[MAX_LINE], int o, int f)
{
    int nb_caract, bool_nourriture;
    unsigned age;
    double posx, posy, butx, buty;
    
    nb_caract = sscanf(tab, " %u %lf %lf %lf %lf %d", &age, &posx, &posy, &butx, 
					   &buty, &bool_nourriture);
    
    fourmi_ouvriere_ajouter();
    
    tete_ouvriere->indice_o = o;
    tete_ouvriere->indice_f = f;
    tete_ouvriere->age = age;
    tete_ouvriere->posx = posx;
    tete_ouvriere->posy = posy;
    tete_ouvriere->butx = butx;
    tete_ouvriere->buty = buty;
    tete_ouvriere->bool_nourriture = bool_nourriture;
    tete_ouvriere->bool_contact = false;
    
    last_o = o;
    
    return fourmi_ouvriere_erreur(nb_caract, f, o, age, posx, posy);
}

void fourmi_ouvriere_ajouter()
{
    OUVRIERE * nouveau = NULL;
    if(!(nouveau = (OUVRIERE *) malloc (sizeof(OUVRIERE))))
        printf("Probleme de mémoire dans la fonction %s\n", __func__);
    
    nouveau->suivant = tete_ouvriere;
    tete_ouvriere = nouveau;
}

bool fourmi_ouvriere_erreur(int nb_caract, int f, int o, unsigned age,
                            double posx, double posy)
{
    if(nb_caract < 6)
    {
        error_lecture_elements_fourmiliere(f, ERR_OUVRIERE, ERR_PAS_ASSEZ);
        return 0;
    }
    
    if(age >= BUG_LIFE)
    {
        error_age_fourmi(f, o, age);
        return 0;
    }
    
    if(posx <= -DMAX || posy <= -DMAX || posx >= DMAX || posy >= DMAX)
    {
        error_pos_domaine(ERR_OUVRIERE, f, posx, posy);
        return 0;
    }
    return 1;
}

bool fourmi_garde_lecture(char tab[MAX_LINE], int f, int * pg, int nbG,
                          double centre_x, double centre_y,
                          double rayon_fourmiliere, bool * pfin_ligne)
{
    char * deb = tab, * fin = NULL;
    int g;
    unsigned age;
    double x, y;
    *pfin_ligne = false;
    
    if(*pg == -1)
        (*pg)++;
    
    for(g=*pg; g < nbG ; g++)
    {
        if(sscanf(deb, "%u %lf %lf", &age, &x, &y) < 3)
        {
            *pg = g;
            *pfin_ligne = true;
            return 1;
        }
        
        fourmi_garde_ajouter();
        
        tete_garde->indice_g = g;
        tete_garde->indice_f = f;
        tete_garde->age = age;
        tete_garde->x = x;
        tete_garde->y = y;
        tete_garde->bool_contact = false;
        tete_garde->butx = centre_x;
        tete_garde->buty = centre_y;
        
        if(!fourmi_garde_erreur(f, g, age, x, y, centre_x, centre_y, 
								rayon_fourmiliere))
            return 0;
        
        int m;
        for(m = 0; m < 3; m++)
        {
            strtod(deb, &fin);
            deb = fin;
        }
    }
    
    last_g = g;
    *pg = g;
    return 1;
}

void fourmi_garde_ajouter()
{
    GARDE * nouveau = NULL;
    if(!(nouveau = (GARDE *) malloc (sizeof(GARDE))))
        printf("Probleme de mémoire dans la fonction %s\n", __func__);
    
    nouveau->suivant = tete_garde;
    tete_garde = nouveau;
}

bool fourmi_garde_erreur(int f, int g, unsigned age, double x, double y,
                         double centre_x, double centre_y, double rayon_fourmiliere)
{
    if(age >= BUG_LIFE)
    {
        error_age_fourmi(f, g, age);
        return 0;
    }
    
    if(x <= -DMAX || y <= -DMAX || x >= DMAX || y >= DMAX)
    {
        error_pos_domaine(ERR_GARDE, g, x, y);
        return 0;
    }
    
    if(!fourmi_dans_fourmiliere(x, centre_x, y, centre_y, rayon_fourmiliere))
    {
        error_pos_garde(f, g);
        return 0;
    }
    
    return 1;
}

bool fourmi_verification_rendu2(void)
{
    OUVRIERE * o1 = tete_ouvriere;
    OUVRIERE * o2 = NULL;
    GARDE * g2 = NULL;
    
    while(o1)
    {
        o2 = o1->suivant;
        while(o2)
        {
            if(o1->indice_f != o2->indice_f &&
               utilitaire_contact_sans_tolerance(o1->posx, o2->posx,
                                                 o1->posy, o2->posy, RAYON_FOURMI, 
                                                 RAYON_FOURMI))
            {
                error_superposition_fourmi(ERR_OUVRIERE, o1->indice_f,
                                           o1->indice_o, ERR_OUVRIERE, 
                                           o2->indice_f, o2->indice_o);
                return 0;
            }
            o2 = o2->suivant;
        }
        g2 = tete_garde;
        while(g2)
        {
            if(o1->indice_f != g2->indice_f &&
               utilitaire_contact_sans_tolerance(o1->posx, g2->x,
                                                 o1->posy, g2->y, RAYON_FOURMI, 
                                                 RAYON_FOURMI))
            {
                error_superposition_fourmi(ERR_OUVRIERE, o1->indice_f,
                                           o1->indice_o, ERR_GARDE, 
                                           g2->indice_f, g2->indice_g);
                return 0;
            }
            g2 = g2->suivant;
        }
        o1 = o1->suivant;
    }
    return 1;
}

void fourmi_ouvriere_dessine(void)
{
    OUVRIERE * courant = tete_ouvriere;
    
    while(courant)
    {
        graphic_draw_circle(courant->posx, courant->posy, RAYON_FOURMI, 
							GRAPHIC_EMPTY, courant->indice_f);
        if(courant->bool_nourriture)
            graphic_draw_circle(courant->posx, courant->posy, RAYON_FOOD, 
								GRAPHIC_EMPTY, NOIR);
        courant = courant->suivant;
    }
}

void fourmi_garde_dessine(void)
{
    GARDE * courant = tete_garde;
    
    while(courant)
    {
        graphic_draw_circle(courant->x, courant->y, RAYON_FOURMI, GRAPHIC_FILLED,
                            courant->indice_f);
        graphic_draw_circle(courant->x, courant->y, RAYON_FOURMI, GRAPHIC_EMPTY, 
							NOIR);
        courant = courant->suivant;
    }
}

void fourmi_vider(void)
{
    fourmi_ouvriere_vider();
    fourmi_garde_vider();

    paix = true;
}

void fourmi_ouvriere_vider(void)
{
    OUVRIERE * a_retirer = NULL;
    
    while(tete_ouvriere)
    {
        a_retirer = tete_ouvriere;
        tete_ouvriere = tete_ouvriere->suivant;
        free(a_retirer);
        a_retirer = NULL;
    }
    
    tete_ouvriere = NULL;
}

void fourmi_garde_vider(void)
{
    GARDE * a_retirer = NULL;
    
    while(tete_garde)
    {
        a_retirer = tete_garde;
        tete_garde = tete_garde->suivant;
        free(a_retirer);
        a_retirer = NULL;
    }
    
    tete_garde = NULL;
}

int fourmi_ouvriere_update(unsigned indice_f, int * pnourriture_rapportee,
                           int centre_x, int centre_y)
{
    OUVRIERE * a_tester = tete_ouvriere, * precedent = NULL;
    int compteur_o = 0;
    
    while(a_tester)
    {
        if(a_tester->indice_f == indice_f)
        {
            a_tester->age++;
            if(a_tester->age >= BUG_LIFE) //Si trop vieille, elle meurt de vieillesse
            {
                fourmi_ouvriere_dead(a_tester, precedent, &compteur_o);
                a_tester = precedent;
                break;
            }           
            
            fourmi_ouvriere_update_pos(a_tester);
            
            if(!(a_tester->bool_nourriture)) //Chemin de l'aller
            {
				fourmi_ouvriere_bon_choix_aller(a_tester, indice_f, centre_x, 
												centre_y);
				
				if(paix)
				{
					if(nourriture_manger(a_tester->posx, a_tester->posy))
					{
						a_tester->bool_nourriture= true; //Chemin du retour
						fourmi_ouvriere_bon_choix_retour(a_tester, centre_x, 
														 centre_y);
					}
				}
				else //etat de guerre
					a_tester->bool_nourriture = fourmiliere_manger(a_tester->posx, 
																   a_tester->posy, 
																   indice_f); 
            }
            else // si nourriture en possession, on la rapporte pour le retour
                fourmi_ouvriere_bon_choix_retour(a_tester, centre_x, centre_y);
            
            fourmi_ouvriere_collision(a_tester);
            
            if(a_tester->bool_nourriture &&
               (utilitaire_calcul_norme(a_tester->posx, centre_x,
                                        a_tester->posy, centre_y) < RAYON_FOURMI))
            {
                a_tester->bool_nourriture = false;
                (*pnourriture_rapportee)++;
            }
            // si l'ouvriere est revenue au centre de la fourmiliere
            // elle dépose la nourriture
            
            if(a_tester->bool_contact)
            {
                fourmi_ouvriere_dead(a_tester, precedent, &compteur_o);
                a_tester = precedent;
                break;
            }
        }
        precedent = a_tester;
        a_tester = a_tester->suivant;
    }
    
    return compteur_o;
}

void fourmi_ouvriere_dead(OUVRIERE * a_tester, OUVRIERE * precedent,
                          int * pcompteur_o)
{
    if(a_tester->bool_nourriture)
        nourriture_update_ouvriere_dead(a_tester->posx, a_tester->posy);
				
    if(a_tester == tete_ouvriere)
        tete_ouvriere = tete_ouvriere->suivant;
    else
        precedent->suivant = a_tester->suivant;
    
    if(a_tester)
		free(a_tester);
    a_tester = NULL;
    (*pcompteur_o)++;
}

void fourmi_ouvriere_collision(OUVRIERE * ouvriere)
{
    OUVRIERE * ocomparee = ouvriere->suivant;
    GARDE * gcomparee = tete_garde;
    
    while(ocomparee)
    {
        if(ouvriere->indice_f != ocomparee->indice_f &&
           utilitaire_contact(ouvriere->posx, ocomparee->posx,
                              ouvriere->posy, ocomparee->posy, RAYON_FOURMI, 
                              RAYON_FOURMI))
        {
            ouvriere->bool_contact = true;
            ocomparee->bool_contact = true;
        }
        
        ocomparee = ocomparee->suivant;
    }
    
    while(gcomparee)
    {
        if(ouvriere->indice_f != gcomparee->indice_f &&
           utilitaire_contact(ouvriere->posx, gcomparee->x,
                              ouvriere->posy, gcomparee->y, RAYON_FOURMI, 
                              RAYON_FOURMI))
        {
            ouvriere->bool_contact = true;
            gcomparee->bool_contact = true;
        }
        
        gcomparee = gcomparee->suivant;
    }
}

int fourmi_garde_update(unsigned indice_f, double centre_x,
                        double centre_y, double rayon_fourmiliere)
{
    GARDE * a_tester = tete_garde, * precedent = NULL;
    int compteur_g = 0;
    while(a_tester)
    {
        if(a_tester->indice_f == indice_f)
        {
            a_tester->age++;
            if(a_tester->age >= BUG_LIFE)//Si trop vieille, elle meurt de vieillesse
            {
                fourmi_garde_dead(a_tester, precedent, &compteur_g);
                a_tester = precedent;
                break;
            }
            
            fourmi_garde_update_but(indice_f, centre_x, centre_y,
                                    rayon_fourmiliere, a_tester);
            fourmi_garde_update_pos(a_tester);
            
            fourmi_garde_collision(a_tester);
            
            if(a_tester->bool_contact)
            {
                fourmi_garde_dead(a_tester, precedent, &compteur_g);
                a_tester = precedent;
                break;
            }
        }
        
        precedent = a_tester;
        a_tester = a_tester->suivant;
    }
    
    return compteur_g;
}

void fourmi_garde_dead(GARDE * a_tester, GARDE * precedent, int * pcompteur_g)
{
    if(a_tester == tete_garde)
        tete_garde = tete_garde->suivant;
    else
        precedent->suivant = a_tester->suivant;
    
    free(a_tester);
    a_tester = NULL;
    (*pcompteur_g)++;
}

void fourmi_garde_collision(GARDE * garde)
{
    OUVRIERE * ocomparee = tete_ouvriere;
    
    while(ocomparee)
    {
        if(garde->indice_f != ocomparee->indice_f &&
           utilitaire_contact(garde->x, ocomparee->posx,
                              garde->y, ocomparee->posy, RAYON_FOURMI, 
                              RAYON_FOURMI))
        {
            garde->bool_contact = true;
            ocomparee->bool_contact = true;
        }
        
        ocomparee = ocomparee->suivant;
    }
}

void fourmi_ouvriere_naissance(double centre_x, double centre_y, unsigned indice_f)
{
    last_o++;
    tete_ouvriere->indice_o = last_o;
    tete_ouvriere->indice_f = indice_f;
    tete_ouvriere->age = 0;
    tete_ouvriere->posx = centre_x;
    tete_ouvriere->posy = centre_y;
    tete_ouvriere->bool_nourriture = false;
    tete_ouvriere->bool_contact = false;
}

void fourmi_garde_naissance(double x, double y, unsigned indice_f)
{
    last_g++;
    tete_garde->indice_g = last_g;
    tete_garde->indice_f = indice_f;
    tete_garde->age = 0;
    tete_garde->x = x;
    tete_garde->y = y;
    tete_garde->bool_contact = false;
}

bool fourmi_nourriture_superposition(double nourriture_x, double nourriture_y)
{
    OUVRIERE * a_tester = tete_ouvriere;
    
    while(a_tester)
    {
        if(utilitaire_calcul_norme(nourriture_x, a_tester->posx, nourriture_y,
                                   a_tester->posy) - 
                                   (RAYON_FOURMI + RAYON_FOOD) <= EPSIL_ZERO)
            return 1;
        
        a_tester = a_tester->suivant;
    }
   
    return 0;
}

void fourmi_ouvriere_bon_choix_aller(OUVRIERE * ouvriere, unsigned indice_f,
                                     double centre_x, double centre_y)
{
    double nourriture_x, nourriture_y, fourmiliere_a_attaquer_x;
    double fourmiliere_a_attaquer_y;
    
    //Si fourmi en état de paix et se trouve dans une fourmilière étrangère, 
    //elle ne veut pas l'attaquer 
    if(paix && fourmiliere_fourmi_inside(ouvriere->posx, ouvriere->posy, indice_f))
    {
        ouvriere->butx = centre_x;
        ouvriere->buty = centre_y;
    }
    else if(nourriture_la_plus_proche(ouvriere->posx, ouvriere->posy,
                                      &nourriture_x, &nourriture_y,
                                      ouvriere->indice_f, ouvriere->indice_o))
    {
        paix = true;
        ouvriere->butx = nourriture_x;
        ouvriere->buty = nourriture_y;
    }
    else if(fourmiliere_la_plus_proche(ouvriere->posx, ouvriere->posy, indice_f, 
									   &fourmiliere_a_attaquer_x, 
									   &fourmiliere_a_attaquer_y))
    {
        paix = false;
        ouvriere->butx = fourmiliere_a_attaquer_x;
        ouvriere->buty = fourmiliere_a_attaquer_y;
    }
    else
    {
		ouvriere->butx = centre_x;
        ouvriere->buty = centre_y;
	}
}

bool fourmi_nourriture_deja_prise(unsigned indice_f, unsigned indice_o,
                                  double nourriture_x, double nourriture_y)
{
    OUVRIERE * a_tester = tete_ouvriere;
    
    while(a_tester)
    {
        if(a_tester->indice_f == indice_f && a_tester->indice_o != indice_o
           && a_tester->butx == nourriture_x && a_tester->buty == nourriture_y)
            return 1;
        
        a_tester = a_tester->suivant;
    }
    return 0;
}

bool fourmi_etrangere_plus_proche(unsigned indice_f, double nourriture_x,
                                  double nourriture_y, double distance)
{
    OUVRIERE * a_tester = tete_ouvriere;
    
    while(a_tester)
    {
        if(a_tester->indice_f != indice_f &&
           utilitaire_calcul_norme(a_tester->posx, nourriture_x, 
                                   a_tester->posy, nourriture_y) <= distance)
            return 1;
        
        a_tester = a_tester->suivant;
    }
    
    return 0;
}

void fourmi_ouvriere_bon_choix_retour(OUVRIERE * ouvriere, double centre_x, 
                                      double centre_y)
{
    ouvriere->butx = centre_x;
    ouvriere->buty = centre_y;
}

void fourmi_ouvriere_update_pos(OUVRIERE * ouvriere)
{
    VECTOR v;
    v.x = ouvriere->butx - ouvriere->posx;
    v.y = ouvriere->buty - ouvriere->posy;
    v.norme = utilitaire_calcul_norme(ouvriere->butx, ouvriere->posx, 
                                      ouvriere->buty, ouvriere->posy);
    
    if(v.norme > RAYON_FOURMI)
    {
        VECTOR v_normalise = utilitaire_normalise_vector(v);
        ouvriere->posx += BUG_SPEED*DELTA_T*v_normalise.x;
        ouvriere->posy += BUG_SPEED*DELTA_T*v_normalise.y;
    }
    else 
    {
        ouvriere->posx += v.x;
        ouvriere->posy += v.y;
    }
}

void fourmi_garde_update_but(unsigned indice_f, double centre_x, 
                             double centre_y, double rayon_fourmiliere, 
                             GARDE * garde)
{
    OUVRIERE * ouvriere = tete_ouvriere;
    
    while(ouvriere)
    {	
        // si ouvrière etrangère et que le centre de l'ouvrière est dans 
        // la fourmilière
        if(ouvriere->indice_f != indice_f && 
           utilitaire_calcul_norme(ouvriere->posx, centre_x, ouvriere->posy, 
								   centre_y)
           <= rayon_fourmiliere) 
        {										
            garde->butx = ouvriere->posx;
            garde->buty = ouvriere->posy;
            
            if(!fourmi_dans_fourmiliere(garde->x, centre_x, garde->y, centre_y, 
										rayon_fourmiliere))
            {
                garde->butx = centre_x;
                garde->buty = centre_y;
            }
            
            return;
        }
        ouvriere = ouvriere->suivant;
    }
    
    garde->butx = centre_x;
    garde->buty = centre_y;
}

void fourmi_garde_update_pos(GARDE * garde)
{
    VECTOR v;
    v.x = garde->butx - garde->x;
    v.y = garde->buty - garde->y;
    v.norme = utilitaire_calcul_norme(garde->butx, garde->x, 
                                      garde->buty, garde->y);
    
    if(v.norme > RAYON_FOURMI)
    {
        VECTOR v_normalise = utilitaire_normalise_vector(v);
        garde->x += BUG_SPEED*DELTA_T*v_normalise.x;
        garde->y += BUG_SPEED*DELTA_T*v_normalise.y;
    }
    else
    {
        garde->x += v.x;
        garde->y += v.y;
    }	
}

bool fourmi_dans_fourmiliere(double fourmi_x, double centre_x, 
                             double fourmi_y, double centre_y,
                             double rayon_fourmiliere)
{
    if(utilitaire_calcul_norme(fourmi_x, centre_x, fourmi_y, centre_y)
       <= (rayon_fourmiliere - EPSIL_ZERO - RAYON_FOURMI))
        return 1;
    return 0;
}

bool fourmi_contact_fourmiliere(double fourmi_x, double centre_x, 
                                double fourmi_y, double centre_y, 
                                double rayon_fourmiliere)
{
    if(utilitaire_calcul_norme(fourmi_x, centre_x, fourmi_y, centre_y)
       - (rayon_fourmiliere + RAYON_FOURMI) <= EPSIL_ZERO)
        return 1;
    return 0;
}

void fourmi_ouvriere_ecriture(FILE * fsortie, int indice_f)
{
    OUVRIERE * courant = tete_ouvriere;
    
    fprintf(fsortie, "    # Ouvrieres\n");
    while(courant)
    {
        if(indice_f == courant->indice_f)
        {
            fprintf(fsortie, "         %u %lf %lf %lf %lf %d\n",
                    courant->age, courant->posx, courant->posy,
                    courant->butx, courant->buty, courant->bool_nourriture);
        }
        courant = courant->suivant;
    }
    
}

void fourmi_garde_ecriture(FILE * fsortie, int indice_f)
{
    unsigned compteur = 0; //Pour afficher 2 gardes par ligne
    GARDE * courant = tete_garde;
    
    fprintf(fsortie, "    # Gardes\n");
    fprintf(fsortie, "         ");
    while(courant)
    {
        if(indice_f == courant->indice_f)
        {
            if(compteur == 2)
            {
                fprintf(fsortie, "\n");
                fprintf(fsortie, "        ");
                compteur = 0;
            }
            fprintf(fsortie, "%u %lf %lf ", courant->age, courant->x,
                    courant->y);
            compteur ++;
        }
        courant = courant->suivant;
    }
    
    fprintf(fsortie, "\n");
}
