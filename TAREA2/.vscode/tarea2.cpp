#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Jugador {
private:
    int vida;
    int ataque;
    float precision;
    int recuperacion;
    int velocidad;

public:
    Jugador(int v = 30, int a = 5, float p = 0.8f, int r = 3, int vel = 5)
        : vida(v), ataque(a), precision(p), recuperacion(r), velocidad (vel) {}

    int getVida() const { return vida; }
    int getAtaque() const { return ataque; }
    float getPrecision() const { return precision; }
    int getRecuperacion() const { return recuperacion; }
    int getVelocidad() const { return velocidad; }

    void setVida(int v) { vida = v; }
    void setAtaque(int a) { ataque = a; }
    void setPrecision(float p) { precision = p; }
    void setRecuperacion(int r) { recuperacion = r; }
    void setVelocidad(int vel) { velocidad = vel; }

    void aplicarMejora(int opcion) {
        switch(opcion) {
            case 1: vida += 5; break;
            case 2: precision += 0.1f; break;
            case 3: ataque += 1; break;
            case 4: recuperacion += 1; break;
            case 5: velocidad += 1; break;
        }
    }

    void mostrarEstado() const {
        cout << "Jugador - Vida: " << vida 
             << " | Ataque: " << ataque 
             << " | Velocidad: " << velocidad
             << " | Recuperación: " << recuperacion 
             << " | Precisión: " << precision * 100 << "%" << endl;
    }
};
