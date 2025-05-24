#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class enemigo {
    private:
        string nombre;
        int vida;
        int ataque;
        float precision;
        int velocidad;
        float probabilidad;
    public:
        enemigo(string n = "", int v = 0, int a = 0, float p = 0.0f, int vel = 0, float prob = 0.0f)
            : nombre(n), vida(v), ataque(a), precision(p), velocidad(vel), probabilidad(prob) {}

        string getnombre() const { return nombre; }
        int getVida() const { return vida; }
        int getAtaque() const { return ataque; }
        float getPrecision() const { return precision; }
        int getVelocidad() const { return velocidad; }
        float getProbabilidad() const { return probabilidad; }

        void setVida(int v) { vida = v; }

        void recibirDano(int dano) { 
            vida -= dano; 
            if (vida < 0) vida = 0;
        }
        
        bool estaMuerto() const { return vida <= 0; }

        void mostrarEstado() const {
            cout << nombre << " - Vida: " << vida 
                 << " | Ataque: " << ataque 
                 << " | Precisión: " << precision * 100 << "%"
                 << " | Velocidad: " << velocidad << endl;

        }
};

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

    void atacar(enemigo& e) const {
        if ((rand() % 100) < (precision * 100)) {
            e.recibirDano(ataque);
            cout << "¡Ataque exitoso! Causaste " << ataque << " de daño.\n";
        } else {
            cout << "¡Fallaste el ataque!\n";
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

void combatir(Jugador& jugador, enemigo& rival) {
    cout << "\n--- COMBATE INICIADO ---\n";
    jugador.mostrarEstado();
    rival.mostrarEstado();

    while (jugador.estaVivo() && !rival.estaMuerto()) {
        if (jugador.getVelocidad() >= rival.getVelocidad()) {
            cout << "\nJugador ataca primero.\n";

            if ((rand() % 100) < (jugador.getPrecision() * 100)) {
                rival.recibirDano(jugador.getAtaque());
                cout << "Atacas a " << rival.getnombre() << ". ";
                cout << rival.getnombre() << " recibe " << jugador.getAtaque() << " de daño.\n";
            } else {
                cout << rival.getnombre() << " esquiva tu ataque.\n";
            }

            if (!rival.estaMuerto()) {
                if ((rand() % 100) < (rival.getPrecision() * 100)) {
                    jugador.recibirDano(rival.getAtaque());
                    cout << rival.getnombre() << " te golpea. ";
                    cout << "Recibes " << rival.getAtaque() << " de daño.\n";
                } else {
                    cout << "Esquivaste el ataque de " << rival.getnombre() << ".\n";
                }
            }

        } else {
            cout << "\n" << rival.getnombre() << " ataca primero.\n";

            if ((rand() % 100) < (rival.getPrecision() * 100)) {
                jugador.recibirDano(rival.getAtaque());
                cout << rival.getnombre() << " te golpea. ";
                cout << "Recibes " << rival.getAtaque() << " de daño.\n";
            } else {
                cout << "Esquivaste el ataque de " << rival.getnombre() << ".\n";
            }

            if (jugador.estaVivo()) {
                if ((rand() % 100) < (jugador.getPrecision() * 100)) {
                    rival.recibirDano(jugador.getAtaque());
                    cout << "Atacas a " << rival.getnombre() << ". ";
                    cout << rival.getnombre() << " recibe " << jugador.getAtaque() << " de daño.\n";
                } else {
                    cout << rival.getnombre() << " esquiva tu ataque.\n";
                }
            }
        }

        cout << "\nEstado actual:\n";
        jugador.mostrarEstado();
        rival.mostrarEstado();
    }

    if (jugador.estaVivo()) {
        cout << "\n¡Ganaste el combate!\n";
    } else {
        cout << "\nHas sido derrotado...\n";
    }
}
