/*TOMA Andrei-311CB*/
#include "structures.h"

int Remaining(void *x)
{
	return ((Resource *)x)->dimension - ((Resource *)x)->currently_downloaded;
}

void AfisareWebPage(void *x, FILE *out)
{
	fprintf(out, "%s\n", (char *)x); 
}

void DistrugeWebPage(void *x)
{
	free(((TWebPage *)x)->resources); //eliberare memorie pentru resources
	free((TWebPage *)x); //eliberare memorie pentru pagina web
	return;
}

void AfisareResurse(void *x, FILE *out)
{
	Resource *r = (Resource *)x;
	if (r->name[0] == '\0') return;
	fprintf(out,
		"[\"%s\" : %ld/%ld]\n", r->name, r->dimension - r->currently_downloaded,
		r->dimension);
}

void AfisareResursaDescarcata (void *x, FILE *out)
{
	Resource *r = (Resource *)x;
	if (r->name[0] == '\0') return;
	fprintf(out, "[\"%s\" : completed]\n", r->name);
}

void DistrugeInfo(void *x)
{
	//distrugere stive de back si forward
	DistrS(&((TTab*)x)->back_stack, DistrugeWebPage); 
	DistrS(&((TTab*)x)->forward_stack, DistrugeWebPage); 
	if (((TTab *)x)->current_page) //daca pagina curenta este alocata
		DistrugeWebPage(((TTab *)x)->current_page);
	free((TTab*)x); //elibereaza tabul
}

TTab *alocaTab()
{
	TTab *tab = NULL;
	tab = (TTab *)malloc(sizeof(TTab)); //aloca tab
	if (!tab) return NULL; //verificare alocare
	tab->current_page = NULL;
	//initializare stive de back si forward
	tab->back_stack = InitS(sizeof(TWebPage));
	tab->forward_stack = InitS(sizeof(TWebPage));
	return tab;
}

ACelSt InitBrowser()
{
	ACelSt cel;
	cel = (ACelSt)malloc(sizeof(TCelSt)); //alocare celula din lista
	if (!cel) return NULL; //verificare alocare
	cel->urm = NULL;
	cel->info = alocaTab(); //alocare camp info
	return cel;	
}

int newtab(ACelSt l, ACelSt *current_tab)
{
	if (!l)
		return 0;
	ACelSt p = NULL, ultim = NULL, newtab = NULL;
	newtab = InitBrowser(); //creare tab nou
	//determinare ultima celula din lista
	for (p = l, ultim = NULL; p != NULL; ultim = p, p = p->urm);
	//adaugare la finalul listei
	if (ultim == NULL) 
		l->urm = newtab;
	else
		ultim->urm = newtab;
	*current_tab = newtab;
	return 1;
}

void deltab(ACelSt l, ACelSt *current_tab, void (*freeInfo)(void *))
{
	ACelSt p = NULL, penultim = NULL, aux = NULL;
	//determinarea penultimei celule
	for (p = l, penultim = NULL; p->urm != NULL; penultim = p, p = p->urm);
	//refacere legaturi
	if ((*current_tab)->urm == NULL)
		*current_tab = penultim;
	aux = p;
	penultim->urm = NULL;
	//eliberare ultima celula
	freeInfo(aux->info);
	free(aux);
}

void change_tab(ACelSt l, ACelSt *current_tab, int index_tab)
{
	int index = 0;
	ACelSt p = NULL;
	for (p = l; p != NULL; p = p->urm)
	{
		if (index == index_tab) //se ajunge pe indexul corespunzator
			break;
		index++;
	}
	(*current_tab) = p; //schimbare current tab
}

void print_open_tabs(ACelSt l, FILE *out)
{
	if (!l) return;
	ACelSt p = NULL;	
	int index = 0;
	for (p = l; p != NULL; p = p->urm)
	{
		if (PAGE(p))
			fprintf(out, "(%d: %s)\n", index, PAGE(p)->url);
		else
			fprintf(out, "(%d: empty)\n", index);
		index++;
	}
}

void go_to(ACelSt current_tab, char *url, void *global_history, 
	void *priorityQ, void *downloadQ, int bandwidth)
{
	int n=0;
	if (!PAGE(current_tab)) //daca nu este nicio pagina web in tabul curent
	{
		//alocare current_page
		PAGE(current_tab) = (TWebPage *)malloc(sizeof(TWebPage));
		if (!PAGE(current_tab)) return; //verificare alocare
	}
	else
	{
		//altfel este introdusa pagina web curenta in stiva de back
		Push(BACK_ST(current_tab), PAGE(current_tab));
		//resetare stiva de forward
		ResetS(FORWARD_ST(current_tab), DistrugeWebPage); 
	}
	strcpy(PAGE(current_tab)->url, url);
	//creare vector de resurse
	PAGE(current_tab)->resources = get_page_resources(url, &n);
	PAGE(current_tab)->num_res = n;
	//introducere in istoric global
	IntrQ(global_history, url);
	//descarcare resurse timp de o secunda
	if (!VIDAQ(priorityQ))
		wait(priorityQ, downloadQ, UNITATE_TIMP, bandwidth);
}

void back(ACelSt current_tab, FILE *out)
{
	if (VIDAS(BACK_ST(current_tab)))
	{
		fprintf(out, "can't go back, no pages in stack\n");
		return;
	}
	//pagina curenta este introdusa in stiva de forward
	Push(FORWARD_ST(current_tab), PAGE(current_tab));
	//pagina curenta devine elementul extras din stiva de back
	Pop(BACK_ST(current_tab), PAGE(current_tab));
}

void forward(ACelSt current_tab, FILE *out)
{
	if (VIDAS(FORWARD_ST(current_tab)))
	{
		fprintf(out, "can't go forward, no pages in stack\n");
		return;
	}
	//pagina curenta este introdusa in stiva de back
	Push(BACK_ST(current_tab), PAGE(current_tab));
	//pagina curenta devine elementul extras din stiva de forward
	Pop(FORWARD_ST(current_tab), PAGE(current_tab));
}

void history(void *global_history, FILE *out)
{
	//afiseaza intregul istoric
	AfiQ(global_history, AfisareWebPage, free, out);
}

void delete_history(void *global_history, int entries)
{
	if (entries == 0)
	{
		ResetQ(global_history, free); //resetare intreg istoric global
		return;
	}
	int contor = 0;
	char *aux = (char *)malloc(sizeof(char)*URL_LENGTH);
	while(contor != entries)
	{
		//extragere din coada de istoric global entries intrari pt a fi sterse
		ExtrQ(global_history, aux);
		contor++;
	}
	free(aux); //eliberare memorie
}

void list_dl(ACelSt current_tab, FILE *out)
{
	unsigned int i;
	if (!PAGE(current_tab)) return;
	for (i = 0; i < PAGE(current_tab)->num_res; i++)
		fprintf(out, "[%d - \"%s\" : %ld]\n", i, PAGE(current_tab)->
			resources[i].name, PAGE(current_tab)->resources[i].dimension);
}

void download(void *priorityQ, unsigned int index, ACelSt current_tab)
{
	if (!PAGE(current_tab)) return;
	if (PAGE(current_tab)->num_res <= index) return;
	//sortare in coada de prioritati dupa dimensiunea ramasa de descarcat
	SortQ(priorityQ, Remaining, &PAGE(current_tab)->resources[index]);
}

void downloads(void *priorityQ, void *downloadQ, FILE *out)
{
	if (!VIDAQ(priorityQ))
		//afisare descarcari active
		AfiQ(priorityQ, AfisareResurse, free, out);
	if (!VIDAQ(downloadQ))
		//afisare descarcari finalizate
		AfiQ(downloadQ, AfisareResursaDescarcata, free, out);
}

void wait(void *priorityQ, void *downloadQ, int seconds, int bandwidth)
{
	int download_size=seconds*bandwidth;
	Resource *res = (Resource *)malloc(sizeof(Resource));
	while(download_size>0)
	{
		//daca este descarcata intreaga resursa
		if ((int)RESURSA(priorityQ)->dimension -
			(int)RESURSA(priorityQ)->currently_downloaded-download_size <= 0)
		{
			//actualizare dimensiune ramasa de descarcare
			download_size -= (int)RESURSA(priorityQ)->dimension - 
			(int)RESURSA(priorityQ)->currently_downloaded;
			//actualizare camd currently_downloaded
			RESURSA(priorityQ)->currently_downloaded=
			(int)RESURSA(priorityQ)->dimension;
			//extragere resursa din coada de prioritati
			ExtrQ(priorityQ, res);
			//introducere resursa in codada de descarcari finalizate
			IntrQ(downloadQ, res);
		}
		//daca este descarcata o parte din resursa
		else if ((int)RESURSA(priorityQ)->dimension -
			(int)RESURSA(priorityQ)->currently_downloaded-download_size > 0)
		{
			//actualizare camp currently_downloaded
			RESURSA(priorityQ)->currently_downloaded += download_size;
			//memeoria de descarcare ramasa devine 0
			download_size = 0;
		}
	}
	free(res);
}