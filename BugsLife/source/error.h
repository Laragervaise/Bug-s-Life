/*!
 \file error.h
 \brief Module qui gère les messages d'erreur. NE PAS MODIFIER
 \author PROG II
 \version 1.0
 \date 10 mars 2017
 */

#ifndef ERROR_H
#define ERROR_H

//---------------------------------------------------------------------
// ces déclarations combinent deux outils du langage C : enum et typedef
// enum : déclare une liste de symboles équivalents à des entiers
//        le premier symbole vaut 0, le suivant vaut 1, etc...
// typedef : the nom de type rend le code plus lisible

//---------------------------------------------------------------------
// liste des symboles à utiliser pour appeler les fonctions d'erreur
// selon l'entité qui produit l'erreur
typedef enum ErreurOrig {
    ERR_FOURMILIERE, ERR_OUVRIERE, ERR_GARDE, ERR_NOURRITURE
} ERREUR_ORIG;

//---------------------------------------------------------------------
// liste des symboles à utiliser pour appeler les fonctions d'erreur
// pour les cas où on a trop ou pas assez d'entité dans la liste
typedef enum ErreurType {
    ERR_TROP, ERR_PAS_ASSEZ
} ERREUR_TYPE;

// exemple d'appel: echec de la lecture des paramètres d'un photon
// le symbole ERR_PAS_ASSEZ est aussi utilisé en cas d'erreur de syntaxe
// car le programme ne peut pas faire la différence entre les deux cas.
//
// error_lecture_elements(ERR_OUVRIERE , ERR_PAS_ASSEZ)

//---------------------------------------------------------------------
// utilisable pour afficher des messages d'erreurs qui ne font pas
// partie de la liste des erreurs testées pour le rendu1
void error_msg(char * msg);

//---------------------------------------------------------------------
// Erreurs testées en mode Error pour le rendu1 (et les rendus suivants)
//   ----> le paramètre indice commence à zéro

//---------------------------------------------------------------------
// Rendu1 en mode Error: appeler en cas d'absence d'erreur
void error_success(void);

//---------------------------------------------------------------------
// impossible d'ouvrir le fichier, fichier inexistant
void error_fichier_inexistant(void);

//---------------------------------------------------------------------
// si la lecture d'une ligne du fichier échoue (on reçoit EOF)
// alors que la lecture des entités n'est pas terminée
void error_fichier_incomplet(void);


//---------------------------------------------------------------------
// si le nombre de fourmiliere dépasse MAX_FOURMILIERE 
void error_nb_fourmiliere(int nb_fourmiliere);

//---------------------------------------------------------------------
// Rendu1 en mode Error:la position n'est pas dans l'intervalle [-DMAX. DMAX]
// IMPORTANT: la valeur de l'indice est de type "indice de tableau"
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_pos_domaine(ERREUR_ORIG origine,unsigned indice, double x, double y);

//---------------------------------------------------------------------
// Rendu1 en mode Error: le rayon de la fourmiliere est "trop grand"
//     "trop grand" <=> rayon_lu > rayon_max donné par equ.(1) + EPSIL_ZERO 
// IMPORTANT: la valeur de l'entier fourmiliere est de type "indice de
//            tableau". Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_rayon_fourmiliere(unsigned fourmiliere) ;

//---------------------------------------------------------------------
// Rendu1 en mode Error: l'age d'une fourmi est supérieur ou égal au maximum 
// IMPORTANT: la valeur de l'entier fourmiliere est de type "indice de
//            tableau". Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_age_fourmi(unsigned fourmiliere, unsigned fourmi, unsigned age);

//---------------------------------------------------------------------
// Rendu1 en mode Error: un garde n'est pas complètement inclus dans sa fourmiliere 
// IMPORTANT: la valeur des indices est de type "indice de tableau". 
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
// 
void error_pos_garde(unsigned fourmiliere, unsigned garde);

//---------------------------------------------------------------------
// Rendu1 en mode Error: échec de la lecture de liste pour une fourmiliere
//                       -> trop, pas assez ou syntaxe
// ex: Erreur : fourmiliere 0: liste de garde -> trop ou erreur de syntaxe

void error_lecture_elements_fourmiliere(unsigned indice, ERREUR_ORIG origine, 
										ERREUR_TYPE type);

//---------------------------------------------------------------------
// Rendu1 en mode Error: échec de la lecture de liste de nourriture 
//                       -> trop, pas assez ou syntaxe

void error_lecture_elements_nourriture(ERREUR_TYPE type);

//---------------------------------------------------------------------
// Rendu2 en mode Vérification: cas de superposition interdit pour
//           2 fourmilieres 
//           La section 3.3 décrit le test à effectuer
// IMPORTANT: la valeur des indices est de type "indice de tableau". 
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
//
void error_superposition_fourmiliere(unsigned id1, unsigned id2);

//---------------------------------------------------------------------
// Rendu2 en mode Vérification: cas de superposition interdit pour
//           les combinaisons entre fourmis (if= indice fourmiliere)
//           La section 3.3 décrit le test à effectuer
// IMPORTANT: la valeur des indices est de type "indice de tableau". 
//            Elle est comprise entre 0 et (nb_element-1).
//            0 désigne le premier élément apparaissant dans le fichier etc...
//
void error_superposition_fourmi(ERREUR_ORIG origine1, unsigned if1, unsigned id1,
								ERREUR_ORIG origine2, unsigned if2, unsigned id2);

#endif

