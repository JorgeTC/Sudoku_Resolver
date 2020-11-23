#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Casilla.h"
#include "Macros.h"

#ifndef TIPOS_H
#define TIPOS_H

class casilla;
class Sudoku;
class sup;

class Sudoku {
   std::vector<sup> suposiciones;
public:   
   // Tablero, debiera ser privado
   casilla m_tablero[9][9];
#if !_DEBUG
   bool bComentar = false;
#else
   bool bComentar = true;
#endif
   // Constructor
   Sudoku();
   bool load(std::string fileName);
   bool pide(bool print = true, bool cursor = true);
   // Interfaz b�sica
   void print();
   void printCur(int const fila, int const columna);
   bool resolver();
   bool terminado();
   void vacia();
   // Rellenar el tablero
   void copiar(int tablero[9][9]);

   // Validez de contenido en una casilla
   bool comprobar(int const fila, int const columna, int const n);
   bool comprobar(casilla const& place, int const n);
   int  posibleNumeroEn(int const fila, int const columna, int& candidato);
   int  posiblenumeroEn(casilla const& place, int& candidato);

private:
   // Funciones de resoluci�n
         // Resolver por descarte
   int  descartar();
   int  descarte(int const fila, int const columna);
   int  descarte(casilla const& place);
         // Resoluci�n m�s delicada
   bool  estudiaTablero();
   bool  estudiaCuadrante(int const cuadrante);
   bool  estudiaFila(int const fila);
   bool  estudiaColumna(int const columna);
   void actualizaLista(std::vector<int>& list, int objetivo);
         // Suponer en una casilla
   int  probar(bool bError);
         // Elegir la mejor casilla para la suposici�n
   casilla elegir();
   // Funciones auxiliares para elegir
   void emptyInLine(int nLine, int& cont);
   void emptyInLine(casilla tablero[9][9], int nLine, int& cont);
   void emptyInCol(int nCol, int& cont);
   void emptyInCol(casilla tablero[9][9], int nCol, int& cont);
   void emptyInCuad(int nLine, int& cont);
   void emptyInCuad(casilla tablero[9][9], int nLine, int& cont);

   void Comentar(int nMessage, int nFila, int nColumna);
   void Comentar(int nMessage, sup Suposicion);
};

#define DESCARTAR       1
#define ESTUDIA_CUAD    2
#define ESTUDIA_FILA    3
#define ESTUDIA_COL     4
#define NUEVA_SUP       5
#define DESCARTA_SUP    6
#define DESCARTA_CAS    7

#endif // ! TIPOS_H