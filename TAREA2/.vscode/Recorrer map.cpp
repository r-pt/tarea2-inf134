#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;

// Estructura para una habitación
struct NodoHabitacion {
    int id;
    char nombre[64];
    char descripcion[256];
    char tipo[16]; // INICIO, FIN, EVENTO, COMBATE
    NodoHabitacion* izquierda; // Primera conexión
    NodoHabitacion* centro;    // Segunda conexión
    NodoHabitacion* derecha;   // Tercera conexión

    NodoHabitacion(int id, const char* nombre, const char* descripcion, const char* tipo)
        : id(id), izquierda(nullptr), centro(nullptr), derecha(nullptr) {
        strncpy(this->nombre, nombre, 63);
        strncpy(this->descripcion, descripcion, 255);
        strncpy(this->tipo, tipo, 15);
    }
};

// Estructura para un enemigo
struct NodoEnemigo {
    char nombre[64];
    int vida;
    int ataque;
    int velocidad;
    float precision;
    float probabilidad;
    NodoEnemigo* siguiente;

    NodoEnemigo(const char* nombre, int vida, int ataque, int velocidad, float precision, float probabilidad)
        : vida(vida), ataque(ataque), velocidad(velocidad), precision(precision), probabilidad(probabilidad), siguiente(nullptr) {
        strncpy(this->nombre, nombre, 63);
    }
};

// Estructura para un evento
struct NodoEvento {
    char nombre[64];
    float probabilidad;
    char descripcion[128];
    char opcionA[64];
    char efectoA[32];
    char opcionB[64];
    char efectoB[32];
    NodoEvento* siguiente;

    NodoEvento(const char* nombre, float probabilidad, const char* descripcion, const char* opcionA, const char* efectoA, const char* opcionB, const char* efectoB)
        : probabilidad(probabilidad), siguiente(nullptr) {
        strncpy(this->nombre, nombre, 63);
        strncpy(this->descripcion, descripcion, 127);
        strncpy(this->opcionA, opcionA, 63);
        strncpy(this->efectoA, efectoA, 31);
        strncpy(this->opcionB, opcionB, 63);
        strncpy(this->efectoB, efectoB, 31);
    }
};

// Clase para el jugador
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
        : vida(v), ataque(a), precision(p), recuperacion(r), velocidad(vel) {}

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

    void aplicarMejora(int opcion) {
        switch(opcion) {
            case 1: vida = min(vida + 5, vidaMaxima); break;
            case 2: precision = min(precision + 0.1f, 1.0f); break;
            case 3: ataque++; break;
            case 4: recuperacion++; break;
            case 5: velocidad++; break;
            default: cout << "Opción inválida." << endl; break;
        }
    }

    void recibirDano(int dano) {
        vida -= dano;
        if (vida < 0) vida = 0;
    }

    bool estaVivo() const {
        return vida > 0;
    }
};

// Función para leer el archivo .map
void leerMapa(const char* nombreArchivo, NodoHabitacion*& raizHabitaciones, NodoEnemigo*& listaEnemigos, NodoEvento*& listaEventos) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea, seccion;
    NodoHabitacion* ultimaHabitacion = nullptr;
    NodoEnemigo* ultimoEnemigo = nullptr;
    NodoEvento* ultimoEvento = nullptr;

    while (getline(archivo, linea)) {
        // Eliminar espacios en blanco al inicio y al final de la línea
        while (!linea.empty() && isspace(linea[0])) linea.erase(0, 1);
        while (!linea.empty() && isspace(linea.back())) linea.pop_back();
        if (linea.empty()) continue;

        if (linea == "HABITACIONES") {
            int cantidad;
            archivo >> cantidad;
            if (archivo.fail() || cantidad <= 0) {
                cout << "Error al leer la cantidad de habitaciones.\n";
                return;
            }
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                int id;
                char nombre[64], tipo[16], descripcion[256];
                archivo >> id;
                if (archivo.fail()) {
                    cout << "Error al leer el ID de la habitación.\n";
                    return;
                }
                archivo.ignore();
                archivo.getline(nombre, 64, '(');
                archivo.getline(tipo, 16, ')');
                archivo.ignore();
                archivo.getline(descripcion, 256);

                NodoHabitacion* nuevaHabitacion = new NodoHabitacion(id, nombre, descripcion, tipo);
                if (!raizHabitaciones) {
                    raizHabitaciones = nuevaHabitacion;
                } else {
                    ultimaHabitacion->derecha = nuevaHabitacion;
                }
                ultimaHabitacion = nuevaHabitacion;
            }
        } else if (linea == "ENEMIGOS") {
            int cantidad;
            archivo >> cantidad;
            if (archivo.fail() || cantidad <= 0) {
                cout << "Error al leer la cantidad de enemigos.\n";
                return;
            }
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                char nombre[64];
                int vida, ataque, velocidad;
                float precision, probabilidad;
                archivo.getline(nombre, 64, '|');
                archivo >> vida >> ataque >> velocidad >> precision >> probabilidad;
                if (archivo.fail()) {
                    cout << "Error al leer los atributos del enemigo.\n";
                    return;
                }
                archivo.ignore();

                NodoEnemigo* nuevoEnemigo = new NodoEnemigo(nombre, vida, ataque, velocidad, precision, probabilidad);
                if (!listaEnemigos) {
                    listaEnemigos = nuevoEnemigo;
                } else {
                    ultimoEnemigo->siguiente = nuevoEnemigo;
                }
                ultimoEnemigo = nuevoEnemigo;
            }
        } else if (linea == "EVENTOS") {
            int cantidad;
            archivo >> cantidad;
            if (archivo.fail() || cantidad <= 0) {
                cout << "Error al leer la cantidad de eventos.\n";
                return;
            }
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                char nombre[64], descripcion[128], opcionA[64], efectoA[32], opcionB[64], efectoB[32];
                float probabilidad;
                archivo.getline(nombre, 64);
                archivo >> probabilidad;
                if (archivo.fail() || probabilidad < 0.0f || probabilidad > 1.0f) {
                    cout << "Error al leer la probabilidad del evento.\n";
                    return;
                }
                archivo.ignore();
                archivo.getline(descripcion, 128);
                archivo.getline(opcionA, 64);
                archivo.getline(efectoA, 32);
                archivo.getline(opcionB, 64);
                archivo.getline(efectoB, 32);

                NodoEvento* nuevoEvento = new NodoEvento(nombre, probabilidad, descripcion, opcionA, efectoA, opcionB, efectoB);
                if (!listaEventos) {
                    listaEventos = nuevoEvento;
                } else {
                    ultimoEvento->siguiente = nuevoEvento;
                }
                ultimoEvento = nuevoEvento;
            }
        }
    }

    archivo.close();
}
