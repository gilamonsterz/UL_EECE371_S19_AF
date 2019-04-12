/*
 * User functions:
 * call initThing(rows,columns): use in startup() to initialize LCD up to 64x64 rows x columns
 * call placeThing(row, column, thing, xrate, yrate, clearThing, count) 
 *        int row, int column: initial location of character (only used once in the very first loop of the program)
 *        char 'thing': character to be displayed
 *        int xrate, int yrate: how many LCD segments to move 'thing' each program loop in the x and y directions (negative int for 
 *          left/down, 0 for stationary, positice int for forward/up)
 *        clearThing: whether or not the moving character clears the segment it moves through or if the original contents of the segment
 *          remain after the character moves away
 *        count: the number of program loops that have elapsed beginning at 0
 * call deleteThing(row,column): Simpler version of placeThing(). Writes an empty space " " to (int row, int column) 
 * call writeThing(row,column,thing): Simpler version of placeThing(). Writes char 'thing' to (int row, int column). 
 * call fillscreen(character): fills the screen with char 'character'       
 * call render(): updates the LCD with the current game state
 *        
 */


#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

struct LCDstruct
{
  int row;
  int col;
  char myArray[64][64];
} LCDbuffer;

void myLCD(int row, int col)
{
  lcd.begin(col, row);
  LCDbuffer.row = row;
  LCDbuffer.col = col;
}

void initThing(int row, int col)
{

  myLCD(row, col);
  for (int r = 0; r < row; r++)
  {
    for (int c = 0; c < col; c++)
    {
      LCDbuffer.myArray[r][c] = ' ';
    }
  }
}


void fillscreen(char thing) {
  for (int r = 0; r < LCDbuffer.row; r++) {
    for (int c = 0; c < LCDbuffer.col; c++) {
      placeThing(r, c, thing, 0, 0, 0, 0);
    }
  }
}

void placeThing(int row, int col, char thing, int Xrate, int Yrate, int clearThing, int count)
{
  if (count < 1) {
    LCDbuffer.myArray[row][col] = thing;
  }
  Xvelocity(thing, Xrate, clearThing);
  Yvelocity(thing, Yrate, clearThing);
}

void Xvelocity(char thing, int rate, int clearThing)
{
  char x;
  int done = 0;
  if ((rate > 0) || (rate < 0)) {
    for (int r = 0; r < LCDbuffer.row; r++)
    {
      if (done == 1) {
        break;
      }
      for (int c = 0; c < LCDbuffer.col; c++)
      {
        if (LCDbuffer.myArray[r][c] == thing)
          if ((c <= LCDbuffer.col) && (c >= 0)) {
            {
              x = LCDbuffer.myArray[r][c + rate];
              LCDbuffer.myArray[r][c + rate] = thing;
              if (clearThing == 1) {
                LCDbuffer.myArray[r][c] = ' ';
              }
              else {
                LCDbuffer.myArray[r][c] = x;
              }
              done = 1;
              break;
            }
          }
          if (c == LCDbuffer.col) {}
      }
    }
  }
}


void Yvelocity(char thing, int rate, int clearThing)
{
  char x;
  int done = 0;
  if ((rate > 0) || (rate < 0)) {
    for (int r = 0; r < LCDbuffer.row; r++)
    {
      if (done == 1) {
        break;
      }
      for (int c = 0; c < LCDbuffer.col; c++)
      {

        if (LCDbuffer.myArray[r][c] == thing) {
          
          if ((r < LCDbuffer.row-1) && (r >= 0)) {
            x = LCDbuffer.myArray[r + rate][c];
            LCDbuffer.myArray[r + rate][c] = thing;
            {
              LCDbuffer.myArray[r + rate][c] = thing;
              if (clearThing == 1) {
                LCDbuffer.myArray[r][c] = ' ';
              }
              else {
                LCDbuffer.myArray[r][c] = x;
              }
              done = 1;
              break;
            }
          }
          
        }
      }
    }
  }
}

void render() {
  for (int r = 0; r < LCDbuffer.row; r++) {
    for (int c = 0; c < LCDbuffer.col; c++) {
      lcd.setCursor(c, r);
      lcd.print(LCDbuffer.myArray[r][c]);
    }
  }
}

void deleteThing(int row, int col)
{
  lcd.setCursor(row, col);
  lcd.print(" ");
}

void writeThing(int row, int col, char thing)
{
  lcd.setCursor(row, col);
  lcd.print(thing);
}
