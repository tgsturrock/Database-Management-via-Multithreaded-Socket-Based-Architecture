/*
 * tread.c
 *
 *  Created on: Nov 24, 2021
 *      Author: thomas
 */

#include "recherche.h"
#include "imdb.h"
#include "resultat.h"
#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/**Lab4 Serveur-HLR06
 * Le module responsable des accès en lecture et en écriture à la base de données
 * est responsable d'en maintenir l'intégrité en mode multitâche
 */
void* handle_connection(void* p_socket_client){

	int num_titre;
	float cote;
	int noctets;
	int desc_socket_client = *(int*)p_socket_client;
	free(p_socket_client);
	//Cree une structure critere pour enmagasiner les criteres de recherche
	t_critere critere = creer_critere();

	//Le serveur recoit les critere de recherche envoye par le client
	serveur_recoit_critere(desc_socket_client, critere);


	//Recherche dans la base de donnee pour des titre qui concorde avec les criteres de recherche
	printf("[*] Exploration de la base de donnees\n");
	t_resultat resultat = lecture(critere);

	/**Lab4 Serveur-HLR02
	 * On se sert de mutex lock lorsqu'une operation de lecture se poursuit
	 * Donc aucun autre thread ne peut modifier la base de donner
	 * On utilise mutex_unlock lorsque l"operation de lecture est terminer.
	 */

	pthread_mutex_lock(&mutex);
	//Ajout des cotes de moyenne et nombre de vote aux resultats
	lecture_cote(resultat);

	//Affichage des resultats cote serveur
	printf("[*] Visualisation des résultats\n");
	fichier_resultat(resultat);
	pthread_mutex_unlock(&mutex);

	serveur_envoit_resultat(desc_socket_client, resultat);

	/*Si le champ relié à l'argument -v a bien été reçu lors du requis Comm-HLR02,
	 *le serveur est capable de récupérer le titre à évaluer par le client.*/
	if(get_evaluation(critere) == 1){

		noctets = read(desc_socket_client, &num_titre, sizeof(int));
		if(noctets != sizeof(int)) {
			printf("Erreur lors de la lecture du numero de titre a evaluer \n");
			exit(1);
		}

		/*Le serveur cherche les données de classement du titre à évaluer et les envoie au client.*/
		t_titre titre_chercher;
		printf("[*] Titre a evaluer:\n");
		titre_chercher = print_titre(resultat,(num_titre-1));

		//Le serveur envoi la cote du titre demande
		serveur_envoi_cote(desc_socket_client, titre_chercher);
		//comm-HLR08 finie


		/*Le serveur est capable de recevoir la note sur 10 du client
		 *et de calculer la nouvelle cote de classement du titre évalué.*/
		noctets = read(desc_socket_client, &cote , sizeof(float));
		if(noctets < sizeof(float)) {
			printf("Probleme lors de la lecture de la cote envoyer par l'usager dans le FIFO\n");
			exit(1);
		}
		/**Lab4 Serveur-HLR02
		 * On utilise mutex lock pour qu'il n'y ait qu'un seul thread puisse modifier la base de donnees a la fois.
		 */
		pthread_mutex_lock(&mutex);
		//Mise a jour de la base de donnees
		fichier_cote(titre_chercher, cote);
		pthread_mutex_unlock(&mutex);
		//Lab4 Serveur-HLR02 finie

		serveur_envoi_nouvcote(desc_socket_client, titre_chercher);

	}



	//libere la memoire
	detruire_resultat(resultat);
	detruire_critere(critere);
	close(desc_socket_client);
	pthread_exit(NULL);

	return NULL;
}
//Lab4 Serveur-HLR06
