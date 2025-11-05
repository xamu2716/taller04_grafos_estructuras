#ifndef NODO_H
#define NODO_H

class Nodo {
private:
    int id;
    double x;
    double y;
    bool visitado;
public:
    Nodo();
    Nodo(int id_, double x_, double y_);

    int obtenerId() const;
    double obtenerX() const;
    double obtenerY() const;
    bool estaVisitado() const;

    void fijarVisitado(bool v);
    bool esBase() const; // true si id == -1
    double distanciaA(const Nodo& otro) const;
};

#endif
