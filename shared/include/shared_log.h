#ifndef SHARED_LOG
#define SHARED_LOG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/log.h>

t_log* iniciar_logger(char*, char*);
void destruir_logger(t_log*);
void shared_log_trace(t_log*, char*);
void shared_log_debug(t_log*, char*);
void shared_log_info(t_log*, char*);
void shared_log_warning(t_log*, char*); 
void shared_log_error(t_log*, char*);

pthread_mutex_t log_mutex;

#endif