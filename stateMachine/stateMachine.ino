

long tR2G = 10000;
long tG2Y = 8000;
long tY2R = 2000;
long timer;

int state = 0;   //initial state all red

int crosswalk1BTN =0;
int crosswalk2BTN = 0;
int road1Sense =0;
int road2Sense =0;


void setup() {
  Serial.begin(1152000);
  while (!Serial);
  delay(5000);
  stateOutput(state);


}

void stateOutput(int state){

  switch (state) {
    case 0: 
    //All red
    Serial.println("Road 1: Red, Crosswalk 1: Red");
    Serial.println("Road 2: Red, Crosswalk 2: Red");
    Serial.println();
    break;
    case 1:
    //car1 red, ped 1 green / car 2 green, ped 2 red
    Serial.println("Road 1: Red, Crosswalk 1: Green");
    Serial.println("Road 2: Green, Crosswalk 2: Red");
    Serial.println();
    break;
    case 2:
    //car1 green, ped 1 red / car 2 red, ped 2 green
    Serial.println("Road 1: Green, Crosswalk 1: Red");
    Serial.println("Road 2: Red, Crosswalk 2: Green");
    Serial.println();
    break;
    case 3:
    //car1 yellow, ped 1 red / car 2 red, ped 2 red
    Serial.println("Road 1: Yellow, Crosswalk 1: Red");
    Serial.println("Road 2: Red, Crosswalk 2: Red");
    Serial.println();
    break;
    case 4:
      //car1 red, ped 1 red /car 2 yellow, ped 2 red
    Serial.println("Road 1: Red, Crosswalk 1: Red");
    Serial.println("Road 2: Yellow, Crosswalk 2: Red");
    Serial.println();
    break;

  }
}



void loop() {

//enter state 2
state = 2;
stateOutput(state);

//timer&&(car2Sensor||ped1sensor) until state 3

timer = millis();
while(((millis()-timer)<tG2Y)&&(((millis()-timer)<4000)&&(road2Sense != 1 || crosswalk1BTN != 1))){
  //do nothing 
}
state = 3;
stateOutput(state);

//timer until state 1

timer = millis();
while((millis()-timer<tY2R)){
  //do nothing
}

state = 1;
stateOutput(state);

//timer&&(car1Sensor||ped2Sensor) until state 4
timer = millis();
while(((millis()-timer)<tG2Y)&&(((millis()-timer)<4000)&&(road1Sense != 1 || crosswalk2BTN != 1))){
  //do nothing 
}

state = 4;
stateOutput(state);

//timer until return to state 2

timer = millis();
while((millis()-timer<tY2R)){
  //do nothing
}


  }
