/*!
 \file error.c
 \brief Module qui gère les messages d'erreur. NE PAS MODIFIER
 \author PROG II
 \version 1.0
 \date 10 mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "constantes.h"

// chaînes prédéfinies pour composer les messages d'erreur

const static char nom_orig[][20] = {"fourmiliere", "ouvriere", "garde", 
									"nourriture"};
const static char nom_type[][20] = {"trop", "pas assez"};

//---------------------------------------------------------------------
// utilisable pour afficher des messages d'erreurs qui ne font pas
// partie de la liste des erreurs testées pour le rendu1
void error_msg(char * msg) {
    printf("erreur : %s\n", msg);
    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------
// Erreurs testées en mode Error pour le rendu1 (et les rendus suivants)
//   ----> le paramètre indice commence à zéro

//---------------------------------------------------------------------
// Rendu1 en mode Error: appeler en cas d'absence d'erreur
void error_success(void) {
    printf("Fichier correct\n");
}

//---------------------------------------------------------------------
// impossible d'ouvrir le fichier, fichier inexistant
void error_fichier_inexistant(void) {
    printf("Erreur : Fichier inexistant\n");
}

//---------------------------------------------------------------------
// si la lecture d'une ligne du fichier échoue (on reçoit EOF)
// alors que la lecture des entités n'est pas terminée
void error_fichier_incomplet(void) {
    printf("Erreur : Fichier incomplet (EOF inattendu)\n");
}


//---------------------------------------------------------------------
// si le nombre de fourmilieres est plus grand que MAX_FOURMILIERE
void error_nb_fourmiliere(int nb_fourmiliere) {
	printf("Erreur : Nombre de fourmiliere trop grand (%d>%d)\n",
			nb_fourmiliere, MAX_FOURMILIERE);
}


//---------------------------------------------------------------------
// Rendu1 en mode Error:la position n'est pas dans l'intervalle [-DMAX. DMAX]
// IMPORTANT: la valeur de l'indice est de type "indice de tableau"
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_pos_domaine(ERREUR_ORIG origine,unsigned indice, double x, double y) {
    printf("Erreur : au moins une coordonnée de la %s %u est incorrecte"
		   "-> %g et/ou %g\n", nom_orig[origine],indice,x,y);
}

//---------------------------------------------------------------------
// Rendu1 en mode Error: le rayon de la fourmiliere est "trop grand"
//     "trop grand" <=> rayon_lu > rayon_max donné par equ.(1) + EPSIL_ZERO 
// IMPORTANT: la valeur de l'entier fourmiliere est de type "indice de
//            tableau". Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_rayon_fourmiliere(unsigned fourmiliere) {
    printf("Erreur : le rayon de la fourmiliere %u est trop grand\n", fourmiliere);
}	

//---------------------------------------------------------------------
// Rendu1 en mode Error: l'age d'une fourmi est supérieur ou égal au maximum 
// IMPORTANT: la valeur de l'entier fourmiliere est de type "indice de
//            tableau". Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_age_fourmi(unsigned fourmiliere, unsigned fourmi, unsigned age) {
    printf("Erreur : age %u trop grand pour fourmi %u de fourmiliere %u\n", 
           age, fourmi, fourmiliere);
}	

//---------------------------------------------------------------------
// Rendu1 en mode Error: un garde n'est pas complètement inclus dans sa fourmiliere 
// IMPORTANT: la valeur des indices est de type "indice de tableau". 
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_pos_garde(unsigned fourmiliere, unsigned garde) {
    printf("Erreur : le garde %u déborde de la fourmiliere %u\n", 
           garde, fourmiliere);
}	

//---------------------------------------------------------------------
// Rendu1 en mode Error: échec de la lecture de liste pour une fourmiliere
//                       -> trop, pas assez ou syntaxe
// ex: Erreur : fourmiliere 0: liste de garde -> trop ou erreur de syntaxe

void error_lecture_elements_fourmiliere(unsigned indice, ERREUR_ORIG origine, 
										ERREUR_TYPE type) {
    printf("Erreur : fourmiliere %u: liste de %s -> %s ou erreur de syntaxe\n",
           indice, nom_orig[origine], nom_type[type]);
}

//---------------------------------------------------------------------
// Rendu1 en mode Error: échec de la lecture de liste de nourriture 
//                       -> trop, pas assez ou syntaxe

void error_lecture_elements_nourriture(ERREUR_TYPE type) {
    printf("Erreur :liste de nourriture -> %s ou erreur de syntaxe\n",
           nom_type[type]);
}

//---------------------------------------------------------------------
// Rendu2 en mode Vérification: cas de superposition interdit pour
//           2 fourmilieres 
//           La section 3.3 décrit le test à effectuer
// IMPORTANT: la valeur des indices est de type "indice de tableau". 
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
//
void error_superposition_fourmiliere(unsigned id1, unsigned id2) {
    printf("Erreur : superposition fourmiliere interdite entre %u et %u\n", 
           id1, id2);
}

//---------------------------------------------------------------------
// Rendu2 en mode Vérification: cas de superposition interdit pour
//           les combinaisons entre fourmis (if= indice fourmiliere)
//           La section 3.3 décrit le test à effectuer
// IMPORTANT: la valeur des indices est de type "indice de tableau". 
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
//
void error_superposition_fourmi(ERREUR_ORIG origine1, unsigned if1, unsigned id1,
								ERREUR_ORIG origine2, unsigned if2, unsigned id2) {
    printf("Erreur : superposition interdite entre %s (%u / %u) et  %s (%u / %u)\n",
           nom_orig[origine1], if1, id1, nom_orig[origine2], if2, id2);
}
