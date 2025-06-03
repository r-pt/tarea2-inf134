#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>   // Para rand(), srand(), atoi(), atof()
#include <cstring>
#include <cctype>
#include <ctime>     // Para time()
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
        cout << "Tu Estado:\n";
        cout << "====================\n";
        cout << "Vida: " << vida << " / " << vidaMaxima << endl;
        cout << "Ataque: " << ataque << endl;
        cout << "Velocidad: " << velocidad << endl;
        cout << "Recuperación: " << recuperacion << endl;
        cout << "Precisión: " << precision * 100 << "%" << endl;
        cout << "====================\n";
    }

    void aplicarMejora(int opcion) {
// Use switch, pq segun el discord se podia
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

// Estructura auxiliar para conexiones de habitaciones (arcos)
struct Arco {
    int origen;
    int destino;
    Arco* siguiente;
    Arco(int o, int d) : origen(o), destino(d), siguiente(nullptr) {}
};

void conectarHabitaciones(Habitacion* raiz, Arco* listaArcos);

/*****
* void leerMapa
******
* Lee el archivo data.map y construye el árbol de habitaciones, la lista de enemigos y la lista de eventos.
* Es robusta a cambios de orden y formato, y soporta el formato especial de eventos separado por '&'.
******
* Input:
* const char* nombreArchivo: nombre del archivo a leer
* Habitacion*& raizHabitaciones: referencia al puntero raíz del árbol de habitaciones
* Enemigo*& listaEnemigos: referencia al puntero de la lista de enemigos
* Evento*& listaEventos: referencia al puntero de la lista de eventos
* Arco*& listaArcos: referencia al puntero de la lista de arcos
******
* Returns:
* void, modifica los punteros por referencia
*****/
void leerMapa(
    const char* nombreArchivo,
    Habitacion*& raizHabitaciones,
    Enemigo*& listaEnemigos,
    Evento*& listaEventos,
    Arco*& listaArcos
) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea, seccion;
    Habitacion* ultimaHabitacion = nullptr;
    Enemigo* ultimoEnemigo = nullptr;
    Evento* ultimoEvento = nullptr;
    Arco* ultimoArco = nullptr;

    while (getline(archivo, linea)) {
        // Eliminar espacios en blanco al inicio y al final de la línea
        while (!linea.empty() && isspace(linea[0])) linea.erase(0, 1);
        while (!linea.empty() && isspace(linea.back())) linea.pop_back();
        if (linea.empty()) continue;

        // Sección de habitaciones
        if (linea == "HABITACIONES") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                int id;
                string nombre, tipo, descripcion;
                archivo >> id;
                archivo.ignore();
                getline(archivo, nombre, '(');
                getline(archivo, tipo, ')');
                archivo.ignore();
                getline(archivo, descripcion);

                // Eliminar espacios extra
                while (!nombre.empty() && isspace(nombre.back())) nombre.pop_back();
                while (!tipo.empty() && isspace(tipo.front())) tipo.erase(0, 1);
                while (!tipo.empty() && isspace(tipo.back())) tipo.pop_back();
                while (!descripcion.empty() && isspace(descripcion.front())) descripcion.erase(0, 1);

                // Crear nueva habitación y enlazarla en una lista temporal
                Habitacion* nueva = new Habitacion(id, nombre.c_str(), descripcion.c_str(), tipo.c_str());
                if (!raizHabitaciones) {
                    raizHabitaciones = nueva; // La primera habitación será la raíz del árbol
                } else {
                    ultimaHabitacion->derecha = nueva; // Se enlazan temporalmente por derecha
                }
                ultimaHabitacion = nueva;
            }
        }
        // Sección de arcos (conexiones entre habitaciones)
        else if (linea == "ARCOS") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                string arcoLinea;
                getline(archivo, arcoLinea);
                // Formato: origen -> destino
                size_t flecha = arcoLinea.find("->");
                if (flecha != string::npos) {
                    int origen = stoi(arcoLinea.substr(0, flecha));
                    int destino = stoi(arcoLinea.substr(flecha + 2));
                    Arco* nuevoArco = new Arco(origen, destino);
                    if (!listaArcos) {
                        listaArcos = nuevoArco;
                    } else {
                        ultimoArco->siguiente = nuevoArco;
                    }
                    ultimoArco = nuevoArco;
                }
            }
        }
        // Sección de enemigos
        else if (linea == "ENEMIGOS") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                string enemigoLinea;
                getline(archivo, enemigoLinea);
                cout << "Leyendo enemigo: '" << enemigoLinea << "'" << endl;
                 if (enemigoLinea.empty()) {
                     cout << "Línea vacía detectada, se omite." << endl; // <-- AÑADE ESTA LÍNEA
                     continue;
               }

                // Formato: Nombre | Vida X | Ataque Y | Velocidad Z| Precision K | Probabilidad P
                stringstream ss(enemigoLinea);
                string nombre, vidaStr, ataqueStr, velocidadStr, precisionStr, probabilidadStr;
                getline(ss, nombre, '|');
                getline(ss, vidaStr, '|');
                getline(ss, ataqueStr, '|');
                getline(ss, velocidadStr, '|');
                getline(ss, precisionStr, '|');
                getline(ss, probabilidadStr, '|');

                // Extraer valores numéricos de cada stat
                // Función auxiliar para extraer el número al final del string (sin istringstream)
                // Tuve que añadir esto porque seguía dando error al leer el valor de cada stat desde el archivo .map
                auto extraerNumero = [](const string& str) -> string {
                    size_t i = 0;
                    while (i < str.size() && !isdigit(str[i]) && str[i] != '-' && str[i] != '+') ++i;
                    if (i == str.size()) return "";
                    return str.substr(i);
                };

                string vidaNum = extraerNumero(vidaStr);
                string ataqueNum = extraerNumero(ataqueStr);
                string velocidadNum = extraerNumero(velocidadStr);
                string precisionNum = extraerNumero(precisionStr);
                string probabilidadNum = extraerNumero(probabilidadStr);

                int vida = stoi(vidaNum);
                int ataque = stoi(ataqueNum);
                int velocidad = stoi(velocidadNum);
                float precision = stof(precisionNum);
                float probabilidad = stof(probabilidadNum);
                // Eliminar espacios demás 
                while (!nombre.empty() && isspace(nombre.back())) nombre.pop_back();

                // Ahora sí, crea el enemigo con la velocidad correcta, eso faltaba antes, lo tomaba como 0 
                Enemigo* nuevoEnemigo = new Enemigo(nombre.c_str(), vida, ataque, velocidad, precision, probabilidad);
                if (!listaEnemigos) {
                    listaEnemigos = nuevoEnemigo;
                } else {
                    ultimoEnemigo->siguiente = nuevoEnemigo;
                }
                ultimoEnemigo = nuevoEnemigo;
            }
        }
        // Sección de eventos
        else if (linea == "EVENTOS") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            for (int i = 0; i < cantidad; ++i) {
                // Cada evento comienza con un '&'
                string separador;
                getline(archivo, separador);
                if (separador != "&") continue; // Saltar si no es un evento válido

                string nombre, probabilidadStr, descripcion, opcionA, efectoA, opcionB, efectoB;
                getline(archivo, nombre);
                getline(archivo, probabilidadStr);
                getline(archivo, descripcion);
                getline(archivo, opcionA);
                getline(archivo, efectoA);
                getline(archivo, opcionB);
                getline(archivo, efectoB);

                // Extraer probabilidad
                float probabilidad = 0.0f;
                size_t pos = probabilidadStr.find(' ');
                if (pos != string::npos)
                    probabilidad = stof(probabilidadStr.substr(pos + 1));

                Evento* nuevoEvento = new Evento(
                    nombre.c_str(), probabilidad, descripcion.c_str(),
                    opcionA.c_str(), efectoA.c_str(), opcionB.c_str(), efectoB.c_str()
                );
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
    // Conectar habitaciones según los arcos
    if (listaArcos) {
        conectarHabitaciones(raizHabitaciones, listaArcos);
    }
}

// Busca una habitación por ID en el árbol ternario
// Se usa para encontrar el nodo de origen y destino al conectar habitaciones
Habitacion* buscarHabitacion(Habitacion* raiz, int id) {
    if (!raiz) return nullptr;
    if (raiz->id == id) return raiz;
    Habitacion* encontrada = buscarHabitacion(raiz->izquierda, id);
    if (encontrada) return encontrada;
    encontrada = buscarHabitacion(raiz->centro, id);
    if (encontrada) return encontrada;
    return buscarHabitacion(raiz->derecha, id);
}

// Conecta las habitaciones usando la lista de arcos para formar el árbol ternario
// Cada habitación puede tener hasta 3 hijos: izquierda, centro y derecha
void conectarHabitaciones(Habitacion* raiz, Arco* listaArcos) {
    while (listaArcos) {
        Habitacion* origen = buscarHabitacion(raiz, listaArcos->origen);
        Habitacion* destino = buscarHabitacion(raiz, listaArcos->destino);
        if (origen && destino) {
            // Inserta destino en el primer puntero libre (izquierda, centro, derecha)
            if (!origen->izquierda)
                origen->izquierda = destino;
            else if (!origen->centro)
                origen->centro = destino;
            else if (!origen->derecha)
                origen->derecha = destino;
            // Si ya tiene 3 hijos, no conecta más
        }
        listaArcos = listaArcos->siguiente;
    }
}
// -------------------- Función para mostrar las habitaciones --------------------


void mostrarHabitaciones(Habitacion* h) {
    if (!h) return;
    cout << "ID: " << h->id << " | Nombre: " << h->nombre << " | Tipo: " << h->tipo << endl;
    mostrarHabitaciones(h->izquierda);
    mostrarHabitaciones(h->centro);
    mostrarHabitaciones(h->derecha);
}

// -------------------- MAIN --------------------
int main() {
    // srand(time(0)) inicializa la semilla del generador de números aleatorios.
    // Esto es fundamental para que cada vez que ejecutes el programa, rand() produzca secuencias diferentes.
    // Si no llamas a srand(time(0)), rand() siempre dará la misma secuencia de números cada vez que ejecutes el programa.
    srand(time(0));

    Habitacion* raizHabitaciones = nullptr;
    Enemigo* listaEnemigos = nullptr;
    Evento* listaEventos = nullptr;
    Arco* listaArcos = nullptr;

    // Cargar el mapa desde el archivo
    leerMapa("data.map", raizHabitaciones, listaEnemigos, listaEventos, listaArcos);

    if (!raizHabitaciones) {
        cout << "No se pudo cargar el mapa. :´[ ." << endl;
        return 1;
    }

    // Crear "jugador"
    Jugador jugador;

    // Mostrar estado inicial del pokemon que somos que no me acuerdo cual iba a ser
    jugador.mostrarEstado();

    return 0;
}

