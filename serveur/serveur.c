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
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t lock;


int main(int argc, char *argv[]) {
	int noctets = 0;
	int descripteur_socket_serveur, descripteur_socket_client;
	unsigned int taille_adresse_serveur, taille_adresse_client;
	struct sockaddr_in adresse_serveur, adresse_client;
	int status;
	pid_t* pid = NULL;

///commentaire
	int num_titre;
	float cote;

	close(descripteur_socket_serveur);
	close(descripteur_socket_client);
	/**
	 * Lab4 comm-HLR01
	 * Le client et le serveur doivent communiquer par l'entremise de sockets.
	 */
	/* creer un socket pour le serveur */
	descripteur_socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

	if (descripteur_socket_serveur == -1)
    {
        printf("Echec lors de la creation du socker serveur");
    }
    puts("Socket cree");

	  /* relier le socket a l'adresse IP de l'hote et au port choisi */
	adresse_serveur.sin_family = AF_INET; /* On va etablir une communication TCP/IP */
	adresse_serveur.sin_addr.s_addr = inet_addr("127.0.0.1"); /* l'adresse IP de l'hote du serveur */
	adresse_serveur.sin_port = htons(10001);
	taille_adresse_serveur = sizeof(adresse_serveur);
	status = bind(descripteur_socket_serveur, (struct sockaddr *)&adresse_serveur, taille_adresse_serveur);
	if(status == -1) {
		printf("Échec lors de la configuration du socket serveur\n");
	}

	listen(descripteur_socket_serveur, 10);

	while(1) {

	    printf("En attente de clients...\n");

	    /* Accepter une connexion en provenance d'un client */
	    descripteur_socket_client = accept(descripteur_socket_serveur, (struct sockaddr *)&adresse_client, &taille_adresse_client);

	    printf("Connexion etablie avec un client a l'adresse IP %s\n", inet_ntoa(adresse_client.sin_addr));

	    //Cree une structure critere pour enmagasiner les criteres de recherche
	    	t_critere critere = creer_critere();

	    	//Le serveur recoit les critere de recherche envoye par le client
	    	serveur_recoit_critere(descripteur_socket_client, critere);
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


	    	serveur_envoit_resultat(descripteur_socket_client, resultat);

	    	//Lab3 comm-HLR07
	    	/*Si le champ relié à l'argument -v a bien été reçu lors du requis Comm-HLR02,
	    	 *le serveur est capable de récupérer le titre à évaluer par le client.*/
	    	if(get_evaluation(critere) == 1){

	    		noctets = read(descripteur_socket_client, &num_titre, sizeof(int));
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

	    		//Le serveur envoise la cote du titre demande
	    		serveur_envoi_cote(descripteur_socket_client, titre_chercher);
	    		//comm-HLR08 finie


	    		//Lab3 comm-HLR11
	    		/*Le serveur est capable de recevoir la note sur 10 du client
	    		 *et de calculer la nouvelle cote de classement du titre évalué.*/
	    		noctets = read(descripteur_socket_client, &cote , sizeof(float));
	    		if(noctets < sizeof(float)) {
	    			printf("Probleme lors de la lecture de la cote envoyer par l'usager dans le FIFO\n");
	    			exit(1);
	    		}

	    		//Mise a jour de la base de donnees
	    		fichier_cote(titre_chercher, cote);
	    		//comm-HLR11 finie

	    		serveur_envoi_nouvcote(descripteur_socket_client, titre_chercher);

	    	}
	    	//Tube-HLR04 finie
	    	//comm-HLR03 finie

	    	//libere la memoire
	    	detruire_resultat(resultat);
	    	detruire_critere(critere);


	    	close(descripteur_socket_serveur);
	    	close(descripteur_socket_client);
	    	//comm-HLR01 finie

	}

	printf("[-] Fermeture du serveur.\n");
	return 0;
}
//HLR26 finie

//HLR26 finie

