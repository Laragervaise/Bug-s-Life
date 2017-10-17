/*!
 \file nourriture.h
 \brief Module qui gère les informations relatives à la nourriture
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#ifndef NOURRITURE_H
#define NOURRITURE_H

#include <stdbool.h>
#include <stdio.h>
#include "constantes.h"

typedef struct Nourriture NOURRITURE;

#include "fourmi.h"
#include "fourmiliere.h"

//---------------------------------------------------------------------
// mémorise la nourriture
bool nourriture_lecture(char tab[MAX_LINE], int * pn, bool * pfin_ligne);

//---------------------------------------------------------------------
// ajoute une nourriture à la liste chainée
void nourriture_ajouter();

//---------------------------------------------------------------------
// détecte les erreurs relatives à la nourriture (rendu 1)
bool nourriture_erreur(int n, double x, double y);

//---------------------------------------------------------------------
// dessine les nourritures dans la fenetre OPEN GL
void nourriture_dessine(void);

//---------------------------------------------------------------------
// vide la liste chainée entièrement
void nourriture_vider(void);

//---------------------------------------------------------------------
// initialise la valeur du nombre de nourritures
void nourriture_set_nb(int set);

//---------------------------------------------------------------------
// crée une nourriture la où la souris clique en mode manual
void nourriture_update_manual_creation(double x, double y);

//---------------------------------------------------------------------
// crée une nourriture de position aléatiore si mode automatic
// appelle des fonctions pour vérifier qu'elle ne se superpose à rien
// sinon on recherche une nouvelle position aléatoire
void nourriture_update_automatic_creation(void);

//---------------------------------------------------------------------
// teste si la nourriture créée se superpose à quelquechose en appelant
// des fonctions pour chaque type de superposition
bool nourriture_superposition(double x, double y);

//---------------------------------------------------------------------
// teste si une nourriture se superpose à n'importe quelle autre nourriture
bool nourriture_entre_nourriture_superposition(double x, double y);

//---------------------------------------------------------------------
// teste si l'ouvriere se superpose à une nourriture (pour qu'elle la mange)
bool nourriture_manger(double pos_ouvriere_x, double pos_ouvriere_y);

//---------------------------------------------------------------------
// tester si la nourriture se superpose à la fourmi
bool nourriture_contact_fourmi(double x1, double x2, double y1, double y2);

//---------------------------------------------------------------------
// place la nourriture là ou l'ouvriere meurt lorsqu'une ouvrière en possession
// de nourriture meurt
void nourriture_update_ouvriere_dead(double posx_o, double posy_o);

//---------------------------------------------------------------------
// trouve la nourriture la plus proche de l'ouvrière
// qui n'est pas déja le but d'une ouvrière de la meme fourmilière
// et dont une ouvrière d'une autre fourmiliere n'est pas plus proche
// (critères verifiés en appelant deux fonctions de fourmi)
bool nourriture_la_plus_proche(double ouviere_x, double ouvriere_y,
                               double *  pnourriture_x, double * pnourriture_y,
                               unsigned indice_f, unsigned indice_o);
                               
//---------------------------------------------------------------------
// écrit dans le fichier fsortie les informations relatives aux nourritures
void nourriture_ecriture(FILE * fsortie);

#endif
