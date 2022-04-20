#include "../include/log.h"

void iniciar_logger() {
    logger_consola = log_create("./cfg/Consola.log", "Consola", true, LOG_LEVEL_INFO);
}