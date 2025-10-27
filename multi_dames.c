#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Les structures :

#define TAILLE 8 // ---> a changer en demandant a l'utilisateur
typedef struct {
    // 0 si vide, 1 si blanc, 2 si rouge et 3 si noir
    int pion[TAILLE][TAILLE];
} Plateau;

typedef struct {
    int etat; // 1 si dans la partie, 0 sinon
    int score;
} Joueur;

#define MAX_JOUEURS 4 // ---> a changer en demandant a l'utilisateur
typedef struct {
    Plateau plateau;
    Joueur joueur[MAX_JOUEURS];
    int nb_joueurs;
    int joueur_courant;
    int tour;
    int pion_est_saisi; // 1 si un pion est saisi, 0 sinon
    int pion_i, pion_j; // la ligne et colonne du pion saisi (0 sinon)
} Jeu;

// Les fonctions :

int jeu_capturer(Jeu *jeu, int i, int j){ // assurer que i j capturable
    if ((*jeu).plateau.pion[i][j] == 1) {
        (*jeu).joueur[(*jeu).joueur_courant].score  += 1;
    } else if ((*jeu).plateau.pion[i][j] == 2) {
        (*jeu).joueur[(*jeu).joueur_courant].score  += 5;
    } else if ((*jeu).plateau.pion[i][j] == 3) {
        (*jeu).joueur[(*jeu).joueur_courant].score  += 8;
    } else {
        return 0;
    }
    (*jeu).plateau.pion[i][j] = 0;
    return 1;
}

int jeu_saisir_pion(Jeu *jeu, int i, int j){
    (*jeu).pion_est_saisi = 1;
    (*jeu).pion_i = i;
    (*jeu).pion_j = j;
    return 1;
}

int jeu_sauter_vers(Jeu *jeu, int i, int j){ // assurer qu'on peu bien sauter en i j
    int cap_i = ((*jeu).pion_i + i)/2;
    int cap_j = ((*jeu).pion_j + j)/2;
    jeu_capturer(jeu, cap_i, cap_j);
    (*jeu).pion_i = i;
    (*jeu).pion_j = j;
    return 1;
}

int jeu_arreter(Jeu *jeu){
    int nb_j = 0;
    for (int i = 0; i < (*jeu).nb_joueurs; i++){
        if ((*jeu).joueur[i].etat == 1){
            nb_j += 1;
        }
    }
    if (nb_j > 1){
        (*jeu).joueur[(*jeu).joueur_courant].etat = 0;
        return 1;
    }
    return 0;
}

int jeu_joueur_suivant(Jeu *jeu){
    do {
        (*jeu).joueur_courant = (*jeu).joueur_courant % (*jeu).nb_joueurs + 1;
    } while ((*jeu).joueur[(*jeu).joueur_courant].etat == 0);
    (*jeu).pion_est_saisi = 0;
    (*jeu).pion_i = 0;
    (*jeu).pion_j = 0;
    return 1;
}

void jeu_charger(Jeu *jeu) {
    printf("Score:\n    J1  J2  J3  J4\n    %d   %d   %d   %d\nTour: %d\nJoueur %d\nPlateau:\n", (*jeu).joueur[0].score, (*jeu).joueur[1].score, (*jeu).joueur[2].score, (*jeu).joueur[3].score, (*jeu).tour, (*jeu).joueur_courant + 1);
    for (int i = 0; i < TAILLE + 1; i++){
        for (int j = 0; j < TAILLE + 1; j++){
            if ((i == 0 || i == 1) && j == 0)
                printf("  ");
            else if (i == 0)
                printf(" %d", j);
            else if (i == 1)
                printf("--");
            else if (j == 0)
                printf("%d ", i);
            else if (j == 1)
                printf("|%d", (*jeu).plateau.pion[i-1][j-1]);
            else
                printf(" %d", (*jeu).plateau.pion[i-1][j-1]);
        }
        printf("\n");
    }
}

void jeu_ecrire(Jeu *jeu){
    
}

int plateau_pion_peut_sauter(Plateau *plateau, int i, int j){
    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1}, dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int i = 0; i < 8; i++){
        if ((*plateau).pion[i+dx[i]][j+dy[i]] != 0 && (*plateau).pion[i+2*dx[i]][j+2*dy[i]] == 0)
            return 1;
    }
    return 0;
}

int case_est_valide(int i, int j){

}

void jeu_afficher(Jeu *jeu){

}

void jeu_table_flip(Jeu *jeu){

}

int init_plateau(Plateau *plateau){
    srand(time(NULL));
    int pion_dispo[] = {34, 20, 10};
    int pion_couleur[] = {1, 2, 3};
    int n;
    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){
            do {
                n = rand() % 3;
            } while (pion_dispo[n] == 0);
            pion_dispo[n] -= 1;
            (*plateau).pion[i][j] = pion_couleur[n];
        }
    }
    return 1;
}

int fin_de_partie(Plateau *plateau){
    for (int i=0; i<TAILLE; i++){
        for (int j=0; j<TAILLE; j++){
            if (!plateau_pion_peut_sauter(plateau, i, j)){
                return 1;
            }
        }
    }
    return 0;
}

int main(void) {
    Jeu jeu;
    init_plateau(&jeu.plateau);
    printf("Combien de joueur :");
    scanf("%d", &jeu.nb_joueurs);
    for (int i = 0; i < jeu.nb_joueurs; i++){
        jeu.joueur[i].etat = 1;
        jeu.joueur[i].score = 0;
    }
    jeu.tour = 0;
    jeu.joueur_courant = 0;
    do{
        jeu_charger(&jeu);
    } while (!fin_de_partie);
    return 0;
}
