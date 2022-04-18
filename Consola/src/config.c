#include "../include/config.h"

#define CONFIG_FILE_PATH "./cfg/Consola.config"

void iniciar_config() {

  t_config* config_consola = config_create(CONFIG_FILE_PATH);

  procesar_archivo_config_consola(config_consola);

  //config_destroy(config_consola);
}

void procesar_archivo_config_consola(t_config* config_consola) {
    
  ip_kernel = config_get_string_value(config_consola, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config_consola, "PUERTO_KERNEL");
}