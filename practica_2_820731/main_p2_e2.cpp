//------------------------------------------------------------------------------
// File:   main_p2_e2.cpp
// Author: Inés Román Gracia
// Date:   14/10/21
// Coms:   Ejercicio 1 de la práctica 2 de PSCD
//         Compilar mediante:
//           g++ main_p2_e2.cpp -o main_p2_e2 -pthread -std=c++11
//------------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>

const int N = 512; // número de componentes del vector
const int N_BUSC = 8; // número de procesos buscadores
using VectInt = int[N];
using namespace std;

void search(const VectInt v, const int i, const int d, const int value, int& maxVeces, int& indMin, int& indMax, atomic_flag& tas){
    // Obtenemos cuántas veces está el número en el trozo de vector:
    int veces = 0;
    for(int j = i; j < d; j++){
        if(v[j] == value){
            veces++;
        }
    }

    // Si alcanza el máximo en ese trozo , guardamos el número de repeticiones y los índices mayor y menor del trozo:
    while(tas.test_and_set()){} // se ejecuta de forma atómica
    if(veces > maxVeces){
        maxVeces = veces;
        indMin = i;
        indMax = d;
    }
    tas.clear();
}

void coordinador(VectInt v, bool& enable, bool acabado[N_BUSC]){
    // Abrimos el fichero para obtener las componentes del vector:
    ifstream f;
    f.open("datos.txt");
    int n = 0;
    if(f.is_open()){ // si el fichero se ha abierto correctamente
      while(!f.eof()){ // mientras haya datos que leer
          f >> v[n];
          n++;
    }
      f.close();
    }
    else{
      cerr << "No se ha podido abrir el fichero. " << endl;
    }

    // Inicializamos a false el vector de booleanos que nos indica que los procesos buscadores acaban
    for(int i = 0; i < N_BUSC; i++){
        acabado[i] = false;
    }

    // Con el vector de enteros leídos del fichero listo, los procesos buscadores acaban la espera:
    enable = true;

    // Espera a que acaben todos los procesos:
    for(int i = 0; i < N_BUSC; i++){
        while(!acabado[i]){
        }
    }
}

void espera(const VectInt v, const int i, const int d, const int value, int& maxVeces, int& indMin, int& indMax, atomic_flag& tas, bool& enable, bool& acabado){
    // Esperamos a tener el vector de enteros leídos del fichero:
    while(!enable){
    }

    // Cuando el vector esté listo, comienza la búsqueda:
    search(v, i, d, value,maxVeces, indMin, indMax, tas);
    acabado = true;
}

int main(){
    // Declaración de las variables:
    VectInt v; // enteros del fichero
    bool acabado[N_BUSC]; // indica que los procesos han acabado
    thread C, P[N_BUSC];
    int maxVeces = 0;
    int indMin = -1;
    int indMax = -1;
    std::atomic_flag tas = ATOMIC_FLAG_INIT; //false
    bool enable = false; // cuando vale true comienzan los procesos buscadores
    int valor; // Obtenemos el valor del que calculamos las repeticiones a través de teclado
    cout << "Introduzca el valor a buscar:" << endl;
    cin >> valor;
    
    //Se ponen en marcha los procesos:
    C = thread(&coordinador, ref(v), ref(enable), ref(acabado));
    for(int i = 0; i < N_BUSC; i++){
        P[i] = thread(&espera, ref(v), N/(N_BUSC)*i, N/(N_BUSC)*i + N/(N_BUSC), ref(valor), ref(maxVeces), ref(indMin), ref(indMax), ref(tas), ref(enable), ref(acabado[i]));
    }

    // Terminan los procesos:
    C.join();
    for(int i = 0; i < N_BUSC; i++){
        P[i].join();
    }

    cout << "Máximo de veces: " << maxVeces << endl;
    cout << "Índice mínimo: " << indMin << endl;
    cout << "Índice máximo: " << indMax << endl;

    return 0;
}