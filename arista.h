#ifndef ARISTA_H
#define ARISTA_H

class Arista {
private:
    int origen;
    int destino;
    double costo;
public:
    Arista();
    Arista(int o, int d, double c);

    int obtenerOrigen() const;
    int obtenerDestino() const;
    double obtenerCosto() const;
};

#endif
