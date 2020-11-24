/*TOMA Andrei-311CB*/
#include "utils.h"

#ifndef _LISTA_
#define _LISTA_

typedef struct celst
{
	struct celst *urm;
	void *info;
} TCelSt, *ACelSt;

/*distruge lista generica*/
void DistrugeL(ACelSt *l, void(*freeInfo)(void *));

#endif