/*
 *  comm.c
 *
 *  Par : Thomas Giguere Sturrock, Ylann Blouzat
 *  Created on: 15 nov. 2021
 *
 *  Programme qui communique avec un client. Le serveur interagie avec un base de donnees
 *  afin de pouvoir tirer de l'information et/ou lui apporter des modifications selon la demande
 *  du client.
 *
 */
#include "recherche.h"
#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>



void client_envoi_critere(int descripteur_socket_client, t_critere critere){

	int taille_titre;
	int taille_genre;
	int taille_categorie;
	int noctets=0;
	char null[2] = "0";

	//Lab3 comm-HLR01
	/*Le client envoie une operation de recherche au serveur ou les criteres sont inclus */
	//On envois le champ titre et sa taille au serveur
	taille_titre=strlen(get_titre(critere))+1;
	noctets=write(descripteur_socket_client, &taille_titre, sizeof(int));
	if(noctets < sizeof(int)) {
		printf("Probleme lors de l'ecriture de la taille du champ titre dans le FIFO\n");
		exit(1);
	}
	noctets=write(descripteur_socket_client,get_titre(critere),taille_titre*sizeof(char));
	if(noctets < taille_titre*sizeof(char)) {
		printf("Probleme lors de l'ecriture du champ titre dans le FIFO\n");
		exit(1);
	}

	/*
	 * Si le critere genre est present on l'envoit avec sa taille au serveur sinon on envois une valeur
	 * "null" pour indiquer au serveur qu'aucun genre a ete specifier par l'usage.
	 */
	if (get_genre(critere)){
		taille_genre=strlen(get_genre(critere))+1;
		noctets=write(descripteur_socket_client, &taille_genre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du champ genre dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_socket_client,get_genre(critere),taille_genre*sizeof(char));
		if(noctets < taille_genre*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ genre dans le FIFO\n");
			exit(1);
		}
	}
	else if (get_genre(critere) == NULL){
		set_genre(critere,null);
		taille_genre=strlen(null)+1;
		noctets=write(descripteur_socket_client, &taille_genre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture taille du champ genre null dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_socket_client,get_genre(critere),taille_genre*sizeof(char));
		if(noctets < taille_genre*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ genre nill dans le FIFO\n");
			exit(1);
		}
	}

	/*
	 * Si le critere categorie est présente on l'envoit avec sa taille au serveur sinon on envois une valeur
	 * "null" pour indiquer au serveur qu'aucune categorie a ete specifier par l'usage.
	 */
	if (get_categorie(critere)){
		taille_categorie=strlen(get_categorie(critere))+1;
		noctets=write(descripteur_socket_client, &taille_categorie, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_socket_client,get_categorie(critere),taille_categorie*sizeof(char));
		if(noctets < taille_categorie*sizeof(char)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
	}
	else if (get_categorie(critere) == NULL){
		set_categorie(critere,null);
		taille_categorie=strlen(null)+1;
		noctets=write(descripteur_socket_client, &taille_categorie, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_socket_client,get_categorie(critere),taille_categorie*sizeof(char));
		if(noctets < taille_categorie*sizeof(char)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
	}

	/*
	 * Si des annees de parution sont présentes dans les critères on les envoit au serveur sinon on envois
	 * des valeurs "null" pour indiquer au serveur qu'aucune annee de parution a ete specifier par l'usage.
	 */
	if (get_annee_parution_min(critere) != -1 && get_annee_parution_max(critere) != -1){
		int annee_min = get_annee_parution_min(critere);
		noctets=write(descripteur_socket_client, &annee_min , sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		int annee_max = get_annee_parution_max(critere);
		noctets=write(descripteur_socket_client, &annee_max, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
	}
	else{
		int annee_min = 0;
		noctets=write(descripteur_socket_client, &annee_min , sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		int annee_max = 0;
		noctets=write(descripteur_socket_client, &annee_max, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}

	}
	int flag = get_evaluation(critere);
	noctets=write(descripteur_socket_client, &flag, sizeof(int));
	if(noctets < sizeof(int)) {
		printf("Probleme lors de l'ecriture dans le FIFO\n");
		exit(1);
	}
	// Tube-HLR01 finie
	// Comm-HLR01 finie

	/*Lab3 Tube-HLR03 : On verifie le fonctionnement on affichant les champs du critere envoyer par le client*/
	printf("[*] Envoi des criteres de recherche:\n");
	printf("\t[+] Titre: %s\n", get_titre(critere));
	if (strcmp(get_genre(critere),null)!=0){
		printf("\t[+] Genre: %s\n", get_genre(critere));
	}
	if (strcmp(get_categorie(critere),null)!=0){
		printf("\t[+] Categorie: %s\n", get_categorie(critere));
	}
	if (get_annee_parution_min(critere) != -1){
		printf("\t[+] Annee_min: %i\n", get_annee_parution_min(critere));
		printf("\t[+] Annee_max: %i\n", get_annee_parution_max(critere));
	}
	/* Tube-HLR03 finie */
}

int client_recoi_resultat(int descripteur_socket_serveur ){

	char *titreR = NULL, *genreR = NULL, *categorieR = NULL, *IDr = NULL;
	int taille_titre;
	int taille_genre;
	int taille_categorie;
	int nb_titre;

	int annee_parution =-1;
	int noctets = 0;

	//Lab3 Tube-HLR04
	/*reception des resultats envoyer par le serveur puis on les affiche */
	printf("[*] Reception des resultats\n");
	//On fait la lecture du nombre de resultats reçus
	noctets = read(descripteur_socket_serveur, &nb_titre, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture du nombre de titres venant du serveur\n");
		exit(1);
	}
	//On affiche le nombre de résultats reçus
	printf("\t[+] %d résultats reçus\n ", nb_titre);


	/*
	 * Pour chaque resultats reçus on affiche ses champs a l'usager
	 */

	//Lab3 comm-HLR04
	/*Le client est capable de recuperer les resultats de recherche envoyes par le serveur
	 *et de les afficher a l<utilisateur*/
	int i=0;//variable servant comme compteur
	while(i<nb_titre){

		printf("\t[%i] ",i+1);

		//Lecture et affichage du champ ID
		noctets=read(descripteur_socket_serveur, &taille_genre, sizeof(int));
		if(noctets == sizeof(int)) {
			IDr= malloc(taille_genre*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ ID du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_socket_serveur, IDr,taille_genre*sizeof(char));
		if(noctets != taille_genre*sizeof(char)) {
			printf("Erreur lors de la lecture du champ ID du FIFO\n");
			exit(1);
		}
		printf("%s\t",IDr);

		//Lecture et affichage du champ categorie
		noctets = read(descripteur_socket_serveur, &taille_categorie, sizeof(int));
		if(noctets == sizeof(int)) {
			categorieR = malloc(taille_categorie*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ categorie du FIFO\n");
			exit(1);
		}
		noctets = read(descripteur_socket_serveur, categorieR ,taille_categorie*sizeof(char));
		if(noctets != taille_categorie*sizeof(char)) {
			printf("Erreur lors de la lecture du champ categorie du FIFO\n");
			exit(1);
		}
		printf("%s\t",categorieR);

		//Lecture et affichage du champ titre
		noctets = read(descripteur_socket_serveur, &taille_titre, sizeof(int));
		if(noctets == sizeof(int)) {
			titreR = malloc(taille_titre*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ titre du FIFO\n");
			exit(1);
		}
		noctets = read(descripteur_socket_serveur,titreR,taille_titre*sizeof(char));
		if(noctets != taille_titre*sizeof(char)) {
			printf("Erreur lors de la lecture du champ titre du FIFO\n");
			exit(1);
		}
		printf("%s\t",titreR);

		//Lecture et affichage de l'annee de parution
		noctets = read(descripteur_socket_serveur, &annee_parution, sizeof(int));
		if(noctets != sizeof(int)) {
			printf("Erreur lors de la lecture de l'annee de parution du FIFO\n");
			exit(1);
		}else{
		printf("%i\t",annee_parution);
		}
		//Lecture et affichage du champ genre
		noctets=read(descripteur_socket_serveur, &taille_genre, sizeof(int));
		if(noctets == sizeof(int)) {
			genreR = malloc(taille_genre*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ genre du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_socket_serveur, genreR,taille_genre*sizeof(char));
		if(noctets != taille_genre*sizeof(char)) {
			printf("Erreur lors de la lecture du champ genre du FIFO\n");
			exit(1);
		}
		printf("%s\n",genreR);


		free(IDr);
		free(categorieR);
		free(titreR);
		free(genreR);
		i++;
	}
	return nb_titre;
    // Tube-HLR04 finie
	// comm-HLR04 finie

}

void client_envoi_titre(int descripteur_socket_client, int nb_titre){

	int num_titre;
	int noctets = 0;
	printf("[-] Veuillez choisir un titre a evaluer\n  >  ");
	scanf("%i",&num_titre);

	//On verifie que le numero de titre specifier par l'usager est valide
	while( num_titre<1 || num_titre>nb_titre ){
		printf("[!] Veuillez choisir une valeur de titre valide\n  >  ");
		scanf("%i",&num_titre);
	}
	//Lab3 comm-HLR06
	/* Le client envoie au serveur le titre à évaluer parmi les résultats reçus. */
	noctets=write(descripteur_socket_client, &num_titre, sizeof(int));
	if(noctets < sizeof(int)) {
		printf("Probleme lors de l'ecriture du numero de titre dans le FIFO\n");
		exit(1);
	}
	//comm-HLR05 finie
}

void client_recoi_cote(int descripteur_socket_serveur){

	int taille_ID_eval;
	char* ID_eval;
	int taille_cote_eval;
	char* cote_eval;
	int nb_vote_eval;
	float note;

	int noctets = 0;

	//Lab 3 - comm HLR09
	/* On recoit et affiche les informations du titre a evaluer */
	noctets=read(descripteur_socket_serveur,&taille_ID_eval,sizeof(int));
	if(noctets == sizeof(int)) {
		ID_eval = malloc(taille_ID_eval*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture de la taille du champ ID du FIFO\n");
		exit(1);
	}
	noctets=read(descripteur_socket_serveur, ID_eval,taille_ID_eval*sizeof(char));
	if(noctets != taille_ID_eval*sizeof(char)) {
		printf("Erreur lors de la lecture du champ ID du FIFO\n");
		exit(1);
	}
	printf("\t- Titre: %s\n",ID_eval);

	noctets=read(descripteur_socket_serveur,&taille_cote_eval,sizeof(int));
	if(noctets == sizeof(int)) {
		cote_eval = malloc(taille_cote_eval*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture de la taille du champ cote du FIFO\n");
		exit(1);
	}
	noctets=read(descripteur_socket_serveur, cote_eval,taille_cote_eval*sizeof(char));
	if(noctets != taille_cote_eval*sizeof(char)) {
		printf("Erreur lors de la lecture du champ cote du FIFO\n");
		exit(1);
	}
	printf("\t- Cote moyenne: %s /10 \n",cote_eval);

	noctets = read(descripteur_socket_serveur, &nb_vote_eval, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture du nombre de vote du FIFO\n");
		exit(1);
	}
	printf("\t- Nombre de votes : %i\n\n",nb_vote_eval);
	//comm-HLR09 finie

	free(ID_eval);
	free(cote_eval);
}

void client_envoi_cote(int descripteur_socket_client){

	int noctets = 0;
	float note;

	//Lab 3 comm-HLR10
	/* On demande la note et on l'envoie au serveur */
	printf("[-] Quelle note sur 10 donnez-vous ?\n  >  ");
	scanf("%f",&note);

	//Verifie que la note se situe entre 0 et 10
	while(note<0 || note>10){
		printf("[!]Vous devez choisir une note sur 10 \n  >  ");
		scanf("%f",&note);
	}
	//On envoi la note au serveur
	noctets=write(descripteur_socket_client, &note, sizeof(float));
	if(noctets < sizeof(float)) {
		printf("Probleme lors de l'ecriture de la note dans le FIFO\n");
		exit(1);
	    //comm-HLR10 finie
	}
}

void client_recoit_nouvelle_cote(int descripteur_socket_serveur){

	int noctets = 0;
    int taille_ID;
    int taille_cote;
    int nb_vote;
    char* ID;
	char* nouvelle_cote;

	//Lab 3 comm-HLR13
	/* On recoit et affiche les nouvelles informations*/

	noctets=read(descripteur_socket_serveur,&taille_ID,sizeof(int));
	if(noctets == sizeof(int)) {
		ID = malloc(taille_ID*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture de la taille du champ ID du FIFO\n");
		exit(1);
	}
	noctets=read(descripteur_socket_serveur,ID,taille_ID*sizeof(char));
	if(noctets != taille_ID*sizeof(char)) {
		printf("Erreur lors de la lecture du champ ID du FIFO\n");
		exit(1);
	}
	printf("\t- Titre: %s\n",ID);

	noctets=read(descripteur_socket_serveur,&taille_cote,sizeof(int));
	if(noctets == sizeof(int)) {
		nouvelle_cote = malloc(taille_cote*sizeof(char));
	}
	else {
		printf("Erreur lors de la lecture de la taille du champ cote du FIFO\n");
		exit(1);
	}
	noctets=read(descripteur_socket_serveur, nouvelle_cote,taille_cote*sizeof(char));
	if(noctets != taille_cote*sizeof(char)) {
		printf("Erreur lors de la lecture du champ cote du FIFO\n");
		exit(1);
	}
	printf("\t- Nouvelle cote moyenne: %s /10 \n",nouvelle_cote);

	noctets = read(descripteur_socket_serveur, &nb_vote, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture du nombre de vote du FIFO\n");
		exit(1);
	}
	printf("\t- Nouveau nombre de votes: %i \n",nb_vote);
	//comm-HLR13 finie
	free(ID);
	free(nouvelle_cote);
}

