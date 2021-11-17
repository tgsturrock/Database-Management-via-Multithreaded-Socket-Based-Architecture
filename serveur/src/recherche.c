#include "recherche.h"

/**
 * Structure de données qui permet de contenir tous les arguments
 * reçus dans le programme principal. Ce sont les critères de recherche.
 *
 * titre: titre recherche (primaryTitle)
 * categorie: type du titre (titleType)
 * genre: genre du titre (genres)
 * annee_parution_min: début de l'intervalle d'années de parution (startYear)
 * annee_parution_max: fin de l'intervalle d'années de parution (startYear)
 */
struct critere {
    char *titre;
    char *categorie;
    char *genre;
    //Lab3-HLR04
    /*
     * Noveau membre qui permet de savoir si l<utilisateur a fait la demande
     * d'evaluation de titre
     */
    int evaluation;

    int annee_parution_min;
    int annee_parution_max;
};
// Constructeur
t_critere creer_critere(void) {
    t_critere critere;

    critere = (t_critere) malloc(sizeof(struct critere));
    assert(critere != NULL);
    
    if (critere) {
        critere->titre = NULL;
        critere->categorie = NULL;
        critere->genre = NULL;
      //HLR04 continuite
        critere->evaluation = -1;
      //HLR04 finie
        critere->annee_parution_min = -1;
        critere->annee_parution_max = -1;
    }
    
    return critere;
}

// Destructeur
void detruire_critere(t_critere critere) {
    if(critere->categorie!=NULL){
    	free(critere->categorie);
    }
    if(critere->titre!=NULL){
    	free(critere->titre);
    }
    if(critere->genre!=NULL){
    	free(critere->genre);
    }
	free(critere);
}

// Mutateurs
void set_titre(t_critere critere, char *titre) {
    critere->titre = titre;
}

void set_genre(t_critere critere, char *genre) {
    critere->genre = genre;
}

void set_categorie(t_critere critere, char *categorie) {
    critere->categorie = categorie;
}

void set_annee_parution_min(t_critere critere, int annee) {
    critere->annee_parution_min = annee;
}

void set_annee_parution_max(t_critere critere, int annee) {
    critere->annee_parution_max = annee;
}

void set_intervalle_annees(t_critere critere, char *annees) {
    char *token;
    
    // Début de l'intervalle d'années
    token = strsep(&annees, ":");
    critere->annee_parution_min = atoi(token);

    // Fin de l'intervalle d'années
    token = strsep(&annees, ":");

    // Si un intervalle était donné, le token n'est pas NULL
    if (token) 
        critere->annee_parution_max = atoi(token);
    // Sinon une année singulière a été donnée. On la place quand même 
    // dans les deux champs contenant les années de parution.
    else 
        critere->annee_parution_max = critere->annee_parution_min;
}
void set_evaluation(t_critere critere, int evaluation){
	critere->evaluation=evaluation;
}
// Observateurs
char* get_titre(t_critere critere) {
    return critere->titre;
}

char* get_genre(t_critere critere) {
    return critere->genre;
}

char* get_categorie(t_critere critere) {
    return critere->categorie;
}

int get_annee_parution_min(t_critere critere) {
    return critere->annee_parution_min;
}

int get_annee_parution_max(t_critere critere) {
    return critere->annee_parution_max;
}
/*
 * Fonction publique qui permet d'observer la valeur associe a la demande d'evaluation d'un
 * titre par le client.
 */
int get_evaluation (t_critere critere){
	return critere->evaluation;
}
