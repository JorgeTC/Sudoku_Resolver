#pragma once
#include "Sudoku.h"

// Valores para la variable nPuesto
#define NO_PUESTO    0
#define ESTUDIO_F    10
#define ESTUDIO_CO   20
#define ESTUDIO_CU   30
#define DESCARTE     40
// Macro para asignar valores a MetaFila y MetaColumna
#define SET_VECTOR(vt, val1, val2)  {vt[0] = val1; vt[1] = val2;}
// Obtener de un código completo sus subcódigos
#define GET_FILA_CODE(n, code)      {n = (code & 7);} // Nos quedamos con los tres primeros bits
#define GET_COLUMN_CODE(n, code)    {n = (code >> 3);} // Nos quedamos con el resto de bits. Sólo 3 de ellos pueden ser 1
#define SET_FILA_CODE(n, code)      {code = ( (code & (7 << 3)) | n);}
#define SET_COLUMN_CODE(n, code)    {code = ( (code & 7) | (n << 3) );}
// Comprobar bits
#define FILA_DETERMINADA(n)         bool( n == 1 || n == 2 || n == 4 )
#define CUALQUIER_FILA(n)           bool( n == 7)
#define DOS_POSIBILIDADES(n)        bool( n == 3 || n == 5 || n == 6 )
// Contar bits. No consideramos n = 0 un caso posible.
#define POSIBILIDADES(n)            int(FILA_DETERMINADA(n) ? 1 : (CUALQUIER_FILA(n) ? 3 : 2))


class Trinidad {
   Sudoku *m_ps;
   bool  m_bValidOri;
   // En m_posibles[i][j] tenemos un número del 0 al 63.
   // Indice en qué posiciones del cuadrante j-ésimo puede ir el
   // número i
   short int   m_posibles[9][9]{0};   
   public:
   int nPuesto = NO_PUESTO;
   casilla   m_Candidata;

   public:
   Trinidad(Sudoku *ps);
   ~Trinidad();
   bool ponUnNumero();

   private:
   void normalizarCodigos();
   void codigoCuadrante(int cuadrante, int candidato);
   bool estudiaTablero();
   bool estudiaCuadrante(int const c);
   bool estudiaFila( int const c );
   bool estudiaColumna( int const c );
   bool poner(int candidato);
   bool descarte();
   void normalizar(int *vt1, int *vt2, int *vt3);
};

