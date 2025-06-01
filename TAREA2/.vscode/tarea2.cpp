#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <sstream>
using namespace std;

// Estructuras necesarias
struct Habitacion {
    int id;
    char nombre[64];
    char descripcion[256];
    char tipo[16];
    Habitacion* izquierda;
    Habitacion* centro;
    Habitacion* derecha;

    Habitacion(int id, const char* nombre, const char* descripcion, const char* tipo)
        : id(id), izquierda(nullptr), centro(nullptr), derecha(nullptr) {
        strncpy(this->nombre, nombre, 63);
        strncpy(this->descripcion, descripcion, 255);
        strncpy(this->tipo, tipo, 15);
    }
};

struct Enemigo {
    char nombre[64];
    int vida;
    int ataque;
    int velocidad;
    float precision;
    float probabilidad;
    Enemigo* siguiente;

    Enemigo(const char* nombre, int vida, int ataque, int velocidad, float precision, float probabilidad)
        : vida(vida), ataque(ataque), velocidad(velocidad), precision(precision), probabilidad(probabilidad), siguiente(nullptr) {
        strncpy(this->nombre, nombre, 63);
    }
};

struct Evento {
    char nombre[64];
    float probabilidad;
    char descripcion[128];
    char opcionA[64];
    char efectoA[32];
    char opcionB[64];
    char efectoB[32];
    Evento* siguiente;

    Evento(const char* nombre, float probabilidad, const char* descripcion, const char* opcionA, const char* efectoA, const char* opcionB, const char* efectoB)
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

struct Mejora {
    char descripcion[64];
    Mejora* siguiente;

    Mejora(const char* descripcion) : siguiente(nullptr) {
        strncpy(this->descripcion, descripcion, 63);
    }
};

struct Arco {
    int origen;
    int destino;
    Arco* siguiente;

    Arco(int origen, int destino) : origen(origen), destino(destino), siguiente(nullptr) {}
};

Habitacion* buscarHabitacion(Habitacion* raiz, int id) {
    if (!raiz) return nullptr;
    if (raiz->id == id) return raiz;

    Habitacion* encontrada = buscarHabitacion(raiz->izquierda, id);
    if (encontrada) return encontrada;

    encontrada = buscarHabitacion(raiz->centro, id);
    if (encontrada) return encontrada;

    return buscarHabitacion(raiz->derecha, id);
}

// Función para conectar habitaciones según los arcos
void conectarHabitaciones(Habitacion* raiz, Arco* listaArcos) {
    while (listaArcos) {
        Habitacion* origen = buscarHabitacion(raiz, listaArcos->origen);
        Habitacion* destino = buscarHabitacion(raiz, listaArcos->destino);

        if (origen && destino) {
            if (!origen->izquierda) {
                origen->izquierda = destino;
            } else if (!origen->centro) {
                origen->centro = destino;
            } else if (!origen->derecha) {
                origen->derecha = destino;
            } else {
                cout << "Error: No se puede conectar más de tres habitaciones desde la habitación " << origen->id << endl;
            }
        }

        listaArcos = listaArcos->siguiente;
    }
}

void leerMapa(const char* nombreArchivo, Habitacion*& raizHabitaciones, Enemigo*& listaEnemigos, Evento*& listaEventos, Mejora*& listaMejoras, Arco*& listaArcos) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    Habitacion* ultimaHabitacion = nullptr;
    Enemigo* ultimoEnemigo = nullptr;
    Evento* ultimoEvento = nullptr;
    Mejora* ultimaMejora = nullptr;
    Arco* ultimoArco = nullptr;

    while (getline(archivo, linea)) {
        // Imprimir cada línea procesada
        cout << linea << endl;

        // Eliminar espacios en blanco al inicio y al final de la línea
        while (!linea.empty() && isspace(linea[0])) linea.erase(0, 1);
        while (!linea.empty() && isspace(linea.back())) linea.pop_back();
        if (linea.empty()) continue;

        if (linea == "HABITACIONES") {
            int cantidad;
            archivo >> cantidad;
            cout << cantidad << endl; // Imprimir cantidad de habitaciones
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                getline(archivo, linea);
                cout << linea << endl; // Imprimir línea de habitación
                int id;
                char nombre[64], tipo[16];
                string descripcion;

                stringstream ss(linea);
                ss >> id;
                ss.ignore();
                ss.getline(nombre, 64, '(');
                ss.getline(tipo, 16, ')');
                ss.ignore();

                // Leer las líneas de descripción
                string descLinea;
                descripcion = "";
                while (getline(archivo, descLinea)) {
                    if (!descLinea.empty() && isdigit(descLinea[0])) {
                        archivo.seekg(-descLinea.size() - 1, ios::cur); // Retroceder para procesar esta línea en el siguiente ciclo
                        break;
                    }
                    cout << descLinea << endl; // Imprimir línea de descripción
                    descripcion += descLinea + " ";
                }

                Habitacion* nuevaHabitacion = new Habitacion(id, nombre, descripcion.c_str(), tipo);
                if (!raizHabitaciones) {
                    raizHabitaciones = nuevaHabitacion;
                } else {
                    ultimaHabitacion->derecha = nuevaHabitacion;
                }
                ultimaHabitacion = nuevaHabitacion;
            }
        } else if (linea == "ARCOS") {
            int cantidad;
            archivo >> cantidad;
            cout << cantidad << endl; // Imprimir cantidad de arcos
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                getline(archivo, linea);
                cout << linea << endl; // Imprimir línea de arco
                int origen, destino;
                char flecha[3];
                stringstream ss(linea);
                ss >> origen >> flecha >> destino;

                Arco* nuevoArco = new Arco(origen, destino);
                if (!listaArcos) {
                    listaArcos = nuevoArco;
                } else {
                    ultimoArco->siguiente = nuevoArco;
                }
                ultimoArco = nuevoArco;
            }
        } else if (linea == "ENEMIGOS") {
            int cantidad;
            archivo >> cantidad;
            cout << cantidad << endl; // Imprimir cantidad de enemigos
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                getline(archivo, linea);
                cout << linea << endl; // Imprimir línea de enemigo
                char nombre[64];
                int vida, ataque, velocidad;
                float precision, probabilidad;
                stringstream ss(linea);
                ss.getline(nombre, 64, '|');
                ss >> vida >> ataque >> velocidad >> precision >> probabilidad;

                Enemigo* nuevoEnemigo = new Enemigo(nombre, vida, ataque, velocidad, precision, probabilidad);
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
            cout << cantidad << endl; // Imprimir cantidad de eventos
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                getline(archivo, linea);
                cout << linea << endl; // Imprimir línea de evento
                char nombre[64], descripcion[128], opcionA[64], efectoA[32], opcionB[64], efectoB[32];
                float probabilidad;
                stringstream ss(linea);
                ss.getline(nombre, 64);
                ss >> probabilidad;
                ss.ignore();
                ss.getline(descripcion, 128);
                ss.getline(opcionA, 64);
                ss.getline(efectoA, 32);
                ss.getline(opcionB, 64);
                ss.getline(efectoB, 32);

                Evento* nuevoEvento = new Evento(nombre, probabilidad, descripcion, opcionA, efectoA, opcionB, efectoB);
                if (!listaEventos) {
                    listaEventos = nuevoEvento;
                } else {
                    ultimoEvento->siguiente = nuevoEvento;
                }
                ultimoEvento = nuevoEvento;
            }
        } else if (linea == "MEJORAS DE COMBATE") {
            while (getline(archivo, linea) && linea != "FIN DE ARCHIVO") {
                cout << linea << endl; // Imprimir línea de mejora
                Mejora* nuevaMejora = new Mejora(linea.c_str());
                if (!listaMejoras) {
                    listaMejoras = nuevaMejora;
                } else {
                    ultimaMejora->siguiente = nuevaMejora;
                }
                ultimaMejora = nuevaMejora;
            }
        } else if (linea == "FIN DE ARCHIVO") {
            cout << linea << endl; // Imprimir "FIN DE ARCHIVO"
        }
    }

    archivo.close();
}

void mostrarArbol(Habitacion* habitacion, int nivel = 0) {
    if (!habitacion) return;

    for (int i = 0; i < nivel; ++i) cout << "  ";
    cout << "Habitación ID: " << habitacion->id << ", Nombre: " << habitacion->nombre << ", Tipo: " << habitacion->tipo << endl;

    mostrarArbol(habitacion->izquierda, nivel + 1);
    mostrarArbol(habitacion->centro, nivel + 1);
    mostrarArbol(habitacion->derecha, nivel + 1);
}

// Función principal
int main() {
    Habitacion* raizHabitaciones = nullptr;
    Enemigo* listaEnemigos = nullptr;
    Evento* listaEventos = nullptr;
    Mejora* listaMejoras = nullptr;
    Arco* listaArcos = nullptr;

    leerMapa("data.map", raizHabitaciones, listaEnemigos, listaEventos, listaMejoras, listaArcos);
    mostrarArbol(raizHabitaciones);

    return 0;
}
