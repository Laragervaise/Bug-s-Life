/*!
 \file utilitaire.c
 \brief Module qui met à disposition des outils mathématiques
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "constantes.h"
#include "utilitaire.h"

double utilitaire_calcul_norme(double x1, double x2, double y1, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

VECTOR utilitaire_normalise_vector(VECTOR vecteur)
{
    VECTOR v_normalise;
    v_normalise.x = vecteur.x / vecteur.norme;
    v_normalise.y = vecteur.y / vecteur.norme;
    v_normalise.norme = 1;
    
    return v_normalise;
}

double utilitaire_distance_point_droite(double posx, double butx, double posy,
                                        double buty, double pointx,
                                        double pointy)
{
    double pente, ordonnee_origine, d;
    
    VECTOR v;
    v.x = butx - posx;
    v.y = buty - posy;
    
    if(!v.x)
        d = fabs(pointx - posx);
    else if(!v.y)
        d = fabs(pointy - posy);
    else
    {
        pente = v.y/v.x;
        ordonnee_origine = posy - pente*posx;
        
        d = (fabs(pente*pointx - pointy + ordonnee_origine))/
        (sqrt(1+pow(pente, 2)));
    }
    
    return d;
}

bool utilitaire_contact_sans_tolerance(double x1, double x2, double y1,
                                       double y2, double r1, double r2)
{
    if(utilitaire_calcul_norme(x1, x2, y1, y2)-(r1+r2) <= 0)
        return 1;
    return 0;
}

bool utilitaire_contact(double x1, double x2, double y1,
                        double y2, double r1, double r2)
{
    if(utilitaire_calcul_norme(x1, x2, y1, y2)-(r1+r2) <= EPSIL_ZERO)
        return 1;
    
    return 0;
}
