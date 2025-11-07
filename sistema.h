#ifndef SISTEMA_H
#define SISTEMA_H

#include <vector>
#include <utility>
#include "nodo.h"

class Sistema {
private:
    std::vector<Nodo> productos;               // no incluye base
    Nodo base;                                 // id = -1, (0,0)
    int k;                                     // capacidad por viaje

    std::vector< std::vector<double> > distPP; // distancias producto-producto
    std::vector<double> distPB;                // distancias producto-base

    void construirGrafoCompleto();

    double distProductoProducto(int a, int b) const;
    double distProductoBase(int idx) const;

    // Construye un viaje usando Prim modificado:
    //  - raíz fija en la base (0,0),
    //  - sólo productos no atendidos globalmente,
    //  - máximo k productos nuevos en este viaje.
    // Devuelve en 'seleccion' los índices de productos en el orden de visita.
    void construirViajePrim(std::vector<int>& seleccion,
                            const std::vector<bool>& usadoGlobal) const;

public:
    Sistema();

    void cargarEscenario(const std::vector< std::pair<double,double> >& coords, int capacidad);

    // Genera la secuencia completa de visita, incluyendo (0,0) entre viajes y al final
    // si incluirRegresos es true (como exige el enunciado).
    void resolver(std::vector< std::pair<double,double> >& secuenciaSalida,
                  bool incluirRegresos) const;
};

#endif
