/*TOMA Andrei-311CB*/
#include "coada.h"

void *InitQ(int d)
{
	void *c = (AQ)malloc(sizeof(TCoada)); //alocare coada
	if (!c) return NULL; //verificare alocare
	((AQ)c)->dime = d; //setare dimensiunea unui element din coada
	// setare coada vida
	((AQ)c)->ic = NULL; 
	((AQ)c)->sc = NULL;
	return (void *)c;
}

int IntrQ(void *c, void *ae)
{
	ACelSt aux = (ACelSt)malloc(sizeof(TCelSt)); //alocare celula
	if (!aux) return 0; //verificare alocare
	aux->info = malloc(DIMEQ(c)); //alocare campul info cu dimensiunea cozii
	if (!aux->info) //verificare alocare
	{
		free(aux);
		return 0;
	}
	memcpy(aux->info, ae, DIMEQ(c)); //copiere in aux->info val de la adr ae
	aux->urm = NULL;
	// introudcere in coada
	if (VIDAQ(c))
	{
		IC(c) = aux;
		SC(c) = aux;
		return 1;
	}
	SC(c)->urm = aux;
	SC(c) = aux;
	return 1;
}

int ExtrQ(void *c, void *ae)
{
	if (VIDAQ(c)) return 0; //verificare coada vida
	ACelSt aux = IC(c);
	//actualizare inceput coada
	IC(c) = aux->urm;
	memcpy(ae, aux->info, DIMEQ(c)); //extragere din coada
	// eliberare memorie
	free(aux->info);
	free(aux);
	if (IC(c) == NULL)
		SC(c) = NULL;
	return 1;
}

void AfiQ(void *c, void (*Afis)(void *, FILE *), void (*Distr)(void *),
	FILE *out)
{
	if (VIDAQ(c)) return;
	void *x = malloc(DIMEQ(c));
	if (!x) return;
	void *c_aux = InitQ(DIMEQ(c));
	while (ExtrQ(c, x)) //extragere din coada
	{
		Afis(x, out); //afisare
		IntrQ(c_aux, x); //introducere in coada auxiliare
	}
	free(x);
	ConcatQ(c, c_aux); //refacere coada c
	DistrQ(&c_aux, Distr); //eliberare memorie pentru c_aux
}

int ConcatQ(void *ad, void *as)
{
	if (DIMEQ(ad) != DIMEQ(as)) return 0;

	if (VIDAQ(ad))
	{
		IC(ad) = IC(as);
		SC(ad) = SC(as);
		IC(as) = NULL;
		SC(as) = NULL;
		return 1;
	}
	if (VIDAQ(as)) return 0;
	//adaugare coada sursa la coada destinatie
	SC(ad)->urm = IC(as); 
	//actualizare sfarsit coada destinatie
	SC(ad) = SC(as);
	IC(as) = NULL;
	SC(as) = NULL;
	return 1;
}

void ResetQ(void *c, void (*Distr)(void *))
{
	ACelSt p;
	p = IC(c);
	IC(c) = NULL;
	SC(c) = NULL;
	DistrugeL(&p, Distr);
}

void DistrQ(void **c, void (*Distr)(void *))
{
	ResetQ(*c, Distr);
	free(*c);
}

void SortQ(void *c, int (*cmp)(void *), void *res)
{
	//test coada vida
	if (VIDAQ(c))
	{
		//este introdus elementul res in coada sortata
		IntrQ(c, res);
		return;
	}
	void *c1 = InitQ(DIMEQ(c)); //coada auxiliara
	void *x = malloc(DIMEQ(c)); //variabila in care extragem valori din coada c
	if (cmp(res) > cmp(IC(c)->info))
	{
		while (!VIDAQ(c))
		{
			//daca elementul din coada este mai mare decat elementul pe care
			//vrem sa il introducem se opreste
			if (cmp(IC(c)->info) >= cmp(res))
				break;
			//altfel extrage elemente din coada si le introdce in c1
			ExtrQ(c, x);
			IntrQ(c1, x);
		}
		//introducem elementul res in c1
		IntrQ(c1, res);
		ConcatQ(c1, c);
	}
	else
	{
		//elementul res trebuie introdus la inceputul cozii c
		//extragem toate elementele din c si le introducem in c1
		while (ExtrQ(c, x))
			IntrQ(c1, x);
		//introducere res la inceputul cozii
		IntrQ(c, res);
	}
	//refacere coada c
	ConcatQ(c, c1);
	//eliberare memorie pentru x si coada auxiliara
	free(x);
	DistrQ(&c1, free);
}