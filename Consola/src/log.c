#include "../include/log.h"

void iniciar_logger() {
    logger_consola = log_create("./cfg/proceso1.log", "Consola", true, LOG_LEVEL_INFO);
}