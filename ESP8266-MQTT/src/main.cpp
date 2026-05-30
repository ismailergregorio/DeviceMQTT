#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include "dados_mqtt.h"
#include "updateOTA.h"
#include "conexaoWifi.h"
#include "funcoesExecusao.h"

// --- CONFIG WIFI ---
const char *ssid = "Ismailer Gregorio Oi Fibra 2.4G";
const char *password = "27270404";

// --- CONFIG MQTT ---
const char *mqtt_server = "ubunto-serve.local";
const int mqtt_port = 1883;
const char *mqtt_user = "admin";
const char *mqtt_pass = "123";

// --- MQTT
WiFiClient espClient;
PubSubClient client(espClient);

String i = "2";

String id = "000" + i;
String deviceId = "ESP8266_TESTE" + i;
String nome = "ESP TESTE" + i;
String descricao = "Dispositivo de teste";
String ip = WiFi.localIP().toString();
String verssao = "v.0.0.1";
String placa = "ESP8266";

DadosMqtt dadosMqtt(&client, id, deviceId, nome, descricao, placa, verssao);

unsigned long ultimoEnvio = 0;
const long intervalo = 5000;

unsigned long tempoInicioSequencia = 0;
unsigned long tempoPasso = 0;

const int intervaloSequencia = 5000;
const int intervaloPasso = 100;

bool sequenciaAtiva = false;

int passoAtual = 1;

String listaDeComandos[] = {
    "/search",
    "/statusB",
    "/wifi",
    "/mqtt",
    "/health1",
    "/health2",
    "/update"};

void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Conectando no MQTT... ");

    // ID único do ESP (pra não dar conflito)
    String clientId = "ESP8266-";
    clientId += String(ESP.getChipId());

    bool connected;

    // Se você usa user/senha
    connected = client.connect(clientId.c_str(), mqtt_user, mqtt_pass);

    if (connected)
    {
      Serial.println("Conectado!");
      int tamanhoLista = sizeof(listaDeComandos) / sizeof(listaDeComandos[0]);

      for (int i = 0; i < tamanhoLista; i++)
      {
        client.subscribe(("app/" + deviceId + listaDeComandos[i]).c_str());
      }

      client.subscribe("app/devices/search");
      dadosMqtt.enviarStatusA();
    }
    else
    {
      Serial.print("Falhou. rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 3s...");
      delay(3000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{

  String message = "";
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  Serial.print("Mensagem: ");
  Serial.println(message);

  int tamanhoLista = sizeof(listaDeComandos) / sizeof(listaDeComandos[0]);

  for (int i = 0; i < tamanhoLista; i++)
  {
    int valor = String(topic).indexOf(listaDeComandos[i]);

    if (valor != -1)
    {
      return enviaComando(listaDeComandos[i], message, dadosMqtt);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(A0, INPUT);

  ConexaoWifi(ssid, password);

  client.setServer(mqtt_server, mqtt_port);

  client.setCallback(callback);
}

void loop()
{
  // Se cair WiFi, reconecta
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi desconectado!");
    ConexaoWifi(ssid, password);
  }

  // Se cair MQTT, reconecta
  if (!client.connected())
  {
    Serial.println("MQTT desconectado!");
    reconnectMQTT();
  }

  client.loop();
  executarSequenciasLoop(tempoInicioSequencia,
                         intervaloSequencia,
                         tempoPasso, intervaloPasso,
                         sequenciaAtiva,
                         passoAtual,
                         dadosMqtt);
}