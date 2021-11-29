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

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>




int main(int argc, char *argv[]) {
	char *titre = NULL, *genre = NULL, *annees = NULL, *categorie = NULL;
	int desc_socket_client;
	unsigned int taille_adresse_serveur;
	struct sockaddr_in adresse_serveur;
	int success;


	char null[2] = "0";
	int nb_titre;
	int flag = 0;

	//Lab3 client-HLR02
	/**
	 * On se sert de getopt afin de passer au travers du tableau argv pour retrouver les arguments passer en parametere
	 * afin d'associer des valeurs au chanps qui sont associees a ces arguments.
	 */

	int opt;//variable servant a passer d'un argument a l'autre.
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
		case 'v':
			flag = 1;
			break;
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
	while(titre == NULL){
		printf ("[!] Veuillez saisir un titre\n");
		scanf("%s", titre);
	}
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

	/**
	 * Lab4 comm-HLR01
	 * Le client et le serveur doivent communiquer par l'entremise de sockets.
	 */
	/* creer un socket pour le client */
	desc_socket_client = socket(AF_INET, SOCK_STREAM, 0);
	/* paramètres de connexion au serveur */
	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_addr.s_addr = inet_addr("127.0.0.1");
	adresse_serveur.sin_port = htons(10001);
	taille_adresse_serveur = sizeof(adresse_serveur);

	/**
	 * Lab4 comm-HLR04
	 * Si le client démarre en l'absence d'un serveur actif, le client émet un message d'erreur à l'utilisateur et termine
	 */
	/* connecter le socket du client au socket du serveur */
	printf("[*] Tentative de connexion au serveur...\n");
	success = connect(desc_socket_client, (struct sockaddr*)&adresse_serveur, taille_adresse_serveur);

	if(success == -1) {
		printf("[!] Échec lors de la connexion au serveur\n");
		exit(1);
	} else {
		printf("[*] Client connecté au serveur avec succès\n");
	}
	//comm-HLR04 finie

	//Le client envoi les criteres de recherche
	client_envoi_critere(desc_socket_client, critere);

	//Le client recoit les resultat envoye par le serveur et conserve la valeur du nombre de titre
	nb_titre = client_recoi_resultat(desc_socket_client);

	//Lab3 comm-HLR05
	/*Si l'argument -v a été donné par l'utilisateur, le client doit demander à l'utilisateur
	 *quel titre désire-t-il évaluer dans la liste des résultats reçus du serveur.*/
	if(get_evaluation(critere) == 1){

		client_envoi_titre(desc_socket_client, nb_titre);

		client_recoi_cote(desc_socket_client);

		client_envoi_cote(desc_socket_client);

		client_recoit_nouvelle_cote(desc_socket_client);

	}
	//comm-HLR06 finie

	//On desaloue la memoire dedier au criteres de recherche
	detruire_critere(critere);
	close(desc_socket_client);
	//comm-HLR01 finie

	printf("[-] Fermeture de l'application.\n");

	return 0;
}
//HLR26 finie

