/*
   Gradient Descent w/ momentum and optional "kicker"
   Performs ~980+ iterations/s (with logging and kicker turned off) on MKR4000.

   Logging can be turned on by setting logging to "1", and is designed for being dumped into a
   log by puTTY with single space delimiters.

   There is a toggelable kicker module that tries to randomly pop out of local minimas, a bit like a
   genetic algorithim mutation.

   Everything was initialized as float to reduce bugs from my bad coding. I also gave up on fully
   matrix/array batches to make debugging easier.
   It would speed up if I converted everything into integers, tried some polynomial approximattions of 
   the exp() function. Optimizations I made were to keep my float arrays 1 dimensional, replaced 
   the sigmoid derivative function with algebra, and removed some repeated operations in loops.

*/
#include "pitches.h"

float X0[4] = {0, 1, 0, 1};
float X1[4] = {0, 0, 1, 1};
float Y[4] = {0, 1, 1, 0};
float H0[4];
float H0B[4];
float H0sig[4];
float H1[4];
float H1B[4];
float H1sig[4];
float O[4];
float OB[4];
float Osig[4];

float dL;
float x;
float y;

float deltaB2;
float deltaB1;
float deltaB0;
float deltaWo1;
float deltaWo0;
float deltaWh00;
float deltaWh01;
float deltaWh10;
float deltaWh11;

float MWh00;
float MWh01;
float MWh10;
float MWh11;
float MWo0;
float MWo1;
float MB0;
float MB1;
float MB2;

float Wh00 = 20;
float Wh01 = 20;
float Wh10 = -20;
float Wh11 = -20;
float Wo0 = 20;
float Wo1 = 20;

float b0 = -10;
float b1 = 30;
float b2 = -30;

float OutMSE = 0;
float deltaMSE = 0;
float iterationOutMSE = 0;

float const ETA = -0.2;
float const BETA = 1;

int r, i, d;
long int count = 0;
long int report;
long int sessionTime0;
long int iterationTime;
long int sessionTime;
long int iterationCount;
long int complete;
int mutationCount = 0;
int check = 0;

int testInitiation = 0; // initialize with assignment 4 variables
int logging = 1;        // toggle logging, dump to file and separate with space delimiter
int kicker = 0;         // toggle random kicker/mutator to help with getting stuck in minimas

void setup() {
  Serial.begin(115200);
  delay(2000);
  x = analogRead(5);
  y = analogRead(1);
  x -=y;
  delay(200);
  Serial.println(x);
  randomSeed(x);
  

  if (testInitiation == 0) {
    //Weight and bias generation
    Wh00 = 5-random(10);
    Wh01 = 5-random(10);
    Wh10 = 5-random(10);
    Wh11 = 5-random(10);
    Wo0 = 5-random(10);
    Wo1 = 5-random(10);
    b0 = 5-random(10);
    b1 = 5-random(10);
    b2 = 5-random(10);
  }

  delay(5000);
  sessionTime, report = millis();
  if (logging == 1) {
    Serial.print("T(ms)");
    Serial.print("  ");
    Serial.print("DWo0");
    Serial.print("  ");
    Serial.print("DWo1");
    Serial.print("  ");
    Serial.print("DWH00");
    Serial.print("  ");
    Serial.print("DWH01");
    Serial.print("  ");
    Serial.print("DWH10");
    Serial.print("  ");
    Serial.print("DWH11");
    Serial.print("  ");
    Serial.print("DWB0");
    Serial.print("  ");
    Serial.print("DWB1");
    Serial.print("  ");
    Serial.print("DWB2");
    Serial.print("  ");
    Serial.print("MSE");
    Serial.print("  ");
    Serial.print("DMSE");
    Serial.print("  ");
    Serial.print("i/sec");
    Serial.print("  ");
    Serial.println("Out");
  }

}

float sig(float a) {
  return (1 / (1 + exp(-1 * a)));
}

void loop() {

  for (r = 0; r < 4; r++) {
    //layer0 to hidden
    H0[r] = X0[r] * Wh00 + X1[r] * Wh01;
    H0B[r] = H0[r] + b0;
    H0sig[r] = sig(H0B[r]);
    H1[r] = X0[r] * Wh10 + X1[r] * Wh11;
    H1B[r] = H1[r] + b1;
    H1sig[r] = sig(H1B[r]);


    //layer1 to output
    O[r] = H0sig[r] * Wo0 + H1sig[r] * Wo1;
    OB[r] = O[r] + b2;
    Osig[r] = sig(OB[r]);

  } //r

  //Check
  if ((Osig[0] < 0.2) && (Osig[1] > 0.7) && (Osig[2] > 0.7) && (Osig[3] < 0.2)) {
    if (check == 0) {
      complete = count;
      sessionTime0 = millis();
      check = 1;
    }
  }

  OutMSE = 0;
  for (r = 0; r < 4; r++) {
    x = (Y[r] - Osig[r]);
    OutMSE += x * x;
  }
  OutMSE = 0.5 * OutMSE;

  //output error

  deltaB2 = 0;
  deltaB1 = 0;
  deltaB0 = 0;
  deltaWo1 = 0;
  deltaWo0 = 0;
  deltaWh00 = 0;
  deltaWh01 = 0;
  deltaWh10 = 0;
  deltaWh11 = 0;


  //Gradient
  for (r = 0; r < 4; r++) {
    dL = (Osig[r] - Y[r]) * Osig[r] * (1 - Osig[r]);
    x = dL * Wo1 * (H1sig[r] * (1 - H1sig[r]));
    y = dL * Wo0 * (H0sig[r] * (1 - H0sig[r]));

    deltaB2 += dL;
    deltaB1 += x;
    deltaB0 += y;
    deltaWo1 += dL * H1sig[r];
    deltaWo0 += dL * H0sig[r];
    deltaWh00 += y * X0[r];
    deltaWh01 += y * X1[r];
    deltaWh10 += x * X0[r];
    deltaWh11 += x * X1[r];
  }

  if (kicker == 1) {
    if (deltaMSE < -0.0001) {
      mutationCount = 0;
    }
    if ((deltaMSE > -0.0001) && (OutMSE > 0.25)) {
      mutationCount++;
      if (mutationCount > 15000) {
        b2 += random(-10, 10);
        b1 += random(-10, 10);
        b0 += random(-10, 10);
        Wo1 += random(-10, 10);
        Wo0 += random(-10, 10);
        Wh00 += random(-10, 10);
        Wh01 += random(-10, 10);
        Wh10 += random(-10, 10);
        Wh11 += random(-10, 10);
        mutationCount = 0;
        Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      }
    }
  }

  // this looks weird b/c i was trying to reduce # of operations

  //Weight update with momentum

  b2 += BETA * MB2;
  MB2 = ETA * deltaB2;
  b2 += MB2;
  b1 += BETA * MB1;
  MB1 = ETA * deltaB1;
  b1 += MB1;
  b0 += BETA * MB0;
  MB0 = ETA * deltaB0;
  b0 += MB0;
  Wo1 += BETA * MWo1;
  MWo1 = ETA * deltaWo1;
  Wo1 += MWo1;
  Wo0 += BETA * MWo0;
  MWo0 = ETA * deltaWo0;
  Wo0 += MWo0;
  Wh00 += BETA * MWh00;
  MWh00 = ETA * deltaWh00;
  Wh00 += MWh00;
  Wh01 += BETA * MWh01;
  MWh01 = ETA * deltaWh01;
  Wh01 += MWh01;
  Wh10 += BETA * MWh10;
  MWh10 = ETA * deltaWh10;
  Wh10 += MWh10;
  Wh11 += BETA * MWh11;
  MWh11 = ETA * deltaWh11;
  Wh11 += MWh11;

  if (millis() - report >= 5000) {

    if (check == 1) {
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      Serial.print("Completed on iteration ");
      Serial.println(complete);
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      Serial.print((sessionTime0 - sessionTime) / 1000);
      Serial.print(" sec at ");
      Serial.print(iterationTime);
      Serial.println(" i/s");
      Serial.println("Output");
      for (r = 0; r < 4; r++) {
        Serial.print(O[r]);
        Serial.print("  ");
        Serial.println(Osig[r]);
      }
      Serial.println("Wo0 Wo1");
      Serial.print(Wo0);
      Serial.print(" ");
      Serial.println(Wo1);

      Serial.println("Wh00 Wh01");
      Serial.print(Wh00);
      Serial.print(" ");
      Serial.println(Wh01);

      Serial.println("Wh10 Wh11");
      Serial.print(Wh10);
      Serial.print(" ");
      Serial.println(Wh11);

      Serial.println("Bias 0,1,3");
      Serial.print(b0);
      Serial.print(" ");
      Serial.print(b1);
      Serial.print(" ");
      Serial.println(b2);
      Serial.println("MSE*10^6");
      Serial.println(OutMSE*1000000);
      Darude();
    }

    iterationTime = (count - iterationCount) / 5;

    if (logging == 1) {
      Serial.print(millis() - sessionTime);
      Serial.print("  ");
      Serial.print(Wo0);
      Serial.print("  ");
      Serial.print(Wo1);
      Serial.print("  ");
      Serial.print(Wh00);
      Serial.print("  ");
      Serial.print(Wh01);
      Serial.print("  ");
      Serial.print(Wh10);
      Serial.print("  ");
      Serial.print(Wh11);
      Serial.print("  ");
      Serial.print(b0);
      Serial.print("  ");
      Serial.print(b1);
      Serial.print("  ");
      Serial.print(b2);
      Serial.print("  ");
      Serial.print(OutMSE * 1000000);
      Serial.print("  ");
      deltaMSE = OutMSE - iterationOutMSE;
      Serial.print(deltaMSE * 1000000);
      Serial.print("  ");
      Serial.print(iterationTime);
      Serial.print("  ");
      Serial.print(Osig[0]);
      Serial.print(Osig[1]);
      Serial.print(Osig[2]);
      Serial.println(Osig[3]);

      iterationOutMSE = OutMSE;
      iterationCount = count;
    }
    report = millis();
  }

  count++;
} //main
























void Darude() {
  d = 1;
  while (d == 1) {

    // Jordi Agricola 2014
    // port 8

    int shortTone = 56;
    int longTone = 140;
    int standardDelay = 31;
    int delayBetweenBars = 31.5;

    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, longTone);
    delay(longTone);
    noTone(8);
    delay(delayBetweenBars);

    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8, NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8,  NOTE_E4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8, NOTE_E4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_E4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_E4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_E4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_E4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_E4, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8,  NOTE_D4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8, NOTE_D4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_D4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_D4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_D4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_D4, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_D4, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8, NOTE_A3, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, longTone);
    delay(longTone);
    noTone(8);
    delay(delayBetweenBars);

    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8, NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8, NOTE_E4, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, longTone);
    delay(longTone);
    noTone(8);
    delay(delayBetweenBars);

    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8, NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, shortTone);
    delay(shortTone);
    noTone(8);
    delay(standardDelay);
    tone(8,  NOTE_B3, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);

    tone(8, NOTE_E4, longTone);
    delay(longTone);
    noTone(8);
    delay(standardDelay);
  }

}
