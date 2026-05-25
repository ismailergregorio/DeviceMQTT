#include <ESP8266httpUpdate.h>
void executarOTA(String url)
{
    Serial.println("Iniciando OTA...");
    Serial.println(ESP.getFreeHeap());
    Serial.println("URL: " + url);

    WiFiClient client;

    // 🔥 MUITO IMPORTANTE
    ESPhttpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    ESPhttpUpdate.rebootOnUpdate(true);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, "http://192.168.100.5:8081/"+url);

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("Erro OTA (%d): %s\n",
                      ESPhttpUpdate.getLastError(),
                      ESPhttpUpdate.getLastErrorString().c_str());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("Sem atualizações.");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("Atualização concluída!");
        break;
    }
}