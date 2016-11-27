#include <Key.h>
#include <Keypad.h>


const unsigned long period=50;  //little period used to prevent error
unsigned long kdelay=0;        // variable used in non-blocking delay 


const byte ROWS_CNT = 4;             //number of rows of keypad
const byte COLUMNS_CNT = 4;            //number of columnss of keypad
const byte OUT_PINS[ROWS_CNT] = {2, 3, 4, 5}; //array of pins used as output for rows of keypad
const byte IN_PINS[COLUMNS_CNT] = {6, 7, 8, 9}; //array of pins used as input for columnss of keypad

int col4LED = 10;
int col3LED = 11;
int col2LED = 12;
int col1LED = 13;

int row4LED = A2;
int row3LED = A3;
int row2LED = A4;
int row1LED = A5;

int blueLED = A1;

int pushButton = A0;

int buttonValue = 0;
int buttonValueOriginal = 0;
int buttonState = 0;

int keyPadValueOriginal;
int keyPadValue;

byte keypad()
{
  static bool no_press_flag = true;
  byte col = 0, row = 0;
  byte result = 50;
  // TODO: Wait a while
  for (byte i = 0; i < ROWS_CNT; ++i) {
    digitalWrite(OUT_PINS[i], LOW);
    // TODO: Wait a while
    for (byte j = 0; j < COLUMNS_CNT; ++j) {
      if (digitalRead(IN_PINS[j]) == LOW) {
        result = i << 2 | j;
        break;
      }
    }
    digitalWrite(OUT_PINS[i], HIGH);
    if (result != 50) {
      break;
    }
  }
  return result;
}

/*
byte keypad() // function used to detect which button is used 
{
 static bool no_press_flag=0;  //static flag used to ensure no button is pressed 
  for(byte x=0;x<columns;x++)   // for loop used to read all inputs of keypad to ensure no button is pressed 
  {
     if (digitalRead(Input[x])==HIGH);   //read evry input if high continue else break;
     else
     break;
     if(x==(columns-1))   //if no button is pressed 
     {
      no_press_flag=1;
      h=0;
      v=0;
     }
  }
  if(no_press_flag==1) //if no button is pressed 
  {
    for(byte r=0;r<rows;r++) //for loop used to make all output as low
    digitalWrite(Output[r],LOW);
    for(h=0;h<columns;h++)  // for loop to check if one of inputs is low
    {
      if(digitalRead(Input[h])==HIGH) //if specific input is remain high (no press on it) continue
      continue;
      else    //if one of inputs is low
      {
          for (v=0;v<rows;v++)   //for loop used to specify the number of row
          {
          digitalWrite(Output[v],HIGH);   //make specified output as HIGH
          if(digitalRead(Input[h])==HIGH)  //if the input that selected from first sor loop is change to high
          {
            no_press_flag=0;                //reset the no press flag;
            for(byte w=0;w<rows;w++) // make all outputs as low
            digitalWrite(Output[w],LOW);
            return v*4+h;  //return number of button 
          }
          }
      }
    }
  }
 return 50;
}
*/

// the setup routine runs once when you press reset:
void setup() {
  
  for(byte i=0;i<ROWS_CNT;i++)  //for loop used to make pin mode of outputs as output
  {
  pinMode(OUT_PINS[i],OUTPUT);
  }
  for(byte s=0;s<COLUMNS_CNT;s++)  //for loop used to make pin mode of inputs as inputPullup;
  {
    pinMode(IN_PINS[s],INPUT_PULLUP);
  }
  
  pinMode(col1LED , OUTPUT);
  pinMode(col2LED , OUTPUT);
  pinMode(col3LED , OUTPUT);
  pinMode(col4LED , OUTPUT);
  pinMode(row1LED , OUTPUT);
  pinMode(row2LED , OUTPUT);
  pinMode(row3LED , OUTPUT);
  pinMode(row4LED , OUTPUT);
  
  
  pinMode(blueLED , OUTPUT);


  pinMode(pushButton, INPUT);
  
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  buttonValueOriginal = buttonValue;
  buttonValue = digitalRead(pushButton);

  // Serial.println(buttonValue);
  
  if(buttonValue == HIGH) { 
    if (buttonValue != buttonValueOriginal)
    {
      buttonState = !buttonState;
    }
    delay(60);
  }
  if(buttonState == 1) {
    digitalWrite(blueLED, HIGH);
  }
  if(buttonState == 0) {
    digitalWrite(blueLED, LOW);
  }
 
  //if(millis()-kdelay > period) //used to make non-blocking delay
  //{
    keyPadValueOriginal = keyPadValue;
    keyPadValue = keypad();
    //kdelay = millis();  //capture time from millis function
    if (keyPadValueOriginal != keyPadValue) 
    {
      switch (keyPadValue)  //switch used to specify which button
      {
            case 0:
            Serial.print("0");
        digitalWrite(col1LED, HIGH);
        digitalWrite(row1LED, HIGH);
       break;  
            case 1:
            Serial.print("1");
        digitalWrite(col2LED, HIGH);
        digitalWrite(row1LED, HIGH);
       break;
            case 2:
            Serial.print("2");
        digitalWrite(col3LED, HIGH);
        digitalWrite(row1LED, HIGH);
       break;
            case 3:
            Serial.print("3");
        digitalWrite(col4LED, HIGH);
        digitalWrite(row1LED, HIGH);
       break;
            case 4:
            Serial.print("4");
        digitalWrite(col1LED, HIGH);
        digitalWrite(row2LED, HIGH);
       break;
            case 5:
            Serial.print("5");
        digitalWrite(col2LED, HIGH);
        digitalWrite(row2LED, HIGH);
       break;
            case 6:
            Serial.print("6");
        digitalWrite(col3LED, HIGH);
        digitalWrite(row2LED, HIGH);
       break;
            case 7:
            Serial.print("7");
        digitalWrite(col4LED, HIGH);
        digitalWrite(row2LED, HIGH);
       break;
            case 8:
            Serial.print("8");
        digitalWrite(col1LED, HIGH);
        digitalWrite(row3LED, HIGH);
       break;
            case 9:
            Serial.print("9");
        digitalWrite(col2LED, HIGH);
        digitalWrite(row3LED, HIGH);
       break;
            case 10:
            Serial.print("A");
            digitalWrite(col3LED, HIGH);
        digitalWrite(row3LED, HIGH);
       break;
            case 11:
            Serial.print("B");
            digitalWrite(col4LED, HIGH);
        digitalWrite(row3LED, HIGH);
            
       break;
            case 12:
            Serial.print("C");   
            digitalWrite(col1LED, HIGH);
        digitalWrite(row4LED, HIGH);
       break;
            case 13:
            Serial.print("D");
            digitalWrite(col2LED, HIGH);
        digitalWrite(row4LED, HIGH);
       break;
            case 14:
            Serial.print("E");
            digitalWrite(col3LED, HIGH);
        digitalWrite(row4LED, HIGH);
       break;
            case 15:
            Serial.print("F");
            digitalWrite(col4LED, HIGH);
        digitalWrite(row4LED, HIGH);
  
            
       break;
            default:
        digitalWrite(col1LED, LOW);
        digitalWrite(col2LED, LOW);
        digitalWrite(col3LED, LOW);
        digitalWrite(col4LED, LOW);
        digitalWrite(row1LED, LOW);
        digitalWrite(row2LED, LOW);
        digitalWrite(row3LED, LOW);
        digitalWrite(row4LED, LOW);
        
      }
    //}
  }
  
}
