//------------------------------------------------------------------------------
// File:   ControlCabinas.hpp
// Author: Inés Román Gracia
//         Francisco Javier Pizarro Martínez
// Date:   18/11/2021
// Coms:   Fichero cabecera en el que se define el monitor
//         Práctica 4 de pscd
//------------------------------------------------------------------------------

//Librerias necesarias para la creación y correcto funcionamiento del monitor
#include <mutex>
#include <condition_variable>
#include <iostream>
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    extern Logger _logger;
    extern const string nada;
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

//Definición de la clase monitor
class monitorCabinas {
    public://Declaración de los métodos que nos ofrece la clase
        monitorCabinas();//Constructor de la clase
        void entrarCabina(int n, int j, int& cab);//Gestiona la entrada de los clientes
        void salirCabina(int n, int j, int cab);//Gestiona la salida de los clientes
        void empezarLimpiar(int i);//Gestiona la entrada del limpiador
        void acabarLimpiar(int i);//Gestiona la salida del limpiador
    private://Variables privadas de la clase
        int nUser;
        bool wantClean;
        bool cabinas[N_CAB];
        mutex mtxMonitor;//Herramientas que mas tarde se usa para garantizar la exclusión mutua
        condition_variable esperandoCabina;//Condiciones en las que van a esperar los procesos que no puedan entrar como si fuese una cola FIFO
        condition_variable esperandoLimpiar;
};