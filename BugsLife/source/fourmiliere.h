/*!
 \file fourmiliere.h
 \brief Module qui gère les informations relatives aux fourmilieres
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#ifndef FOURMILIERE_H
#define FOURMILIERE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"

#define NB_LINES   11
#define NB_COLUMNS 4

typedef struct Fourmiliere FOURMILIERE;

#include "fourmi.h"

//---------------------------------------------------------------------
// mémorise les fourmilières
bool fourmiliere_lecture(char tab[MAX_LINE], int f, int * pnbO,
                         int * pnbG, double * pcentre_x,
                         double * pcentre_y,
                         double * prayon_fourmiliere);

//---------------------------------------------------------------------
// ajoute une fourmilière à la liste chainée
void fourmiliere_ajouter(int f);

//---------------------------------------------------------------------
// détecte les erreurs relatives aux fourmilières (rendu 1)
bool fourmiliere_erreur(int nb_caract, int f, FOURMILIERE * tete_fourmiliere);

//---------------------------------------------------------------------
// détecte les erreurs de superposition (rendu 2)
bool fourmiliere_verification_rendu2(void);

//---------------------------------------------------------------------
// dessine les fourmilieres dans la fenêtre OPEN GL
void fourmiliere_dessine(void);

//---------------------------------------------------------------------
// vide la liste chainée intégralement
void fourmiliere_vider(void);

//---------------------------------------------------------------------
// initialise la valeur du nombre de fourmilières
void fourmiliere_set_nb(int set);

//---------------------------------------------------------------------
// retourne la valeur du nombre de fourmilières
int fourmiliere_get_nb();

//---------------------------------------------------------------------
// met à jour les differents champs des fourmilières
// et appelle les fonctions fourmi_update
void fourmiliere_update(void);

//---------------------------------------------------------------------
// choisit la naissance d'une garde ou d'une ouvrière
void fourmiliere_naissance_choix_fourmi(FOURMILIERE * a_tester);

//---------------------------------------------------------------------
// met à jour le rayon qui sera dessiné de sorte à ce que
// les fourmilieres ne se superposent pas
double fourmiliere_nouveau_rayon(FOURMILIERE * f1);

//---------------------------------------------------------------------
// detruit une fourmilière en particulier
void fourmiliere_dead(FOURMILIERE * a_tester, FOURMILIERE * precedent);

//---------------------------------------------------------------------
// renvoit 1 si la fourmi a une fourmiliere sur son chemin vers la nourriture
// 0 si il n'y a pas de fourmiliere sur le chemin 
bool fourmiliere_sur_chemin(double posx, double butx, double posy, double buty, 
							unsigned indice_f);

//---------------------------------------------------------------------
// teste si une nourriture se superpose à n'importe quelle fourmilière
bool fourmiliere_nourriture_superposition(double nourriture_x, double nourriture_y);

//---------------------------------------------------------------------
// teste si la fourmi se trouve dans une fourmilière étrangère
bool fourmiliere_fourmi_inside(double ouvriere_x, double ouvriere_y, 
							   unsigned indice_f);

//---------------------------------------------------------------------
// trouve la fourmilière la plus proche de l'ouvrière
bool fourmiliere_la_plus_proche(double ouvriere_x, double ouvriere_y, 
								unsigned indice_f, double * fourmiliere_a_attaquer_x, 
								double * fourmiliere_a_attaquer_y);

//---------------------------------------------------------------------
// teste si l'ouvriere est arrivée au centre d'une autre fourmiliere 
// (pour qu'elle prenne VAL_FOOD)                   
bool fourmiliere_manger(double pos_ouvriere_x, double pos_ouvriere_y, 
						unsigned indice_f);

//---------------------------------------------------------------------
// écrit dans le fichier fsortie les informations relatives aux fourmilières
void fourmiliere_ecriture(FILE * fsortie);

//---------------------------------------------------------------------
// récupère les informations des fourmilières pour remplir le rollout
void fourmiliere_recup_info(int tab_info[NB_COLUMNS][NB_LINES]);

//---------------------------------------------------------------------
// écrit dans le fichier "out.dat" pour l'affichage du graphique avec gnuplot
void fourmiliere_ecriture_gnuplot(FILE * fichier, int nb_update);

#endif
