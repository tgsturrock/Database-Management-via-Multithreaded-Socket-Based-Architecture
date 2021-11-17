/*
 *  serveur.c
 *
 *  Par : Thomas Giguere Sturrock, Ylann Blouzat
 *  Created on: 25 oct. 2021
 *
 *  Programme qui communique avec un client. Le serveur interagie avec un base de donnees
 *  afin de pouvoir tirer de l'information et/ou lui apporter des modifications selon la demande
 *  du client.
 *
 */

/*
 * Lab3 Serveur-HLR01
 * La portion du programme responsable de l'interatction avec la base de donnees
 * da ete decouple a partir de implementation du laboratoire #2.
 *
 * Serveur-HLR01 finie
 */

#include "recherche.h"
#include "imdb.h"
#include "resultat.h"
#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_CLIENT_LECTURE "/tmp/fifo1"
#define FIFO_SERVEUR_ECRITURE "/tmp/fifo2"


int main(int argc, char *argv[]) {
	char *titre = NULL, *genre = NULL, *categorie = NULL;
	int descripteur_fifo_client_lecture;
	int descripteur_fifo_serveur_ecriture;
	int noctets=0;
	int erreur=0;

	char null[2]="0";

	int annee_parution_min;
	int annee_parution_max;
	int taille_titre;
	int taille_genre;
	int taille_categorie;
	int taille_ID;

	int flag;
	int num_titre;
	int taille_cote;
	int vote;
	float cote;

	//Demarage du serveur
	serveur_demarage();

	//Ouverture des fifos du cote serveur
	descripteur_fifo_client_lecture = open(FIFO_CLIENT_LECTURE, O_RDONLY);
	descripteur_fifo_serveur_ecriture = open(FIFO_SERVEUR_ECRITURE, O_WRONLY);

	//On affiche que la connection est etablie
	printf("[!] Connexion avec le client établie\n");

	//Cree une structure critere pour enmagasiner les criteres de recherche
	t_critere critere = creer_critere();

	//Le serveur recoit les critere de recherche envoye par le client
	serveur_recoit_critere(descripteur_fifo_client_lecture,critere);
	/* Tube-HLR03 finie */

	//Recherche dans la base de donnee pour des titre qui concorde avec les criteres de recherche
	printf("[*] Exploration de la base de donnees\n");
	t_resultat resultat = lecture(critere);

	//Ajout des cotes de moyenne et nombre de vote aux resultats
	lecture_cote(resultat);
	//Lab2-HLR24 finie

	//Affichage des resultats cote serveur
	printf("[*] Visualisation des résultats\n");
	fichier_resultat(resultat);
	//HLR25 finie


	serveur_envoit_resultat(descripteur_fifo_serveur_ecriture, resultat);

	//Lab3 comm-HLR07
	/*Si le champ relié à l'argument -v a bien été reçu lors du requis Comm-HLR02,
	 *le serveur est capable de récupérer le titre à évaluer par le client.*/
		if(get_evaluation(critere) == 1){
		noctets = read(descripteur_fifo_client_lecture, &num_titre, sizeof(int));
		if(noctets != sizeof(int)) {
			printf("Erreur lors de la lecture du numero de titre a evaluer \n");
			exit(1);
		}
	//comm-HLR07 finie

		//Lab3 comm-HLR08
		/*Le serveur cherche les données de classement du titre à évaluer et les envoie au client.*/
		t_titre titre_chercher;
		printf("[*] Titre a evaluer:\n");
		titre_chercher = print_titre(resultat,(num_titre-1));

		serveur_envoi_cote(descripteur_fifo_serveur_ecriture, titre_chercher);
		//comm-HLR08 finie


		//Lab3 comm-HLR11
		/*Le serveur est capable de recevoir la note sur 10 du client
		 *et de calculer la nouvelle cote de classement du titre évalué.*/
		noctets = read(descripteur_fifo_client_lecture, &cote , sizeof(float));
		if(noctets < sizeof(float)) {
			printf("Probleme lors de la lecture de la cote envoyer par l'usager dans le FIFO\n");
			exit(1);
		}
		fichier_cote(titre_chercher, cote);
		//comm-HLR11 finie

		//Lab3 comm-HLR12
		/*Le serveur cherche les nouvelles données de classement du titre évalué et les envoie au client.*/
		//On envoit le champ cote et sa taille au client
		taille_cote= strlen(get_moyenne(titre_chercher))+1;
		noctets = write(descripteur_fifo_serveur_ecriture, &taille_cote, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du nouveau champ cote moyenne dans le FIFO\n");
			exit(1);
		}
		noctets = write(descripteur_fifo_serveur_ecriture,get_moyenne(titre_chercher),taille_cote*sizeof(char));
		if(noctets < taille_cote*sizeof(char)) {
			printf("Probleme lors de l'ecriture du nouveau champ cote moyenne dans le FIFO\n");
			exit(1);
		}
		//On envoi le nombre de vote
		vote = get_vote(titre_chercher);
		noctets = write(descripteur_fifo_serveur_ecriture, &vote , sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture du nouveau nombre de vote dans le FIFO\n");
			exit(1);
		}
		//comm-HLR12 finie

	}
	//Tube-HLR04 finie
	//comm-HLR03 finie

	//Lab2-HLR26
	/*
	 * Toute la memoire alloue est desallouee avant de quitter le progamme.
	 */
	for (int i = 0; i < get_nb_titre(resultat); i++) {
		if (get_titre_r(resultat, i) != NULL) {
			detruire_titre(get_titre_r(resultat, i));
		}
	}
	//libere la memoire
	detruire_resultat(resultat);
	detruire_critere(critere);

	free(titre);
	free(genre);
	free(categorie);

	close(descripteur_fifo_serveur_ecriture);
	close(descripteur_fifo_client_lecture);
	unlink(FIFO_SERVEUR_ECRITURE);
	unlink(FIFO_CLIENT_LECTURE);
	printf("[-] Fermeture du serveur.\n");
	return 0;
	}
//HLR26 finie

//HLR26 finie

