#include "nodo.h"
#include <cmath>

Nodo::Nodo() : id(-1), x(0.0), y(0.0), visitado(false) {}
Nodo::Nodo(int id_, double x_, double y_) : id(id_), x(x_), y(y_), visitado(false) {}

int Nodo::obtenerId() const { return id; }
double Nodo::obtenerX() const { return x; }
double Nodo::obtenerY() const { return y; }
bool Nodo::estaVisitado() const { return visitado; }

void Nodo::fijarVisitado(bool v) { visitado = v; }
bool Nodo::esBase() const { return id == -1; }

double Nodo::distanciaA(const Nodo& otro) const {
    double dx = x - otro.x;
    double dy = y - otro.y;
    return std::sqrt(dx*dx + dy*dy);
}
