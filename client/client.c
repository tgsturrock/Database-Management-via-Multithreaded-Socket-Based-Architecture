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
#include "comm.h"
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


	//Le client envoi les criteres de recherche
	client_envoi_critere(descripteur_fifo_client_ecriture, critere);


	client_recoi_resultat(descripteur_fifo_serveur_lecture);

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
	if(get_evaluation(critere) == 1){
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

	//On desaloue la memoire dedier au criteres de recherche
	detruire_critere(critere);
	close(descripteur_fifo_client_ecriture);
	close(descripteur_fifo_serveur_lecture);
	printf("[-] Fermeture de l'application.\n");

	return 0;
}
//HLR26 finie

