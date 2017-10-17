/*!
 \file modele.c
 \brief Module qui ouvre le fichier et distribue les informations aux
 modules associés
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "constantes.h"
#include "error.h"
#include "fourmi.h"
#include "fourmiliere.h"
#include "nourriture.h"
#include "modele.h"

enum Etat_lecture {ETAT_NB_FOURMILIERE, ETAT_FOURMILIERE, ETAT_OUVRIERE,
    ETAT_GARDE, ETAT_FIN_FOURMILIERE, ETAT_NB_NOURRITURE,
    ETAT_NOURRITURE, ETAT_FIN_NOURRITURE};

static int etat = ETAT_NB_FOURMILIERE;
static int nb_update = 0; //Nombre de mises à jour

bool modele_lecture(char * nom_fichier, PRGMMODE nom_mode)
{
    // f : indice de fourmiliere, o : indice d'ouvrière,
    // g : indice de garde, n : indice de nourriture
    // des indices sont initialisés a -1 pour ne pas entrer dans les
    // conditions d'erreur lorsque le nombre d'entités est égal à 1
    int nbO, nbG, nb_fourmiliere, nb_nourriture,  f = -1, o, g, n = -1;
    double centre_x, centre_y, rayon_fourmiliere;
    bool fin_ligne = false;
    char tab[MAX_LINE];
    FILE * fichier = NULL;
    
    if((fichier = fopen(nom_fichier,"r")))
    {
        while(fgets(tab, MAX_LINE, fichier))
        {
            char premier_char[2];
            if((strstr(tab,"#")) || (sscanf(tab,"%1s", premier_char) != 1))
                continue;
            
            if(!modele_automate_lecture(tab, &nbO, &nbG, &centre_x,
                                        &centre_y, &rayon_fourmiliere, &f, &o, &g,
                                        &n, &nb_fourmiliere, &nb_nourriture, 
                                        &fin_ligne))
                return 0;
            
            fourmiliere_set_nb(nb_fourmiliere);
            nourriture_set_nb(nb_nourriture);
        }
        
        if(etat != ETAT_FIN_NOURRITURE)
        {
            error_fichier_incomplet();
            return 0;
        }
        
        else if(nom_mode == ERROR)
            error_success();
    }
    
    else
    {
        error_fichier_inexistant();
        return 0;
    }
    
    fclose(fichier);
    return 1;
}

bool modele_automate_lecture(char * tab, int * pnbO, int * pnbG,
                             double * pcentre_x, double * pcentre_y,
                             double * prayon_fourmiliere, int * pf, int * po,
                             int * pg, int * pn, int * pnb_fourmiliere,
                             int * pnb_nourriture, bool * pfin_ligne)
{
    switch(etat)
    {
        case ETAT_NB_FOURMILIERE :
            if(!modele_etat_nb_fourmiliere(tab, pnb_fourmiliere))
                return 0;
            break;
            
        case ETAT_FOURMILIERE :
            if(!modele_etat_fourmiliere(tab, pnbO, pnbG, pcentre_x, pcentre_y,
                                        prayon_fourmiliere, pf, po, pg, 
                                        *pnb_fourmiliere))
                return 0;
            break;
            
        case ETAT_OUVRIERE :
            if(!modele_etat_ouvriere(tab, *pnbO, *pnbG, *pf, po, *pnb_fourmiliere))
                return 0;
            break;
            
        case ETAT_GARDE :
            if(!modele_etat_garde(tab, *pnbG, *pf, pg, pfin_ligne, *pcentre_x,
                                  *pcentre_y, *prayon_fourmiliere, *pnb_fourmiliere))
                return 0;
            break;
            
        case ETAT_FIN_FOURMILIERE :
            if(!modele_etat_fin_fourmiliere(tab, *pf, *pnb_fourmiliere))
                return 0;
            break;
            
        case ETAT_NB_NOURRITURE:
            modele_etat_nb_nourriture(tab, pnb_nourriture);
            break;
            
        case ETAT_NOURRITURE:
            if(!modele_etat_nourriture(tab, *pnb_nourriture, pn, pfin_ligne))
                return 0;
            break;
    }
    return 1;
}

bool modele_etat_nb_fourmiliere(char * tab, int * pnb_fourmiliere)
{
    sscanf(tab, "%d", pnb_fourmiliere);
    
    if(fourmiliere_get_nb() > MAX_FOURMILIERE)
    {
        error_nb_fourmiliere(*pnb_fourmiliere);
        return 0;
    }
    
    if(!*pnb_fourmiliere)
        etat = ETAT_NB_NOURRITURE;
    else etat = ETAT_FOURMILIERE;
    
    return 1;
}

bool modele_etat_fourmiliere(char * tab, int * pnbO, int * pnbG,
                             double * pcentre_x, double * pcentre_y,
                             double * prayon_fourmiliere, int * pf,
                             int * po, int * pg, int nb_fourmiliere)
{
    (*pf)++;
    
    if(!fourmiliere_lecture(tab, *pf, pnbO, pnbG, pcentre_x, pcentre_y,
                            prayon_fourmiliere))
        return 0;
    
    if(*pnbO)
        etat = ETAT_OUVRIERE;
    if(!*pnbO && *pnbG)
        etat = ETAT_GARDE;
    if(!*pnbO && !*pnbG && *pf!= nb_fourmiliere -1)
        etat = ETAT_FOURMILIERE;
    if(!*pnbO && !*pnbG && *pf == nb_fourmiliere -1)
        etat = ETAT_FIN_FOURMILIERE;
    
    *po = -1; *pg = -1; // initialisation de l'indice des ouvrières et des gardes
    return 1;
}

bool modele_etat_ouvriere(char * tab, int nbO, int nbG, int f, int * po,
                          int nb_fourmiliere)
{
    if(!modele_recherche_fin_liste(tab))
    {
        (*po)++;
        
        if(*po == nbO)
        {
            error_lecture_elements_fourmiliere(f, ERR_OUVRIERE, ERR_TROP);
            return 0;
        }
        
        if(!fourmi_ouvriere_lecture(tab, *po, f))
            return 0;
        
        etat = ETAT_OUVRIERE;
    }
    
    else if(*po < nbO -1)
    {
        error_lecture_elements_fourmiliere(f, ERR_OUVRIERE, ERR_PAS_ASSEZ);
        return 0;
    }
    
    else if(nbG)
        etat = ETAT_GARDE;
    
    else if(f < nb_fourmiliere-1)
        etat = ETAT_FOURMILIERE;
    else etat = ETAT_FIN_FOURMILIERE;
    
    return 1;
}

bool modele_etat_garde(char * tab, int nbG, int f, int * pg,
                       bool * pfin_ligne, double centre_x,
                       double centre_y, double rayon_fourmiliere,
                       int nb_fourmiliere)
{
    if(!modele_recherche_fin_liste(tab))
    {
        if(*pg == nbG -1 && !*pfin_ligne)
        {
            error_lecture_elements_fourmiliere(f, ERR_GARDE, ERR_TROP);
            return 0;
        }
        
        if(!fourmi_garde_lecture(tab, f, pg, nbG, centre_x, centre_y, 
								 rayon_fourmiliere, pfin_ligne))
            return 0;
        
        etat = ETAT_GARDE;
    }
    
    else if(*pg < nbG -1)
    {
        error_lecture_elements_fourmiliere(f, ERR_GARDE, ERR_PAS_ASSEZ);
        return 0;
    }
    
    else if(f < nb_fourmiliere -1)
        etat = ETAT_FOURMILIERE;
    else etat = ETAT_FIN_FOURMILIERE;
    
    return 1;
}

bool modele_etat_fin_fourmiliere(char * tab, int f, int nb_fourmiliere)
{
    if(modele_recherche_fin_liste(tab))
    {
        if(f < nb_fourmiliere -1)
        {
            error_lecture_elements_fourmiliere(f, ERR_FOURMILIERE, ERR_PAS_ASSEZ);
            return 0;
        }
        else etat = ETAT_NB_NOURRITURE;
    }
    
    else
    {
        error_lecture_elements_fourmiliere(f, ERR_FOURMILIERE, ERR_TROP);
        return 0;
    }
    
    return 1;
}

void modele_etat_nb_nourriture(char * tab, int * pnb_nourriture)
{
    sscanf(tab, "%d", pnb_nourriture);
    
    if(!*pnb_nourriture)
        etat = ETAT_FIN_NOURRITURE;
    else etat = ETAT_NOURRITURE;
    
}

bool modele_etat_nourriture(char * tab, int nb_nourriture, int * pn,
                            bool * pfin_ligne)
{
    if(!modele_recherche_fin_liste(tab))
    {
        if(*pn == nb_nourriture -1 && !*pfin_ligne)
        {
            error_lecture_elements_nourriture(ERR_TROP);
            return 0;
        }
        
        if(!nourriture_lecture(tab, pn, pfin_ligne))
            return 0;
        
        etat = ETAT_NOURRITURE;
    }
    
    else if(*pn < nb_nourriture)
    {
        error_lecture_elements_nourriture(ERR_PAS_ASSEZ);
        return 0;
    }
    
    else etat = ETAT_FIN_NOURRITURE;
    
    return 1;
}

bool modele_recherche_fin_liste(char * chaine) 
{
    if(strstr(chaine, "FIN_LISTE"))
        return 1;
    return 0;
}

bool modele_verification_rendu2(void)
{
    if(!fourmiliere_verification_rendu2())
        return 0;
    return 1;
}

void modele_update(bool automatic, bool record_active, FILE * fichier)
{
    if(fichier && record_active)
    {
        nb_update++;
        fourmiliere_ecriture_gnuplot(fichier, nb_update);
    }
    
    fourmiliere_update();
    
    if(automatic)
        nourriture_update_automatic_creation();
}

void modele_nourriture_manual_creation(double x, double y)
{
    nourriture_update_manual_creation(x, y);
}

void modele_dessine(void)
{
    fourmiliere_dessine();
    fourmi_ouvriere_dessine();
    fourmi_garde_dessine();
    nourriture_dessine();
}

void modele_nettoyer(void)
{
    fourmiliere_vider(); 
    fourmi_vider();
    nourriture_vider();
    fourmiliere_set_nb(0);
    nourriture_set_nb(0);
    etat = ETAT_NB_FOURMILIERE;
}

void modele_ecriture(char* nom_fichier)
{
    FILE * fsortie = NULL;
    fsortie = fopen((char*) nom_fichier, "w");
    
    fprintf(fsortie, "# Fichier sauvegarde Bug's Life \n\n");
    if(fsortie)
    {
        fourmiliere_ecriture(fsortie);
        nourriture_ecriture(fsortie);
    }
    
    fclose(fsortie);
}

void modele_recup_info(int tab_info[NB_COLUMNS][NB_LINES]) 
{
    fourmiliere_recup_info(tab_info);
}
