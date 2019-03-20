#include <math.h>

const int ri = 4; //input rows
const int ci = 2; //input columns/input node count
const int ro = ri; //output rows/input rows
const int co = 1; //output columns/node count
const int hn = 2; //hidden node count

const float ETA = 0.25; // learn rate
const int b0 = -5; //layer 1 bias (gets doubled)
const int b1 = 15; //layer 2 bias (gets doubled)

const int inArray[ri][ci] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1}
};

const int outArray[ro][co] = {
  {0},
  {1},
  {1},
  {0}
};

float inWeight[ci][hn]; //row for each input column, column for each hidden node
float intoHid[ri][hn]; // dot of inArray and inWeight = [inArray row]  [weight column]
float hidWeight[hn][co]; //row for each input row, column for each hid node
float intoOut[ri][co]; //dot of intoHid and hidWeight. row for each in row, column for each output

float errorT[ro][co]; // total error
float errorAccum; //accumulator for error propagation
float hiddenError;
float hidWeightSum;
float errorDelta;
float hiddenDelta[hn][hn];
float inError[hn][co];
float inDelta[ri][ci];
float inNodeDelta[co][ci];

float iterationOut[ro][co];

int check;
int breakOut;
int i, j, k, r, c;

void setup() {
  Serial.begin(9600);
  randomSeed(12);

  //hidWeight[hn][co]
  for (r = 0; r < hn; r++) {
    for (c = 0; c < co; c++) {
      hidWeight[r][c] = float(random(100)) / 100;
    }
  }

  //inWeight[ci][hn]
  for (r = 0; r < ci; r++) {
    for (c = 0; c < hn; c++) {
      inWeight[r][c] = float(random(100)) / 100;
    }
  }

}

//sigmoid
float sigmoid(float a) {
  return (1 / (1 + exp(-a)));
}
float sigDeriv(float a) {
  return a * (1 - a);
}


void loop() {

  Serial.println();
  Serial.println();
  Serial.print("inArray");
  for (i = 0; i < ri; i++) { //interate trough rows
    Serial.print('\n');
    for (j = 0; j < ci; j++) { //iterate through column
      Serial.print(inArray[i][j]);
      Serial.print(" ");
    }
  }
  Serial.println();
  Serial.println();
  Serial.print("inWeight");
  for (r = 0; r < ci; r++) { //interate trough rows
    Serial.print('\n');
    for (c = 0; c < hn; c++) { //iterate through column
      Serial.print(inWeight[r][c]);
      Serial.print(" ");
    }
  }
  Serial.println();
  Serial.println();
  Serial.print("hidWeight");
  for (r = 0; r < hn; r++) { //interate trough rows
    Serial.print('\n');
    for (c = 0; c < co; c++) { //iterate through column
      Serial.print(hidWeight[r][c]);
      Serial.print(" ");
    }
  }
memset(intoHid,0,sizeof(intoHid));
  //input layer

  for (r = 0; r < ri; r++) {
    for (i = 0; i < ci; i++) {
      for (c = 0; c < ci; c++) {
        intoHid[r][i] += inArray[r][c] * inWeight[i][c] + b0;
      }
    }
  }

  Serial.println();
  Serial.println();
  Serial.print("intoHid");
  for (r = 0; r < ri; r++) { //interate trough rows
    Serial.print('\n');
    for (c = 0; c < hn; c++) { //iterate through column
      Serial.print(intoHid[r][c]);
      Serial.print(" ");
    }
  }
memset(intoOut,0,sizeof(intoOut));
  //hidden layer
  for (r = 0; r < ri; r++) {
    for (i = 0; i < co; i++) {
      for (c = 0; c < hn; c++) {
        intoOut[r][i] += sigmoid(intoHid[r][c]) * hidWeight[c][i] + b1;
      }
    }
  }

  Serial.println();
  Serial.println();
  Serial.print("intoOut");
  for (r = 0; r < ri; r++) { //interate trough rows
    Serial.print('\n');
    for (c = 0; c < co; c++) { //iterate through column
      Serial.print(intoOut[r][c]);
      Serial.print(" ");
    }
  }

  //Activation/Output
  for (r = 0; r < ro; r++) {
    for (c = 0; c < co; c++) {
      iterationOut[r][c] = sigmoid(intoOut[r][c]);
    }
  }

  Serial.println();
  Serial.println("Nomralized Output");
  for (i = 0; i < ro; i++) { //interate trough rows
    Serial.print('\n');
    for (j = 0; j < co; j++) { //iterate through column
      Serial.print(iterationOut[i][j]);
      Serial.print(" ");
    }
  }

  //check
  check = 0;
  breakOut = 0;
  for (r = 0; r < ro; r++) {
    for (c = 0; c < co; c++) {
      if (outArray[r][c] == iterationOut[r][c]) {
        check++;
      }
      else {
        breakOut = 1;
        break;
      }
    }
    if (breakOut = 1) {
      break;
    }
  }
  if (check >= ro) {
    Serial.println("!!!!!!!!!!!!!!!!!!!");
    Serial.println("!!!!!!!!!!!!!!!!!!!");
    Serial.println("Complete");
    Serial.println("!!!!!!!!!!!!!!!!!!!");
    Serial.println("!!!!!!!!!!!!!!!!!!!");
    Serial.println("Output");
    for (i = 0; i < ro; i++) { //interate trough rows
      Serial.print('\n');
      for (j = 0; j < co; j++) { //iterate through column
        Serial.print(intoOut[i][j]);
        Serial.print(" ");
      }
    }
    Serial.println("Nomralized Output");
    for (i = 0; i < ro; i++) { //interate trough rows
      Serial.print('\n');
      for (j = 0; j < co; j++) { //iterate through column
        Serial.print(iterationOut[i][j]);
        Serial.print(" ");
      }
    }

    Serial.println("Final Input Weights:");
    for (i = 0; i < ci; i++) { //interate trough rows
      Serial.print('\n');
      for (j = 0; j < hn; j++) { //iterate through column
        Serial.print(inWeight[i][j]);
        Serial.print(" ");
      }
    }
    Serial.println("Final Hidden Weights:");
    for (i = 0; i < hn; i++) { //interate trough rows
      Serial.print('\n');
      for (j = 0; j < co; j++) { //iterate through column
        Serial.print(hidWeight[i][j]);
        Serial.print(" ");
      }
    }
    delay(20000);
  }

  //iterate weights
  //error 0.5*(target-out)^2

  for (r = 0; r < ri; r++) {
    for (c = 0; c < co; c++) {
      errorT[r][c] = iterationOut[r][c] * (1 - iterationOut[r][c]) * (outArray[r][c] - iterationOut[r][c]); //delta output sum
      errorDelta += 0.5 * errorT[r][c] * errorT[r][c];
    }
  }
  Serial.println();
  Serial.println("Total Error");
  Serial.print('\n');
  Serial.print(errorDelta * 1000);





  //backprop to hidden, check against inWeight
  //hiderror
  for (r = 0; r < hn; r++) {
    for (i = 0; i < co; i++) {
      for (c = 0; c < hn; c++) {
        hiddenError = errorDelta * hidWeight[c][i]; //z2 error
        hiddenDelta[r][c] = iterationOut[r][i] * (1 - iterationOut[r][i]) * hiddenError;
      }
    }
  }


  //iterate hidden weights
  for (r = 0; r < hn; r++) {
    for (c = 0; c < co; c++) {
      hidWeight[r][c] += ETA * hiddenDelta[r][c];
    }
  }

  for (r = 0; r < ci; r++) {
    for (i = 0; i < hn; i++) {
      for (c = 0; c < hn; c++) {
        inError[r][i] += hiddenDelta[r][i] * inWeight[i][c];
      }
    }
  }

  for (r = 0; r < ci; r++) {
    for (k = 0; k < ci; k++) {
      for (c = 0; c < ci; c++) {
        inDelta[r][k] += sigDeriv(sigmoid(intoHid[r][k])) * inError[r][c];
      }
    }
  }
  for (j = 0; j < co; j++) {
    for (r = 0; r < ri; r++) {
      for (c = 0; c < ci; c++) {
        inNodeDelta[j][c] += 0.5 * inDelta[r][c];
      }
    }
  }


  //iterate input weights
  for (j = 0; j < co; j++) {
    for (r = 0; r < ci; r++) {
      for (c = 0; c < hn; c++) {
        for (k = 0; k < ri; k++) {
          inWeight[r][c] += inArray[k][c] * ETA * inNodeDelta[j][c];
        }
      }
    }
  }

  delay(2000);
}

/*

  Serial.println();
  Serial.println();
  Serial.print("inArray");
  for(i=0;i<ri;i++){//interate trough rows
  Serial.print('\n');
  for(j=0;j<ci;j++){//iterate through column
    Serial.print(inArray[i][j]);
    Serial.print(" ");
  }
  }


*/
