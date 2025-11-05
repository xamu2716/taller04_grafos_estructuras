#include "sistema.h"
#include <cmath>
#include <cstddef>

Sistema::Sistema() : base(Nodo(-1, 0.0, 0.0)), k(1) {}

void Sistema::cargarEscenario(const std::vector< std::pair<double,double> >& coords, int capacidad) {
    productos.clear();
    distPP.clear();
    distPB.clear();
    k = capacidad > 0 ? capacidad : 1;

    int m = (int)coords.size();
    productos.reserve(m);
    for (int i = 0; i < m; i++) {
        productos.push_back(Nodo(i, coords[i].first, coords[i].second));
    }
    construirGrafoCompleto();
}

void Sistema::construirGrafoCompleto() {
    int m = (int)productos.size();
    distPP.assign(m, std::vector<double>(m, 0.0));
    distPB.assign(m, 0.0);

    for (int i = 0; i < m; i++) {
        distPB[i] = base.distanciaA(productos[i]);
        for (int j = i + 1; j < m; j++) {
            double dx = productos[i].obtenerX() - productos[j].obtenerX();
            double dy = productos[i].obtenerY() - productos[j].obtenerY();
            double d = std::sqrt(dx*dx + dy*dy);
            distPP[i][j] = d;
            distPP[j][i] = d;
        }
    }
}

double Sistema::distProductoProducto(int a, int b) const {
    return distPP[a][b];
}

double Sistema::distProductoBase(int idx) const {
    return distPB[idx];
}

int Sistema::indiceProductoMasCercanoABase(const std::vector<bool>& usado) const {
    int mejor = -1;
    double mejorD = 0.0;
    int m = (int)productos.size();
    for (int i = 0; i < m; i++) {
        if (!usado[i]) {
            double d = distProductoBase(i);
            if (mejor == -1 || d < mejorD) {
                mejor = i;
                mejorD = d;
            }
        }
    }
    return mejor;
}

int Sistema::indiceMasCercanoAlActual(int idxActual, const std::vector<bool>& usado) const {
    int mejor = -1;
    double mejorD = 0.0;
    int m = (int)productos.size();
    for (int i = 0; i < m; i++) {
        if (!usado[i] && i != idxActual) {
            double d = distProductoProducto(idxActual, i);
            if (mejor == -1 || d < mejorD) {
                mejor = i;
                mejorD = d;
            }
        }
    }
    return mejor;
}


void Sistema::resolver(std::vector< std::pair<double,double> >& secuenciaSalida, bool incluirRegresos) const {
    secuenciaSalida.clear();

    int m = (int)productos.size();
    if (m == 0) {
        // solo base
        if (incluirRegresos) {
            secuenciaSalida.push_back(std::make_pair(base.obtenerX(), base.obtenerY()));
        }
        return;
    }

    // copia de usados local 
    std::vector<bool> usado(m, false);


    while (true) {
        // verificar si quedan sin usar
        bool queda = false;
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            if (!usado[i]) {
                queda = true;
                cnt++;
            }
        }
        if (!queda) break;

        // construir viaje
        // 1) inicio
        int inicio = -1;
        double mejorD = 0.0;
        for (int i = 0; i < m; i++) {
            if (!usado[i]) {
                double d = distPB[i];
                if (inicio == -1 || d < mejorD) {
                    inicio = i;
                    mejorD = d;
                }
            }
        }
        if (inicio == -1) break;

        std::vector<int> seleccion;
        seleccion.reserve(k);
        seleccion.push_back(inicio);
        usado[inicio] = true;

        // 2) crecer con vecino mas cercano
        while ((int)seleccion.size() < k) {
            int ultimo = seleccion[(int)seleccion.size() - 1];
            int prox = -1;
            double proxD = 0.0;
            for (int i = 0; i < m; i++) {
                if (!usado[i] && i != ultimo) {
                    double d = distPP[ultimo][i];
                    if (prox == -1 || d < proxD) {
                        prox = i;
                        proxD = d;
                    }
                }
            }
            if (prox == -1) break;
            seleccion.push_back(prox);
            usado[prox] = true;
        }

        // 3) volcar a la secuencia con (0,0) al inicio y fin del viaje
        if (incluirRegresos) {
            secuenciaSalida.push_back(std::make_pair(base.obtenerX(), base.obtenerY()));
        }
        for (int t = 0; t < (int)seleccion.size(); t++) {
            int idx = seleccion[t];
            secuenciaSalida.push_back(std::make_pair(productos[idx].obtenerX(), productos[idx].obtenerY()));
        }
        if (incluirRegresos) {
            secuenciaSalida.push_back(std::make_pair(base.obtenerX(), base.obtenerY()));
        }
    }
}
