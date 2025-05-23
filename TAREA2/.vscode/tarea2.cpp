#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Jugador {
private:
    int vida;
    int vidaMaxima = 35;
    int ataque;
    float precision;
    int recuperacion;
    int velocidad;

public:
    Jugador(int v = 30, int a = 5, float p = 0.8f, int r = 3, int vel = 5)
        : vida(v), ataque(a), precision(p), recuperacion(r), velocidad (vel) {}

    int getVida() const { return vida; }
    int getVidaMaxima() const { return vidaMaxima; }
    int getAtaque() const { return ataque; }
    float getPrecision() const { return precision; }
    int getRecuperacion() const { return recuperacion; }
    int getVelocidad() const { return velocidad; }

    void setVida(int v) { 
        if (v < 0) vida = 0;
        else if (v > vidaMaxima) vida = vidaMaxima;
        else vida = v;
    }
    void setAtaque(int a) { 
        if (a >= 0) ataque = a; 
    }
    void setPrecision(float p) { 
        if (p >= 0.0f && p <= 1.0f) precision = p; 
    }
    void setRecuperacion(int r) { 
        if (r >= 0) recuperacion = r; 
    }
    void setVelocidad(int vel) { 
        if (vel >= 0) velocidad = vel; 
    }
    void setVidaMaxima(int v) { 
        if (v >= 0) vidaMaxima = v; 
    }

    void aplicarMejora(int opcion) {
        switch(opcion) {
            case 1: setVida(vida + 5); break;
            case 2: setPrecision(precision + 0.1f); break;
            case 3: setAtaque(ataque + 1); break;
            case 4: setRecuperacion(recuperacion + 1); break;
            case 5: setVelocidad(velocidad + 1); break;
            default: cout << "Opción inválida." << endl; break;
        }
    }

    int atacar(enemigo& enemigo) const {
        int dano = ataque;
        if ((rand() % 100) < (precision * 100)) {
            enemigo.recibirDano(dano);
            return dano;
        } else {
            return 0;
        }
    }

    void recuperarVida() {
        setVida(vida + recuperacion);
    }

    void recibirDano(int dano) { 
        setVida(vida - dano);
    }

    bool estaVivo() const { 
        return vida > 0; 
    }

    void mostrarEstado() const {
        cout << "====================\n";
        cout << "Estado del Jugador:\n";
        cout << "====================\n";
        cout << "Vida: " << vida << " / " << vidaMaxima << endl;
        cout << "Ataque: " << ataque << endl;
        cout << "Velocidad: " << velocidad << endl;
        cout << "Recuperación: " << recuperacion << endl;
        cout << "Precisión: " << precision * 100 << "%" << endl;
        cout << "====================\n";
    }
};
