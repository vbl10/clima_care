#ifndef CC_UTIL_H
#define CC_UTIL_H
#include <Arduino.h>

namespace cc {
  String espera_linha(Stream& stream);
  bool consulta_banco(const String& sql, String* resposta = nullptr);
};

#endif