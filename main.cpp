#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include "sistema.h"

struct Escenario {
    int m;
    std::vector< std::pair<double,double> > coords;
};

int main() {
    int opcion = 0;

    int N = 0;
    int k = 0;
    std::vector<Escenario> escenarios;

    bool resuelto = false;
    std::vector< std::vector< std::pair<double,double> > > salidas;

    do {
        std::cout << "===== MENU PRINCIPAL =====\n";
        std::cout << "1. Cargar datos desde archivo\n";
        std::cout << "2. Resolver ruta\n";
        std::cout << "3. Mostrar resultado en pantalla\n";
        std::cout << "4. Guardar resultado en archivo\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        if (opcion == 1) {
            std::string nombre;
            std::cout << "Nombre del archivo de entrada: ";
            std::cin >> nombre;

            std::ifstream in(nombre.c_str());
            if (!in) {
                std::cout << "No se pudo abrir el archivo.\n";
                continue;
            }

            if (!(in >> N)) { std::cout << "Error leyendo N.\n"; continue; }
            if (!(in >> k)) { std::cout << "Error leyendo k.\n"; continue; }

            escenarios.clear();
            for (int i = 0; i < N; i++) {
                Escenario e;
                if (!(in >> e.m)) { std::cout << "Error leyendo m.\n"; escenarios.clear(); break; }
                e.coords.clear();
                for (int j = 0; j < e.m; j++) {
                    double x, y;
                    if (!(in >> x >> y)) {
                        std::cout << "Error leyendo coordenadas.\n";
                        escenarios.clear();
                        break;
                    }
                    e.coords.push_back(std::make_pair(x, y));
                }
                if ((int)e.coords.size() != e.m) { escenarios.clear(); break; }
                escenarios.push_back(e);
            }

            if (N > 0 && (int)escenarios.size() == N) {
                resuelto = false;
                salidas.clear();
                std::cout << "Archivo cargado correctamente.\n";
            } else {
                std::cout << "Error al cargar los datos.\n";
                N = 0;
                k = 0;
            }
        }
        else if (opcion == 2) {
            if (N <= 0 || (int)escenarios.size() != N) {
                std::cout << "Primero cargue un archivo valido.\n";
                continue;
            }

            salidas.clear();
            salidas.resize(N);

            for (int i = 0; i < N; i++) {
                Sistema sis;
                sis.cargarEscenario(escenarios[i].coords, k);
                sis.resolver(salidas[i], true); // incluir regresos a base
            }

            resuelto = true;
            std::cout << "Escenarios resueltos.\n";
        }
        else if (opcion == 3) {
            if (!resuelto) {
                std::cout << "Primero resuelva (opcion 2).\n";
                continue;
            }

            std::cout << N << "\n";
            std::cout << k << "\n";
            for (int i = 0; i < N; i++) {
                std::cout << escenarios[i].m << "\n";
                for (int t = 0; t < (int)salidas[i].size(); t++) {
                    std::cout << salidas[i][t].first << " " << salidas[i][t].second << "\n";
                }
            }
        }
        else if (opcion == 4) {
            if (!resuelto) {
                std::cout << "Primero resuelva (opcion 2).\n";
                continue;
            }

            std::string nombre;
            std::cout << "Nombre del archivo de salida: ";
            std::cin >> nombre;

            std::ofstream out(nombre.c_str());
            if (!out) {
                std::cout << "No se pudo abrir el archivo de salida.\n";
                continue;
            }

            out << N << "\n";
            out << k << "\n";
            for (int i = 0; i < N; i++) {
                out << escenarios[i].m << "\n";
                for (int t = 0; t < (int)salidas[i].size(); t++) {
                    out << salidas[i][t].first << " " << salidas[i][t].second << "\n";
                }
            }
            std::cout << "Salida guardada correctamente.\n";
        }
        else if (opcion != 5) {
            std::cout << "Opcion invalida.\n";
        }

    } while (opcion != 5);

    std::cout << "Saliendo...\n";
    return 0;
}
