//------------------------------------------------------------------------------
// File:   practica_4.cpp
// Author: Inés Román Gracia
//         Francisco Javier Pizarro Martínez
// Date:   10/11/2021
// Coms:   Código principal del ejercicio a desarrollar de la práctica 4 de pscd
//         Implementación del mismo problema que la práctica anterior usando
//         monitores
//         Compilar mediante
//         make -f Makefile_p4
//------------------------------------------------------------------------------

//INCLUIMOS LAS LIBRERIAS
#include <thread>
#include <cstdlib>
//Módulos
#include "ControlCabinas.hpp"
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger _logger("_log_.log", 1024);
    const string nada("");
    #define ADD_EVENT(e) {_logger.addMessage(nada+e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif
//Función que ejecuta cada uno de los procesos usuarios, necesita el monitor y
//además tiene un parámetro adicional que le indica su número de usuario
void usuario(monitorCabinas& mon, int n) {
    ADD_EVENT("USER_BEGIN_" + to_string(n));
    int nCab;
    for(int i = 0; i < N_TIMES_USER; i++) {
        ADD_EVENT("USER_WAIT_" + to_string(n) + "," + to_string(i));
        mon.entrarCabina(n,i,nCab);
        //Sección crítica

        this_thread::sleep_for(chrono::milliseconds((rand()%31)+10));
        //Salida de la sección crítica
        mon.salirCabina(n, i , nCab);
        this_thread::sleep_for(chrono::milliseconds((rand()%31)+20));
    }
    ADD_EVENT("USER_END_" + to_string(n));
}

//Función que ejecuta el proceso limpiador, necesita el monitor
void cleaning(monitorCabinas& mon) {
    ADD_EVENT("CLEANING_BEGIN");
    for(int i = 0; i < N_TIMES_CLEANING; i++) {
        this_thread::sleep_for(chrono::milliseconds(PER_CLEANING));
        ADD_EVENT("CLEANING_WAIT" + "," + to_string(i));
        mon.empezarLimpiar(i);
        //Sección critica
        this_thread::sleep_for(chrono::milliseconds((rand()%41)+80));
        mon.acabarLimpiar(i);
        //Salida de la sección crítica
        }
    ADD_EVENT("CLEANING_END");
}

int main() {
    ADD_EVENT("MAIN_BEGIN");
    //Declaración de variables
    monitorCabinas mon;   
    srand(time(nullptr));
    //Declaración e inicialización de los procesos
    thread user[N_USER], limpiador;
    limpiador = thread(&cleaning, ref(mon));
    for(int i = 0; i < N_USER; i++) {
        user[i] = thread(&usuario, ref(mon), i);
    }
    //Esperamos a que todos los procesos acaben
    limpiador.join();
    for(int i = 0; i < N_USER; i++) {
        user[i].join();
    }
    ADD_EVENT("MAIN_END");
    return 0;
}