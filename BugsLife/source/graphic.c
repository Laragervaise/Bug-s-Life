/*!
 \file graphic.c
 \brief Module qui gère des fonctions de dessin
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "graphic.h"

struct Couleur
{
    float r;
    float g;
    float b;
};

void graphic_draw_circle (float xc, float yc, float r, int filled, int col)
{
    int i;
    const int SIDES = 50;
    
    COULEUR tab_couleurs[NB_COLOR];
    
    tab_couleurs[ROUGE] = (COULEUR) { 1., 0., 0. };
    tab_couleurs[VERT]= (COULEUR) { 0., 1., 0. };
    tab_couleurs[BLEU]= (COULEUR) { 0., 0., 1.};
    tab_couleurs[MAGENTA] = (COULEUR) { 1., 0., 1. };
    tab_couleurs[CYAN] = (COULEUR) {0., 1., 1.};
    tab_couleurs[JAUNE] = (COULEUR) { 1., 1., 0. };
    tab_couleurs[MARRON] = (COULEUR) {0.34, 0.16, 0.};
    tab_couleurs[ORANGE] = (COULEUR) { 1., 0.5 , 0. };
    tab_couleurs[GRIS] = (COULEUR) {0.5, 0.5, 0.5};
    tab_couleurs[ROSE] = (COULEUR) {0.992, 0.424, 0.62};
    tab_couleurs[NOIR] = (COULEUR) {0., 0., 0.};
    
    
    COULEUR couleur = tab_couleurs[col];
    
    graphic_set_color3f(couleur.r, couleur.g, couleur.b);
    
    if (filled == GRAPHIC_FILLED)
        glBegin (GL_POLYGON);
    else
        glBegin (GL_LINE_LOOP);
    
    for (i=0; i < SIDES; i++)
    {
        float alpha = i * 2. * M_PI / SIDES;
        float x = xc + r*cos(alpha), y = yc + r*sin(alpha);
        glVertex2f(x,y);
    }
    
    glEnd ();
}

void graphic_set_color3f(float r, float g,float b) 
{
    glColor3f(r, g, b);
}
