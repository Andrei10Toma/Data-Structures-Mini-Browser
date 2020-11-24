/*TOMA Andrei-311CB*/
#include "utils.h"
#include "lista_generica.h"

#ifndef _STIVA_
#define _STIVA_

typedef struct stiva
{
	int dime; /*dimensiunea unui element din stiva*/
	ACelSt vf; /*varful stivei*/
} TStiva, *ASt;

#define VF(a) (((ASt)(a))->vf) /*varful stivei*/
#define DIMES(a) (((ASt)(a))->dime) /*dimensiunea unui element din stiva*/
#define VIDAS(a) (((ASt)(a))->vf == NULL) /*verificare stiva vida*/


/*initializare stiva vida cu dimensiunea unui element d*/
void *InitS(int d);

/*inserare in stiva valoareae de la zona alocata de la adresa ae*/
int Push(void *a, void *ae);

/*extragere din stiva a in zona alocata de la adresa ae*/
int Pop(void *a, void *ae);

/*afiseaza stiva in fisierul out*/
void AfiS(void *a, void (*Afis)(void *, FILE *), void (*Distr)(void *),
	FILE *out);

/*distrugere stiva (eliberare memorie pentru intreaga stiva)*/
void DistrS(void **a, void (*Distr)(void *));

/*resetare stiva (transforma stiva in stiva vida)*/
void ResetS(void *a, void (*Distr)(void *));

/*rastoarna stiva sursa in stiva destinatie*/
int Rastoarna(void *ad, void *as);

#endif