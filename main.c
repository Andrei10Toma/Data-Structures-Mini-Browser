/*TOMA Andrei-311CB*/
#include "structures.h"

// obtine bandwidth
int get_bandwidth(char *cmd)
{
	char *r = strchr(cmd, ' ');
	r = r + 1;
	return atoi(r);
}

// obitne indexul tabului pentru a schimba tabul curent
int get_index_tab(char *cmd)
{
	char *index = strchr(cmd, ' ');
	index = index + 1;
	return atoi(index);
}

// obtine url pentru comanda goto
char *get_url_goto (char *cmd)
{
	char *url = strchr(cmd, ' ');
	url = url + 1;
	return url;
}

// obitne numarul de intrari pentru a fi sterse din istoricul global
int entries_history_delete (char *cmd)
{
	char *entries = strchr(cmd, ' ');
	entries = entries + 1;
	return atoi(entries);
}

// obtine indexul resursei care urmeaza sa fie descarcata
int index_download_resource(char *cmd)
{
	char *index = strchr(cmd, ' ');
	index = index + 1;
	return atoi(index);
}

// obtine numarul de secunde pentru comanda wait
int get_seconds(char *cmd)
{
	char *seconds = strchr(cmd, ' ');
	seconds = seconds + 1;
	return atoi(seconds);
}

int main(int argc, char *argv[])
{
	FILE *in, *out;
	char cmd[LINE_LENGTH]; int bandwidth = 1024;
	in=fopen(argv[argc-2], "rt"); //deschidere fisier pentru citire
	if (in == NULL) return -1; //verificare deschidere fisier cu succes
	out=fopen(argv[argc-1], "wt"); //deschidere fisier de output
	if (out == NULL) //verificare deschidere fisier
	{
		fclose(in);
		return -1;
	}
	//coada istoric global
	void *global_history = InitQ(sizeof(char) * URL_LENGTH);
	void *priorityQ = InitQ(sizeof(Resource));//coada prioritate descarcare
	void *downloadQ = InitQ(sizeof(Resource));//coada descarcari finalizate
	//initializare browser si setare tab curent
	ACelSt lt = InitBrowser(), current_tab = lt;
	//citire comenzi din in pana cand se ajunge la finalul fisierului
	while (fgets(cmd, LINE_LENGTH, in))
	{
		//interpretare comenzi si apelarea functiei respective pentru
		//fiecare comanda
		cmd[strlen(cmd) - 1] = '\0'; //eliminare '\n' de la finalul comenzii
		if (strncmp(cmd, "set_band", strlen("set_band")) == 0)
			bandwidth = get_bandwidth(cmd);
		if (strncmp(cmd, "newtab", strlen("nwetab")) == 0)
			newtab(lt, &current_tab);
		if (strncmp(cmd, "deltab", strlen("deltab")) == 0)
			deltab(lt, &current_tab, DistrugeInfo);
		if (strncmp(cmd, "change_tab", strlen("change_tab")) == 0)
			change_tab(lt, &current_tab, get_index_tab(cmd));
		if (strncmp(cmd, "print_open_tabs", strlen("print_open_tabs")) == 0)
			print_open_tabs(lt, out);
		if (strncmp(cmd, "goto", strlen("goto")) == 0)
			go_to(current_tab, get_url_goto(cmd), global_history, priorityQ,
				downloadQ, bandwidth);
		if (strncmp(cmd, "back", strlen("back")) == 0)
			back(current_tab, out);
		if (strncmp(cmd, "forward", strlen("forward")) == 0)
			forward(current_tab, out);
		if (strncmp(cmd, "history", strlen("history")) == 0)
			history(global_history, out);
		if (strncmp(cmd, "del_history", strlen("del_history")) == 0)
			delete_history(global_history, entries_history_delete(cmd));
		if (strncmp(cmd, "list_dl", strlen("list_dl")) == 0)
			list_dl(current_tab, out);
		if (strncmp(cmd, "download ", strlen("download ")) == 0)
			download(priorityQ, index_download_resource(cmd), current_tab);
		if (strncmp(cmd, "downloads", strlen("downloads")) == 0)
			downloads(priorityQ, downloadQ, out);
		if (strncmp(cmd, "wait", strlen("wait")) == 0)
			wait(priorityQ, downloadQ, get_seconds(cmd), bandwidth);
	}
	DistrQ(&global_history, free); //distruge coada de istoric global
	DistrQ(&priorityQ, free); //distruge coada de prioritate pentru descarcari
	DistrQ(&downloadQ, free); //distruge coada de descarcari finalizate
	DistrugeL(&lt, DistrugeInfo); //distrugere lista de taburi
	//inchidere fisiere
	fclose(in);
	fclose(out);
	return 0;
}