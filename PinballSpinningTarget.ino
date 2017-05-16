/*
  Code for using a photo interrupter with Arduino 
  to count the number of state changes and show 
  the count on a display
  Made by Anders Holm - 16.may 2017
*/

// this constant won't change:
const int photointerrupterPin = 2;    // the pin that the photointerrupter is attached to
const int ledPin = 3;                 // the pin that the LED is attached to

// Variables will change:
int score = 0;                    // counter for adding the score
int photointerrupterState = 0;         // current state of the photointerrupter
int photointerrupterLastState = 0;     // previous state of the photointerrupter

#define MAX7219_DIN 6  // set up for the display
#define MAX7219_CS  7
#define MAX7219_CLK 8

void setup() {
  pinMode(photointerrupterPin, INPUT); // initialize the photointerrupter pin as a input
  pinMode(ledPin, OUTPUT);             // initialize the LED as an output
  Serial.begin(9600);                  // initialize serial communication
  
  initialiseMAX7219();
  output(0x0f, 0x00); //display test register - test mode off
  output(0x0c, 0x01); //shutdown register - normal operation
  output(0x0b, 0x07); //scan limit register - display digits 0 thru 7
  output(0x0a, 0x0f); //intensity register - max brightness
  output(0x09, 0xff); //decode mode register - CodeB decode all digits
  output(0x08, 0x0c); //digit 7 (leftmost digit) data
  output(0x07, 0x0b);
  output(0x06, 0x0d);
  output(0x05, 0x0e);
  output(0x04, 0x08);
  output(0x03, 0x07);
  output(0x02, 0x06);
  output(0x01, 0x05); //digit 0 (rightmost digit) data
}

void loop() {
  // read the photointerrupter input pin:
  photointerrupterState = digitalRead(photointerrupterPin);

  // compare the photointerrupterState to its previous state
  if (photointerrupterState != photointerrupterLastState) {
    // if the state has changed, increment the score counter
    if (photointerrupterState == HIGH) {
      // if the current state is HIGH then the photointerrupter went from off to on:
      digitalWrite(ledPin, LOW);
      score++;
      Serial.print("Blocked  ");
      Serial.print("Counter:  ");
      Serial.println(score);
    }
    else {
      // if the current state is LOW then the button
      // wend from on to off:
      digitalWrite(ledPin, HIGH);
      Serial.println("Clear");
    }
    delay(50);        // Delay a little bit to avoid bouncing
  }
  // save the current state as the last state, for next time through the loop
  photointerrupterLastState = photointerrupterState;

  displayScore(score);
}


//This shows the score variable on the 8-digit display
void displayScore(int score)
{
  //calculate number of digits
  String myString = String(score);
  int numberOfDigits = myString.length();
    //clear unused digits
    for (int i = 8; i > numberOfDigits; i--)
    {
      output(i, 0x0F);
    }
    //write digits to the display
    for (int i = 1; i <= numberOfDigits; i++)
    {
      output(i, myString[numberOfDigits-i]);
    }
} //end of score display

void initialiseMAX7219()   //  MAX7219 7-Segment LED Display 
{
  digitalWrite(MAX7219_CS, HIGH);
  pinMode(MAX7219_DIN, OUTPUT);
  pinMode(MAX7219_CS, OUTPUT);
  pinMode(MAX7219_CLK, OUTPUT);
}

void output(byte address, byte data)  //common routine to talkt o MAX7219
{
  digitalWrite(MAX7219_CS, LOW);
  shiftOut(MAX7219_DIN, MAX7219_CLK, MSBFIRST, address);
  shiftOut(MAX7219_DIN, MAX7219_CLK, MSBFIRST, data);
  digitalWrite(MAX7219_CS, HIGH);
}
