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

	/* Lab3 Tube-HLR02
	 * Ouverture des fifos, connection avec le client puis
	 * Reception du critere de recherche cote serveur */
	printf("[*] Démarrage du serveurs\n");

	unlink(FIFO_SERVEUR_ECRITURE);
	unlink(FIFO_CLIENT_LECTURE);

	//Cree les fifo d'ecriture et de lecture entre le serveur et le client
	erreur = mkfifo(FIFO_CLIENT_LECTURE , 0666);
	if(erreur != 0) {
		unlink(FIFO_SERVEUR_ECRITURE);
		unlink(FIFO_CLIENT_LECTURE);
		printf("Erreur client lors de la creation du FIFO_CRITERE_ECRITURE\n");
		exit(1);
	}
	erreur = mkfifo(FIFO_SERVEUR_ECRITURE , 0666);
	if(erreur != 0) {
		unlink(FIFO_SERVEUR_ECRITURE);
		unlink(FIFO_CLIENT_LECTURE);
		printf("Erreur lors de la creation du FIFO_RESULTAT_LECTURE\n");
		exit(1);
	}
	else{
		//Si tout fonctionne on attend la connexion avec le client
		printf("[*] En attente d'une connexion avec le clients...\n\n");
	}

	//Ouverture des fifos du cote serveur
	descripteur_fifo_client_lecture = open(FIFO_CLIENT_LECTURE, O_RDONLY);
	descripteur_fifo_serveur_ecriture = open(FIFO_SERVEUR_ECRITURE, O_WRONLY);

	//On affiche que la connection est etablie
	printf("[!] Connexion avec le client établie\n");

	//Lab3 comm-HLR02
	/* Pour une operation de recherche, le serveur est capable de recupere tous les critere de recherche
	 * envoyes par le client*/
	//Lecture du champ titre et de sa taille
	noctets = read(descripteur_fifo_client_lecture, &taille_titre, sizeof(int));
	if(noctets == sizeof(int)) {
		titre = malloc(taille_titre*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture du de la taille du champ titre\n");
		exit(1);
	}
	noctets = read(descripteur_fifo_client_lecture, titre, taille_titre*sizeof(char));
	if(noctets != taille_titre*sizeof(char)) {
		printf("Erreur lors de la lecture du champ titre\n");
		exit(1);
	}

	//Lecture du champ genre et de sa taille
	noctets = read(descripteur_fifo_client_lecture, &taille_genre, sizeof(int));
	if(noctets == sizeof(int)) {
		genre = malloc(taille_genre*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture de la taille du champ genre\n");
		exit(1);
	}
	noctets = read(descripteur_fifo_client_lecture, genre, taille_genre*sizeof(char));
	if(noctets != taille_genre*sizeof(char)) {
		printf("Erreur lors de la lecture du du champ genre\n");
		exit(1);
	}

	//Lecture du champ categorie et de sa taille
	noctets = read(descripteur_fifo_client_lecture, &taille_categorie, sizeof(int));
	if(noctets == sizeof(int)) {
		categorie = malloc(taille_categorie*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture de la taille du champ categorie\n");
		exit(1);
	}
	noctets = read(descripteur_fifo_client_lecture, categorie, taille_categorie*sizeof(char));
	if(noctets != taille_categorie*sizeof(char)) {
		printf("Erreur lors de la lecture du champ categorie\n");
		exit(1);
	}

	//Lecture des champs d'annees de parutions
	noctets = read(descripteur_fifo_client_lecture, &annee_parution_min, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture de l'annee de parution minimum\n");
		exit(1);
	}
	noctets = read(descripteur_fifo_client_lecture, &annee_parution_max, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture de l'annee de parution maximum\n");
		exit(1);
	}
	//Lecture du critere d'evaluation
	noctets = read(descripteur_fifo_client_lecture, &flag, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture de l'annee de parution maximum\n");
		exit(1);
	}
	//Tube-HLR02 finie
	//comm-HLR02 finie

	// Création de la structure critere et stockage des arguments reçus
	t_critere critere = creer_critere();
	//
	set_titre(critere, titre);

	if (strcmp(genre,null) != 0){
		set_genre(critere, genre);
	}
	if (strcmp(categorie,null) != 0){
		set_categorie(critere, categorie);
	}
	if (annee_parution_min != 0){
		set_annee_parution_min(critere, annee_parution_min);
	}
	if (annee_parution_max != 0){
		set_annee_parution_max(critere,annee_parution_max);
	}

	/* Lab3 Tube-HLR03 : On verifie le fonctionnement
	 * On affiche les valeurs des champs recu par le client.
	 */

	printf("[*] Réception des criteres de recherche:\n");
	printf("\t[+] Titre: %s\n", get_titre(critere));
	if (get_genre(critere) != NULL){
		printf("\t[+] Genre: %s\n", get_genre(critere));
	}
	if (get_categorie(critere)!= NULL){
		printf("\t[+] Categorie: %s\n", get_categorie(critere));
	}
	if(annee_parution_min == annee_parution_max && annee_parution_min != 0 && annee_parution_max != 0){
		printf("\t[+] Annee de parution: %i\n", get_annee_parution_min(critere));
	}
	else if (annee_parution_min != 0 && annee_parution_max != 0){
		printf("\t[+] Annee_min: %i\n", get_annee_parution_min(critere));
		printf("\t[+] Annee_max: %i\n", get_annee_parution_max(critere));
	}

	/* Tube-HLR03 finie */

	//recherche dans la base de donnee pour des titre qui concorde avec les criteres de recherche
	printf("[*] Exploration de la base de donnees\n");
	t_resultat resultat = lecture(critere);

	//Ajout des cotes de moyenne et nombre de vote aux resultats
	lecture_cote(resultat);
	//Lab2-HLR24 finie

	//Affichage des resultats cote serveur
	printf("[*] Visualisation des résultats\n");
	fichier_resultat(resultat);
	//HLR25 finie


	//Lab3 Tube-HLR04
	/* Le serveur envois les resultats de la recherche au client*/
	int nb_titre = 0;
	nb_titre = get_nb_titre(resultat);

	//Envoit le nombre de titres contenu dans la strcuture resultat
	noctets=write(descripteur_fifo_serveur_ecriture, &nb_titre, sizeof(int));
	if(noctets < sizeof(int)) {
		printf("Probleme lors de l'ecriture du nombre de titre dans le FIFO\n");
		exit(1);
	}

	//t_titre titre_resultat = cree_titre();
	printf("[*] Envoi des resultats\n");
	//Lab3 comm-HLR03
	/*Le serveur retourne les resultats de titres d'une recherche vers le client*/
	for(int i =0 ;i<nb_titre;i++){

		//titre_resultat = get_titre_r(resultat, i);

		//On envoit le champ ID et sa taille au client
		taille_ID = strlen(get_ID_t(get_titre_r(resultat, i)))+1;
		noctets = write(descripteur_fifo_serveur_ecriture, &taille_ID, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du champ ID dans le FIFO\n");
			exit(1);
		}
		noctets = write(descripteur_fifo_serveur_ecriture,get_ID_t(get_titre_r(resultat, i)),taille_ID*sizeof(char));
		if(noctets < taille_ID*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ ID dans le FIFO\n");
			exit(1);
		}


		//On envoit la taille du champ categorie et le champ categorie au client
		taille_categorie=strlen(get_categorie_t(get_titre_r(resultat, i)))+1;
		noctets = write(descripteur_fifo_serveur_ecriture, &taille_categorie, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille de la categorie dans le FIFO\n");
			exit(1);
		}
		noctets = write(descripteur_fifo_serveur_ecriture, get_categorie_t(get_titre_r(resultat, i)),taille_categorie*sizeof(char));
		if(noctets < taille_categorie*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ categorie dans le FIFO\n");
			exit(1);
		}

		//On envoit la taille du champ titre et le champ titre au client
		taille_titre = strlen(get_titre_t(get_titre_r(resultat, i)))+1;
		noctets = write(descripteur_fifo_serveur_ecriture, &taille_titre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du champ titre dans le FIFO\n");
			exit(1);
		}
		noctets = write(descripteur_fifo_serveur_ecriture,get_titre_t(get_titre_r(resultat, i)),taille_titre*sizeof(char));
		if(noctets < taille_titre*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ titre dans le FIFO\n");
			exit(1);
		}
		//On envoit l'annee de parution du film
		annee_parution_min = get_annee_parution_min_t(get_titre_r(resultat, i));
		noctets = write(descripteur_fifo_serveur_ecriture, &annee_parution_min , sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de l'annee de parution dans le FIFO\n");
			exit(1);
		}

		//On envoit la taille du champ genre et le champ genre au client
		taille_genre=strlen(get_genre_t(get_titre_r(resultat, i)))+1;
		noctets = write(descripteur_fifo_serveur_ecriture, &taille_genre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du champ genre dans le FIFO\n");
			exit(1);
		}
		noctets = write(descripteur_fifo_serveur_ecriture,get_genre_t(get_titre_r(resultat, i)),taille_genre*sizeof(char));
		if(noctets < taille_genre*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ genre dans le FIFO\n");
			exit(1);
		}
	}
	printf("[*] Resultats envoyes \n\n");

	//Lab3 comm-HLR07
	/*Si le champ relié à l'argument -v a bien été reçu lors du requis Comm-HLR02,
	 *le serveur est capable de récupérer le titre à évaluer par le client.*/
		if(flag == 1){
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

		//On envoit le champ ID et sa taille au client
		taille_ID = strlen(get_ID_t(titre_chercher))+1;
		noctets = write(descripteur_fifo_serveur_ecriture, &taille_ID, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du champ ID dans le FIFO\n");
			exit(1);
		}
		noctets = write(descripteur_fifo_serveur_ecriture,get_ID_t(titre_chercher),taille_ID*sizeof(char));
		if(noctets < taille_ID*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ ID dans le FIFO\n");
			exit(1);
		}

		//On envoit le champ cote et sa taille au client
		if(get_moyenne(titre_chercher)==NULL){
			taille_cote= strlen(null)+1;
			noctets = write(descripteur_fifo_serveur_ecriture, &taille_cote, sizeof(int));
			if(noctets < sizeof(int)) {
				printf("Probleme lors de l'ecriture de la taille du champ cote moyenne dans le FIFO\n");
				exit(1);
			}
			noctets = write(descripteur_fifo_serveur_ecriture,null,taille_cote*sizeof(char));
			if(noctets < taille_cote*sizeof(char)) {
				printf("Probleme lors de l'ecriture du champ cote moyenne dans le FIFO\n");
				exit(1);
			}
		}
		else{
			taille_cote= strlen(get_moyenne(titre_chercher))+1;
			noctets = write(descripteur_fifo_serveur_ecriture, &taille_cote, sizeof(int));
			if(noctets < sizeof(int)) {
				printf("Probleme lors de l'ecriture de la taille du champ cote moyenne dans le FIFO\n");
				exit(1);
			}
			noctets = write(descripteur_fifo_serveur_ecriture,get_moyenne(titre_chercher),taille_cote*sizeof(char));
			if(noctets < taille_cote*sizeof(char)) {
				printf("Probleme lors de l'ecriture du champ cote moyenne dans le FIFO\n");
				exit(1);
			}
		}

		//On envoi le nombre de vote
		if(get_vote(titre_chercher)==-1){
			vote = 0;
			noctets = write(descripteur_fifo_serveur_ecriture, &vote , sizeof(int));
			if(noctets < sizeof(int)) {
				printf("Probleme lors de l'ecriture du champ nombre de votes dans le FIFO\n");
				exit(1);
			}
		}
		else{
			vote = get_vote(titre_chercher);
			noctets = write(descripteur_fifo_serveur_ecriture, &vote , sizeof(int));
			if(noctets < sizeof(int)) {
				printf("Probleme lors de l'ecriture du champ nombre de votes dans le FIFO\n");
				exit(1);
			}
		}
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

