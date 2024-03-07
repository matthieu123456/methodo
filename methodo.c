#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define PIQUE 1
#define TREFLE 2
#define COEUR 3
#define CARREAU 4

#define NBCARTEFAMILLE 13
#define NBCARTEMAX 52
#define NBFAMILLE 4
#define MAXCARTEJOUEUR 10
#define MAXCARTECROUPIER 10

//structure de carte 1 famille et une valeur
typedef struct{
    int famille; //pour les familles je vais utiliser '1' pour pique, '2' pour carreau, '3' pour coeur, '4' pour trèfle.
    int value;    //pour la valeur de la carte on ira de 1 pour AS ... 10 pour 10, puis 11 pour valet, 12 dame, 13 roi.
} CARTE;

//paquet de cartes, cartes des joueurs, nb cartes du joueur.
CARTE paquet[NBCARTEMAX]; //ici je crée un tableau de 52 CARTES
CARTE croupier[MAXCARTECROUPIER];
CARTE joueur[MAXCARTEJOUEUR];

int nbpoint_croupier;
int nbpoint_joueur;
int num_carte_croupier;
int num_carte_joueur;

bool continue_game = true;

//fonction d'init du paquet
void init_paquet(void){
    //init du paquet de carte, on fait une boucle qui va venir mettre toutes les cartes dans le tableau (paquet)
    //la premiere carte est un AS de pique, la dernière un ROI de trèfle
    for(int j=0; j < NBFAMILLE; j++){
        for(int i=0; i < NBCARTEFAMILLE; i++){
                paquet[(j * NBCARTEFAMILLE) + i].famille = (j+1);
                paquet[(j * NBCARTEFAMILLE) + i].value = (i+1);
        }
    }
}

//init des cartes possédées (humm satan !) par le croupier ou le joueur.
void init_cartes(CARTE cpersonne[], int max){
    for(int i=0; i < max; i++){
        cpersonne[i].famille = -1;
        cpersonne[i].value = -1;
    }
}

//fonction de mélange du paquet
void melanger(void){
    //on définit une carte temporaire, on va juste faire un random 52 fois pour échanger des cartes aléatoirement dans le paquet.
    //certaines cartes ne vont pas bouger certainement.
    CARTE carte_tempon;
    //init du random
    srand(time(NULL));
    for(int i=0; i < NBCARTEMAX; i++){
        int c0 = rand() % NBCARTEMAX;
        int c1 = rand() % NBCARTEMAX;
        carte_tempon = paquet[c0];
        paquet[c0] = paquet[c1];
        paquet[c1] = carte_tempon;
    }
}

void donner_une_carte(CARTE cpersonne[], int num_carte){

    //ici on va tirer une carte random, mais avant de la donner on va vérifier que personne ne la possède déjà sinon c'est cheaté
    //srand(time(NULL));
    //Sleep(300); // ?? si je ne mets pas de Sleep il boucle sur le même random ? à voir

    bool carte_deja_donne = false;
    int c = rand() % NBCARTEMAX;

    //vérification des cartes du croupier
    for(int i=0; i < MAXCARTECROUPIER; i++){
        if((croupier[i].famille == paquet[c].famille) && (croupier[i].value == paquet[c].value)){
            carte_deja_donne = true;
        }
    }

    //vérification des cartes du joueur
    for(int i=0; i < MAXCARTEJOUEUR; i++){
        if((joueur[i].famille == paquet[c].famille) && (joueur[i].value == paquet[c].value)){
            carte_deja_donne = true;
        }
    }

    if(carte_deja_donne){
            donner_une_carte(cpersonne, num_carte);
    }
    else {
        cpersonne[num_carte].famille = paquet[c].famille;
        cpersonne[num_carte].value = paquet[c].value;
    }
}

int afficher_cartes_points(CARTE cpersonne[], int max){

    int points = 0;
    bool is_as = false;

    for(int i=0; i < max; i++){

        switch (cpersonne[i].value){
        case -1:
            break;
        case 1:
            printf("AS ");
            is_as = true;
            break;
        case 11:
            printf("VALET ");
            points = points + 10;
            break;
        case 12:
            printf("DAME ");
            points = points + 10;
            break;
        case 13:
            printf("ROI ");
            points = points + 10;
            break;
        default:
            printf("%d ", cpersonne[i].value);
            points = points + cpersonne[i].value;
            break;
        }

        switch (cpersonne[i].famille){
        case PIQUE:
            printf("PIQUE\n");
            break;
        case CARREAU:
            printf("CARREAU\n");
            break;
        case COEUR:
            printf("COEUR\n");
            break;
        case TREFLE:
            printf("TREFLE\n");
            break;
        default: //on fait rien
            break;
        }
    }

    //calcul des points avec les AS
    if(is_as){
        //on a déjà les points mais on ne sait pas combien d'AS on a
        //on va les compter et ajuster le score en fonction
        for(int i=0; i < max; i++){
            if(cpersonne[i].value == 1){
                if(points <= 10) points = points + 11;
                else points = points + 1;
            }
        }
    }
    return points;
}

void init_game(void){


    printf("---------------------------------------\n");
    printf("              NOUVEAU JEU              \n");
    printf("---------------------------------------\n");
    srand(time(NULL));
    //j'initialise mon paquet
    init_paquet();
    //je mélange le paquet
    melanger();
    //init des cartes du croupier et du joueur
    init_cartes(croupier, MAXCARTECROUPIER);
    init_cartes(joueur, MAXCARTEJOUEUR);

    nbpoint_croupier = 0;
    nbpoint_joueur = 0;
    num_carte_croupier = 0;
    num_carte_joueur = 0;

    donner_une_carte(croupier, num_carte_croupier);
    num_carte_croupier++;
    donner_une_carte(croupier, num_carte_croupier);
    num_carte_croupier++;

    //j'affiche les cartes et les points du croupier
    nbpoint_croupier = afficher_cartes_points(croupier, MAXCARTECROUPIER);
    printf("Points du croupier : %d\n", nbpoint_croupier);
    while(nbpoint_croupier < 17){

        donner_une_carte(croupier, num_carte_croupier);
        num_carte_croupier++;
        nbpoint_croupier = afficher_cartes_points(croupier, MAXCARTECROUPIER);
        printf("Points du croupier : %d\n", nbpoint_croupier);

    }

}

int menu(void){

    int choice;
    printf("-- Nouvelle carte     : [0]\n");
    printf("-- Compter les points : [1]\n");
    printf("-- Nouvelle partie    : [2]\n");
    printf("-- Quitter            : [3]\n");
    scanf("%d", &choice);
    return choice;

}

int main(void){

    init_game();
    int choice = 0;
    printf("\n -------------------- \n");

    while(choice == 0){

        if(nbpoint_croupier == 21){
            printf("\n -------------------- \n");
            printf("LE CROUPIER A GAGNE.\n");
            printf("\n -------------------- \n");
            continue_game = false; //on met un flag a false pour ne pas permettre de continuer de jouer.
        }
        if(nbpoint_croupier > 21){
            printf("\n -------------------- \n");
            printf("LE CROUPIER A PERDU...\n");
            printf("\n -------------------- \n");
            continue_game = false;
        }
        if(continue_game){
            if(num_carte_joueur == 0){
                donner_une_carte(joueur, num_carte_joueur);
                num_carte_joueur++;
                nbpoint_joueur = afficher_cartes_points(joueur, MAXCARTEJOUEUR);
                printf("Points du joueur : %d\n", nbpoint_joueur);
            }
        }

        switch (menu()){
        case 0:
            donner_une_carte(joueur, num_carte_joueur);
            num_carte_joueur++;
            nbpoint_joueur = afficher_cartes_points(joueur, MAXCARTEJOUEUR);
            printf("Points du joueur : %d\n", nbpoint_joueur);
            if(nbpoint_joueur > 21){
                printf("\n -------------------- \n");
                printf("\nVOUS AVEZ PERDU...\n");
                printf("\n -------------------- \n");
                continue_game = true;
                choice = 0;
                init_game();
            }
            break;
        case 1:
            printf("Points du croupier : %d\n", nbpoint_croupier);
            printf("Points du joueur : %d\n", nbpoint_joueur);
            if(nbpoint_croupier < nbpoint_joueur){
                printf("\n -------------------- \n");
                printf("VOUS AVEZ GAGNE !\n");
                printf("\n -------------------- \n");
            }
            if(nbpoint_croupier > nbpoint_joueur){
                printf("\n -------------------- \n");
                printf("VOUS AVEZ PERDU !\n");
                printf("\n -------------------- \n");
            }
            if(nbpoint_croupier == nbpoint_joueur){
                printf("\n -------------------- \n");
                printf("EGALITE !\n");
                printf("\n -------------------- \n");
            }
            continue_game = false;
            break;
        case 2:
            continue_game = true;
            choice = 0;
            init_game();
            break;
        case 3:
            continue_game = false;
            choice = -1;
            break;
        default:
            break;
        }



    }

}
