/*TOMA Andrei-311CB*/
#include "stiva.h"

void *InitS(int d)
{
	void *s;
	s = (ASt)malloc(sizeof(TStiva)); //alocare a stivei
	if (!s) return NULL; //verificare alocare
	DIMES(s) = d;
	VF(s) = NULL; //seteaza varful stivei pe NULL
	return (void *)s;
}

int Push(void *a, void *ae)
{
	ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));//aloca o celula
	if (!aux) return 0; //verifica alocarea
	aux->info = malloc(DIMES(a)); //alocare campul info
	if (!aux->info) //verificare alocare
	{ 
		free(aux);
		return 0;
	}
	//copiaza in aux->info valoarea de la adresa ae
	memcpy(aux->info, ae, DIMES(a));
	//adaugare in stiva
	aux->urm = VF(a);
	VF(a) = aux;
	return 1;
}

int Pop(void *a, void *ae)
{
	if (VIDAS(a)) return 0; //verificare stiva vida
	//copiaza in valoarea alocata de la adresa ae informatia din vf stivei
	memcpy(ae, VF(a)->info, DIMES(a));
	//actualizare varf
	ACelSt aux = VF(a);
	VF(a) = aux->urm;
	//eliberare memorie
	free(aux->info);
	free(aux);
	return 1;
}

void AfiS(void *a, void (*Afis)(void *, FILE *), void (*Distr)(void *),
	FILE *out)
{
	void *s = InitS(DIMES(a)); //stiva auxiliara
	void *x = malloc(DIMES(a)); //element in care extragem valoarea din stiva
	if (VIDAS(a)) return; //verificare stiva vida
	while (!VIDAS(a)) 
	{
		Pop(a, x); //extrage elementul x din stiva
		Afis(x, out); //afiseaza elementul x in fiisierul out
		Push(s, x); //adauga elementul x in stiva auxiliara
	}
	free(x);
	Rastoarna(a, s); //refacere stiva initiala a
	DistrS(&s, Distr); //distrugere stiva auxiliara
}

void ResetS(void *a, void (*Distr)(void *))
{
	//elibereaza memoria pentru lista
	//seteaza varful pe NULL
	ACelSt l = VF(a);
	DistrugeL(&l, Distr);
	VF(a) = NULL;
}

void DistrS(void **a, void (*Distr)(void *))
{
	ResetS(*a, Distr);
	free(*a); //eliberare memorie pentru intreaga stiva
}

int Rastoarna(void *ad, void *as)
{
	if (DIMES(ad) != DIMES(as)) return 0;
	if (VIDAS(as)) return 0;
	ACelSt aux;
	while (!VIDAS(as))
	{
		//retinere varf actual
		aux = VF(as);
		//actualizare varf stiva sursa
		VF(as) = VF(as)->urm;
		//legare element din lista sursa in lista destinatie
		aux->urm = VF(ad);
		//actualizare varf stiva destinatie
		VF(ad) = aux;
	}
	//setare varful stivei sursa pe NULL
	VF(as) = NULL;
	return 1;
}