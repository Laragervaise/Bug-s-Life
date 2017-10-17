# Definitions de macros

CC     = gcc
CFLAGS = -Wall
CFILES = modele.c graphic.c fourmiliere.c fourmi.c nourriture.c error.c utilitaire.c main.cpp
OFILES = modele.o graphic.o fourmiliere.o fourmi.o nourriture.o error.o utilitaire.o main.o

# Definition de la premiere regle

rendu3.x: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -lstdc++ -lglut -lGL -lGLU -lm -Llib -lglui -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi -o rendu3.x

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.c~ *.h~

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
modele.o: modele.c utilitaire.h constantes.h tolerance.h error.h \
 graphic.h fourmi.h nourriture.h fourmiliere.h modele.h
graphic.o: graphic.c graphic.h
fourmiliere.o: fourmiliere.c error.h utilitaire.h graphic.h fourmiliere.h \
 constantes.h tolerance.h fourmi.h nourriture.h
fourmi.o: fourmi.c error.h utilitaire.h graphic.h fourmi.h constantes.h \
 tolerance.h nourriture.h fourmiliere.h
nourriture.o: nourriture.c error.h utilitaire.h graphic.h nourriture.h \
 constantes.h tolerance.h fourmi.h fourmiliere.h
error.o: error.c error.h constantes.h tolerance.h
utilitaire.o: utilitaire.c utilitaire.h
main.o: main.cpp modele.h constantes.h tolerance.h
