// (c) by Paolo CRAVERO IK1ZYW 2016. All rights reserved.
//
// No responsibility is taken for any use of this code,
// which is provided as an academic guidance to fellow builders.
// -------------------------------------------------------------

#include <Wire.h>
#include <Button.h>


// **** DEFINITIONS ****
int inputs[4] = {5,9,10,11}; // Q1, Q2, Q4, Q8 outputs

int latches[4] = {8,7,6,4}; // latches LSD to MSD

//int decimalPoint = 12; // pin for decimal point
#define decimalPoint 12

//int oneSecondInterruptPin = 2; // pin for timekeeping, D2
#define oneSecondInterruptPin 2

//int sensorPin = A2; // LDR for intensity control
#define sensorPin A2

#define setHoursPin A0 // pull to ground to set hours
#define setMinutesPin A1 // pull to ground to set minutes

Button button1(setHoursPin, PULLUP); // Connect your button between pin 2 and GND
Button button2(setMinutesPin, PULLUP); // Connect your button between pin 3 and GND

bool blinker = 0;  // blinking control variable
bool dpBlink = 0; // decimal point blinking control; set to 1 to make decimal points blink
bool digitBlink = 0; // digit blinking control; set to 1 to make digits blink
bool allOff = 2; // used for PWM brightness control

bool secondElapsed = 0;

unsigned int intensity = 0; // PWM-like intensity control
int intensitySteps = 10; // how many intensity levels we can do, 1 to 255
int sensorValue = 0;  // variable to store the value coming from the sensor

byte BCD[16][4] ={ // LSB to MSB
{0,0,0,0}, // 0
{1,0,0,0}, // 1
{0,1,0,0}, // 2
{1,1,0,0}, // 3
{0,0,1,0}, // 4
{1,0,1,0}, // 5
{0,1,1,0}, // 6 
{1,1,1,0}, // 7
{0,0,0,1}, // 8
{1,0,0,1}, // 9
{0,1,0,1}, // test A
{1,1,0,1}, // blank B
{0,0,1,1}, // blank C
{1,0,1,1}, // - D
{0,1,1,1}, // blank E
{1,1,1,1}  // blank F
}; 


void setup() {

  Serial.begin(9600);
  Wire.begin();

  pinMode(sensorPin, INPUT_PULLUP);
//  pinMode(setHoursPin, INPUT_PULLUP);
//  pinMode(setMinutesPin, INPUT_PULLUP);
  pinMode(oneSecondInterruptPin, INPUT_PULLUP); // DS1307 square wave output is open-drain.
    
  for(int a = 0; a < 4; a++){pinMode(inputs[a], OUTPUT);} //set outputs
  for(int a = 0; a < 4; a++){pinMode(latches[a], OUTPUT);} //set outputs
  pinMode(decimalPoint, OUTPUT); // set DP output
  digitalWrite(decimalPoint, HIGH); // DP off
  for(int a = 0; a < 4; a++){digitalWrite(latches[a], HIGH);} // unset latch

  if ( button1.isPressed() ) {
    //attivo la comunicazione con il DS1307
    //l'indirizzo dell'RTC è 0x68
    Wire.beginTransmission(0x68);
    //il primo byte stabilisce il registro
    //iniziale da scivere
    Wire.write((byte)0x00);
    //specifico il tempo e la data
    Wire.write((byte)0x00); //1° byte SECONDI da 0x00 a 0x59
    Wire.write((byte)0x54); //2° byte MINUTI da 0x00 a 0x59
    Wire.write((byte)0x80 | 0x23); //3° byte ORE da 0x00 a 0x24
    Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
    Wire.write((byte)0x05); //5° byte GIORNO del mese da 0x00 a 0x31
    Wire.write((byte)0x05); //6° byte MESE da 0x00 a 0x12
    Wire.write((byte)0x16); //7° byte ANNO 0x00 a 0x99
    Wire.endTransmission();
  
    delay(50);
  
    // set square wave output at 1 Hz
    Wire.beginTransmission(0x68);
    Wire.write((byte)0x07);
    Wire.write((byte)0x10); // control byte to set and enable 1 Hz SQW output
    Wire.endTransmission();
  }

  // hello world, show I'm OK
//  printBCD(0,0x12,1,1);
//  printBCD(2,0x34,1,1);
//  delay(1000);
//  printBCD(0,0x56,1,1);
//  printBCD(2,0x79,1,1);
//  delay(1000);

  // start interrupt at the end of startup sequence
  attachInterrupt(digitalPinToInterrupt(oneSecondInterruptPin), oneSecondISR, FALLING);  

}

int decToBcd(int val)
{
  return ( ((val/10)*16) + (val%10) );
}

int bcdToDec(int val)
{
  return ( val / 16 * 10 + val % 16 );
}

// increase a BCD value converting it to decimal and back
byte increaseBCD ( byte myBCD, int lowLimit, int highLimit ) {
  int myDecimal = bcdToDec( myBCD );  // get the decimal value

  myDecimal++;

  if (myDecimal > highLimit) {
    return decToBcd(lowLimit);
  } else {
    return decToBcd(myDecimal);
  }
}


// update a single digit of the display array.
// this is supposed to be the last ring in the display chain
// DONE, implement a form of intensity control  through PWM
// DONE, implement a form of blinking for everything
// DONE, implement a form of blinking for decimal point
// ------------------
// || parameters: digit position, BCD to display (0-F), decimal point status ||
void updateDisplay(int myPosition, int myBCD, int myDP) {

  if ((myPosition >= 0) && (myPosition < 5)) { // just make sure we're not addressing a non-existant display
    for(int c = 0; c < 4; c++){
      if ((( digitBlink == 1 ) && ( blinker == 1 )) || ( allOff == 1) ) {
        digitalWrite(inputs[c], BCD[11][c]);
      } else {
        digitalWrite(inputs[c], BCD[myBCD][c]);
      }
    }

    if ((( dpBlink == 1 ) && (blinker == 1)) || ( allOff == 1) ) {
      digitalWrite(decimalPoint, HIGH);
    } else {
      digitalWrite(decimalPoint, myDP);
    }
    
    digitalWrite(latches[myPosition], LOW);
    digitalWrite(latches[myPosition], HIGH); 
  }
}


void printBCD(int myPosition, int myBCD, int myDPh, int myDPl) {
  int myDigit;
 
  // get lower digit and display it
  myDigit = myBCD & 0x0F;
  updateDisplay(myPosition, myDigit, myDPl);

  // get higher digit and display it; remember to move 1 position upwards
  myDigit = myBCD >> 4;
  updateDisplay(myPosition+1, myDigit, myDPh);

}


void printInt(int myPosition, int myBCD, int myDPh, int myDPl) {
  // unpack an integer into BCD and call printing functions
}

void oneSecondISR() {
  secondElapsed = 1;
}

void loop() {

  int digit;
  int j;
  int lowDigit;
  int highDigit;
  static int mainDP0;
  static int mainDP1;
  static int mainDP2;
  static int mainDP3;
  static byte seconds;
  static byte minutes;
  static byte hours;
  static byte giorno_sett;
  static byte month_day;
  static byte month_nr;
  static byte year_nr;
  static byte new_minutes;
  static byte new_hours;
  static byte new_month_day;
  static byte new_month_nr;
  static byte new_year_nr;

  static byte control;
  static byte inSetMode; // are we setting the time? any value > 0 defines what we are setting

  

  // things to do every second.
  if (secondElapsed == 1) {
    
    secondElapsed = 0; // reset the ISR

    // read the amount of incoming light to control display intensity via PWM
    // LDR value increases with darkness, so the ADC output is closer to ground
    // at maximum light intensity and highest in complete darkness.
    // v20160603: - just do it once a second, not at every loop to speed up PWM
    //            - higher brightness decreased. Previous values: 12, 8, 6, 3, 2, 1.
    sensorValue = analogRead(sensorPin);
    if (sensorValue > 700) {
      intensitySteps = 12;
    } else if (sensorValue > 600) {
      intensitySteps = 8;
    } else if (sensorValue > 450) {
      intensitySteps = 6;    
    } else if (sensorValue > 300) {
      intensitySteps = 4;
    } else if (sensorValue > 100) {
      intensitySteps = 3;
    } else {
      intensitySteps = 2;
    }


    
    //  ** UPDATE THE CLOCK VARIABLES
    Wire.beginTransmission(0x68);
    Wire.write((byte)0x00);
    Wire.endTransmission();

    if ( inSetMode == 0 ) { // if we are operating normally, read an update, otherwise don't mess with values
      //richiedo 7 byte dal dispositivo con
      //indirizzo 0x68
    //  Wire.requestFrom(0x68, 7, 1); // just date and time
      Wire.requestFrom(0x68, 7); // time, date and control byte
      //recupero i 7 byte relativi ai
      //corrispondenti registri
      seconds = Wire.read();
      minutes = Wire.read();
      hours = Wire.read();
      giorno_sett = Wire.read();
      month_day = Wire.read();
      month_nr = Wire.read();
      year_nr = Wire.read();
      control = Wire.read();
    } else {
      Wire.requestFrom(0x68, 1); // read just seconds fr blinking
      // maybe not the best way for blinking, should take advantage of the interrput *****
      seconds = Wire.read();
    }

//    Serial.print(month_day, HEX);
//    Serial.print("/");
//    Serial.print(month_nr, HEX);
//    Serial.print("/");
//    Serial.print(year_nr, HEX);
//        Serial.print(" ");
//    Serial.print(hours, HEX);
//        Serial.print(":");
//    Serial.print(minutes, HEX);
//        Serial.print(":");
//    Serial.println(seconds, HEX);
 
    // blinking control
    blinker = seconds % 2; // blink stuff once a second (0.5 Hz)
  } // end if secondelapsed
  
    // TODO: define what to print on the display:
    // HH:MM
    // moving seconds ... randomised, 5 positions
    // DD.MM
    // YYYY


    // are we setting date/time and so on?
    if ( inSetMode > 0 ) {
      
      switch (inSetMode) {
        case 1:
          delay(100);
          new_hours = hours;
          new_minutes = minutes;
          new_month_day = month_day;
          new_month_nr = month_nr;
          new_year_nr = year_nr;
          inSetMode = 2;
        case 2:
          lowDigit = 0xDD;
          highDigit = new_hours;
          mainDP0 = 1;
          mainDP1 = 1;
          mainDP2 = 0;
          mainDP3 = 0;
          break;
        case 3:
          lowDigit = new_minutes;
          highDigit = 0xDD;
          mainDP0 = 0;
          mainDP1 = 0;
          mainDP2 = 1;
          mainDP3 = 1;
          break;
        case 4: // update month first, so that we can count the right number of days in the month. Leap years not counted.
          lowDigit = new_month_nr;
          highDigit = 0xDD;
          mainDP0 = 0;
          mainDP1 = 1;
          mainDP2 = 1;
          mainDP3 = 1;
          break;
        case 5:
          lowDigit = 0xDD;
          highDigit = new_month_day;
          mainDP0 = 1;
          mainDP1 = 1;
          mainDP2 = 1;
          mainDP3 = 0;
          break;
        case 6:
          lowDigit = new_year_nr;
          highDigit = 0x20;
          mainDP0 = 0;
          mainDP1 = 0;
          mainDP2 = 0;
          mainDP3 = 0;
          break;
        default:  // just in case, exit set mode
          inSetMode = 0;
          digitBlink = 0;
      } // end switch

      if ( button1.uniquePress() ) {
        inSetMode++;
        delay(100);        
        if ( inSetMode > 6 ) { // we're done with setting
          inSetMode = 0;
          digitBlink = 0;
          // TODO **** write data to the RTC

          //attivo la comunicazione con il DS1307
          //l'indirizzo dell'RTC è 0x68
          Wire.beginTransmission(0x68);
          //il primo byte stabilisce il registro
          //iniziale da scivere
          Wire.write((byte)0x00);
          //specifico il tempo e la data
          Wire.write((byte)0x00); //1° byte SECONDI da 0x00 a 0x59
          Wire.write(new_minutes); //2° byte MINUTI da 0x00 a 0x59
          Wire.write((byte)0x80 | new_hours); //3° byte ORE da 0x00 a 0x24
          Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
          Wire.write(new_month_day); //5° byte GIORNO del mese da 0x00 a 0x31
          Wire.write(new_month_nr); //6° byte MESE da 0x00 a 0x12
          Wire.write(new_year_nr); //7° byte ANNO 0x00 a 0x99
          Wire.endTransmission();
            
        }

      } // end if button1 is pressed

      // let's update some digit!!
      if ( button2.uniquePress() ) {
        switch (inSetMode) {
          case 1: break;
          case 2: new_hours = increaseBCD ( new_hours, 0, 23 ); break;
          case 3: new_minutes = increaseBCD ( new_minutes, 0, 59 ); break;
          case 4: new_month_nr = increaseBCD ( new_month_nr, 1, 12 ); break;
          case 5: new_month_day = increaseBCD ( new_month_day, 1, 31 ); break;
          case 6: new_year_nr = increaseBCD ( new_year_nr, 16, 50 ); break; // up to 2050 A.D.
        }
        delay(100);
        
      }

      // not in SET mode
    } else {

      new_year_nr = 0x07 & seconds; // keep just values 0-7 because we're gettinga BCD value from the RTC
      new_year_nr = new_year_nr % 4; // recycling a variable
      lowDigit = minutes;
      highDigit = hours;
  
      // make a rolling decimal point, for some action
      switch (new_year_nr) {
        case 0:
          mainDP0 = 1;  // decimal points
          mainDP1 = 1;  // 0 is ON
          mainDP2 = 1;  // 1 is OFF
          mainDP3 = 0;
          break;
        case 1:
          mainDP0 = 1;  // decimal points
          mainDP1 = 1;  // 0 is ON
          mainDP2 = 0;  // 1 is OFF
          mainDP3 = 1;
          break;
        case 2:
          mainDP0 = 1;  // decimal points
          mainDP1 = 0;  // 0 is ON
          mainDP2 = 1;  // 1 is OFF
          mainDP3 = 1;
          break;
        case 3:
          mainDP0 = 0;  // decimal points
          mainDP1 = 1;  // 0 is ON
          mainDP2 = 1;  // 1 is OFF
          mainDP3 = 1;
          break;
        } // end switch
      //}
  
      // enter SET mode
      if (button1.isPressed()) {
  //      digitBlink = 1; // blink digits when setting
        inSetMode = 1;
        lowDigit = 0xDD;
        highDigit = 0xDD;
      }
      // show D.DM.M 
      if (button2.isPressed()) {
        lowDigit = month_nr;
        highDigit = month_day;
        mainDP0 = 0;
        mainDP1 = 1;
        mainDP2 = 0;
        mainDP3 = 1;
      }
      
    } // end if/else inSetMode

    

  
    
    intensity = intensity + 1;  // display intensity help variable. 
    allOff = intensity % intensitySteps; 
    
    printBCD(0, lowDigit, mainDP1, mainDP0);
    printBCD(2, highDigit, mainDP3, mainDP2);

} 

