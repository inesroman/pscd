//------------------------------------------------------------------------------
// File:   main_p2_e1.cpp
// Author: Inés Román Gracia
// Date:   14/10/21
// Coms:   Ejercicio 1 de la práctica 2 de PSCD
//         Compilar mediante:
//           g++ main_p2_e1.cpp -o main_p2_e1 -pthread -std=c++11
//------------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <thread>
#include <string>

const int N = 512;  // número de componentes del vector 
const int N_BUSC = 8; // número de procesos buscadores
using VectInt = int[N];
using namespace std;


void search(const VectInt v, const int i, const int d, const int value, int& result){
    result = 0;
    for(int j = i; j < d; j++){
        if(v[j] == value){
           result++; 
        }
    }
}

void coordinador(VectInt v, bool& enable, int result[N_BUSC], bool acabado[N_BUSC]){
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
    
    /* Inicializamos a 0 el vector de resultados y a false el vector 
    de booleanos que nos indica que los procesos buscadores acaban */
    for(int i = 0; i < N_BUSC; i++){
        result[i] = 0;
        acabado[i] = false;
    }

    // Con el vector de enteros leídos del fichero listo, los procesos buscadores acaban la espera:
    enable = true;

    // Calculamos el resultado final sumando los resultados de cada proceso cuando hayan acabado:
    int resf = 0;
    for(int i = 0; i < N_BUSC; i++){
        while(!acabado[i]){ // espera a que acaben todos los procesos
        }
        resf += result[i];
    }
    cout << "El resultado es: " << resf << endl;
}

void espera(const VectInt v, const int i, const int d, const int value, int& result, bool& enable, bool& acabado){
    // Esperamos a tener el vector de enteros leídos del fichero:
    while(!enable){ 
    }

    // Cuando el vector esté listo, comienza la búsqueda:
    search(v, i, d, value, result);
    acabado = true;
}

int main(){
    // Declaración de las variables:
    VectInt v; // enteros del fichero
    int result[N_BUSC]; // vector con los resultados de cada proceso
    bool acabado[N_BUSC]; // indica que los procesos han acabado
    thread C, P[N_BUSC];
    bool enable = false; // cuando vale true comienzan los procesos buscadores
    int valor; // Obtenemos el valor del que calculamos las repeticiones a través de teclado
    cout << "Introduzca el valor a buscar:" << endl;
    cin >> valor;
    
    //Se ponen en marcha los procesos:
    C = thread(&coordinador, ref(v), ref(enable), ref(result), ref(acabado)); // proceso coordinador
    for(int i = 0; i < N_BUSC; i++){ // procesos buscadores
        P[i] = thread(&espera, ref(v), N/(N_BUSC)*i, N/(N_BUSC)*i + N/(N_BUSC), ref(valor), ref(result[i]), ref(enable), ref(acabado[i]));
    }

    // Terminan los procesos:
    C.join();
    for(int i = 0; i < N_BUSC; i++){
        P[i].join();
    }
    return 0;
}