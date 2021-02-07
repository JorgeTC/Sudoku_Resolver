#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Casilla.h"
#include "Macros.h"
#include "Settings.h"

#ifndef TIPOS_H
#define TIPOS_H

class casilla;
class Sudoku;
class sup;
class Interfaz;
enum class COMENT_CODE;


class Sudoku {
   std::vector<sup> suposiciones;
   SSetting    SpecialSettings;
   Interfaz*    m_If;
   
public:   
   // Tablero, debiera ser privado
   casilla     m_tablero[9][9];
   int         nSolucionable;

   
   // Constructor
   Sudoku();
   Sudoku(int tablero[9][9]);
   Sudoku(casilla tablero[9][9]);
   bool load(std::string fileName);
   bool pide(bool print = true, bool cursor = true);

   // Interfaz básica
   void print();
   void printCur(int const fila, int const columna);
   bool resolver();
   bool GetSolution();
   bool terminado();
   void vacia();
   int  CuantasSoluciones();
   // Interfaz adjustes especiales
   inline bool    GetSSComent()     { return SpecialSettings.bComentar; };
   inline bool    GetSSValid()      { return SpecialSettings.bValidSpecial; };
   inline bool    GetSSRand()       { return SpecialSettings.bRandSuposition; };
   inline void    SetSSComent(bool bValue)   { SpecialSettings.bComentar = bValue; };
   inline void    SetSSValid(bool bValue)    { SpecialSettings.bValidSpecial = bValue; };
   inline void    SetSSRand(bool bValue)     { SpecialSettings.bRandSuposition = bValue; };
   inline void    ChangeSSComent()  { SpecialSettings.bComentar = !SpecialSettings.bComentar; };
   inline void    ChangeSSValid()   { SpecialSettings.bValidSpecial = !SpecialSettings.bValidSpecial; };
   inline void    ChangeSSRand()    { SpecialSettings.bRandSuposition = !SpecialSettings.bRandSuposition; };

   // Rellenar el tablero
   void copiar(int tablero[9][9]);
   void copiar(casilla tablero[9][9]);

   // Validez de contenido en una casilla
   bool comprobar(int const fila, int const columna, int const n);
   bool comprobar(casilla const& place, int const n);
   bool esResoluble();
   int  posibleNumeroEn(int const fila, int const columna, int& candidato);
   int  posiblenumeroEn(casilla const& place, int& candidato);

private:
   // Funciones de resolución
         // Resolver por descarte
   int  descartar();
   int  descarte(int const fila, int const columna);
   int  descarte(casilla const& place);
         // Resolución más delicada
   bool  estudiaTablero();
   bool  estudiaCuadrante(int const cuadrante);
   bool  estudiaFila(int const fila);
   bool  estudiaColumna(int const columna);
   int  razonar( bool* error );
         // Suponer en una casilla
          int  probar(bool bError);
         // Elegir la mejor casilla para la suposición
   casilla elegir();
   // Funciones auxiliares para elegir
   void emptyInLine(int nLine, int& cont);
   void emptyInLine(casilla tablero[9][9], int nLine, int& cont);
   void emptyInCol(int nCol, int& cont);
   void emptyInCol(casilla tablero[9][9], int nCol, int& cont);
   void emptyInCuad(int nLine, int& cont);
   void emptyInCuad(casilla tablero[9][9], int nLine, int& cont);

   void Comentar( COMENT_CODE nMessage, int nFila = 0, int nColumna = 0);
   void Comentar( COMENT_CODE nMessage, sup Suposicion);
};

#define SIN_SOLUCION       0
#define UNA_SOLUCION       1
#define SOL_NO_UNICA       2

#endif // ! TIPOS_H