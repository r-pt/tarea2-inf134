#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

<<<<<<< HEAD
// ===================
// Estructuras y límites para el .map
// ===================
struct Habitacion {
    int id;
    char nombre[64];
    char descripcion[256];
};

struct Arco {
    int origen;
    int destino;
};

struct Enemigo {
    char nombre[64];
    int vida;
    int ataque;
    int velocidad;
    float precision;
    float probabilidad;
};

struct Evento {
    char nombre[64];
    float probabilidad;
    char descripcion[128];
    char opcionA[64];
    char efectoA[32];
    char opcionB[64];
    char efectoB[32];
};

struct Mejora {
    char descripcion[64];
};

#define MAX_HABITACIONES 20
#define MAX_ARCOS 30
#define MAX_ENEMIGOS 10
#define MAX_EVENTOS 10
#define MAX_MEJORAS 10

// ===================
// Función para leer el archivo .map
// ===================
void leerMap(
    const char* nombreArchivo,
    Habitacion habitaciones[], int& numHabitaciones,
    Arco arcos[], int& numArcos,
    Enemigo enemigos[], int& numEnemigos,
    Evento eventos[], int& numEventos,
    Mejora mejoras[], int& numMejoras
) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    string seccion = "";
    int cantidad = 0;

    numHabitaciones = 0;
    numArcos = 0;
    numEnemigos = 0;
    numEventos = 0;
    numMejoras = 0;

    while (getline(archivo, linea)) {
        // Quitar espacios al inicio y fin
        while (!linea.empty() && isspace(linea[0])) linea.erase(0, 1);
        while (!linea.empty() && isspace(linea.back())) linea.pop_back();
        if (linea.empty()) continue;

        // Detectar secciones
        if (linea == "ARCOS") {
            seccion = "arcos";
            cantidad = 0;
            continue;
        }
        if (linea == "ENEMIGOS") {
            seccion = "enemigos";
            cantidad = 0;
            continue;
        }
        if (linea == "EVENTOS") {
            seccion = "eventos";
            cantidad = 0;
            continue;
        }
        if (linea == "MEJORAS DE COMBATE") {
            seccion = "mejoras";
            continue;
        }
        // Habitaciones y finales: antes de "ARCOS"
        if (seccion == "" && linea.find("->") == string::npos && linea.find("(FIN)") == string::npos && isdigit(linea[0])) {
            seccion = "habitaciones";
        }

        // Procesar cada sección
        if (seccion == "arcos") {
            if (cantidad == 0) {
                cantidad = stoi(linea);
                continue;
            }
            size_t pos = linea.find("->");
            if (pos != string::npos && numArcos < MAX_ARCOS) {
                arcos[numArcos].origen = stoi(linea.substr(0, pos));
                arcos[numArcos].destino = stoi(linea.substr(pos + 2));
                numArcos++;
            }
            cantidad--;
            if (cantidad == 0) seccion = "";
        } else if (seccion == "enemigos") {
            if (cantidad == 0) {
                cantidad = stoi(linea);
                continue;
            }
            if (numEnemigos < MAX_ENEMIGOS) {
                stringstream ss(linea);
                string nombre, temp;
                getline(ss, nombre, '|');
                strncpy(enemigos[numEnemigos].nombre, nombre.c_str(), 63);
                getline(ss, temp, '|'); enemigos[numEnemigos].vida = stoi(temp.substr(temp.find(' ')));
                getline(ss, temp, '|'); enemigos[numEnemigos].ataque = stoi(temp.substr(temp.find(' ')));
                getline(ss, temp, '|'); enemigos[numEnemigos].velocidad = stoi(temp.substr(temp.find(' ')));
                getline(ss, temp, '|'); enemigos[numEnemigos].precision = stof(temp.substr(temp.find(' ')));
                getline(ss, temp, '|'); enemigos[numEnemigos].probabilidad = stof(temp.substr(temp.find(' ')));
                numEnemigos++;
            }
            cantidad--;
            if (cantidad == 0) seccion = "";
        } else if (seccion == "eventos") {
            if (cantidad == 0) {
                cantidad = stoi(linea);
                continue;
            }
            if (linea == "&" && numEventos < MAX_EVENTOS) {
                Evento& evento = eventos[numEventos];
                getline(archivo, linea); strncpy(evento.nombre, linea.c_str(), 63);
                getline(archivo, linea); evento.probabilidad = stof(linea.substr(linea.find(' ')));
                getline(archivo, linea); strncpy(evento.descripcion, linea.c_str(), 127);
                getline(archivo, linea); strncpy(evento.opcionA, linea.substr(3).c_str(), 63);
                getline(archivo, linea); strncpy(evento.efectoA, linea.c_str(), 31);
                getline(archivo, linea); strncpy(evento.opcionB, linea.substr(3).c_str(), 63);
                getline(archivo, linea); strncpy(evento.efectoB, linea.c_str(), 31);
                numEventos++;
                cantidad--;
                if (cantidad == 0) seccion = "";
            }
        } else if (seccion == "mejoras") {
            if (numMejoras < MAX_MEJORAS) {
                strncpy(mejoras[numMejoras].descripcion, linea.c_str(), 63);
                numMejoras++;
            }
        } else if (seccion == "habitaciones") {
            if (isdigit(linea[0]) && numHabitaciones < MAX_HABITACIONES) {
                Habitacion& hab = habitaciones[numHabitaciones];
                stringstream ss(linea);
                ss >> hab.id;
                string nombre;
                getline(ss, nombre);
                strncpy(hab.nombre, nombre.c_str(), 63);
                getline(archivo, linea);
                strncpy(hab.descripcion, linea.c_str(), 255);
                numHabitaciones++;
            }
        }
    }
    archivo.close();
}

// ===================
// Tu clase Jugador y resto del código sigue aquí
// ===================
=======
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

>>>>>>> 6884afb4c8ca53d1ec8b890abc7d625352cfea4d
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

<<<<<<< HEAD
    // NOTA: Debes definir la clase enemigo para que esto funcione
    /*
    int atacar(enemigo& enemigo) const {
        int dano = ataque;
=======
    void atacar(enemigo& e) const {
>>>>>>> 6884afb4c8ca53d1ec8b890abc7d625352cfea4d
        if ((rand() % 100) < (precision * 100)) {
            e.recibirDano(ataque);
            cout << "¡Ataque exitoso! Causaste " << ataque << " de daño.\n";
        } else {
            cout << "¡Fallaste el ataque!\n";
        }
    }
    */

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

<<<<<<< HEAD
// ===================
// Ejemplo de uso en main
// ===================
int main() {
    Habitacion habitaciones[MAX_HABITACIONES]; int numHabitaciones;
    Arco arcos[MAX_ARCOS]; int numArcos;
    Enemigo enemigos[MAX_ENEMIGOS]; int numEnemigos;
    Evento eventos[MAX_EVENTOS]; int numEventos;
    Mejora mejoras[MAX_MEJORAS]; int numMejoras;

    leerMap("data.map", habitaciones, numHabitaciones, arcos, numArcos, enemigos, numEnemigos, eventos, numEventos, mejoras, numMejoras);

    // Aquí puedes usar los datos cargados
    cout << "Habitaciones cargadas: " << numHabitaciones << endl;
    cout << "Enemigos cargados: " << numEnemigos << endl;
    cout << "Eventos cargados: " << numEventos << endl;
    cout << "Mejoras cargadas: " << numMejoras << endl;

    // ... resto de tu juego ...
    return 0;
=======
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
>>>>>>> 6884afb4c8ca53d1ec8b890abc7d625352cfea4d
}
