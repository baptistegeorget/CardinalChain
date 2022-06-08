#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>

int main(void);
void start_level(int *level);

void vider_buffer(void) {
    int c;
    while((c=getchar()) != EOF && c != '\n');
}

void color(int t,int f) {
    /*
     0 : noir      1 : bleu foncé    2 : vert           3 : bleu-gris
     4 : marron    5 : pourpre       6 : kaki           7 : gris clair
     8 : gris      9 : bleu         10 : vert fluo     11 : turquoise
    12 : rouge    13 : rose fluo    14 : jaune fluo    15 : blanc
     */
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}

typedef struct {
    int x; // position x
    int y; // position y
    int state; // état de l'objet: -1 = vide, 0 = chaine, 1 = occupe
    int value; // valeur a afficher
    int color; // couleur
    int scope; // cible: 0 = false, 1 = true
}OBJET;

OBJET build_objet(int x, int y, int state, int value, int color){
    OBJET objet;
    objet.x = x;
    objet.y = y;
    objet.state = state;
    objet.value = value;
    objet.color = color;
    objet.scope = 0;
    return objet;
}

OBJET *build_tab_objet(int row, int column, const int *tab_int) {
    int list_color[5] = {1, 2, 10, 12, 14};
    int tab_increment = 0, list_color_increment = 0, color, state;
    OBJET *tab_objet = malloc (row * column * sizeof (OBJET));
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < column; j++) {
            if(*(tab_int + (i * column) + j) == 0) {
                state = 0;
                color = list_color[list_color_increment];
                list_color_increment += 1;
            } else {
                state = -1;
                color = 15;
            }
            tab_objet[tab_increment] = build_objet(j, i, state, *(tab_int + (i * column) + j), color);
            tab_increment += 1;
        }
    }
    return tab_objet;
}

void move_droite(int column, OBJET **list_chaine, int list_chaine_size) {
    OBJET *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos+1)->value >= pos->value && pos->x < column-1 && (pos+1)->state != 1) {
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos+1);
            }
        }
        pos->scope = 0;
        (pos+1)->scope = 1;
        (pos+1)->color = pos->color;
        (pos+1)->state = 1;
    }
}

void move_gauche(OBJET **list_chaine, int list_chaine_size) {
    OBJET *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos-1)->value >= pos->value && pos->x > 0 && (pos-1)->state != 1) {
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos-1);
            }
        }
        pos->scope = 0;
        (pos-1)->scope = 1;
        (pos-1)->color = pos->color;
        (pos-1)->state = 1;
    }
}

void move_bas(int row, int column, OBJET **list_chaine, int list_chaine_size) {
    OBJET *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos+column)->value >= pos->value && pos->y < row-1 && (pos+column)->state != 1) {
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos+column);
            }
        }
        pos->scope = 0;
        (pos+column)->scope = 1;
        (pos+column)->color = pos->color;
        (pos+column)->state = 1;
    }
}

void move_haut(int column, OBJET **list_chaine, int list_chaine_size) {
    OBJET *pos;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1){
            pos = list_chaine[i];
        }
    }
    if((pos-column)->value >= pos->value && pos->y > 0 && (pos-column)->state != 1){
        for(int i = 0; i < list_chaine_size; i++) {
            if((list_chaine[i])->scope == 1){
                list_chaine[i] = (pos-column);
            }
        }
        pos->scope = 0;
        (pos-column)->scope = 1;
        (pos-column)->color = pos->color;
        (pos-column)->state = 1;
    }
}

int choose_chain(OBJET **list_chaine, int list_chaine_size) {
    int p;
    bool change = false;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1) {
            p = i;
            list_chaine[i]->scope = 0;
        }
    }
    for(int i = p+1; i < list_chaine_size && change == false; i++) {
        if(list_chaine[i]->scope == 0) {
            list_chaine[i]->scope = 1;
            change = true;
        }
    }
    if(change == false){
        for(int i = 0; i < list_chaine_size && change == false; i++) {
            if(list_chaine[i]->scope == 0) {
                list_chaine[i]->scope = 1;
                change = true;
            }
        }
    }
}

void display(OBJET *tab_objet, int row, int column) {
    system("cls");
    int count = 0, bg_color, fg_color;
    printf("\n");
    for (int i = 0; i < row; i++) {
        printf("     ");
        for (int j = 0; j < column; j++) {
            printf("  ");
            if((tab_objet + count)->scope == 1){
                bg_color = 15;
                fg_color = 0;
            }else{
                bg_color = 0;
                fg_color = (tab_objet + count)->color;
            }
            color(fg_color, bg_color);
            if((tab_objet + count)->state == 0){
                printf("x");
            }else{
                printf("%d", (tab_objet + count)->value);
            }
            color(15, 0);
            count += 1;
        }
        printf("\n");
    }
    printf("\n");
}

int win_level(OBJET *tab_objet, int row, int column) {
    for(int i = 0; i < row*column; i++) {
        if((tab_objet+i)->state == -1) {
            return 0;
        }
    }
    return 1;
}

void del_chain(OBJET *tab_objet, int row, int column, OBJET **list_chaine, int list_chaine_size) {
    OBJET **save_pointeur;
    int color;
    for(int i = 0; i < list_chaine_size; i++) {
        if(list_chaine[i]->scope == 1) {
            save_pointeur = &list_chaine[i];
            list_chaine[i]->scope = 0;
            color = list_chaine[i]->color;
        }
    }
    for(int i = 0; i < row*column; i++) {
        if(color == (tab_objet+i)->color && (tab_objet+i)->state != 0) {
            (tab_objet+i)->state = -1;
            (tab_objet+i)->color = 15;
        }else if(color == (tab_objet+i)->color && (tab_objet+i)->state == 0) {
            (tab_objet+i)->scope = 1;
            *save_pointeur = (tab_objet+i);
        }
    }
}

void del_all_chain(int scope_size, OBJET *tab_objet, int row, int column, OBJET **list_chaine, int list_chaine_size) {
    for(int i = 0; i < scope_size; i++) {
        del_chain(tab_objet, row, column, list_chaine, list_chaine_size);
        choose_chain(list_chaine, list_chaine_size);
    }
}

void turn(OBJET *tab_objet, int row, int column, OBJET **list_chaine, int list_chaine_size, int win, int scope_size, FILE *fichier, int *level_actuel) {
    char action;
    display(tab_objet, row, column);
    printf("     Actions :\n\n     - (C) Choisir une chaine\n     - (R) Supprimer la chaine\n     - (A) Recommencer le niveau\n     - (Z) Haut\n     - (S) Bas\n     - (Q) Gauche\n     - (D) Droite\n     - (P) Choix du niveau\n\n     >>>");
    scanf("%s", &action);
    vider_buffer();
    if (action == 'C' || action == 'c') {
        choose_chain(list_chaine, list_chaine_size);
    } else if (action == 'Z' || action == 'z') {
        move_haut(column, list_chaine, list_chaine_size);
    } else if (action == 'S' || action == 's') {
        move_bas(row, column, list_chaine, list_chaine_size);
    } else if (action == 'Q' || action == 'q') {
        move_gauche(list_chaine, list_chaine_size);
    } else if (action == 'D' || action == 'd') {
        move_droite(column, list_chaine, list_chaine_size);
    } else if(action == 'R' || action == 'r') {
        del_chain(tab_objet, row, column, list_chaine, list_chaine_size);
    } else if(action == 'P' || action == 'p') {
        system("cls");
        main();
    } else if(action == 'A' || action == 'a') {
        del_all_chain(scope_size, tab_objet, row, column, list_chaine, list_chaine_size);
    } else {
        turn(tab_objet, row, column, list_chaine, list_chaine_size, win, scope_size, fichier, level_actuel);
    }
    win = win_level(tab_objet, row, column);
    char restart;
    if(win == 1) {
        display(tab_objet, row, column);
        printf("     Niveau termine !\n\n     - (P) Choix du niveau\n     - (S) Niveau suivant\n     - N'importe quelle autre touche pour fermer le jeu\n\n     >>>");
        scanf("%s", &restart);
        vider_buffer();
        if(restart == 'P' || restart == 'p') {
            system("cls");
            free(list_chaine);
            free(tab_objet);
            main();
        }else if(restart == 'S' || restart == 's'){
            system("cls");
            free(list_chaine);
            free(tab_objet);
            *level_actuel = *level_actuel + 1;
            if(*level_actuel == 13) {
                *level_actuel = 1;
            }
            start_level(level_actuel);
        } else {
            exit(EXIT_SUCCESS);
        }
    }
    if(win == 0) {
        turn(tab_objet, row, column, list_chaine, list_chaine_size, win, scope_size, fichier, level_actuel);
    }
}

void start_level(int *level_actuel) {

    // Récupération du niveau dans un fichier
    FILE *fichier = NULL;
    fichier = fopen("level.txt", "r");
    char chaine[1000];
    for(int i = 0; i < *level_actuel; i++) {
        fgets(chaine, 1000, fichier);
    }
    fclose(fichier);

    // Création d'un tableau avec les valeurs du niveau
    char *end;
    int row = (int) strtol(&chaine[0], &end, 10), column = (int) strtol(&chaine[2], &end, 10), count_chaine = 4;
    int *plateau = malloc (row*column * sizeof(int));
    for(int i = 0; i < row*column; i++) {
        plateau[i] = (int) strtol(&chaine[count_chaine], &end, 10);
        count_chaine += 2;
    }

    // Création du plateau de jeu
    OBJET *tab_objet;
    tab_objet = build_tab_objet(row, column, &plateau[0]);

    // Création d'un tableau contenant des pointeurs vers les chaines du niveau
    int count = 0;
    for(int i = 0; i < row*column; i++) {
        if(plateau[i] == 0) {
            count += 1;
        }
    }
    OBJET **list_chaine = malloc (count * sizeof(OBJET));
    int count_board = 0;
    for(int i = 0; i < row*column; i++) {
        if((tab_objet + i)->state == 0) {
            list_chaine[count_board] = (tab_objet+i);
            count_board += 1;
        }
    }

    // Cible la première chaine
    list_chaine[0]->scope = 1;

    // Lancement du niveau
    turn(tab_objet, row, column, list_chaine, count, 0, count, fichier, level_actuel);

    // Libération de la mémoire
    free(list_chaine);
    free(plateau);
}

int main(void) {
    int level = 1;
    int *level_actuel = &level;
    bool start = false;
    char action;
    while(start == false) {
        printf("     Niveau actuel: \"%d\"\n\n     - (Z) Niveau suivant\n     - (S) Niveau precedent\n     - (E) Start\n     - (F) Fermer le jeu\n\n     >>>", level);
        scanf("%c", &action);
        vider_buffer();
        if(action == 'S' || action == 's') {
            if(level > 1) {
                level -= 1;
            }
        }else if(action == 'Z' || action == 'z') {
            if(level < 12) {
                level += 1;
            }
        }else if(action == 'E' || action == 'e') {
            start = true;
        }else if(action == 'F' || action == 'f') {
            exit(EXIT_SUCCESS);
        }
        system("cls");
    }
    start_level(level_actuel);
}
