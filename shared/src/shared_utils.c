#include "shared_utils.h"

void finalizar_programa(t_log* logger, t_config* config) {
    destruir_logger(logger);
    destruir_config(config);
}

//Después se moverá a otro archivo:

void destruir_logger(t_log* logger) {
    log_destroy(logger);
}