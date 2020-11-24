/*TOMA Andrei-311CB*/
#include "lista_generica.h"
#include "utils.h"

#ifndef _COADA_
#define _COADA_

typedef struct coada
{
	int dime; /*dimensiune element*/
	ACelSt ic, sc; /*adr. prima, ultima celula*/
} TCoada, *AQ;

#define DIMEQ(c) (((AQ)(c))->dime) /*dimensiunea elementelor din coada*/
#define IC(c) (((AQ)(c))->ic) /*inceputul cozii*/
#define SC(c) (((AQ)(c))->sc) /*sfarsitul cozii*/
#define VIDAQ(c) (((AQ)(c))->ic == NULL && ((AQ)c)->sc == NULL) /*coada vida*/

/*initializare coada cu dimensiunea d*/
void *InitQ(int d);

/*inserare in coada valoarea de la adresa ae alocata*/
int IntrQ(void *a, void *ae);

/*extragere din coada valoarea si o copiaza la adressa ae*/
int ExtrQ(void *a, void *ae);

/*afiseaza coada a primeste ca parametru un pointer la o functie de afisare
si o funtctie care distruge informatia din lista*/
void AfiQ(void *a, void (*Afis)(void *, FILE *), void (*Distr)(void *), 
	FILE *out);

/*distrugere coada a si mai primeste ca parametru un pointer la o functie care
distruge informatia din lista*/
void DistrQ(void **a, void (*Distr)(void *));

/*reseteaza coada (distruge lista)*/
void ResetQ(void *a, void (*Distr)(void *));

/*concateneaza coada sursa in coada destinatie*/
int ConcatQ(void *ad, void *as);

/*sorteaza coada primind un pointer la o functie de comparare care determina
criteriul de comparare si un parametru care reprezinta ce se va adauga 
in coada*/
void SortQ(void *a, int (*cmp)(void *), void *res);

/*returneaza elementul minim din coada dupa criteriul de comparare dat de
pointerul la functia de comparare*/
int MinimQ(void *c, int (*cmp)(void *));

#endif