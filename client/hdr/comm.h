/*
 * comm.h
 *
 *  Created on: Nov 15, 2021
 *      Author: thomas
 */

#ifndef HDR_COMM_H_
#define HDR_COMM_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


void client_envoi_critere(int descripteur_fifo_client_ecriture, t_critere critere);
int client_recoi_resultat(int descripteur_fifo_serveur_lecture );

void client_envoi_titre(int descripteur_fifo_client_ecriture, int nb_titre);
void client_recoi_cote(int descripteur_fifo_serveur_lecture);
void client_envoi_cote(int descripteur_fifo_client_ecriture);
void client_recoit_nouvelle_cote(int descripteur_fifo_serveur_lecture);

#endif /* HDR_COMM_H_ */
