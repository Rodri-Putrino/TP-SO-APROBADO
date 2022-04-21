#include "../include/shared_log.h"

t_log* iniciar_logger(char* log_file_path, char* nombre_modulo) {
    pthread_mutex_init(&log_mutex, NULL);
    return log_create(log_file_path, nombre_modulo, true, LOG_LEVEL_INFO);
}

void destruir_logger(t_log* logger) {
    log_destroy(logger);
    pthread_mutex_destroy(&log_mutex);
}

void shared_log_trace(t_log* logger, char *mensaje) {
    pthread_mutex_lock(&log_mutex);
    log_trace(logger, mensaje);
    pthread_mutex_unlock(&log_mutex);
}

void shared_log_debug(t_log* logger, char *mensaje) {
    pthread_mutex_lock(&log_mutex);
    log_debug(logger, mensaje);
    pthread_mutex_unlock(&log_mutex);
}

void shared_log_info(t_log* logger, char *mensaje) {
    pthread_mutex_lock(&log_mutex);
    log_info(logger, mensaje);
    pthread_mutex_unlock(&log_mutex);
}

void shared_log_warning(t_log* logger, char *mensaje) {
    pthread_mutex_lock(&log_mutex);
    log_warning(logger, mensaje);
    pthread_mutex_unlock(&log_mutex);
}

void shared_log_error(t_log* logger, char *mensaje) {
    pthread_mutex_lock(&log_mutex);
    log_error(logger, mensaje);
    pthread_mutex_unlock(&log_mutex);
}
