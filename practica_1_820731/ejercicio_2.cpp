//------------------------------------------------------------------------------
// File:   ejercicio_2.cpp
// Author: Inés Román Gracia
// Date:   30/09/21
// Coms:   Ejercicio 2 de la práctica 1 de PSCD
//         Compilar mediante
//           g++ ejercicio_2.cpp -o ejercicio_2 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>

using namespace std;

// número de procesos
const int N = 10; 

void saludo(int id, int retardo, int veces) {
    for(int i = 1; i <= veces; i++) {
        cout << "Soy " + to_string(id) + "\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

int main(int argc, char* argv[]) {
    thread P[N];
	srand(time(nullptr));
	int retardo, veces;
	
	for (int i = 0; i < N; i++){
		retardo = (rand()%201)+100; // número aleatorio entre 100 y 300
		veces = (rand()%11)+5; // número aleatorio entre 5 y 15
		P[i] = thread(&saludo, i+1, retardo, veces); // se ponen en marcha los procesos
	}
    
    for (int i = 0; i < N; i++) {
		// acaban los procesos
         P[i].join();
    }

    cout << "Fin\n";
    return 0;
}