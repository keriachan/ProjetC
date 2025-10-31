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
    int nb_joueurs_actif;
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

int jeu_sauter_vers(Jeu *jeu, int i, int j){ // assurer qu'on peut bien sauter en i j
    int src_i = (*jeu).pion_i;
    int src_j = (*jeu).pion_j;
    int cap_i = ((*jeu).pion_i + i)/2;
    int cap_j = ((*jeu).pion_j + j)/2;
    jeu_capturer(jeu, cap_i, cap_j);
    int couleur_pion_src=(*jeu).plateau.pion[src_i][src_j];
    (*jeu).plateau.pion[src_i][src_j]=0;
    (*jeu).plateau.pion[i][j]=couleur_pion_src;
    (*jeu).pion_i = i;
    (*jeu).pion_j = j;
    return 1;
}

int jeu_arreter(Jeu *jeu){
    /*int nb_j = 0;
    for (int i = 0; i < (*jeu).nb_joueurs; i++){
        if ((*jeu).joueur[i].etat == 1){
            nb_j += 1;
        }
    }
    if (nb_j > 1){
        (*jeu).joueur[(*jeu).joueur_courant].etat = 0;
        return 1;
    }
    return 0;*/
    if ((*jeu).nb_joueurs_actif > 1){
        (*jeu).joueur[(*jeu).joueur_courant].etat = 0;
        (*jeu).nb_joueurs_actif -= 1;
        return 1;
    }
    return 0;
}

int jeu_joueur_suivant(Jeu *jeu){
    do {
        (*jeu).joueur_courant = ((*jeu).joueur_courant + 1) % ((*jeu).nb_joueurs+1);
    } while ((*jeu).joueur[(*jeu).joueur_courant].etat == 0);
    (*jeu).pion_est_saisi = 0;
    (*jeu).pion_i = 0;
    (*jeu).pion_j = 0;
    return 1;
}

void jeu_charger(Jeu *jeu) {
    scanf("%d %d %d", &(*jeu).nb_joueurs, &(*jeu).tour, &(*jeu).joueur_courant);
    for (int i = 0; i < (*jeu).nb_joueurs; i++) {
        scanf("%d %d", &(*jeu).joueur[i].etat, &(*jeu).joueur[i].score);
    }
    scanf("%d %d %d", &(*jeu).pion_est_saisi, &(*jeu).pion_i, &(*jeu).pion_j);
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            scanf("%d", &(*jeu).plateau.pion[i][j]);
        }
    }

    // Mise à jour du nombre de joueurs actifs (utile pour ta logique interne)
    jeu->nb_joueurs_actif = 0;
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        if (jeu->joueur[i].etat == 1)
            jeu->nb_joueurs_actif++;
    }
}

void jeu_ecrire(Jeu *jeu){
    printf("%d %d %d\n", (*jeu).nb_joueurs, (*jeu).tour, (*jeu).joueur_courant);
    for (int i = 0; i < (*jeu).nb_joueurs; i++) {
        printf("%d %d\n", (*jeu).joueur[i].etat, (*jeu).joueur[i].score);
    }
    printf("%d %d %d\n", (*jeu).pion_est_saisi, (*jeu).pion_i, (*jeu).pion_j);
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            printf("%d ", (*jeu).plateau.pion[i][j]);
        }
        printf("\n");
    }
}

int plateau_pion_peut_sauter(Plateau *plateau, int i, int j){
    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1}, dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
    if ((*plateau).pion[i][j]==0){
        return 0;
    }
    for (int dir = 0; dir < 8; dir++){
        if (i+dx[dir]>=0 && i+dx[dir]<TAILLE && j+dy[dir]>=0 && j+dy[dir]<TAILLE && i+2*dx[dir]>=0 && i+2*dx[dir]<TAILLE && j+2*dy[dir]>=0 && j+2*dy[dir]<TAILLE){
            if ((*plateau).pion[i+dx[dir]][j+dy[dir]] != 0 && (*plateau).pion[i+2*dx[dir]][j+2*dy[dir]] == 0)
                return 1;
            }
        }
    return 0;
}

void afficher_sauts_possibles_pion(Plateau *plateau, int i, int j){
    int dx[] = {0, 1, 1, 1, 0, -1, -1, -1}, dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
    printf("Coups possibles : ");
    for (int dir = 0; dir < 8; dir++){
        if (i+dx[dir]>=0 && i+dx[dir]<TAILLE && j+dy[dir]>=0 && j+dy[dir]<TAILLE && i+2*dx[dir]>=0 && i+2*dx[dir]<TAILLE && j+2*dy[dir]>=0 && j+2*dy[dir]<TAILLE){
            if ((*plateau).pion[i+dx[dir]][j+dy[dir]] != 0 && (*plateau).pion[i+2*dx[dir]][j+2*dy[dir]] == 0)
                printf(" | %d %d", i+2*dx[dir]+1, j+2*dy[dir]+1);
            }
        }
    printf("\n");
}

int case_est_valide(int i, int j){
    if (i>=1 && i<=8 && j>=1 && j<=8){
        return 1;
    } else {
        return 0;
    }
}

void jeu_afficher(Jeu *jeu){
    if ((*jeu).nb_joueurs_actif == 2){
        printf("Score:\n    J1  J2\n    %d   %d\n", (*jeu).joueur[0].score, (*jeu).joueur[1].score);
    }
    if ((*jeu).nb_joueurs_actif == 3){
        printf("Score:\n    J1  J2  J3\n    %d   %d   %d\n", (*jeu).joueur[0].score, (*jeu).joueur[1].score, (*jeu).joueur[2].score);
    }
    if ((*jeu).nb_joueurs_actif == 4){
        printf("Score:\n    J1  J2  J3  J4\n    %d   %d   %d   %d\n", (*jeu).joueur[0].score, (*jeu).joueur[1].score, (*jeu).joueur[2].score, (*jeu).joueur[3].score);
    }
    printf("Tour: %d\nJoueur %d\nPlateau:\n", (*jeu).tour, (*jeu).joueur_courant + 1);
    char pions[] = {' ', 'o', 'x', '+'};
    for (int i = 0; i < TAILLE + 2; i++){
        for (int j = 0; j < TAILLE + 1; j++){
            if ((i == 0 || i == 1) && j == 0)
                printf("   ");
            else if (i == 0)
                printf(" %d", j);
            else if (i == 1)
                printf("--");
            else if (j == 0)
                printf("%d ", i-1);
            else if (j == 1)
                printf("| %c", pions[(*jeu).plateau.pion[i-2][j-1]]);
            else
                printf(" %c", pions[(*jeu).plateau.pion[i-2][j-1]]);
        }
        printf("\n");
    }
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
            if (plateau_pion_peut_sauter(plateau, i, j)){
                return 0;
            }
        }
    }
    return 1;
}

int penalite(Plateau *plateau){
    int sum = 0;
    for (int i=0; i<TAILLE; i++){
        for (int j=0; j<TAILLE; i++){
            if ((*plateau).pion[i][j] == "o")
                sum += 1;
            else if ((*plateau).pion[i][j] == "x")
                sum += 5;
            else if ((*plateau).pion[i][j] == "+")
                sum += 8;
        }
    }
    return sum;
}

int main(void) {
    Jeu jeu;
    int i_pion = 0, j_pion = 0;
    init_plateau(&jeu.plateau);
    printf("Combien de joueur : ");
    scanf("%d", &jeu.nb_joueurs);
    jeu.nb_joueurs -= 1;
    for (int i = 0; i <= jeu.nb_joueurs; i++){
        jeu.joueur[i].etat = 1;
        jeu.joueur[i].score = 0;
    }
    jeu.nb_joueurs_actif = jeu.nb_joueurs + 1;
    jeu.tour = 0;
    jeu.joueur_courant = 0;
    jeu.pion_i = i_pion;
    jeu.pion_j = j_pion;
    jeu.pion_est_saisi = 0;
    do{
        for (int i=0; i<jeu.nb_joueurs + 1; i++){
            if (jeu.joueur[jeu.joueur_courant].etat == 1) {
                int arr;
                do {
                    printf("Arrêter ? (1=oui, 0=non) ");
                    scanf("%d", &arr);
                } while (arr!=1 && arr!=0);
                if (arr){
                    jeu.joueur[jeu.joueur_courant].etat = 0;
                    jeu.nb_joueurs_actif -=1;
                } else {
                    jeu_afficher(&jeu);
                    if (jeu.tour == 0){
                        do {
                            printf("Position du pion blanc à retirer : ");
                            scanf("%d %d", &i_pion, &j_pion);
                        } while (!case_est_valide(i_pion, j_pion) || jeu.plateau.pion[i_pion-1][j_pion-1] != 1);
                        jeu_saisir_pion(&jeu, i_pion-1, j_pion-1);
                        jeu_capturer(&jeu, jeu.pion_i, jeu.pion_j);
                    } else {
                        do {
                            printf("Position du pion sauteur : ");
                            scanf("%d %d", &i_pion, &j_pion);
                        } while (!case_est_valide(i_pion, j_pion) || jeu.plateau.pion[i_pion-1][j_pion-1] == 0);
                        jeu_saisir_pion(&jeu, i_pion-1, j_pion-1);
                        do {
                            afficher_sauts_possibles_pion(&jeu.plateau, i_pion, j_pion);
                            printf("Position du saut : ");
                            scanf("%d %d", &i_pion, &j_pion);
                        } while (!case_est_valide(i_pion, j_pion) || jeu.plateau.pion[i_pion-1][j_pion-1] != 0);
                        jeu_sauter_vers(&jeu, i_pion-1, j_pion-1);
                    }
                }
                jeu_joueur_suivant(&jeu);
            }
        }
        jeu.tour += 1;
    } while (!fin_de_partie(&jeu.plateau));
    jeu.joueur[jeu.joueur_courant].score -= penalite(&jeu.plateau);
    printf("Fin de jeu\n");
    return 0;
}
