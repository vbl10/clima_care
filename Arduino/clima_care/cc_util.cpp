#include "cc_util.h"
#include <WiFi.h>
#include <HTTPClient.h>

String cc::espera_linha(Stream& stream) {
  String linha;
  while (!stream.available()) delay(10);
  while (stream.available() && stream.peek() != '\n' && stream.peek() != '\r') {
    linha += (char)stream.read();
    delay(10);
  }
  while (stream.available()) {
    stream.read();
    delay(10);
  }
  return linha;
}

bool cc::consulta_banco(const String& sql, String* resultado) {
  WiFiClient cliente;
  HTTPClient http;
  http.setTimeout(10000);
  http.setConnectTimeout(10000);
  http.begin(cliente, "http://gardeningprojectteste.000webhostapp.com/clima_care.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int http_codigo_resposta = http.POST("query=" + sql);
  String http_resposta = http.getString();
  http.end();
  if (http_codigo_resposta / 100 != 2) {
    if (resultado)
      *resultado = String("Erro (") + http_resposta + "): " + http_resposta;
    return false;
  }
  if (resultado)
    *resultado = http_resposta;
  return true;
}