// Sudoku_Resolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <random>
#include <time.h> 
#include "Tipos.h"
#include "Suponer.h"
#include "Matriz.h"
#include "Ejemplos.h"
#include "Menu principal.h"



int main()
{
   bool bOk = true;

#if (!_DEBUG || true)
   srand(time(NULL));
#endif
   while (bOk) {
      Sudoku juego;
      Menu   mInicial(&juego);

      juego.print();
      juego.GetSolution();
      std::cin >> bOk;
   }
   return 0;
}

