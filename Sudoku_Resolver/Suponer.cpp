#include "stdafx.h"
#include "Suponer.h"
#include "Matriz.h"
#include "Interfaz.h" 

void
sup::listar(casilla x) {
   // Rellena candidatos
   //x.fila, x.columna y x.cuadrante valen -1 por defecto.
   //Solo en aquella estructura donde haya que trabajar hay un número positivo.
   std::vector<int> FirstCandidatos;
   int contador = 0;
   atacar.con = 0; // vaciamos la casilla
   if (x.fila >= 0) { // Lista de numeros que faltan en .fila
      for (int i = 1; i <= 9; i++) {
         contador = 0;
         for (int nCol = 0; nCol < 9 && contador == 0; nCol++) { // Recorremos la fila
            if (atacar.isEmpty() && p_juego->m_tablero[x.fila][nCol].isEmpty()
               && p_juego->comprobar(x.fila, nCol, i)) { //Evitamos pruebas que sabemos que vamos a descartar
               atacar.setFilaColumna(x.fila, nCol);
               atacar.con = 1;
            }
            else if (p_juego->m_tablero[x.fila][nCol].con == i)
               contador++;
         }
         if (contador == 0) //si el número i está en 0 casillas, se añade a faltan
            FirstCandidatos.push_back(i);
      }
   }
   else if (x.columna >= 0) { // Lista de numeros que faltan en .columna
      for (int i = 1; i <= 9; i++) {
         contador = 0;
         for (int nLine = 0; nLine < 9 && contador == 0; nLine++) { // Recorremos la columna
            if (atacar.isEmpty() && p_juego->m_tablero[nLine][x.columna].isEmpty()
               && p_juego->comprobar(nLine, x.columna, i)) { //Evitamos pruebas que sabemos que vamos a descartar
               atacar.setFilaColumna(nLine, x.columna);
               atacar.con = 1;
            }
            else if (p_juego->m_tablero[nLine][x.columna].con == i)
               contador++;
         }
         if (contador == 0)
            FirstCandidatos.push_back(i);
      }
   }
   else if (x.cuad >= 0) { // Lista de numeros que faltan en .cuad
      for (int i = 1; i <= 9; i++) {
         contador = 0;
         for (CuadIndex nCuadIn(x.cuad); nCuadIn < 9 && contador == 0; ++nCuadIn) { // Recorremos el cuadrante
            if (atacar.isEmpty() && p_juego->m_tablero[nCuadIn.Fila()][nCuadIn.Columna()].isEmpty()
               && p_juego->comprobar(nCuadIn.Fila(), nCuadIn.Columna(), i)) { //Evitamos pruebas que sabemos que vamos a descartar
               atacar.setFilaColumna(nCuadIn.Fila(), nCuadIn.Columna());
               atacar.con = 1;
            }
            else if (p_juego->m_tablero[nCuadIn.Fila()][nCuadIn.Columna()].con == i)
               contador++;
         }
         if (contador == 0)
            FirstCandidatos.push_back(i);
      }
   }

   for (size_t i = 0; i < FirstCandidatos.size(); i++) {
      if (p_juego->comprobar(atacar.fila, atacar.columna, FirstCandidatos[i]))
         candidatos.push_back(FirstCandidatos[i]);
   }
}

casilla
Sudoku::elegir() {
   // elige la estructura en la que se va aprobar un número
   // Se guarda -1 en las estructuras que no se vayan a estudiar.
   // Se guarda  j en la  estructura  que  se  vaya  a  estudiar.
   int nEmptyCount = 0; // Cuantas posiciones vacias hay
   casilla target; //en fila, columna y cuad se guarda la casilla donde se va a suponer
   // en con guardo cuántos espacios vacíos hay en la estructura a estudiar
   // Inicializo, necesito un número estrictamente mayor que 9.
   target.con = 10;

   for (int j = 0; j < 9 && target.con > 2; j++) { //comprobar todas las filas
      // Cuento cuántas casillas hay vacías en la fila j
      emptyInLine(j, nEmptyCount);

      if (nEmptyCount != 0 && nEmptyCount < target.con) {
         target.con = nEmptyCount;
         target.fila = j; // La fila que vamos a estudiar
         target.columna = -1; target.cuad = -1; // no estudiamos columnas ni cuadrantes
      }
   }
   if (target.con > 2) {
      for (int j = 0; j < 9 && target.con > 2; j++) { //comprobar todas las columnas
         // Cuento cuántas casillas hay vacías en la columna j
         emptyInCol(j, nEmptyCount);

         if (nEmptyCount != 0 && nEmptyCount < target.con) {
            target.con = nEmptyCount;
            target.columna = j; // La columna que vamos a estudiar
            target.cuad = -1; target.fila = -1; // No estudiamos ni cuadrantes ni filas
         }
      }
   }
   if (target.con > 2) {
      for (int j = 0; j < 9 && target.con>2; j++) { //comprobar todos los cuadrantes
         // Cuento cuántas casillas hay vacías en el cuadrante j
         emptyInCuad(j, nEmptyCount);

         if (nEmptyCount != 0 && nEmptyCount < target.con) {
            target.con = nEmptyCount;
            target.cuad = j; // El cuadrante que vamos a estudiar
            target.fila = -1; target.columna = -1; // No estudiamos ni filas ni columnas
         }
      }
   }
   return target;
}

int
Sudoku::probar(bool bError) {
   // returns 0: si no se puede probar nada
   // returns 1: si se ha conseguido poner algo
   if (!bError) { //primera vez que se llama a la función probar
      sup nueva(*this);

      // Elegimos la casilla en la que vamos a actuar y generamos la lista de candidatos
      nueva.listar(elegir());
      if (nueva.candidatos.size() == 0) {
         return false;
      }
      if (GetSSRand())
         nueva.mezcla();
      CONTENT_IN(m_tablero, nueva.atacar) = nueva.candidatos[0];
      suposiciones.push_back(nueva);
      Comentar( COMENT_CODE::NUEVA_SUP, LAST_ELEMENT(suposiciones));
      LAST_ELEMENT(suposiciones).indice++;
      

      return true;
   }
   else if (bError && !IS_EMPTY(suposiciones)) {
      if (LAST_ELEMENT(suposiciones).indice < LAST_ELEMENT(suposiciones).candidatos.size()) {
         //se llama a la función probar porque una suposición ha salido mal y aún hay margen de elección

            // Recuperamos el estado anterior a la suposición
         copiar(LAST_ELEMENT(suposiciones).estado);
         // Colocamos la siguiente posibilidad
         Comentar( COMENT_CODE::DESCARTA_SUP, LAST_ELEMENT(suposiciones));
         CONTENT_IN(m_tablero, LAST_ELEMENT(suposiciones).atacar) = LAST_ELEMENT(suposiciones).candidatos[LAST_ELEMENT(suposiciones).indice];
         // Avanzamos el scroll
         LAST_ELEMENT(suposiciones).indice++;

         return true;
      }
      else {
         //se debe descartar una suposición sobre la que se apoya una suposición
         while (!(LAST_ELEMENT(suposiciones).indice < LAST_ELEMENT(suposiciones).candidatos.size()) &&
            !IS_EMPTY(suposiciones)) {
            Comentar( COMENT_CODE::DESCARTA_SUP, LAST_ELEMENT(suposiciones));
            // Debemos comprobar en cada iteración que suposiciones no sea vacío
            suposiciones.pop_back();
            if (IS_EMPTY(suposiciones)) {
               break;
            }
         }
         if (suposiciones.size() < 1) {
            return false;
         }
         // Recuperamos el estado anterior a la suposición
         copiar(LAST_ELEMENT(suposiciones).estado);
         Comentar( COMENT_CODE::DESCARTA_SUP, LAST_ELEMENT(suposiciones));
         CONTENT_CS(LAST_ELEMENT(suposiciones).atacar) = LAST_ELEMENT(suposiciones).candidatos[LAST_ELEMENT(suposiciones).indice];
         // Avanzamos el scroll
         LAST_ELEMENT(suposiciones).indice++;
         return true;
      }
   }
   else
      return false;
}

void
sup::mezcla() {
   int tam = candidatos.size();
   if (tam > 1) {
      int tam2 = tam * 3;
      int IndexA, IndexB;
      for (int i = 0; i < tam2; i++) {
         IndexA = rand() % tam;
         do {
            IndexB = rand() % tam;
         } while (IndexB == IndexA);
         // If it's the same index we are not doing any change in the list
         SWAP(candidatos[IndexA], candidatos[IndexB]);
      }
   }
}

sup::sup() {
   indice = 0;
}

sup::sup(casilla tablero[9][9]) {
   copiar(estado, tablero);
}

sup::sup(Sudoku& juego, int fila /* = -1*/, int columna /* = -1*/) {
   p_juego = &juego;
   indice = 0;
   copiar(estado, juego.m_tablero);
   atacar.setFilaColumna(fila, columna);
}
