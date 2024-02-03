//------------------------------------------------------------------------------
// File:   practica_3.cpp
// Author: Inés Román Gracia
//         Francisco Javier Pizarro Martínez
// Date:   3/11/2021
// Coms:   Código principal del ejercicio a desarrollar de la práctica 3 de pscd
//         Compilar mediante
//         make -f Makefile_p3
//------------------------------------------------------------------------------

//INCLUIMOS LAS LIBRERIAS
#include <thread>
#include <cstdlib>
#include <iostream>
#include "Semaphore_V4.hpp"
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger _logger("_log_.log", 1024);
    //Logger _logger("_log_2.log", cout, 1); // si queremos echo de eventos en stdout
    string nada("");
    #define ADD_EVENT(e) {_logger.addMessage(nada+e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif
using namespace std;

//DEFINICIÓN DE VARIABLES GLOBALES
const int N_USER = 20;
const int N_TIMES_USER = 30;
const int N_TIMES_CLEANING = 5;
const int PER_CLEANING = 100;
const int N_CAB = 4;

//Función que se encarga de pasar el testigo, necesita acceso a todas las variables
void pasarTestigo(Semaphore &testigo, Semaphore &b1, Semaphore &b2,bool &want_clean,bool &clean,int &nUser, int &dc1, int &dc2) {
    if((!clean) && (nUser < N_CAB) && (!want_clean) && (dc1 > 0)){
        dc1--;
        b1.signal();
    }
    else if((nUser != 0) && (dc2 > 0)){
        dc2--;
        b2.signal();
    }
    else{
        testigo.signal();
    }
}

//Función que ejecuta cada uno de los procesos usuarios, necesita todos los parámetros para poder llamar a pasarTestigo
//además tiene un parámetro adicional que le indica su número de usuario
void usuario(Semaphore &testigo, Semaphore &b1, Semaphore &b2,bool &want_clean,bool &clean,int &nUser, int &dc1, int &dc2, int n) {
    ADD_EVENT("USER_BEGIN_" + to_string(n));
    int nCab  = 0;
    for(int i = 0; i < N_TIMES_USER; i++) {
        ADD_EVENT("USER_WAIT_" + to_string(n) + "," + to_string(i));
        //Proceso de entrada a la sección crítica
        testigo.wait();
        if(!(((!clean) && (nUser < N_CAB)) && (!want_clean))) {
            dc1++;
            testigo.signal();
            b1.wait();
        }
        nCab = nUser;
        nUser++;
        pasarTestigo(testigo, b1, b2, want_clean, clean, nUser, dc1, dc2);
        //Sección crítica
        ADD_EVENT("USER_IN_" + to_string(n) + "," + to_string(i) + "," + to_string(nCab));
        this_thread::sleep_for(chrono::milliseconds((rand()%31)+10));
        ADD_EVENT("USER_OUT_" + to_string(n) + "," + to_string(i) + "," + to_string(nCab));
        //Salida de la sección crítica
        testigo.wait();
        nUser--;
        pasarTestigo(testigo, b1, b2, want_clean, clean, nUser, dc1, dc2);
        this_thread::sleep_for(chrono::milliseconds((rand()%31)+20));
    }
    ADD_EVENT("USER_END_" + to_string(n));
}

//Función que ejecuta el proceso limpiador, necesita todos los parámetros para poder llamar a pasarTestigo
void cleaning(Semaphore &testigo, Semaphore &b1, Semaphore &b2,bool &want_clean,bool &clean,int &nUser, int &dc1, int &dc2) {
    ADD_EVENT("CLEANING_BEGIN");
    for(int i = 0; i < N_TIMES_CLEANING; i++) {
        this_thread::sleep_for(chrono::milliseconds(PER_CLEANING));
        ADD_EVENT("CLEANING_WAIT" + "," + to_string(i));
        want_clean = true;
        //Proceso de entrada a la sección crítica
        testigo.wait();
        if(nUser != 0) {
            dc2++;
            testigo.signal();
            b2.wait();
        }
        clean = true;
        pasarTestigo(testigo, b1, b2, want_clean, clean, nUser, dc1, dc2);
        ADD_EVENT("CLEANING_IN" + "," + to_string(i));
        //Sección critica
        want_clean = false;
        this_thread::sleep_for(chrono::milliseconds((rand()%41)+80));
        ADD_EVENT("CLEANING_OUT" + "," + to_string(i));
        //Salida de la sección crítica
        testigo.wait();
        clean = false;
        pasarTestigo(testigo, b1, b2, want_clean, clean, nUser, dc1, dc2);
    }
    ADD_EVENT("CLEANING_END");
}

int main() {
    ADD_EVENT("MAIN_BEGIN");
    //Declaración de variables
    Semaphore testigo(1), b1(0), b2(0);
    bool want_clean = false;
    bool clean = false;
    int nUser = 0;
    int dc1 = 0;
    int dc2 = 0;
    srand(time(nullptr));
    //Declaración e inicialización de los procesos
    thread user[N_USER], limpiador;
    limpiador = thread(&cleaning,ref(testigo),ref(b1), ref(b2), ref(want_clean), ref(clean), ref(nUser), ref(dc1), ref(dc2));
    for(int i = 0; i < N_USER; i++) {
        user[i] = thread(&usuario,ref(testigo),ref(b1), ref(b2), ref(want_clean), ref(clean), ref(nUser), ref(dc1), ref(dc2), i);
    }
    //Esperamos a que todos los procesos acaben
    limpiador.join();
    for(int i = 0; i < N_USER; i++) {
        user[i].join();
    }
    ADD_EVENT("MAIN_END");
    return 0;
}