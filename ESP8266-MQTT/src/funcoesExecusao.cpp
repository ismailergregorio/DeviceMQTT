#include <Arduino.h>
#include "dados_mqtt.h"

void executarSequencias(
    unsigned long &tempoInicioSequencia,
    unsigned long intervaloSequencia,
    unsigned long &tempoPasso,
    const int intervaloPasso,
    bool &sequenciaAtiva,
    int &passoAtual,
    DadosMqtt &dadosMqtt
)
{
    if (!sequenciaAtiva &&
        millis() - tempoInicioSequencia >= intervaloSequencia)
    {

        sequenciaAtiva = true;

        passoAtual = 1;

        tempoPasso = millis();

        tempoInicioSequencia = millis();
    }

    // executa passos da sequência
    if (sequenciaAtiva &&
        millis() - tempoPasso >= intervaloPasso)
    {

        tempoPasso = millis();

        switch (passoAtual)
        {
        case 1:
            dadosMqtt.enviarStatusB();
            Serial.println("teste1");
            break;

        case 2:
            dadosMqtt.enviarWifi();
            Serial.println("teste2");
            break;

        case 3:
            dadosMqtt.enviarMQTT();
            Serial.println("teste3");
            break;

        case 4:
            Serial.println("teste4");
            break;

        case 5:
            dadosMqtt.enviarHealth1();
            Serial.println("teste5");
            break;

        case 6:
            dadosMqtt.enviarHealth2();
            Serial.println("teste6");
            break;
        }

        passoAtual++;

        // terminou sequência
        if (passoAtual > 6)
        {
            sequenciaAtiva = false;
        }
    }
}