#include "stdafx.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <time.h>
#include "Menu principal.h"

Menu::Menu(Sudoku *p_juego) {
   m_juego = p_juego;
   std::cin.clear();
   int nAns;
   bool bOk = false;

   while (!bOk) {
      bDefaultLoaded = m_juego->load(strAutomaticLoad);
      std::cin.clear();
      print();
      std::cin >> nAns;
      switch (nAns) {
      case OP_CARGAR_OTRO:
         std::cout << "Introduzca el nombre del archivo a cargar: \n";
         std::cin >> strAutomaticLoad;
         break;
      case OP_CARGADO:
         bOk = true;
         // usamos el tablero cargado
         break;
      case OP_INTRODUCIR:
         bOk = m_juego->pide(true);
         break;
      case OP_SPECIALSETTINGS:
         SpecialSettings();
         break;
      default:
         // Save in tablero the example number x
         bOk = ej(nAns);
         break;
      }
   }
   if (bPrintLoaded)
      GuardarCargado();
}

void
Menu::print() {
   if (bDefaultLoaded) {
      std::cout << _S("Tienes un tablero cargado: ") << strAutomaticLoad << " Pulsa -1 para usarlo.\n";
   }
   std::cout << _S("Si quieres introducir tu propio Sudoku, pulsa 0.") <<
      _S("\nSi quieres algún ejemplo guardado, pulsa un número del 1 al ") << tamEJ << ". ";
}

void
Menu::printSS()
{
   std::cout << "Ajustes especiales.\n" <<
      "0. Salir\n" <<
      "1. Imprimir proceso ";
   OptionsBool(m_juego->GetSSComent());
   std::cout << "2. Guardar tablero cargado ";
   OptionsBool(bPrintLoaded);
   std::cout << "3. Comprobar cada casilla ";
   OptionsBool(m_juego->GetSSValid());
}

#pragma warning(disable:4996)
void
Menu::GuardarCargado(){
   time_t rawtime;
   tm* timeinfo;
   char buffer[80];

   time(&rawtime);
   timeinfo = std::localtime(&rawtime);

   strftime(buffer, sizeof(buffer), "%d%m%Y %H%M%S", timeinfo);
   std::string Nombre(buffer);

   std::ofstream myfile;
   myfile.open(GET_FILE(Nombre));
   std::cout << "\n";
   for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 9; k++) {
         myfile << m_juego->m_tablero[i][k].con << " ";
      }
      myfile << "\n";
   }
   myfile.close();
}

void
Menu::SpecialSettings() {
   int nAns;
   bool bOk = false;

   while (!bOk){
      printSS();

      std::cin >> nAns;
      switch (nAns) {
      case SS_COMENTAR:
         m_juego->ChangeSSComent();
         break;
      case SS_GUARDARCARGADO:
         bPrintLoaded = !bPrintLoaded;
         break;
      case SS_VALIDOESPECIAL:
         m_juego->ChangeSSValid();
         break;
      case SS_SALIR:
      default:
         bOk = true;
         break;
      }
   }
}

void
Menu::OptionsBool(bool b){
   if (b)
      std::cout << "(SI)\n";
   else
      std::cout << "(NO)\n";
}
