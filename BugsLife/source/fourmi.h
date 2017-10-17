/*!
 \file fourmi.h
 \brief Module qui gère les informations relatives aux fourmis ouvrières
  et gardes
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#ifndef FOURMI_H
#define FOURMI_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"

typedef struct Ouvriere OUVRIERE;

typedef struct Garde GARDE;

#include "nourriture.h"
#include "fourmiliere.h"

//---------------------------------------------------------------------
// mémorise les ouvrières
bool fourmi_ouvriere_lecture(char tab[MAX_LINE], int j, int i);

//---------------------------------------------------------------------
// ajoute une ouvrière à la liste chainée
void fourmi_ouvriere_ajouter(void);

//---------------------------------------------------------------------
// détecte les erreurs relatives aux ouvrières pour le rendu 1
bool fourmi_ouvriere_erreur(int nb_caract, int f, int o, unsigned age,
                            double posx, double posy);

//---------------------------------------------------------------------
// mémorise les gardes
bool fourmi_garde_lecture(char tab[MAX_LINE], int f, int * pg, int nbG,
                          double centre_x, double centre_y,
                          double rayon_fourmiliere, bool * pfin_ligne);

//---------------------------------------------------------------------
// ajoute une garde à la liste chainée
void fourmi_garde_ajouter(void);

//---------------------------------------------------------------------
// détecte les erreurs relatives aux gardes (rendu 1)
bool fourmi_garde_erreur(int f, int g, unsigned age, double x, double y,
                         double centre_x, double centre_y,
                         double rayon_fourmiliere);

//---------------------------------------------------------------------
// détecte les erreurs de superposition des fourmis (rendu 2)
bool fourmi_verification_rendu2();

//---------------------------------------------------------------------
// dessine les ouvrières dans la fenètre OPEN GL
void fourmi_ouvriere_dessine(void);

//---------------------------------------------------------------------
// dessine les gardes dans la fenètre OPEN GL
void fourmi_garde_dessine(void);

//---------------------------------------------------------------------
// appelle les fonctions pour vider des éléments des listes
// chainées pour les gardes et les ouvrières
void fourmi_vider(void);

//---------------------------------------------------------------------
// vide la liste chainée des ouvrières
void fourmi_ouvriere_vider(void);

//---------------------------------------------------------------------
// vide la liste chainée des gardes
void fourmi_garde_vider(void);

//---------------------------------------------------------------------
// met à jour les differents champs d'une ouvriere
int fourmi_ouvriere_update(unsigned indice_f, int * pnourriture_rapportee,
                           int centre_x, int centre_y);

//---------------------------------------------------------------------
// detruit une ouvriere en particulier
void fourmi_ouvriere_dead(OUVRIERE * a_tester, OUVRIERE * precedent,
                          int * pcompteur_o);

//---------------------------------------------------------------------
// recherche si l'ouvrière entre en collision avec une autre fourmi
void fourmi_ouvriere_collision(OUVRIERE * ouvriere);

//---------------------------------------------------------------------
// met à jour les differents champs d'une garde
int fourmi_garde_update(unsigned indice_f, double centre_x,
                        double centre_y, double rayon_fourmiliere);

//---------------------------------------------------------------------
// detruit une garde en particulier
void fourmi_garde_dead(GARDE * a_tester, GARDE * precedent, int * pcompteur_g);

//---------------------------------------------------------------------
// recherche si la garde entre en collision avec une ouvrière
void fourmi_garde_collision(GARDE * garde);

//---------------------------------------------------------------------
//  effectue les operations necessaires lors de la naissance d'une ouvriere
void fourmi_ouvriere_naissance(double centre_x, double centre_y, unsigned indice_f);

//---------------------------------------------------------------------
// effectue les opérations nécessaires lors de la naissance d'une garde
void fourmi_garde_naissance(double x, double y, unsigned indice_f);

//---------------------------------------------------------------------
// teste si une nourriture se superpose à n'importe qu'elle ouvriere
bool fourmi_nourriture_superposition(double nourriture_x, double nourriture_y);

//---------------------------------------------------------------------
// met à jour le but de l'ouvriere en fonction du bon élément de nourriture à prendre
void fourmi_ouvriere_bon_choix_aller(OUVRIERE * ouvriere, unsigned indice_f,
                                     double centre_x, double centre_y);

//---------------------------------------------------------------------
// teste si une ouvriere de la meme fourmiliere a deja la nourriture en but
bool fourmi_nourriture_deja_prise(unsigned indice_f, unsigned indice_o,
                                  double nourriture_x, double nourriture_y);

//---------------------------------------------------------------------
// teste si une fourmi étrangère est plus proche de la nourriture que la 
// la fourmi à tester
bool fourmi_etrangere_plus_proche(unsigned indice_f, double nourriture_x,
                                  double nourriture_y, double distance);

//---------------------------------------------------------------------
// met à jour le but de l'ouvriere pour retourner au centre de sa fourmiliere
// sans traverser une autre fourmiliere
void fourmi_ouvriere_bon_choix_retour(OUVRIERE * ouvriere, double centre_x,
                                      double centre_y);

//---------------------------------------------------------------------
// met à jour la position de l'ouvrière en fonction de son but
void fourmi_ouvriere_update_pos(OUVRIERE * ouvriere);

//---------------------------------------------------------------------
// met à jour le but de la garde
void fourmi_garde_update_but(unsigned indice_f, double centre_x,
                             double centre_y, double rayon_fourmiliere,
                             GARDE * garde);

//---------------------------------------------------------------------
// met à jour la position de la garde en fonction de son but
void fourmi_garde_update_pos(GARDE * garde);

//---------------------------------------------------------------------
// teste si la fourmi est dans la fourmiliere
bool fourmi_dans_fourmiliere(double fourmi_x, double centre_x,
                             double fourmi_y, double centre_y,
                             double rayon_fourmiliere);

//---------------------------------------------------------------------
// teste si il y a contact entre la fourmi et la fourmiliere
bool fourmi_contact_fourmiliere(double fourmi_x, double centre_x, 
                                double fourmi_y, double centre_y, 
                                double rayon_fourmiliere);
                                
//---------------------------------------------------------------------
// écrit dans le fichier fsortie les informations relatives aux ouvrières
void fourmi_ouvriere_ecriture(FILE * fsortie, int indice_f);

//---------------------------------------------------------------------
// écrit dans le fichier fsortie les informations relatives aux gardes
void fourmi_garde_ecriture(FILE * fsortie, int indice_f);

#endif
