//------------------------------------------------------------------------------
// File:   ejercicio_3.cpp
// Author: Inés Román Gracia
// Date:   30/09/21
// Coms:   Ejercicio 2 de la práctica 1 de PSCD
//         Compilar mediante
//           g++ ejercicio_3.cpp -o ejercicio_3 -std=c++11 -pthread
//------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include <cstdlib>
#include <cmath>

using namespace std;

const int N = 100; // número de componentes del vector
const int PROC = 3; // número de procesos

void media(float v[N], float& med, bool& enable){
	
	float suma = 0.0;
	for(int i = 0; i < N; i ++){
		suma = suma + v[i];
	}
	med = suma/N;
	enable = true;
	// una vez calculada la media, podemos pasar a calcular la desviavión típica
	
}

void maxmin(float v[N], float& max, float& min){
	
	max = v[0];
	min = v[0];
	
	for(int i = 1; i < N; i++){
		
		if(v[i] > max){
			max = v[i];
		}
		if(v[i] < min){
			min = v[i];
		}
	}
}

void sigma(float v[N], float med, float& d, bool& enable){
	
	// hasta que no se calcule la media no podemos calcular la desviación típica
	while(!enable){
	}
	float suma = 0.0;
	for(int i = 0; i < N; i++){
		suma = suma + (v[i] - med)*(v[i] - med);
	}
	d = sqrt(suma/N);
	
}

int main(int argc, char* argv[]) {
	
	thread P[PROC];
	srand(time(nullptr));
	float v[N];
	float med, max, min, d;
	bool enable = false;
	
	for (int i = 0; i < N; i++){
		v[i] = (float)rand()/1000; // real aleatorio
	}
	
	P[0] = thread(&media, ref(v), ref(med), ref(enable));
	P[1] = thread(&maxmin, ref(v), ref(max), ref(min));
	P[2] = thread(&sigma, ref(v), med, ref(d), ref(enable));
	
	for(int i = 0; i < PROC; i++){
		// acaban los procesos
		P[i].join();
	}
	
	cout << "media: " << med << endl;
	cout << "max: " << max << endl;
	cout << "min: " << min << endl;
	cout << "sigma: " << d << endl;
	
    return 0;
}