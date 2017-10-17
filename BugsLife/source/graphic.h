/*!
 \file graphic.h
 \brief Module qui gère les fonctions de dessin
 \author Bromet Juliette et Gervaise Lara
 \version 3
 \date mai 2017
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#define GRAPHIC_FILLED 1
#define GRAPHIC_EMPTY 0

typedef struct Couleur COULEUR;

enum CouleurNom {ROUGE, VERT, BLEU, MAGENTA, CYAN, JAUNE, MARRON, ORANGE,
				 GRIS, ROSE, NOIR, NB_COLOR};

//---------------------------------------------------------------------
// dessine un cercle de couleur col, plein ou non
void graphic_draw_circle (float xc, float yc, float r, int filled, int col);

//---------------------------------------------------------------------
// met en couleur l'objet avce le code RGB
void graphic_set_color3f(float r, float g,float b);

#endif
