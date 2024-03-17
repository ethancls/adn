#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct
{
    bool compl ;
    int taille;  // taille du brin
    int colonne; // colonne où le premier caractere est placé
    char bases[6];
} brin_t;

typedef struct // Structure de l'adn avec n brins composés de i caracteres positionné colonne j
{
    brin_t brin[10];
    int score;   // Score de la séquence donnée par la fonction d'évaluation
    double life; // Probabilité de survie de la solution
    char S[20];  // Solutions à tester
} adn_t;

typedef struct // Structure de toutes les séquences, seq[0] contient les données du fichier
{
    adn_t *seq;
    char file[64];
    char output[64];
    int nb_brins;
    int adn_size;
    int IOTA;
    int ERA;
    int POP;
    int SOL;
} data_t;
