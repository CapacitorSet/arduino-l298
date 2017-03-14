/*
MIT License

Copyright (c) 2017 CapacitorSet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Serve l'header per avere digitalWrite, HIGH/LOW e così via
#include <Arduino.h>

/* Permette all'utente di usare nomi human-friendly ("MOTORE-A") anziché 0 e 1.
 * L'enum funziona come un define; il typedef serve a creare un nuovo tipo di
 *   variabile che può essere o MOTORE_A o MOTORE_B
 */
typedef enum{MOTORE_A, MOTORE_B} motore_t;

class MotoreCC {
  // Membri privati: l'utente non deve toccarli
  private:
    int enable[2];
    int inAvanti[2];
    int inIndietro[2];
    int coppiaMinima;

    /* Questo, più che un array, viene usato come una lookup table:
     * dato un motore, dice qual è il motore opposto. È più comodo
     * che usare un if.
     */
    int motoreOpposto[2];

  public:
    // Costruttore
    MotoreCC(int _enA, int _enB, int _inAvantiA, int _inIndietroA, int _inAvantiB, int _inIndietroB, int _coppiaMinima) {      
      enable[MOTORE_A] = _enA;
      enable[MOTORE_B] = _enB;
      inAvanti[MOTORE_A]   = _inAvantiA;
      inIndietro[MOTORE_A] = _inIndietroA;
      inAvanti[MOTORE_B]   = _inAvantiB;
      inIndietro[MOTORE_B] = _inIndietroB;
      coppiaMinima = _coppiaMinima;

      motoreOpposto[MOTORE_A] = MOTORE_B;
      motoreOpposto[MOTORE_B] = MOTORE_A;
    }

    void setup() {
      pinMode(enable[MOTORE_A], OUTPUT);
      pinMode(enable[MOTORE_B], OUTPUT);
      pinMode(inAvanti[MOTORE_A], OUTPUT);
      pinMode(inAvanti[MOTORE_B], OUTPUT);
      pinMode(inIndietro[MOTORE_A], OUTPUT);
      pinMode(inIndietro[MOTORE_B], OUTPUT);
      
      disabilita();
    }

    /* Viene usato il method overloading: lo stesso metodo viene definito
     *   con diversi argomenti. Il compilatore sa quale metodo usare in
     *   base al tipo degli argomenti che gli passa l'utente.
     * Se non vengono passati argomenti, usa il secondo metodo; se viene
     *   passato un dato di tipo "motore_t" (cioè o MOTORE_A o MOTORE_B),
     *   usa il primo metodo.
     */
    void abilita(motore_t motore) {
      digitalWrite(enable[motore], HIGH);
    }
    void abilita(void) {
      abilita(MOTORE_A);
      abilita(MOTORE_B);
    }
    
    void disabilita(motore_t motore) {
      digitalWrite(enable[motore], LOW);
    }
    void disabilita(void) {
      disabilita(MOTORE_A);
      disabilita(MOTORE_B);
    }

    // Ferma un motore
    void stop(motore_t motore) {
      disabilita(motore);
    }
    // Ferma entrambi i motori
    void stop(void) {
      stop(MOTORE_A);
      stop(MOTORE_B);
    }

    // Stop veloce di un motore
    void stopVeloce(motore_t motore) {
      abilita(motore);
      digitalWrite(inAvanti[motore], LOW);
      digitalWrite(inIndietro[motore], LOW);
    }
    // Stop veloce di entrambi i motori
    void stopVeloce(void) {
      stopVeloce(MOTORE_A);
      stopVeloce(MOTORE_B);
    }

    // Fai girare in avanti un motore
    void avanti(motore_t motore) {
      abilita(motore);
      digitalWrite(inAvanti[motore], HIGH);
      digitalWrite(inIndietro[motore], LOW);
    }
    // Fai girare all'indietro un motore
    void indietro(motore_t motore) {
      abilita(motore);
      digitalWrite(inAvanti[motore], LOW);
      digitalWrite(inIndietro[motore], LOW);
    }

    // Ruota verso il motore dato
    void ruotaAvantiVerso(motore_t motore, int magnitudine) {
      analogWrite(inAvanti[motore], coppiaMinima);
      analogWrite(inAvanti[motoreOpposto[motore]], magnitudine);
    }
    // Ruota verso il motore dato
    void ruotaIndietroVerso(motore_t motore, int magnitudine) {
      analogWrite(inIndietro[motore], coppiaMinima);
      analogWrite(inIndietro[motoreOpposto[motore]], magnitudine);
    }
};
