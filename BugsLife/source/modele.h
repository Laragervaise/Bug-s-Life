/*!
 \file modele.h
 \brief Module qui ouvre le fichier et distribue les informations aux
  modules associés
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#ifndef MODELE_H
#define MODELE_H

#include <stdio.h>
#include <stdbool.h>

#define NB_LINES   11
#define NB_COLUMNS 4

typedef enum Modes{ERROR, VERIFICATION, GRAPHIC, FINAL, RIEN, INCORRECT} PRGMMODE;

//---------------------------------------------------------------------
// ouvre le fichier et élimine les lignes inutiles
bool modele_lecture(char * nom_fichier, PRGMMODE nom_mode);

//---------------------------------------------------------------------
// détecte les "FIN_LISTE" dans le fichier
bool modele_recherche_fin_liste(char * chaine);

bool modele_automate_lecture(char * tab, int * pnbO, int * pnbG,
                             double * pcentre_x, double * pcentre_y,
                             double * prayon_fourmiliere, int * pf, int * po,
                             int * pg, int * pn, int * pnb_fourmiliere,
                             int * pnb_nourriture, bool * pfin_ligne);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture de nb_fourmiliere
bool modele_etat_nb_fourmiliere(char * tab, int * pnb_fourmiliere);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture des fourmilieres
bool modele_etat_fourmiliere(char * tab, int * pnbO, int * pnbG,
                             double * pcentre_x, double * pcentre_y,
                             double * prayon_fourmiliere, int * pf,
                             int * po, int *pg, int nb_fourmiliere);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture des ouvrières
bool modele_etat_ouvriere(char * tab, int nbO, int nbG, int f, int * po,
                          int nb_fourmiliere);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture des gardes
bool modele_etat_garde(char * tab, int nbG, int f, int * pg,
                       bool * pfin_ligne, double centre_x,
                       double centre_y, double rayon_fourmiliere,
                       int nb_fourmiliere);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture des
bool modele_etat_fin_fourmiliere(char * tab, int f, int nb_fourmiliere);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture de nb_nourriture
void modele_etat_nb_nourriture(char * tab, int * pnb_nourriture);

//---------------------------------------------------------------------
// effectue les instructions relative à la lecture des nourritures
bool modele_etat_nourriture(char * tab, int nb_nourriture, int * pn,
                            bool * pfin_ligne);

//---------------------------------------------------------------------
// appelle la fonction fourmiliere_verification_rendu2
bool modele_verification_rendu2(void);

//---------------------------------------------------------------------
// met à jour toutes les données de la simulation y compris le fichier "out.dat"
void modele_update(bool automatic, bool record_active, FILE * fichier);

//---------------------------------------------------------------------
// appelle la fonction de création de nourriture manuelle
void modele_nourriture_manual_creation(double x, double y);

//---------------------------------------------------------------------
// appelle les fonctions qui dessinent
void modele_dessine(void);

//---------------------------------------------------------------------
// libère la mémoire et reinitialise le nombre de fourmilièeres et 
// le nombre de nourritures
void modele_nettoyer(void);

//---------------------------------------------------------------------
// appelle les fonctions d'écriture
void modele_ecriture(char * nom_fichier);

//---------------------------------------------------------------------
// récupère les informations nécessaires, pour remplir le
// tableau de rollout
void modele_recup_info(int tab_info[NB_COLUMNS][NB_LINES]);

#endif
