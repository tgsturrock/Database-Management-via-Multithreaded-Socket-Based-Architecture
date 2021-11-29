/*
 * thread.h
 *
 *  Created on: Nov 24, 2021
 *      Author: thomas
 */

#ifndef HDR_THREAD_H_
#define HDR_THREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>


void* handle_connection(void* params);
void* thread_fonction(void *arg);

typedef struct thread_args* t_params;


#endif /* HDR_THREAD_H_ */
