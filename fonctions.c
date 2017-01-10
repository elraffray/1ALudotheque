#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fonctions.h"



void printMenu (int *choix)
{
	printf("\n");
	printf("*********                |Menu|                *********\n");
	printf("\n");
	printf("||||||||| 1 : Register a new member            |||||||||\n");
	printf("||||||||| 2 : Delete a member                  |||||||||\n");
	printf("||||||||| 3 : Register a loan                  |||||||||\n");
	printf("||||||||| 4 : Register a new theme afternoon   |||||||||\n");
	printf("||||||||| 5 : Register member to a theme aft.  |||||||||\n");
	printf("||||||||| 6 : Edit the game list               |||||||||\n");
	printf("||||||||| 7 : Edit list of afternoon's members |||||||||\n");
	printf("||||||||| 8 : Edit borrowers list              |||||||||\n");
	printf("||||||||| 9 : Exit                             |||||||||\n");
	printf("\n");
	printf("Please, make your choice :");
	scanf("%d", choix);
}

void Menu(Client **tCli, int nbc, Jeu **tJeu, int nbj)
{
	int choix;
    char tmp;
	while(1)
	{  
        system("clear");
        printMenu(&choix);
        while(choix>9)
        {
            printf("Wrong choice, please try again.\n");
            printMenu(&choix);
        }

		switch(choix)
    	{
            case 1:
                printf("\nYou chose to register a new member.\n");
                tmp = getchar();
                tCli = newClient(tCli, &nbc);
                break;
            case 2:
             	printf("\nYou chose to delete a member.\n");
                tmp = getchar();
                delClient(tCli, &nbc);

                printf("\n");
                break;
            case 3:
                printf("\nYou chose to register a loan.\n");
                printf("\n");
                break;   
            case 4:
                printf("\nYou chose to register a new theme afternoon.\n");
                printf("\n");
                break;
            case 5:
                printf("\nYou chose to register member to a theme aft.\n");
                printf("\n");
                break;
            case 6:
                printf("\nYou chose to edit the game list.\n");
                printf("\n");
                break;
            case 7:
                printf("\nYou chose to edit the list of afternoons' members.\n");
                printf("\n");
                break; 
            case 8:
                printf("\nYou chose to edit borrowers list.\n");
                printf("\n");
                break;
            case 9:
                printf("\nYou chose to exit. \t");
                printf("Glad you came, see you soon!\n");
                printf("\n");
                return; 
            default:
                printf("Wrong choice, please try again.\n");
                break; 
        }
        
        choix = 0;
        printf("\nDo you want to realize another task ? (y/n)\n");
        scanf("%c%*c", &tmp);
        if (tmp == 'n');
        {
            printf("quitting\n");
            //free(tCli);
            free(tJeu);
            return;
        }

    }
}




int subDate(Date d1, Date d2)
{
    return abs(365*(d1.an - d2.an) + 30*(d1.mois - d2.mois) + d1.jour - d2.jour);
}


Jeu readJeu(FILE *fe)
{
    Jeu game;
    fgets(game.nom, 100, fe);
    game.nom[strlen(game.nom)-1] = '\0';
    fscanf(fe, "%d %d\n", &(game.nbdisp), &(game.nbtot));
    return game;
}

Jeu **loadGameList (int *nb)
{
    FILE *fe;
    int i=0;
    Jeu **tJeu;

    fe=fopen("gamelist.don", "r");
    if (fe ==NULL){
        printf("Issue during file oppening\n");
        exit(1);
    }
    fscanf(fe, "%d\n", nb);
    tJeu=(Jeu **)malloc(*nb*sizeof(Jeu *));
    
    if (tJeu==NULL){
        printf("Issue during allocation\n");
        exit(1);
    }

    for (i=0; i < *nb-1; i++)
    {
        tJeu[i] = (Jeu *)malloc(sizeof(Jeu));
        if (tJeu[i] == NULL)
        {
            printf("error malloc tJeu[i]\n");
            exit(1);
       }

        *tJeu[i] = readJeu(fe);
    }

    fclose(fe);
    return tJeu;
}

int findJeu(Jeu **tJeu, int nb, char *nom, bool *t) //DICHOTOMIQUE VOIR COURS //TODO PROBLEME DANS LA FONCTION!
{

    int inf = 0, sup = nb-1, m;
    while (inf <= sup)
    {
        m = (sup+inf)/2;
        printf("%s\n%s\n\n", nom, tJeu[m]->nom);
        if (strcmp(nom, tJeu[m]->nom) == 0)
        {
            *t = true;
            return m;
        }
        if (strcmp(nom, tJeu[m]->nom) < 0)
            sup = m-1;
        else
            inf = m+1;

    }
    printf("not found\n");
    return inf;
}


void newEmprunt(char *nom, char *prenom, char *game, Jeu **tJeu, int nbj, Client **tCli, int nbc) 
{
    Emprunt empr;
    bool t;
    int iCli, iJeu;

    iCli = findCli(tCli, nbc, nom, prenom, &t); //on cherche le client qui veut emprunter
    if (t == false)
    {
        printf("user not found\n");
        return;
    }

    if (tCli[iCli]->paye == false)
    {
        printf("Your subscription has expired\n");
    }
    iJeu = findJeu(tJeu, nbj, game, &t); //on cherche le jeu a emprunter
    if (t == false)
    {
        printf("Game not found\n");
        return;
    }
    if (tJeu[iJeu]->nbdisp == 0) //on regarde si le jeu est disponible
    {
        printf("Ce jeu n'est plus disponible\n");
        return;
    }
    //on créé l'emprunt
    strcpy(empr.jeu.nom, tJeu[iJeu]->nom);
    empr.date.an = system("date +%Y");
    empr.date.mois = system("date +%m");
    empr.date.jour = system("date +%d");

    empr.retard = false;

    if (nbEmpr(*tCli[iCli]) == 3) //on verifie si le client a moins de 3 emprunts en cours
    {
        printf("Vous ne pouvez plus emprunter\n");
        return;
    }

    tCli[iCli]->lEmpr = insEmpr(*tCli[iCli], empr); //on insere l'emprunt
    tJeu[iJeu]->nbdisp -= 1; //ou enleve un exemplaire disponible du jeu

}

void saveEmprunt(Client **tCli, int nb)
{
    int i;
    FILE *fe;

    fe=fopen("emprunts.don", "w");
    if(fe == NULL){
        printf("Erreur ouverture fichier\n");
        exit(1);
    }                          
    
    for (i = 0; i < nb; i++)
    {
        if (tCli[i]->lEmpr != NULL)
        {
            while (tCli[i]->lEmpr != NULL)
            {
                fprintf(fe, "%s\n%s\n%d/%d/%d\n%s\n", 
                        tCli[i]->nom, tCli[i]->prenom, 
                        tCli[i]->lEmpr->empr.date.jour, tCli[i]->lEmpr->empr.date.mois, tCli[i]->lEmpr->empr.date.an,
                        tCli[i]->lEmpr->empr.jeu.nom);
                tCli[i]->lEmpr = tCli[i]->lEmpr->nxt;
            }
        }
    }

    fclose(fe);
}


Afternoon *loadAfternoon(int *nb, Client **tCli, int nbc)
{
    Afternoon *tAft;
    FILE *fe;
    int i, j, nbCli, wh;
    char *nom, *prenom;
    bool t;


    fe=fopen("afternoon.don", "r");
    if (fe ==NULL){
        printf("Issue during file oppening\n");
        exit(1);
    }


    fscanf(fe, "%d\n", nb); //Get number of afternoons
    tAft = (Afternoon *)malloc(*nb * sizeof(Afternoon));
    if (tAft == NULL)
    {
        printf("problem tAft malloc\n");
        exit(1);
    }

    for (i=0; i < *nb; i++)
    {
        fgets(tAft[i].jeu.nom, 100, fe);
        tAft[i].jeu.nom[strlen(tAft[i].jeu.nom)-1] = '\0';

        fscanf(fe, "%d/%d/%d\n", &(tAft[i].date.jour), &(tAft[i].date.mois), &(tAft[i].date.an));
        fscanf(fe, "%d %d\n", &(tAft[i].nbPtot), &nbCli); //get nbPtot & nbCli
        tAft[i].nbPdisp = tAft[i].nbPtot - nbCli;

        for (j=0; j < nbCli; j++)
        {
            fgets(nom, 20, fe);
            nom[strlen(nom)-1] = '\0';
            fgets(prenom, 20, fe);
            prenom[strlen(prenom)-1] = '\0';


            wh = findCli(tCli, nbc, nom, prenom, &t);

            insCliAft(*tCli[wh], tAft[i]);

        }
    }


    fclose(fe);
    return tAft;
}






Afternoon *newAfternoon(Afternoon *otAft, int *nba, Jeu **tJeu, int nb) //TODO Faire la fonction avec tab pointeurs
{

    Afternoon aft, *tAft;

    bool t = false;

    while (t == false)
    {
        printf("Enter the game's name: \n");
        fgets(aft.jeu.nom, 40, stdin);
        aft.jeu.nom[strlen(aft.jeu.nom)-1]='\0';
    
        findJeu(tJeu, nb, aft.jeu.nom, &t);
        if (t == true)
            break;
        printf("Game not found.\n");
     
    }

    printf("Choose the day :\n");
    scanf("%d", &(aft.date.jour));
    printf("choose the month :\n");
    scanf("%d", &(aft.date.mois));
    printf("Choose the year :\n");
    scanf("%d", &(aft.date.an));

    printf("Choose how many people can join this afternoon :\n");
    scanf("%d", &(aft.nbPtot));
    aft.nbPdisp=aft.nbPtot;

    tAft =(Afternoon *)realloc(otAft, (*nba+1)*sizeof(Afternoon));
    if (tAft == NULL)
    {
        printf("problem tAFt malloc\n");
        exit(1);
    }

    tAft[*nba] = aft;
    (*nba)++;
    return tAft;
}



void regForAfternoon(Afternoon tAft[], int nba, Client **tCli, int nbc)
{
    char *jeu, *nom, *prenom;
    bool t = false;
    int wh, i;

    while (t == false)
    {
        printf("Nom du jeu: ");
        fgets(jeu, 30, stdin);
        jeu[strlen(jeu)-1] = '\0'; //nom
        for (i = 0; i < nba; i++)
            if (strcmp(jeu, tAft[i].jeu.nom) == 0)
            { 
                t = true;
                break;
            }
    }

    t = false;
    while (t == false)
    {
        printf("Nom: ");
        fgets(nom, 30, stdin);
        nom[strlen(nom)-1] = '\0'; //nom

        printf("Prenom: ");
        fgets(prenom, 20, stdin);
        prenom[strlen(prenom)-1] = '\0'; //prenom

        wh = findCli(tCli, nbc, nom, prenom, &t); 
        if (t == true)
            break;

        printf("User not found, retry\n");
    }

    if (tAft[i].nbPdisp == 0)
    {
        printf("Booking is full\n");
        return;
    }

    tAft[i].lCli = insCliAft(*tCli[wh], tAft[i]);
    tAft[i].nbPdisp -= 1;
}


liCli insCliAft(Client cli, Afternoon aft)
{
    MaillonC *m;
    m = (MaillonC *)malloc(sizeof(Client));

    if (m == NULL)
    {
        printf("error malloc maillon emprunt\n");
        exit(1);
    }

    m->cli = cli;
    m->nxt = aft.lCli;
    return m;
}



void saveAft(Afternoon tAft[], int nb)
{
    int i;
    FILE *fe;

    fe=fopen("afternoon.don", "w");
    if(fe == NULL){
        printf("Erreur ouverture fichier\n");
        exit(1);
    }                          
    
    fprintf(fe, "%d\n", nb);

    for (i = 0; i < nb; i++)
    {
        fprintf(fe, "%s\n%d/%d/%d\n%d%d", tAft[i].jeu.nom, //header for the afternoon
                tAft[i].date.jour, tAft[i].date.mois, tAft[i].date.an,
                tAft[i].nbPtot, tAft[i].nbPdisp);

        if (tAft[i].lCli != NULL)
        {
            while (tAft[i].lCli != NULL)
            {
                fprintf(fe, "%s\n%s\n", tAft[i].lCli->cli.nom, tAft[i].lCli->cli.prenom);
                tAft[i].lCli = tAft[i].lCli->nxt;
            }
        }
    }

    fclose(fe);
}


void checkTime(Client **tCli, int nb)
{
    lEmprunt tmp;
    int i;

    Date d = {system("date +%d"), system("date +%m"), system("date +%Y")};

    for (i=0; i < nb; i++) //for each client
    {
        if (tCLi[i]->lEmpr != NULL) //if he has emprunts
        {
            tmp = tCLi[i]->lEmpr;
            while (tmp != NULL) //for each emprunt
            {
                if (subDate(d, tmp->empr.date) > 21) //if late
                {
                    tmp->empr.retard = true;
                    tCli[i]->retard = true;
                }
                tmp = tmp->nxt; //next emprunt
            }
        }

        if (subDate(d, tCli[i]->dIns) > 365) //if client outdated
            tCli[i]->paye = false; //he needs to pay again
    }

}






















