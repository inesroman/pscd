//------------------------------------------------------------------------------
// File:   ControlCabinas.cpp
// Author: Inés Román Gracia
//         Francisco Javier Pizarro Martínez
// Date:   18/11/2021
// Coms:   Implementación del código del monitor
//         Práctica 4 de pscd
//------------------------------------------------------------------------------
#include "ControlCabinas.hpp"

//Constructor de la clase
monitorCabinas::monitorCabinas() {
    //Inicializamos las variables
    nUser = 0;
    wantClean = false;
    for(int i = 0; i < N_CAB; i++) {
        cabinas[i] = false;
    }
};

//Definimos los métodos previamente declarados
void monitorCabinas::entrarCabina(int n,int j, int& cab) {
    int i;
    unique_lock<mutex> lck(mtxMonitor);
    while(!((nUser < N_CAB) && (!wantClean))) {
        esperandoCabina.wait(lck);
    }
    for(i = 0; (i < N_CAB) && (cabinas[i] == true); i++);
    cabinas[i] = true;
    cab = i;
    nUser++;
    ADD_EVENT("USER_IN_" + to_string(n) + "," + to_string(j) + "," + to_string(cab));
};

void monitorCabinas::salirCabina(int n,int j, int cab) {
    unique_lock<mutex> lck(mtxMonitor);
    nUser--;
    cabinas[cab] = false;
    if(nUser == 0) {
        esperandoLimpiar.notify_one();
    }
    else {
        esperandoCabina.notify_one();
    }
    ADD_EVENT("USER_OUT_" + to_string(n) + "," + to_string(j) + "," + to_string(cab));
};

void monitorCabinas::empezarLimpiar(int i) {
    unique_lock<mutex> lck(mtxMonitor);
    wantClean = true;
    while(nUser != 0) {
        esperandoLimpiar.wait(lck);
    }
    ADD_EVENT("CLEANING_IN" + "," + to_string(i));
};

void monitorCabinas::acabarLimpiar(int i) {
    unique_lock<mutex> lck(mtxMonitor);
    wantClean = false;
    for(int i = 0; i < N_CAB; i++) esperandoCabina.notify_one();
    ADD_EVENT("CLEANING_OUT" + "," + to_string(i));
};