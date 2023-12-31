#include "cc_sens_reserv.h"
#include "cc_pins.h"
#include <Arduino.h>

float cc::sens_reserv::gatilho = 0.45f; //acima desse valor, o reservatorio esta vazio 

float cc::sens_reserv::f_valor = 0.0f;

char cc::sens_reserv::ligado = 0;
cc::mutex cc::sens_reserv::mtx_ligado;

cc::sens_reserv cc::sens_reserv::unico;

void cc::sens_reserv::iniciar() {
  pinMode(PIN::SENS_RESERV, INPUT);
  pinMode(PIN::LIGA_SENS_RESERV, OUTPUT);
  xTaskCreate(tarefa_leitura, "leitura_sens_reserv", 2048, nullptr, 1, nullptr);
}
bool cc::sens_reserv::vazio() {
  //return f_valor > gatilho;
  return false;
}
float cc::sens_reserv::valor() {
  return f_valor;
}
void cc::sens_reserv::liga(int espera_n_amostras) {
  mtx_ligado.capturar();
  ligado++;
  mtx_ligado.liberar();
  if (espera_n_amostras)
    delay(espera_n_amostras * dt);
}
void cc::sens_reserv::desliga() {
  mtx_ligado.capturar();
  ligado -= (ligado > 0);
  mtx_ligado.liberar();
}
void cc::sens_reserv::tarefa_leitura(void* pv_args) {
  while (1) {
    while (!ligado) delay(10);

    digitalWrite(PIN::LIGA_SENS_RESERV, HIGH);
    delay(1);
    f_valor = (float)analogRead(PIN::SENS_RESERV) / 4095.0f;
    while (ligado) {
      delay(dt);
      f_valor += ((float)analogRead(PIN::SENS_RESERV) / 4095.0f - f_valor) * peso;
    }
    digitalWrite(PIN::LIGA_SENS_RESERV, LOW);
  }
}
cc::terminal::retorno cc::sens_reserv::plot(const terminal::params& args) {
  args.stream.println("Digite qualquer coisa para sair...");
  liga();
  while (!args.stream.available()) {
    args.stream.println(f_valor);
    delay(100);
  }
  desliga();
  while (args.stream.available()) {
    args.stream.read();
    delay(10);
  }
  return terminal::retorno{};
}