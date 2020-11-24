/*TOMA Andrei-311CB*/
#include "stiva.h"
#include "utils.h"
#include "coada.h"
#include "lista_generica.h"

#ifndef _STRUCTURES_
#define _STRUCTURES_

#define LINE_LENGTH 256
#define UNITATE_TIMP 1
#define URL_LENGTH 100

typedef struct web_page
{
	char url[21]; /*urlul paginii web*/
	unsigned int num_res; /*numarul de resurse*/
	struct resurse *resources; /*vector de resurse*/
}TWebPage;

typedef struct tab
{
	TWebPage *current_page; /*referinta catre pagina curenta*/
	void *back_stack; /*stiva de back*/
	void *forward_stack; /*stiva de forward*/
}TTab;

/*macrodefinitii pentru accesarea paginii web curente, stivei de back, 
stivei de forward si operatia de peek pe coada*/
#define PAGE(y) (((TTab *)(y->info))->current_page)
#define BACK_ST(y) (((TTab *)(y->info))->back_stack)
#define FORWARD_ST(y) (((TTab *)(y->info))->forward_stack)
#define RESURSA(c) ((Resource*)(IC(c))->info)

/*initializeaza browserul alocand o celula si in campul info este alocat
un element de tip TTab in care sunt initalizate cele 2 stive si current_page
este setat pe NULL*/
ACelSt InitBrowser();

/*afiseaza url-ul webpageului in fisierul out*/
void AfisareWebPage(void *x, FILE *out);

/*afiseaza vectorul de resurse in curs de descarcare in fisierul out*/
void AfisareResurse(void *x, FILE *out);

/*returneaza diferenta dintre dimensiunea totala a resursei si cat a fost
descarcat din aceasta*/
int Remaining(void *x);

/*afiseaza resursele descarcate*/
void AfisareResursaDescarcata(void *x, FILE *out);

/*distruge informatia din campul info al listei*/
void DistrugeInfo(void *x);

/*distruge un element de tip TWebPage*/
void DistrugeWebPage(void *x);

/*creeaza un tab nou adaugandu-l la sfarsitul listei de taburi*/
int newtab(ACelSt lt, ACelSt *current_tab);

/*sterge ultimul tab*/
void deltab(ACelSt lt, ACelSt *current_tab, void (*freeInfo)(void *));

/*schmba tabul la indexul index*/
void change_tab(ACelSt lt, ACelSt *current_tab, int index);

/*afiseaza in fisierul out taburile deschise si informatia despre acestea*/
void print_open_tabs(ACelSt lt, FILE *out);

/*deschide un weh-page, il adauga in coada de history si de asemenea trece o
secunda pentru descarcarea resurselor*/
void go_to(ACelSt current_tab, char *url, void *history, void *priorityQ, 
	void *downloadQ, int bandwidth);

/*modifica tabul curent cu elemntul extras din stiva de back*/
void back(ACelSt current_tab, FILE *out);

/*modifica tabul curent cu elementul extras din stiva de forward*/
void forward(ACelSt current_tab, FILE *out);

/*afiseaza in fisierul out istoricul global*/
void history(void *global_history, FILE *out);

/*sterge n intrari din istoricul global*/
void delete_history(void *global_history, int n);

/*afiseaza toate resursele descarcabile din tabul curent*/
void list_dl(ACelSt current_tab, FILE *out);

/*adauga in lista de prioritati a n-a resursa din vectorul de resurse din
tabul curent*/
void download(void *priorityQ, unsigned int n, ACelSt current_tab);

/*afiseaza in fisierul out atat descarcarile active cat si descarcarile 
finalizate*/
void downloads(void *priorityQ, void *downloadQ, FILE *out);

/*asteapta trecerea unui timp fictiv (seconds) timp in care se pot descarca
bandwidth*seconds bytes; se descarca resursele in ordinea din coada de 
prioritati*/
void wait(void *priorityQ, void *downloadQ, int seconds, int bandwidth);

#endif