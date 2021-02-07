#include "stdafx.h"
#include "Trinidad.h"
#include "Matriz.h"
#include <math.h>
#include <algorithm>


Trinidad::Trinidad( Sudoku* ps, int nCuadrante ) {
   m_ps = ps;
   m_nCuadrante = nCuadrante;
   // Guardamos los cuadrantes que están en la misma
   // metafila y misma metacolumna
   switch ( m_nCuadrante ) {
      case 0:
         SET_VECTOR( m_nMetaFila, 1, 2 );
         SET_VECTOR( m_nMetaColumna, 3, 6 );
         break;
      case 1:
         SET_VECTOR( m_nMetaFila, 0, 2 );
         SET_VECTOR(m_nMetaColumna, 4, 7 );
         break;
      case 2:
         SET_VECTOR( m_nMetaFila, 0, 1 );
         SET_VECTOR( m_nMetaColumna, 5, 8 );
         break;
      case 3:
         SET_VECTOR( m_nMetaFila, 4, 5 );
         SET_VECTOR( m_nMetaColumna, 0, 6 );
         break;
      case 4:
         SET_VECTOR( m_nMetaFila, 3, 5 );
         SET_VECTOR( m_nMetaColumna, 1, 7 );
         break;
      case 5:
         SET_VECTOR( m_nMetaFila, 3, 4 );
         SET_VECTOR( m_nMetaColumna, 2, 8 );
         break;
      case 6:
         SET_VECTOR( m_nMetaFila, 7, 8 );
         SET_VECTOR( m_nMetaColumna, 0, 3 );
         break;
      case 7:
         SET_VECTOR( m_nMetaFila, 6, 8 );
         SET_VECTOR( m_nMetaColumna, 1, 4 );
         break;
      case 8:
         SET_VECTOR( m_nMetaFila, 6, 7 );
         SET_VECTOR( m_nMetaColumna, 2, 5 );
         break;
   }
   // Números que faltan por poner en el cuadrante
   for ( CuadIndex k( m_nCuadrante ); k < 9; ++k ) { // Actualizamos la lista faltan
      actualizaLista( m_faltan, m_ps->m_tablero[k.Fila()][k.Columna()].con );
   }
}

bool
Trinidad::ponUnNumero() {
   for ( int i : m_faltan) {
      if ( poner(i) )
         // No me permito poner más de un número.
         // Prefiero volver a llamar al resto de funciones.
         return true;
   }
   return false;
}

bool
Trinidad::poner( int candidato ) {
   // Sabemos que candidato no está ya puesto en nuestro cuadrante
   int fila0 = 0;
   int fila1 = 0;
   int filaCurr = 0;
   int columna0 = 0;
   int columna1 = 0;
   int columnaCurr = 0;
   casilla candidata;
   int contador = 0;
   // No puedo permitirme resolver el sudoku entero para cada comprobación de validez
   bool bValidOri = m_ps->GetSSValid();
   m_ps->SetSSValid(false); 

   for ( CuadIndex k( m_nMetaFila[0] ); k < 9; ++k ) {
      if ( m_ps->CONTENT( k.Fila(), k.Columna() ) == candidato ) {
         fila0 = ( 1 << (k.Fila() % 3));
         break;
      }
   }
   if ( !fila0 ) {
      for ( CuadIndex k( m_nMetaFila[0] ); k < 9; ++k ) {
         if ( m_ps->comprobar( k.Fila(), k.Columna(), candidato ) ) {
            // En tres bits guardamos si el candidato puede ir en la fila.
            fila0 = fila0 | ( 1 << ( k.Fila() % 3 ) );
         }
      }
   }

   for ( CuadIndex k( m_nMetaFila[1] ); k < 9; ++k ) {
      if ( m_ps->CONTENT( k.Fila(), k.Columna() ) == candidato ) {
         fila1 = ( 1 << ( k.Fila() % 3 ) );
         break;
      }
   }
   if ( fila1 < 0 ) {
      for ( CuadIndex k( m_nMetaFila[1] ); k < 9; ++k ) {
         if ( m_ps->comprobar( k.Fila(), k.Columna(), candidato ) ) {
            // En tres bits guardamos si el candidato puede ir en la fila.
            fila1 = fila1 | ( 1 << ( k.Fila() % 3 ) );
         }
      }
   }

   for ( CuadIndex k( m_nMetaColumna[0] ); k < 9; ++k ) {
      if ( m_ps->CONTENT( k.Fila(), k.Columna() ) == candidato ) {
         columna0 = ( 1 << ( k.Columna() % 3 ) );
         break;
      }
   }
   if ( !columna0 ) {
      for ( CuadIndex k( m_nMetaColumna[0] ); k < 9; ++k ) {
         if ( m_ps->comprobar( k.Fila(), k.Columna(), candidato ) ) {
            // En tres bits guardamos si el candidato puede ir en la columna.
            columna0 = columna0 | ( 1 << ( k.Columna() % 3 ) );
         }
      }
   }

   for ( CuadIndex k( m_nMetaColumna[1] ); k < 9; ++k ) {
      if ( m_ps->CONTENT( k.Fila(), k.Columna() ) == candidato ) {
         columna1 = ( 1 << ( k.Columna() % 3 ) );
         break;
      }
   }
   if ( !columna1 ) {
      for ( CuadIndex k( m_nMetaColumna[1] ); k < 9; ++k ) {
         if ( m_ps->comprobar( k.Fila(), k.Columna(), candidato ) ) {
            // En tres bits guardamos si el candidato puede ir en la columna.
            columna1 = columna1 | ( 1 << ( k.Columna() % 3 ) );
         }
      }
   }

   normalizar( fila0, fila1, &filaCurr);
   normalizar( columna0, columna1, &columnaCurr );

   // Itero el cuadrante buscando la única casilla donde pueda ir el candidato
   for ( CuadIndex k( m_nCuadrante ); k < 9; ++k ) {
      if ( !EMPTY(m_ps->m_tablero[k.Fila()][k.Columna()]) )
         continue;
      if ( !m_ps->comprobar(k.Fila(), k.Columna(), candidato) )
         continue;
      // No hay ningún número ya puesto que impida a candidato ir en la casilla actual.
      // Veamos si la información guardada en Curr nos dice algo.
      if ( filaCurr | ( 1 << k.Fila() % 3 ) &&
           columnaCurr | ( 1 << k.Columna() % 3 ) ) {
         contador++;
         if (contador > 1 )
            break;
         candidata.fila = k.Fila();
         candidata.columna = k.Columna();
      }
   }
   if ( contador == 1 ) {
      CONTENT_IN( m_ps->m_tablero, candidata ) = candidato;
   }

   // Restauro la configuración anterior
   m_ps->SetSSValid( bValidOri );
   return ( contador == 1 );
}

void
Trinidad::normalizar( int vt1, int vt2, int *vtCurr ) {
   switch ( vt1 ) {
      // Si el vector vt1 tiene una única coordenada no nula,
      // puedo anular esa coordenada en el otro.
      case 1:
      case 2:
      case 4:
         // Aplico un xor y me quedo con los 3 primeros bits
         vt2 = (vt2 & ~vt1) & (8 - 1);
      default:
         break;
   }
   switch ( vt2 ) {
      // Si el vector vt1 tiene una única coordenada no nula,
      // puedo anular esa coordenada en el otro.
      case 1:
      case 2:
      case 4:
         // Aplico un xor y me quedo con los 3 primeros bits
         vt1 = (vt1 & ~vt2) & ( 8 - 1 );
      default:
         break;
   }

   if (vt2 < vt1)
      SWAP(vt2, vt1);

   switch ( vt1 ) { // Fuerzo el razonamiento.
      //Nos gustaría ser capaces de escribirlo de forma algorítmica
      case 1:
         switch ( vt2 ) {
            case 2:
               *vtCurr = 4; return;
            case 4:
               *vtCurr = 2; return;
            case 6:
               *vtCurr = 6; return;
            default:
               *vtCurr = 7; return;
         }
      case 2:
         switch ( vt2 ) {
            case 4:
               *vtCurr = 1; return;
            case 5:
               *vtCurr = 5; return;
            default:
               *vtCurr = 7; return;
         }
      case 3:
         switch ( vt2 ) {
            case 3:
               *vtCurr = 4; return;
            case 4:
               *vtCurr = 3; return;
            case 5:
            case 6:
            case 7:
            default:
               *vtCurr = 7; return;
         }
      case 5:
         switch ( vt2 ) {
            case 5:
               *vtCurr = 2; return;
            case 6:
            case 7:
            default:
               *vtCurr = 7; return;
         }
      case 6:
         switch ( vt2 ) {
            case 6:
               *vtCurr = 1; return;
            case 7:
            default:
               *vtCurr = 7; return;
         }
      default:
         *vtCurr = 7; return;
   }
}
