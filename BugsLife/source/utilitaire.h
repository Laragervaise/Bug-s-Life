/*!
 \file utilitaire.h
 \brief Module qui met à disposition des outils mathématiques
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#ifndef UTILITAIRE_H
#define UTILITAIRE_H

/*---------------------------------------------------------------------
 Structure de données d'un vecteur : type concret
	x , y : coordonnées du vecteur avec par exemple
	x = abcisse du point de d'arrivée - abscisse du point de départ
	norme : norme du vecteur
 ----------------------------------------------------------------------*/
typedef struct Vector VECTOR;
struct Vector
{
    double x;
    double y;
    double norme;
};

//---------------------------------------------------------------------
// calcule une norme avec deux abscisses et deux ordonnées
double utilitaire_calcul_norme(double x1, double x2, double y1, double y2);

//---------------------------------------------------------------------
// normalise le vecteur donné en argument
VECTOR utilitaire_normalise_vector(VECTOR vecteur);

//---------------------------------------------------------------------
// calcule la distance minimale(celle qui est perpendiculaire) entre un point 
// et un segment de droite
double utilitaire_distance_point_droite(double posx, double butx, double posy,
                                        double buty, double pointx,
                                        double pointy);

//---------------------------------------------------------------------
// teste si contact entre deux éléments (test avec 0)
bool utilitaire_contact_sans_tolerance(double x1, double x2, double y1,
                                       double y2, double r1, double r2);

//---------------------------------------------------------------------
// teste si contact entre deux éléments (test avec EPSIL_ZERO)
bool utilitaire_contact(double x1, double x2, double y1,
                        double y2, double r1, double r2);

#endif
