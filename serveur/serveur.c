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
#include "thread.h"
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

#define MAX 60
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[]) {

	int desc_socket_serveur, desc_socket_client;
	unsigned int taille_adresse_serveur, taille_adresse_client;
	struct sockaddr_in adresse_serveur, adresse_client;
	int status;

	/**
	 * Lab4 comm-HLR01
	 * Le client et le serveur doivent communiquer par l'entremise de sockets.
	 */
	/* creer un socket pour le serveur */
	desc_socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

	if (desc_socket_serveur == -1)
    {
		handle_error("socket");
    }
    puts("Socket cree");


    /**Lab4 Comm-HLR02
     * Un serveur unique recoit des requetes de plusieurs clients
     */
	/* relier le socket a l'adresse IP de l'hote et au port choisi */
	adresse_serveur.sin_family = AF_INET; /* On va etablir une communication TCP/IP */
	adresse_serveur.sin_addr.s_addr = inet_addr("127.0.0.1"); /* l'adresse IP de l'hote du serveur */
	adresse_serveur.sin_port = htons(10001);
	taille_adresse_serveur = sizeof(adresse_serveur);
	memset(adresse_serveur.sin_zero, '\0', sizeof adresse_serveur.sin_zero);
	status = bind(desc_socket_serveur, (struct sockaddr *)&adresse_serveur, taille_adresse_serveur);
	if(status == -1) {
		handle_error("bind");
		printf("Échec lors de la configuration du socket serveur\n");
	}

	listen(desc_socket_serveur, 10);
	//Lab4 Comm-HLR02 finie

	int i =0;
	while(1) {
		/**Lab4 Comm-HLR3
		 * une fois demarre, le serveur reste actif en attendant des connexions de clients
		 */
	    printf("En attente de clients...\n");

	    /**Lab4 Serveur-HLR01
	     * Apres avoir recu une requete de client, le serveur delegue l<interpretation et le
	     * taitement de la requete a un nouveau thread
	     */

	    /* Accepter une connexion en provenance d'un client */
	    desc_socket_client = accept(desc_socket_serveur, (struct sockaddr *)&adresse_client, &taille_adresse_client);
	    printf("Connexion etablie avec un client a l'adresse IP %s\n", inet_ntoa(adresse_client.sin_addr));

	    /**Lab4 Serveur-HLR07
	     * Toute mémoire allouée par le thread principal serveur doit être désallouée,
	     * soit par le thread principal soit par un thread associé à un client
	     */
	    int*p_socket_client=malloc(sizeof(int));
		*p_socket_client=desc_socket_client;

		pthread_t tid[MAX];
		 if( pthread_create(&tid[i++], NULL, handle_connection, p_socket_client) != 0 )
		           printf("Failed to create thread\n");

		        if( i >= 50)
		        {
		          i = 0;
		          while(i < 50)
		          {
		            pthread_join(tid[i++],NULL);
		          }
		          i = 0;
		        }
		 //Lab4 Serveur-HLR01 finie
		 //Lab4 Comm-HLR3 finie

	}
	close(desc_socket_serveur);
	//comm-HLR01 finie
	printf("[-] Fermeture du serveur.\n");
	return 0;
}
//HLR26 finie

//HLR26 finie


