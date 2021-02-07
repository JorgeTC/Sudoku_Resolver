#pragma once
#include "Sudoku.h"

class Trinidad {
   Sudoku *m_ps;
   int   m_nCuadrante;
   int   m_nMetaFila[2];
   int   m_nMetaColumna[2];
   std::vector<int> m_faltan = { 1,2,3,4,5,6,7,8,9 };

   public:
   Trinidad(Sudoku *ps, int nCuadrante);
   bool ponUnNumero();

   private:
   bool poner(int candidato);
   void normalizar(int vt1, int vt2, int *vtCurr);
};

#define SET_VECTOR(vt, val1, val2)  {vt[0] = val1; vt[1] = val2;}
