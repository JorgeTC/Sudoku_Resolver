#pragma once
#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include "tipos.h"
#include "Casilla.h"
#include "Macros.h"

#ifndef SUPONER_H
#define SUPONER_H


using namespace std;

class sup {
   Sudoku* p_juego;
public:
   int estado[9][9];    //cómo era el tablero antes de la suposición
   size_t indice;          //cuántos candidatos lleva probados
   casilla atacar;      //cuál es la casilla en la que está probando
   std::vector<int> candidatos; //todos los números que caben en la casillla atacar
   void listar(casilla x);

   // Contructors
   sup();
   sup(Sudoku& juego, int fila = -1, int columna = -1);
   sup(casilla tablero[9][9]);

   // Evitar que siempre se genere el mismo sudoku
   void mezcla();
};


#endif // !SUPONER_H