#include "arista.h"

Arista::Arista() : origen(-1), destino(-1), costo(0.0) {}
Arista::Arista(int o, int d, double c) : origen(o), destino(d), costo(c) {}

int Arista::obtenerOrigen() const { return origen; }
int Arista::obtenerDestino() const { return destino; }
double Arista::obtenerCosto() const { return costo; }
