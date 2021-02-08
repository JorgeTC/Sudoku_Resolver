#pragma once
#include "Sudoku.h"

// Valores para la variable nPuesto
#define NO_PUESTO    0
#define ESTUDIO      1
#define DESCARTE     2
// Macro para asignar valores a MetaFila y MetaColumna
#define SET_VECTOR(vt, val1, val2)  {vt[0] = val1; vt[1] = val2;}

class Trinidad {
   Sudoku *m_ps;
   int   m_nCuadrante;
   int   m_nMetaFila[2];
   int   m_nMetaColumna[2];
   std::vector<int> m_faltan = { 1,2,3,4,5,6,7,8,9 };
   // Guardo en qu� casillas del cuadrante puede ir cada uno de los n�meros que faltan
   std::vector<int> m_posibles;
   int   candidato;
   bool  m_bValidOri;
   public:
   int nPuesto = NO_PUESTO;

   public:
   Trinidad(Sudoku *ps, int nCuadrante);
   ~Trinidad();
   bool ponUnNumero();

   private:
   bool estudiaCuadrante();
   bool poner(int candidato);
   bool descarte();
   void normalizar(int vt1, int vt2, int *vtCurr);
};

