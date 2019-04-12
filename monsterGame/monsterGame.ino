int rows = 2;
int cols = 16;

int cycle = 0;

int forward = 0;
int backward = 1;
int up = 6;
int down = 7;

int meY = 0;
int meX;
int monsterY = 0;
int monsterX;
void setup()
{
  initThing(rows, cols);
  fillscreen('*');
  //pinMode(0,INPUT);
  //pinMode(1,INPUT);
  pinMode(6, INPUT);
  //pinMode(7,INPUT);
  meX = 0;
  monsterX = 14;

}

void loop(){

//game over check
  if ((monsterX == meX) && (monsterY == meY)) {
    fillscreen('!');
    render();
    exit(0);
  }
  if(cycle>cols){
    fillscreen('!');
    render();
    exit(0);
  }

  placeThing(0, 15, 'G', -1, 0, 0, cycle);
  placeThing(0, 0, 'P', 0, 0, 1, cycle);


  //Monster follows you up and down
  if (monsterY < meY) {
    placeThing(0, 15, 'G', -1, 1, 0, cycle);
    monsterY++;
  }
  if (monsterY > meY) {
    placeThing(0, 15, 'G', -1, -1, 0, cycle);
    monsterY--;
  }
  monsterX--;


  //Player Controls: 4 butons, up down left right
  if (digitalRead(0) == LOW) {
    placeThing(0, 0, 'P', 0, 0, 1, cycle);
  }
  if (digitalRead(1) == LOW) {
    placeThing(0, 0, 'P', 0, 0, 1, cycle);
  }
  if (digitalRead(6) == LOW) {
    placeThing(0, 0, 'P', 0, 0, 1, cycle);
  }
  if (digitalRead(7) == LOW) {
    placeThing(0, 0, 'P', 0, 0, 1, cycle);
  }

  if (digitalRead(0) == HIGH) {
    placeThing(0, 0, 'P', 1, 0, 1, cycle);
    meX++;
    if (meX > rows) {
      meX = rows;
    }
  }
  if (digitalRead(1) == HIGH) {
    placeThing(0, 0, 'P', -1, 0, 1, cycle);
    meX--;
    if (meX < cols) {
      meX = cols;
    }
  }

  if (digitalRead(6) == HIGH) {
    placeThing(0, 0, 'P', 0, 1, 1, cycle);
    meY++;
    if (meY > rows) {
      meY = rows;
    }
  }

  if (digitalRead(7) == HIGH) {
    placeThing(0, 0, 'P', 0, -1, 1, cycle);
    meY--;
    if (meY < 0) {
      meY = 0;
    }
  }


  //render screen
  render();
  cycle++;

  //game speed
  delay(500);
}
