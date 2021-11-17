/**
 * Module recherche - recherche.h
 *
 * Par : Enseignants ELE216
 * Date : Aout 2021
 *
 * Le module recherche est responsable de gérer la structure de données qui permet
 * de contenir tous les arguments de recherche reçus par le programme principal.
 * Il définit le type t_critere qui contient les critères de recherches.
 */

#ifndef RECHERCHE_H
#define RECHERCHE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * Encapsulation de la structure critere
 * Les champs sont accessibles seulement à l'intérieur du module recherche.c
 */

typedef struct critere* t_critere;

/**
 * Constructeur de critères
 *
 * Cette fonction publique permet d'allouer dynamiquement une structure de critères.
 * Elle place des valeurs par défaut dans les membres de celle-ci.
 *
 * Retour : pointeur vers la structure allouée
 */
t_critere creer_critere(void);

/**
 * Destructeur de critere
 *
 * Cette fonction publique permet de libérer une structure critere de la mémoire
 *
 * critere : la structure à libérer
 */
void detruire_critere(t_critere critere);


/* MUTATEURS DES CRITÈRES */

/**
 * Mutateur de titre
 *
 * Permet d'ajouter une donnée de titre à la structure critere
 *
 * critere : la structure à modifier
 * titre : chaine de caractères constituant le titre à ajouter
 */
void set_titre(t_critere critere, char *titre);

/**
 * Mutateur de genre
 *
 * Permet d'ajouter une donnée de genre à la structure critere
 *
 * critere : la structure à modifier
 * genre : chaine de caractères constituant le genre à ajouter
 */
void set_genre(t_critere critere, char *genre);

/**
 * Mutateur de catégorie
 *
 * Permet d'ajouter une donnée de catégorie à la structure critere
 *
 * critere : la structure à modifier
 * catégorie : chaine de caractères constituant la catégorie à ajouter
 */
void set_categorie(t_critere critere, char *categorie);

/**
 * Mutateur d'intervalle d'années
 *
 * Permet de reconstituer l'intervalle d'années donnes en argument reçu par
 * le programme principal et de placer celle-ci dans les champs de la structure
 * de critères.
 *
 * L'intervalle doit être spécifié par le caractère ":". Il est aussi possible de donner
 * une année singulière.
 *
 * Exemple d'intervalle d'années : 
 *  annees -> "1990:2000"
 *  Alors,
 *  annee_parution_min -> 1990
 *  anne_parution_max -> 2000
 *
 * Exemple d'année singulière
 *  annees -> "1999"
 *  Alors,
 *  annee_parution_min -> 1999
 *  annee_parution_max -> 1999
 *
 * critere : la structure à modifier
 * annees : chaine de caractères constituant l'année ou l'intervalle d'années
 *          à ajouter
 */
void set_intervalle_annees(t_critere critere, char *annees);

/**
 * Mutateurs d'années de parution
 *
 * Permet d'ajouter les années de parution à la structure critere
 *
 * critere : la structure à modifier
 * année : l'année à ajouter
 *
 * set_annee_parution_min : début de l'intervalle
 * set_annee_parution_max : fin de l'intervalle
 */
void set_annee_parution_min(t_critere critere, int annee);
void set_annee_parution_max(t_critere critere, int annee);

/**
 * Mutateurs du critere d'evaluation
 *
 * Permet d'ajouter le critere d'evaluation à la structure critere
 *
 * critere : la structure à modifier
 * evaluation: critere d'evalluation
 */
void set_evaluation(t_critere critere, int evaluation);

/* OBSERVATEURS DES CRITÈRES */

/**
 * Observateur de titre
 *
 * Permet d'obtenir le titre de la structure critere
 *
 * critere : la structure à observer
 *
 * Retour : chaine de caractères constituant le titre de la structure donnée
 *          en paramètre
 */
char* get_titre(t_critere critere);

/**
 * Observateur de genre
 *
 * Permet d'obtenir le genre de la structure critere
 *
 * critere : la structure à observer
 *
 * Retour : chaine de caractères constituant le genre de la structure donnée
 *          en paramètre
 */
char* get_genre(t_critere critere);

/**
 * Observateur de catégorie
 *
 * Permet d'obtenir la catégorie de la structure critere
 *
 * critere : la structure à observer
 *
 * Retour : chaine de caractères constituant la catégorie de la structure donnée
 *          en paramètre
 */
char* get_categorie(t_critere critere);

/**
 * Observateurs d'années de parution
 *
 * Permet d'obtenir les années de parution de la structure critere
 *
 * critere : la structure à observer
 *
 * Retour : l'année constituant le début ou la fin de l'intervalle de la
 *          structure donnée en paramètre
 *
 *          get_annee_parution_min : début de l'intervalle
 *          get_annee_parution_max : fin de l'intervalle
 */
int get_annee_parution_min(t_critere critere);
int get_annee_parution_max(t_critere critere);

/**
 * Observateur du critere d'evaluation
 *
 * Permet d'obtenir le critere d'evaluation de la structure critere
 *
 * critere : la structure à observer
 *
 * Retour : Entier reprensantant le critere d'evaluation
 *
 */
int get_evaluation (t_critere critere);
#endif
