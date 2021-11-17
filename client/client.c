/**
 * Programme Client - client.c
 *
 * Par : Thomas Giguere Sturrock, Ylann Blouzat
 * Date : 23 Octobre 2021
 *
 * Ce programme est responsable de l'interaction avec l'usager.
 * Il demande a l'usager quelle operation operation qu'il ou elle souhaite effectuer
 * pour ensuite interprete la requete au programme serveur, qui lui renvera les donnees desirer.
 * Le programme montrera alors les donnees a l'usager.
 *
 *
 */

/*
 * Lab3 client-HLR01
 * Decouplage de la partie qui s'occupe d'interagir avec l'utilisateur
 * dans le projet du laboratoire #2.
 *
 * client-HLR01 finie
 */
#include "recherche.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_CLIENT_ECRITURE "/tmp/fifo1"
#define FIFO_SERVEUR_LECTURE "/tmp/fifo2"


int main(int argc, char *argv[]) {
	char *titre = NULL, *genre = NULL, *annees = NULL, *categorie = NULL;
	int descripteur_fifo_client_ecriture;
	int descripteur_fifo_serveur_lecture;
	int noctets=0;
	int erreur=0;
	//char* null = malloc(2*sizeof(char));
	char null[2] = "0";


	int taille_titre;
	int taille_genre;
	int taille_categorie;

	char *titreR = NULL, *genreR = NULL, *categorieR = NULL, *IDr = NULL;
	int annee_parution_min_r=-1, taille_ID, taille_categorieR, taille_titreR, taille_genreR, nb_titre;

	//Lab3 client-HLR02
	/*
	 * On se sert de getopt afin de passer au travers du tableau argv pour retrouver les arguments passer en parametere
	 * afin d'associer des valeurs au chanps qui sont associees a ces arguments.
	 */
	int opt;//variable servant a passer d'un argument a l'autre.
	int flag = 0;
	while((opt = getopt(argc, argv, ":tcagv")) != -1)
	{
		switch(opt)
		{
		case 't':
		titre = argv[optind];
		optind++;
		break;
		case 'c':
			categorie = argv[optind];
			optind++;
			break;
		case 'a':
			annees = argv[optind];
			optind++;
			break;
		case 'g':
			genre = argv[optind];
			optind++;
			break;
			//Lab3-HLR03
			/*
			 * Argument -v qui permet a l'utilisateur
			 * de demander a evaluer un titre
			 */
		case 'v':
			flag = 1;
			break;
			//HLR03 finie
		case ':':
			printf ("Veuillez entree une valeur pour l'option desire\n");
			break;
		case '?':
			printf ("unknown option: %c\n", optopt);
			break;
		}
	}
	// optind is for the extra arguments
	// which are not parsed
	for( ;optind < argc; optind++){
		printf("extra arguments: %s\n", argv[optind]);
	}
	//On s<assure qu'au moins un titre est passe en parametre
	if(titre == NULL){
		printf ("Veuillez saisir un titre\n");
		return 0;
	}
	//client-HLR02 finie


	// Création de la structure critere et stockage des arguments reçus
	t_critere critere = creer_critere();
	/*
	 * On ecrit dans les champs du critere de recherche,si un argument n'a
	 * pas ete specifier par l'usage on inscrit une valeur "null" dans le champ correspondant.
	*/
	set_titre(critere, titre);
	if (categorie)
		set_categorie(critere, categorie);
	else{
		set_categorie(critere, null);
	}
	if (genre)
		set_genre(critere, genre);
	else{
		set_genre(critere, null);
	}
	if (annees)
		set_intervalle_annees(critere, annees);
	else{
		set_intervalle_annees(critere, null);
	}
	if(flag == 1)
		set_evaluation(critere, flag);

	/*Lab3 Tube-HLR01 On envoi les criteres de recherche en les ecrivant dans un FIFO*/
	//Ouverture des fifos pour communiquer avec le serveur
	descripteur_fifo_client_ecriture = open(FIFO_CLIENT_ECRITURE,O_WRONLY);
	descripteur_fifo_serveur_lecture = open(FIFO_SERVEUR_LECTURE, O_RDONLY);
	//On indique que la connection a ete etablie avec le serveur
	printf("[*] Connection etablie avec le serveur.\n");

	//Lab3 comm-HLR01
	/*Le client envoie une operation de recherche au serveur ou les criteres sont inclus */
	//On envois le champ titre et sa taille au serveur
	taille_titre=strlen(get_titre(critere))+1;
	noctets=write(descripteur_fifo_client_ecriture, &taille_titre, sizeof(int));
	if(noctets < sizeof(int)) {
		printf("Probleme lors de l'ecriture de la taille du champ titre dans le FIFO\n");
		exit(1);
	}
	noctets=write(descripteur_fifo_client_ecriture,get_titre(critere),taille_titre*sizeof(char));
	if(noctets < taille_titre*sizeof(char)) {
		printf("Probleme lors de l'ecriture du champ titre dans le FIFO\n");
		exit(1);
	}

	/*
	 * Si le critere genre est present on l'envoit avec sa taille au serveur sinon on envois une valeur
	 * "null" pour indiquer au serveur qu'aucun genre a ete specifier par l'usage.
	 */
	if (genre){
		taille_genre=strlen(get_genre(critere))+1;
		noctets=write(descripteur_fifo_client_ecriture, &taille_genre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture de la taille du champ genre dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_fifo_client_ecriture,get_genre(critere),taille_genre*sizeof(char));
		if(noctets < taille_genre*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ genre dans le FIFO\n");
			exit(1);
		}
	}
	else if (genre == NULL){
		genre = null;
		taille_genre=strlen(null)+1;
		noctets=write(descripteur_fifo_client_ecriture, &taille_genre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture taille du champ genre null dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_fifo_client_ecriture,get_genre(critere),taille_genre*sizeof(char));
		if(noctets < taille_genre*sizeof(char)) {
			printf("Probleme lors de l'ecriture du champ genre nill dans le FIFO\n");
			exit(1);
		}
	}

	/*
	 * Si le critere categorie est présente on l'envoit avec sa taille au serveur sinon on envois une valeur
	 * "null" pour indiquer au serveur qu'aucune categorie a ete specifier par l'usage.
	 */
	if (categorie){
		taille_categorie=strlen(get_categorie(critere))+1;
		noctets=write(descripteur_fifo_client_ecriture, &taille_categorie, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_fifo_client_ecriture,get_categorie(critere),taille_categorie*sizeof(char));
		if(noctets < taille_categorie*sizeof(char)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
	}
	else if (categorie == NULL){
		categorie = null;
		taille_categorie=strlen(null)+1;
		noctets=write(descripteur_fifo_client_ecriture, &taille_categorie, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		noctets=write(descripteur_fifo_client_ecriture,get_categorie(critere),taille_categorie*sizeof(char));
		if(noctets < taille_categorie*sizeof(char)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
	}

	/*
	 * Si des annees de parution sont présentes dans les critères on les envoit au serveur sinon on envois
	 * des valeurs "null" pour indiquer au serveur qu'aucune annee de parution a ete specifier par l'usage.
	 */
	if (annees){
		int annee_min = get_annee_parution_min(critere);
		noctets=write(descripteur_fifo_client_ecriture, &annee_min , sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		int annee_max = get_annee_parution_max(critere);
		noctets=write(descripteur_fifo_client_ecriture, &annee_max, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
	}
	else if(annees == NULL){
		int annee_min = 0;
		noctets=write(descripteur_fifo_client_ecriture, &annee_min , sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}
		int annee_max = 0;
		noctets=write(descripteur_fifo_client_ecriture, &annee_max, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture dans le FIFO\n");
			exit(1);
		}

	}

	noctets=write(descripteur_fifo_client_ecriture, &flag, sizeof(int));
	if(noctets < sizeof(int)) {
		printf("Probleme lors de l'ecriture dans le FIFO\n");
		exit(1);
	}
	// Tube-HLR01 finie
	// Comm-HLR01 finie

	/*Lab3 Tube-HLR03 : On verifie le fonctionnement on affichant les champs du critere envoyer par le client*/
	printf("[*] Envoi des criteres de recherche:\n");
	printf("\t[+] Titre: %s\n", get_titre(critere));
	if (genre != null){
		printf("\t[+] Genre: %s\n", get_genre(critere));
	}
	if (categorie != null){
		printf("\t[+] Categorie: %s\n", get_categorie(critere));
	}
	if (annees){
		printf("\t[+] Annee_min: %i\n", get_annee_parution_min(critere));
		printf("\t[+] Annee_max: %i\n", get_annee_parution_max(critere));
	}
	/* Tube-HLR03 finie */

	detruire_critere(critere);

	//Lab3 Tube-HLR04
	/*reception des resultats envoyer par le serveur puis on les affiche */
	printf("[*] Reception des resultats\n");
	//On fait la lecture du nombre de resultats reçus
	noctets = read(descripteur_fifo_serveur_lecture, &nb_titre, sizeof(int));
	if(noctets != sizeof(int)) {
		printf("Erreur lors de la lecture du nombre de titres venant du serveur\n");
		exit(1);
	}
	//On affiche le nombre de résultats reçus
	printf("\t[+] %d résultats reçus\n ", nb_titre);

	int i=0;//variable servant comme compteur
	/*
	 * Pour chaque resultats reçus on affiche ses champs a l'usager
	 */

	//Lab3 comm-HLR04
	/*Le client est capable de recuperer les resultats de recherche envoyes par le serveur
	 *et de les afficher a l<utilisateur*/
	while(i<nb_titre){

		printf("\t[%i] ",i+1);

		//Lecture et affichage du champ ID
		noctets=read(descripteur_fifo_serveur_lecture, &taille_genre, sizeof(int));
		if(noctets == sizeof(int)) {
			IDr= malloc(taille_genre*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ ID du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_fifo_serveur_lecture, IDr,taille_genre*sizeof(char));
		if(noctets != taille_genre*sizeof(char)) {
			printf("Erreur lors de la lecture du champ ID du FIFO\n");
			exit(1);
		}
		printf("%s\t",IDr);

		//Lecture et affichage du champ categorie
		noctets = read(descripteur_fifo_serveur_lecture, &taille_categorieR, sizeof(int));
		if(noctets == sizeof(int)) {
			categorieR = malloc(taille_categorieR*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ categorie du FIFO\n");
			exit(1);
		}
		noctets = read(descripteur_fifo_serveur_lecture, categorieR ,taille_categorieR*sizeof(char));
		if(noctets != taille_categorieR*sizeof(char)) {
			printf("Erreur lors de la lecture du champ categorie du FIFO\n");
			exit(1);
		}
		printf("%s\t",categorieR);

		//Lecture et affichage du champ titre
		noctets = read(descripteur_fifo_serveur_lecture, &taille_titreR, sizeof(int));
		if(noctets == sizeof(int)) {
			titreR = malloc(taille_titreR*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ titre du FIFO\n");
			exit(1);
		}
		noctets = read(descripteur_fifo_serveur_lecture,titreR,taille_titreR*sizeof(char));
		if(noctets != taille_titreR*sizeof(char)) {
			printf("Erreur lors de la lecture du champ titre du FIFO\n");
			exit(1);
		}
		printf("%s\t",titreR);

		//Lecture et affichage de l'annee de parution
		noctets = read(descripteur_fifo_serveur_lecture, &annee_parution_min_r, sizeof(int));
		if(noctets != sizeof(int)) {
			printf("Erreur lors de la lecture de l'annee de parution du FIFO\n");
			exit(1);
		}else{
		printf("%i\t",annee_parution_min_r);
		}
		//Lecture et affichage du champ genre
		noctets=read(descripteur_fifo_serveur_lecture, &taille_genreR, sizeof(int));
		if(noctets == sizeof(int)) {
			genreR = malloc(taille_genreR*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ genre du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_fifo_serveur_lecture, genreR,taille_genreR*sizeof(char));
		if(noctets != taille_genreR*sizeof(char)) {
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
    // Tube-HLR04 finie
	// comm-HLR04 finie


	int num_titre;
	int taille_ID_eval;
	char* ID_eval;
	int taille_cote_eval;
	char* cote_eval;
	int nb_vote_eval;
	float note;
	int taille_nouvelle_cote;
	char* nouvelle_cote;
	int nouveau_nb_vote;


	//Lab3 comm-HLR05
	/*Si l'argument -v a été donné par l'utilisateur, le client doit demander à l'utilisateur
	 *quel titre désire-t-il évaluer dans la liste des résultats reçus du serveur.*/
	if(flag==1){
		printf("[-] Veuillez choisir un titre a evaluer\n  >  ");
		scanf("%i",&num_titre);

		//On verifie que le numero de titre specifier par l'usager est valide
		while( num_titre<1 || num_titre>nb_titre ){
			printf("[!] Veuillez choisir une valeur de titre valide\n  >  ");
			scanf("%i",&num_titre);
		}
		//comm-HLR05 finie

		//Lab3 comm-HLR06
		/* Le client envoie au serveur le titre à évaluer parmi les résultats reçus. */
		noctets=write(descripteur_fifo_client_ecriture, &num_titre, sizeof(int));
		if(noctets < sizeof(int)) {
			printf("Probleme lors de l'ecriture du numero de titre dans le FIFO\n");
			exit(1);
		}
		//Lab 3 - comm HLR09
		/* On recoit et affiche les informations du titre a evaluer */
		noctets=read(descripteur_fifo_serveur_lecture,&taille_ID_eval,sizeof(int));
		if(noctets == sizeof(int)) {
			ID_eval = malloc(taille_ID_eval*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ ID du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_fifo_serveur_lecture, ID_eval,taille_ID_eval*sizeof(char));
		if(noctets != taille_ID_eval*sizeof(char)) {
			printf("Erreur lors de la lecture du champ ID du FIFO\n");
			exit(1);
		}
		printf("\t- Titre: %s\n",ID_eval);

		noctets=read(descripteur_fifo_serveur_lecture,&taille_cote_eval,sizeof(int));
		if(noctets == sizeof(int)) {
			cote_eval = malloc(taille_cote_eval*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ cote du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_fifo_serveur_lecture, cote_eval,taille_cote_eval*sizeof(char));
		if(noctets != taille_cote_eval*sizeof(char)) {
			printf("Erreur lors de la lecture du champ cote du FIFO\n");
			exit(1);
		}
		printf("\t- Cote moyenne: %s /10 \n",cote_eval);

		noctets = read(descripteur_fifo_serveur_lecture, &nb_vote_eval, sizeof(int));
		if(noctets != sizeof(int)) {
			printf("Erreur lors de la lecture du nombre de vote du FIFO\n");
			exit(1);
		}
		printf("\t- Nombre de votes : %i\n\n",nb_vote_eval);
		//comm-HLR09 finie

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
		noctets=write(descripteur_fifo_client_ecriture, &note, sizeof(float));
		if(noctets < sizeof(float)) {
			printf("Probleme lors de l'ecriture de la note dans le FIFO\n");
			exit(1);
		}
		free(ID_eval);
		free(cote_eval);

	    //comm-HLR10 finie

	    //Lab 3 comm-HLR13
		/* On recoit et affiche les nouvelles informations*/

		noctets=read(descripteur_fifo_serveur_lecture,&taille_ID_eval,sizeof(int));
		if(noctets == sizeof(int)) {
			ID_eval = malloc(taille_ID_eval*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ ID du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_fifo_serveur_lecture, ID_eval,taille_ID_eval*sizeof(char));
		if(noctets != taille_ID_eval*sizeof(char)) {
			printf("Erreur lors de la lecture du champ ID du FIFO\n");
			exit(1);
		}
		printf("\t- Titre: %s\n",ID_eval);

		noctets=read(descripteur_fifo_serveur_lecture,&taille_nouvelle_cote,sizeof(int));
		if(noctets == sizeof(int)) {
			nouvelle_cote = malloc(taille_nouvelle_cote*sizeof(char));
		}
		else {
			printf("Erreur lors de la lecture de la taille du champ cote du FIFO\n");
			exit(1);
		}
		noctets=read(descripteur_fifo_serveur_lecture, nouvelle_cote,taille_nouvelle_cote*sizeof(char));
		if(noctets != taille_nouvelle_cote*sizeof(char)) {
			printf("Erreur lors de la lecture du champ cote du FIFO\n");
			exit(1);
		}
		printf("\t- Nouvelle cote moyenne: %s /10 \n",nouvelle_cote);

		noctets = read(descripteur_fifo_serveur_lecture, &nouveau_nb_vote, sizeof(int));
		if(noctets != sizeof(int)) {
			printf("Erreur lors de la lecture du nombre de vote du FIFO\n");
			exit(1);
		}
		printf("\t- Nouveau nombre de votes: %i \n",nouveau_nb_vote);
		//comm-HLR13 finie
		free(ID_eval);
		free(nouvelle_cote);
	}
	//comm-HLR06 finie

	close(descripteur_fifo_client_ecriture);
	close(descripteur_fifo_serveur_lecture);
	printf("[-] Fermeture de l'application.\n");

	return 0;
}
//HLR26 finie

