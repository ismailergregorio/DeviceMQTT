#ifndef FUNCOESEXECUSAO_H
#define FUNCOESEXECUSAO_H
#include <Arduino.h>
#include "dados_mqtt.h"
void executarSequencias(
    unsigned long &tempoInicioSequencia,
    unsigned long intervaloSequencia,
    unsigned long &tempoPasso,
    const int intervaloPasso,
    bool &sequenciaAtiva,
    int &passoAtual,
    DadosMqtt &dadosMqtt);
#endif