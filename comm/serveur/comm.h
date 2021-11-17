/*
 * comm.h
 *
 *  Created on: Nov 15, 2021
 *      Author: thomas
 */

#ifndef HDR_COMM_H_
#define HDR_COMM_H_
#include "recherche.h"
#include "imdb.h"
#include "resultat.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void serveur_demarage();
void serveur_recoit_critere(int descripteur_fifo_client_lecture,t_critere critere);
void serveur_envoit_resultat(int descripteur_fifo_serveur_ecriture, t_resultat resultat);
void serveur_envoi_cote(int descripteur_fifo_serveur_ecriture  ,t_titre titre_chercher);
void serveur_envoi_nouvcote(int descripteur_fifo_serveur_ecriture, t_titre titre_chercher);


#endif /* HDR_COMM_H_ */
