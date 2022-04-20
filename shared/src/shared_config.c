#include "../include/shared_config.h"

t_config* iniciar_config(char* CONFIG_FILE_PATH) {

  t_config* config_consola = config_create(CONFIG_FILE_PATH);

  return config_consola;
}

t_config* destruir_config(t_config* config) {
  config_destroy(config);
}