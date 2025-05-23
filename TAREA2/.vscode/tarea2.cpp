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
