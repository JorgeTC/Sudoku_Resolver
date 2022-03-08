#pragma once
#pragma once
#include "stdafx.h"
#include "Sudoku.h"

#define IN_RANGE(n, min, max) (n >= min && n <= max)
#define tamEJ  10

class Menu {
private:
   Sudoku* m_juego;
   bool bDefaultLoaded =  false;
   bool bPrintLoaded = false;
   std::string strAutomaticLoad = _S("Trinitario");

   bool ej(int n);
   void print();
   void printSS();

   void GuardarCargado();
public:
   Menu(Sudoku * p_juego);
   void SpecialSettings();
   void OptionsBool(bool b);
};

// Loaded options
#define OP_CARGADO         -1
#define OP_CARGAR_OTRO     -2
#define OP_INTRODUCIR      0
#define OP_SPECIALSETTINGS 100
// Special settings
#define SS_SALIR           0
#define SS_COMENTAR        1
#define SS_GUARDARCARGADO  2
#define SS_VALIDOESPECIAL  3
#define SS_MEZCLA          4


