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
	char null[2] = "0";

	int taille_titre;
	int taille_genre;
	int taille_categorie;
	int nb_titre;


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
	/**
	 * On ecrit dans les champs du critere de recherche
	 * Si un argument n'a pas ete specifier par l'usage on inscrit une valeur "null" dans le champ correspondant.
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

	//Le client recoit les resultat envoye par le serveur et conserve la valeur du nombre de titre
	nb_titre = client_recoi_resultat(descripteur_fifo_serveur_lecture);

	//Lab3 comm-HLR05
	/*Si l'argument -v a été donné par l'utilisateur, le client doit demander à l'utilisateur
	 *quel titre désire-t-il évaluer dans la liste des résultats reçus du serveur.*/
	if(get_evaluation(critere) == 1){

		client_envoi_titre(descripteur_fifo_client_ecriture, nb_titre);

		client_recoi_cote(descripteur_fifo_serveur_lecture);

		client_envoi_cote(descripteur_fifo_client_ecriture);

		client_recoit_nouvelle_cote(descripteur_fifo_serveur_lecture);

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

