#include "l298.cpp"

// Implementazione di esempio

// En1, En2, In1, In2, In3, In4, Coppia minima
MotoreCC m(3, 4, 5, 6, 9, 10, 38);

void setup() {
  m.setup();

  // Fai andare avanti entrambi i motori per 1 secondo
  // Ferma A e fai andare indietro B per 1 secondo
  // Fai ruotare a velocità massima verso A per 3 secondi
  // Ferma tutto
  m.avanti(MOTORE_A);
  m.avanti(MOTORE_B);
  delay(1000);
  m.stopVeloce(MOTORE_A);
  m.indietro(MOTORE_B);
  delay(1000);
  m.ruotaAvantiVerso(MOTORE_A, 255);
  delay(3000);
  m.stopVeloce();
}

void loop() {
}

