#include <stdio.h>

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
    if ((*jeu).nb_joueurs > 1){
        (*jeu).joueur[(*jeu).joueur_courant].etat = 0;
        (*jeu).nb_joueurs -= 1;
        return 1;
    }
    return 0;
}

int jeu_joueur_suivant(Jeu *jeu){
    (*jeu).joueur_courant = (*jeu).joueur_courant % (*jeu).nb_joueurs + 1;
    (*jeu).pion_est_saisi = 0;
    (*jeu).pion_i = 0;
    (*jeu).pion_j = 0;
    return 1;
}

void jeu_charger(Jeu *jeu) {

}

void jeu_ecrire(Jeu *jeu){

}

int plateau_pion_peut_sauter(Plateau *plateau, int i, int j){
    if ((*plateau).pion[i][j] == 0 && (*plateau).pion[][j] != 0) // wsh mais on verifie que le pion peu sauter en i j ou que le pion en i j peu sauter cest quoi ce kk
}

int case_est_valide(int i, int j){

}

void jeu_afficher(Jeu *jeu){

}

void jeu_table_flip(Jeu *jeu){

}

int main(void) {
    Jeu jeu;
    int i, j;
    jeu_charger(&jeu);
    scanf("%d%d", &i, &j);
    if (!jeu_sauter_vers(&jeu, i, j)) {
        printf("Action impossible\n");
    }
    jeu_ecrire(&jeu);
    return 0;
}
