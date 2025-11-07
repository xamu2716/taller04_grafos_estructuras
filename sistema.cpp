#include "sistema.h"
#include <cmath>
#include <cstddef>

Sistema::Sistema() : base(Nodo(-1, 0.0, 0.0)), k(1) {}

void Sistema::cargarEscenario(const std::vector< std::pair<double,double> >& coords,
                              int capacidad) {
    productos.clear();
    distPP.clear();
    distPB.clear();
    k = (capacidad > 0) ? capacidad : 1;

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
            double d = std::sqrt(dx * dx + dy * dy);
            distPP[i][j] = d;
            distPP[j][i] = d;
        }
    }
}

double Sistema::distProductoProducto(int a, int b) const {
    if (a < 0 || b < 0) return 0.0;
    if (a >= (int)distPP.size() || b >= (int)distPP.size()) return 0.0;
    return distPP[a][b];
}

double Sistema::distProductoBase(int idx) const {
    if (idx < 0 || idx >= (int)distPB.size()) return 0.0;
    return distPB[idx];
}

void Sistema::construirViajePrim(std::vector<int>& seleccion,
                                 const std::vector<bool>& usadoGlobal) const {
    seleccion.clear();

    int m = (int)productos.size();

    // Contar productos pendientes (no atendidos globalmente).
    int pendientes = 0;
    for (int i = 0; i < m; i++) {
        if (!usadoGlobal[i]) {
            pendientes++;
        }
    }
    if (pendientes == 0) {
        return;
    }

    // No podemos tomar más de k productos en este viaje.
    int capacidadViaje = (k < pendientes) ? k : pendientes;

    const double INF = 1e18;

    std::vector<double> key(m, INF);
    std::vector<int> parent(m, -1);
    std::vector<bool> enMST(m, false); // productos ya agregados en el MST de este viaje.

    // Inicialización según Prim(G={V,E}, x):
    for (int i = 0; i < m; i++) {
        if (!usadoGlobal[i]) {
            key[i] = distProductoBase(i);
            parent[i] = -1;
        }
    }

    int agregados = 0;

    while (agregados < capacidadViaje) {
        // Escoger el vértice v (producto) con key mínima
        // entre los no usados globalmente y no en el MST de este viaje.
        int v = -1;
        double mejorKey = INF;

        for (int i = 0; i < m; i++) {
            if (!usadoGlobal[i] && !enMST[i]) {
                if (key[i] < mejorKey) {
                    mejorKey = key[i];
                    v = i;
                }
            }
        }

        if (v == -1) {
            // No hay candidato válido (no debería ocurrir en grafo completo).
            break;
        }

        // Agregar v al MST parcial del viaje.
        enMST[v] = true;
        agregados++;

        // Actualizar claves usando la arista {v, w}.
        for (int w = 0; w < m; w++) {
            if (!usadoGlobal[w] && !enMST[w] && w != v) {
                double peso = distProductoProducto(v, w);
                if (peso < key[w]) {
                    key[w] = peso;
                    parent[w] = v;
                }
            }
        }
    }

    if (agregados == 0) {
        return;
    }

    int baseIdx = m;
    std::vector< std::vector<int> > adj(m + 1);

    for (int i = 0; i < m; i++) {
        if (!enMST[i]) {
            continue;
        }

        int p = parent[i];
        if (p == -1) {
            // Conectado directamente a la base.
            adj[baseIdx].push_back(i);
            adj[i].push_back(baseIdx);
        } else if (p >= 0 && p < m && enMST[p]) {
            // Conectado a otro producto del MST parcial.
            adj[p].push_back(i);
            adj[i].push_back(p);
        }
    }

    // Recorrer el árbol en preorden desde la base
    // para determinar el orden de visita del viaje.
    std::vector<bool> visit(m + 1, false);
    std::vector<int> stack;

    visit[baseIdx] = true;
    stack.push_back(baseIdx);

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();

        const std::vector<int>& vecinos = adj[u];
        for (int i = (int)vecinos.size() - 1; i >= 0; i--) {
            int v = vecinos[i];
            if (!visit[v]) {
                visit[v] = true;
                if (v != baseIdx) {
                    // En preorden: añadimos el producto apenas se descubre.
                    seleccion.push_back(v);
                }
                stack.push_back(v);
            }
        }
    }
}

void Sistema::resolver(std::vector< std::pair<double,double> >& secuenciaSalida,
                       bool incluirRegresos) const {
    secuenciaSalida.clear();

    int m = (int)productos.size();

    if (m == 0) {
        // Escenario sin productos: opcionalmente solo base.
        if (incluirRegresos) {
            secuenciaSalida.push_back(
                std::make_pair(base.obtenerX(), base.obtenerY()));
        }
        return;
    }

    std::vector<bool> usadoGlobal(m, false);

    while (true) {
        bool quedaPendiente = false;
        for (int i = 0; i < m; i++) {
            if (!usadoGlobal[i]) {
                quedaPendiente = true;
                break;
            }
        }

        if (!quedaPendiente) {
            break; // todos los productos atendidos
        }

        // Construir un viaje usando Prim modificado.
        std::vector<int> seleccion;
        construirViajePrim(seleccion, usadoGlobal);

        if (seleccion.empty()) {
            // Seguridad para evitar bucles infinitos en caso patológico.
            break;
        }

        // Volcar viaje:
        // base -> productos seleccionados -> base
        if (incluirRegresos) {
            secuenciaSalida.push_back(
                std::make_pair(base.obtenerX(), base.obtenerY()));
        }

        for (std::size_t i = 0; i < seleccion.size(); i++) {
            int idx = seleccion[i];
            if (idx >= 0 && idx < m && !usadoGlobal[idx]) {
                secuenciaSalida.push_back(
                    std::make_pair(productos[idx].obtenerX(),
                                   productos[idx].obtenerY()));
                usadoGlobal[idx] = true;
            }
        }

        if (incluirRegresos) {
            secuenciaSalida.push_back(
                std::make_pair(base.obtenerX(), base.obtenerY()));
        }
    }
}
