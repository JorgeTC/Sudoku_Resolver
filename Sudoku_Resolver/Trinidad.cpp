#include "stdafx.h"
#include "Trinidad.h"
#include "Matriz.h"
#include <math.h>
#include <algorithm>


Trinidad::Trinidad( Sudoku* ps ) {
   m_ps = ps;

   // Itero los números. Van del 1 al 9.
   for ( int numero = 1; numero <= 9; numero++ ) {
      // Veamos en qué casillas puede ir el número i
      // Itero los cuadrantes. Van del 0 al 8.
      for ( int cuad = 0; cuad < 9; cuad++ ) {
         // Haremos el cálculo cuadrante a cuadrante
         codigoCuadrante(cuad, numero);
      }
   }
   normalizarCodigos();

   // No puedo permitirme resolver el sudoku entero para cada comprobación de validez
   m_bValidOri = m_ps->GetSSValid();
   m_ps->SetSSValid( false );
}

Trinidad::~Trinidad() {
   // Restauro la configuración anterior
   m_ps->SetSSValid( m_bValidOri );
}

bool
Trinidad::ponUnNumero() {

   if ( estudiaTablero() )
      return true;

   return descarte();
}

void
Trinidad::normalizarCodigos() {
   int   code1;
   int   code2;
   int   code3;
   for ( int candidato = 1; candidato <= 9; candidato++ ) {
      // Primero las tres metafilas
      for ( int i = 0; i <= 6; i+=3 ) {
         GET_FILA_CODE( code1, m_posibles[candidato - 1][i] );
         GET_FILA_CODE( code2, m_posibles[candidato - 1][i + 1] );
         GET_FILA_CODE( code3, m_posibles[candidato - 1][i + 2] );
         normalizar(&code1, &code2, &code3);
         SET_FILA_CODE( code1, m_posibles[candidato - 1][i] );
         SET_FILA_CODE( code2, m_posibles[candidato - 1][i + 1] );
         SET_FILA_CODE( code3, m_posibles[candidato - 1][i + 2] );
      }
      // Las tres metacolumnas
      for ( int i = 0; i < 3; i++ ) {
         GET_COLUMN_CODE( code1, m_posibles[candidato - 1][i] );
         GET_COLUMN_CODE( code2, m_posibles[candidato - 1][i + 3] );
         GET_COLUMN_CODE( code3, m_posibles[candidato - 1][i + 6] );
         normalizar( &code1, &code2, &code3 );
         SET_COLUMN_CODE( code1, m_posibles[candidato - 1][i] );
         SET_COLUMN_CODE( code2, m_posibles[candidato - 1][i + 3] );
         SET_COLUMN_CODE( code3, m_posibles[candidato - 1][i + 6] );
      }
   }
   // Aún no hemos cruzado la información de las filas y las columnas.
}

void
Trinidad::codigoCuadrante( int cuadrante, int candidato ) {

   short int *currCodigo = &m_posibles[candidato - 1][cuadrante];

   for ( CuadIndex k( cuadrante ); k < 9; ++k ) {
      // Busco si el candidato está ya colocado en cuadrante
      if ( m_ps->CONTENT( k.Fila(), k.Columna() ) == candidato ) {
         // Anotamos la fila
         *currCodigo = ( 1 << ( k.Fila() % 3 ) );
          // Anotamos la columna
         *currCodigo = *currCodigo | ( 1 << ( ( k.Columna() % 3) + 3 ) );
         return;
      }
   }
   // Si no está colocado, busco en qué casillas puede ir
   for ( CuadIndex k( cuadrante ); k < 9; ++k ) {
      if ( !EMPTY( m_ps->m_tablero[k.Fila()][k.Columna()] ) )
         continue;
      if( m_ps->comprobar( k.Fila(), k.Columna(), candidato ) ) {
         // En tres bits guardamos si el candidato puede ir en la fila.
         *currCodigo = *currCodigo | ( 1 << ( k.Fila() % 3 ) );
         // En los tres siguientes bits anotamos si puede ir en la columna.
         *currCodigo = *currCodigo | ( 1 << ( ( k.Columna() % 3 ) + 3 ) );
      }
   }
}

bool
Trinidad::estudiaTablero() {
   for ( int k = 0; k < 9; k++ ) {
      if ( estudiaCuadrante( k ) )
         return true;
      if ( estudiaFila( k ) )
         return true;
      if ( estudiaColumna( k ) )
         return true;
   }
   return false;
}

#define POSIBLE_EN(n, fil, col)  bool((m_posibles[n - 1][LINE_COLUMN_CUADRANT(fil, col)] & (1 << (fil % 3))) && \
                                    (m_posibles[n - 1][LINE_COLUMN_CUADRANT(fil, col)] & (1 << ( (col % 3) + 3 ))))

bool
Trinidad::estudiaCuadrante( int const cuadrante ) {
   std::vector<int> faltan = { 1,2,3,4,5,6,7,8,9 };
   int contador;


   for ( CuadIndex k( cuadrante ); k < 9; ++k ) { // Actualizamos la lista faltan
      actualizaLista( faltan, m_ps->m_tablero[k.Fila()][k.Columna()].con );
   }

   for ( int candidato : faltan ) {
      contador = 0;
      for ( CuadIndex k( cuadrante ); k < 9; ++k ) {
         if ( !EMPTY( m_ps->m_tablero[k.Fila()][k.Columna()] ) )
            continue;
         if ( POSIBLE_EN( candidato, k.Fila(), k.Columna() ) ) {
            if ( ( ++contador ) > 1 )
               break;
            m_Candidata.fila = k.Fila();
            m_Candidata.columna = k.Columna();
         }
      }
      if ( contador == 1 ) { // Si solo hemos encontrado una casilla donde puede ir candidato
         CONTENT_IN( m_ps->m_tablero, m_Candidata ) = candidato;
         nPuesto = ESTUDIO_CU + cuadrante;
         return true;
      }
   }

   return false;
}

bool
Trinidad::estudiaFila( int const fila ) {
   std::vector<int> faltan = { 1,2,3,4,5,6,7,8,9 };
   m_Candidata.fila = fila;
   int contador;



   for ( int i = 0; i < 9; i++ ) { // Actualizamos la lista faltan
      actualizaLista( faltan, m_ps->m_tablero[fila][i].con );
   }

   for ( int candidato : faltan ) {
      contador = 0;
      for ( int j = 0; j < 9; j++ ) {
         if ( !EMPTY( m_ps->m_tablero[fila][j] ) )
            continue;
         if( POSIBLE_EN( candidato, fila, j ) ) {
            if ( ( ++contador ) > 1 )
               break;
            m_Candidata.columna = j;
         }
      }
      if ( contador == 1 ) { // Si solo hemos encontrado una casilla donde puede ir faltan[i]candidato
         CONTENT_IN( m_ps->m_tablero, m_Candidata ) = candidato;
         nPuesto = ESTUDIO_F + fila;
         return true;
      }
   }
   return false;
}

bool
Trinidad::estudiaColumna( int const columna ) {
   std::vector<int> faltan = { 1,2,3,4,5,6,7,8,9 };
   m_Candidata.columna = columna;
   int contador;

   
      for ( int i = 0; i < 9; i++ ) {
         actualizaLista( faltan, m_ps->m_tablero[i][columna].con );
      }
      for ( int candidato : faltan ) {
         contador = 0;
         for ( int j = 0; j < 9; j++ ) {
            if ( !EMPTY( m_ps->m_tablero[j][columna] ) )
               continue;
            if ( POSIBLE_EN( candidato, j, columna ) ) {
               if ( ( ++contador ) > 1 )
                  break;
               m_Candidata.fila = j;
            }
         }
         if ( contador == 1 ) { // Si solo hemos encontrado una casilla donde puede ir faltan[i]
            CONTENT_IN( m_ps->m_tablero, m_Candidata ) = candidato;
            nPuesto = ESTUDIO_CO + columna;
            return true;
         }
      }
   return false;
}



bool
Trinidad::descarte() {
   // Antes de ser llamada necesita haber sido llamada estudiaTablero.
   // Necesitamos que se haya rellenado m_posibles.

   int nSuma;
   int candidato;

   for ( int fila = 0; fila < 9; fila++ ) {
      for ( int columna = 0; columna < 9; columna++ ) {
         if ( !EMPTY( m_ps->m_tablero[fila][columna] ) )
            continue;

         nSuma = 0;
         for ( int i = 1; i <= 9; i++ ) {
            // Contamos cuántos números pueden ir en cada casilla.
            if ( !POSIBLE_EN(i, fila, columna) )
               continue;
            candidato = i;
            if ( ( ++nSuma ) > 1 )
               break;
         }

         if ( nSuma == 1 ) {
            // Si sólo hay un número que pueda ir en alguna casilla, la rellenamos
            m_ps->CONTENT( fila, columna ) = candidato;
            m_Candidata.fila = fila; m_Candidata.columna = columna;
            nPuesto = DESCARTE;
            return true;
         }
      }
   }
   return false;
}

#define SORT_POSIBILIDADES(a,b,c)   {if( POSIBILIDADES( *a ) > POSIBILIDADES( *b ) ) \
                                       SWAP( a, b ); \
                                    if ( POSIBILIDADES( *b ) > POSIBILIDADES( *c ) ) \
                                       SWAP( b, c ); \
                                    if ( POSIBILIDADES( *a ) > POSIBILIDADES( *b ) ) \
                                       SWAP( a, b ); }

void
Trinidad::normalizar( int *vt1, int *vt2, int *vt3 ) {
   // Buscamos
   int *nMin = vt1;
   int *nMed = vt2;
   int *nMax = vt3;

   SORT_POSIBILIDADES(nMin, nMed, nMax);

   if ( FILA_DETERMINADA( *nMin ) ) {
      // Ponemos a 0 la coordenada que nos indica nMin
      *nMed = *nMed & ~*nMin;
      *nMax = *nMax & ~*nMin;
      if ( FILA_DETERMINADA( *nMed ) ) {
         // Ponemos a 0 la coordenada que nos indica nMed
         *nMax = *nMax & ~*nMed;
      }
      // Quizás ahora nMax sea de tipo 1
      SORT_POSIBILIDADES( nMin, nMed, nMax );
      // nMin no nos puede dar información nueva
      if ( FILA_DETERMINADA( *nMed ) ) {
         // Ponemos a 0 la coordenada que nos indica nMed
         *nMax = *nMax & ~*nMed;
      }
   }
   else if ( DOS_POSIBILIDADES( *nMin ) ) {
      if ( *nMin == *nMed )
         *nMax = *nMax & (~*nMin & 0b111 );
      if ( *nMin == *nMax )
         *nMed = *nMed & (~*nMed & 0b111 );
   }
}
