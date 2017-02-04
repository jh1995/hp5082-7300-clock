// (c) by Paolo CRAVERO IK1ZYW 2016. All rights reserved.
//
// No responsibility is taken for any use of this code,
// which is provided as an academic guidance to fellow builders.
// -------------------------------------------------------------

#include <Wire.h>
#include <Button.h>
#include <EEPROM.h> // used to store DST status


// **** CONFIGURATION ****
const bool MSDblank = 1;  // set to 1 to blank MSD when its value is zero


int DSTdays[100] = {
0x25, // 2016-03-27 to 2016-10-30
0x14, // 2017-03-26 to 2017-10-29
0x03, // 2018-03-25 to 2018-10-28
0x62, // 2019-03-31 to 2019-10-27
0x40, // 2020-03-29 to 2020-10-25
0x36, // 2021-03-28 to 2021-10-31
0x25, // 2022-03-27 to 2022-10-30
0x14, // 2023-03-26 to 2023-10-29
0x62, // 2024-03-31 to 2024-10-27
0x51, // 2025-03-30 to 2025-10-26
0x40, // 2026-03-29 to 2026-10-25
0x36, // 2027-03-28 to 2027-10-31
0x14, // 2028-03-26 to 2028-10-29
0x03, // 2029-03-25 to 2029-10-28
0x62, // 2030-03-31 to 2030-10-27
0x51, // 2031-03-30 to 2031-10-26
0x36, // 2032-03-28 to 2032-10-31
0x25, // 2033-03-27 to 2033-10-30
0x14, // 2034-03-26 to 2034-10-29
0x03, // 2035-03-25 to 2035-10-28
0x51, // 2036-03-30 to 2036-10-26
0x40, // 2037-03-29 to 2037-10-25
0x36, // 2038-03-28 to 2038-10-31
0x25, // 2039-03-27 to 2039-10-30
0x03, // 2040-03-25 to 2040-10-28
0x62, // 2041-03-31 to 2041-10-27
0x51, // 2042-03-30 to 2042-10-26
0x40, // 2043-03-29 to 2043-10-25
0x25, // 2044-03-27 to 2044-10-30
0x14, // 2045-03-26 to 2045-10-29
0x03, // 2046-03-25 to 2046-10-28
0x62, // 2047-03-31 to 2047-10-27
0x40, // 2048-03-29 to 2048-10-25
0x36, // 2049-03-28 to 2049-10-31
0x25, // 2050-03-27 to 2050-10-30
0x14, // 2051-03-26 to 2051-10-29
0x62, // 2052-03-31 to 2052-10-27
0x51, // 2053-03-30 to 2053-10-26
0x40, // 2054-03-29 to 2054-10-25
0x36, // 2055-03-28 to 2055-10-31
0x14, // 2056-03-26 to 2056-10-29
0x03, // 2057-03-25 to 2057-10-28
0x62, // 2058-03-31 to 2058-10-27
0x51, // 2059-03-30 to 2059-10-26
0x36, // 2060-03-28 to 2060-10-31
0x25, // 2061-03-27 to 2061-10-30
0x14, // 2062-03-26 to 2062-10-29
0x03, // 2063-03-25 to 2063-10-28
0x51, // 2064-03-30 to 2064-10-26
0x40, // 2065-03-29 to 2065-10-25
0x36, // 2066-03-28 to 2066-10-31
0x25, // 2067-03-27 to 2067-10-30
0x03, // 2068-03-25 to 2068-10-28
0x62, // 2069-03-31 to 2069-10-27
0x51, // 2070-03-30 to 2070-10-26
0x40, // 2071-03-29 to 2071-10-25
0x25, // 2072-03-27 to 2072-10-30
0x14, // 2073-03-26 to 2073-10-29
0x03, // 2074-03-25 to 2074-10-28
0x62, // 2075-03-31 to 2075-10-27
0x40, // 2076-03-29 to 2076-10-25
0x36, // 2077-03-28 to 2077-10-31
0x25, // 2078-03-27 to 2078-10-30
0x14, // 2079-03-26 to 2079-10-29
0x62, // 2080-03-31 to 2080-10-27
0x51, // 2081-03-30 to 2081-10-26
0x40, // 2082-03-29 to 2082-10-25
0x36, // 2083-03-28 to 2083-10-31
0x14, // 2084-03-26 to 2084-10-29
0x03, // 2085-03-25 to 2085-10-28
0x62, // 2086-03-31 to 2086-10-27
0x51, // 2087-03-30 to 2087-10-26
0x36, // 2088-03-28 to 2088-10-31
0x25, // 2089-03-27 to 2089-10-30
0x14, // 2090-03-26 to 2090-10-29
0x03, // 2091-03-25 to 2091-10-28
0x51, // 2092-03-30 to 2092-10-26
0x40, // 2093-03-29 to 2093-10-25
0x36, // 2094-03-28 to 2094-10-31
0x25, // 2095-03-27 to 2095-10-30
0x03, // 2096-03-25 to 2096-10-28
0x62, // 2097-03-31 to 2097-10-27
0x51, // 2098-03-30 to 2098-10-26
0x40, // 2099-03-29 to 2099-10-25
0x36, // 2100-03-28 to 2100-10-31
0x25, // 2101-03-27 to 2101-10-30
0x14, // 2102-03-26 to 2102-10-29
0x03, // 2103-03-25 to 2103-10-28
0x51, // 2104-03-30 to 2104-10-26
0x40, // 2105-03-29 to 2105-10-25
0x36, // 2106-03-28 to 2106-10-31
0x25, // 2107-03-27 to 2107-10-30
0x03, // 2108-03-25 to 2108-10-28
0x62, // 2109-03-31 to 2109-10-27
0x51, // 2110-03-30 to 2110-10-26
0x40, // 2111-03-29 to 2111-10-25
0x25, // 2112-03-27 to 2112-10-30
0x14, // 2113-03-26 to 2113-10-29
0x03, // 2114-03-25 to 2114-10-28
0x62, // 2115-03-31 to 2115-10-27
};




// **** DEFINITIONS ****
int inputs[4] = {5,9,10,11}; // Q1, Q2, Q4, Q8 outputs

int latches[4] = {8,7,6,4}; // latches LSD to MSD

#define DSTADDRESS 1 // EEPROM position for DST status
   byte seconds;
   byte minutes;
   byte hours;
   byte weekday;
   byte month_day;
   byte month_nr;
   byte year_nr;

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
int sensorValue = 0;  // variable to store the value coming from the LDR sensor
int sensorValuePrev = 0; // variable to store the previos value of LDR sensor

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

  int oldDST;
  int curDST;
  int intHours;
  
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
    Wire.write((byte)0x00); //2° byte MINUTI da 0x00 a 0x59
    Wire.write((byte)0x80 | 0x08); //3° byte ORE da 0x00 a 0x24
    Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
    Wire.write((byte)0x06); //5° byte GIORNO del mese da 0x00 a 0x31
    Wire.write((byte)0x09); //6° byte MESE da 0x00 a 0x12
    Wire.write((byte)0x16); //7° byte ANNO 0x00 a 0x99
    Wire.endTransmission();
  
    delay(50);
  
    // set square wave output at 1 Hz
    Wire.beginTransmission(0x68);
    Wire.write((byte)0x07);
    Wire.write((byte)0x10); // control byte to set and enable 1 Hz SQW output
    Wire.endTransmission();

    delay(100);
  }
      
    // DO DST CHECK AND CORRECT IF NEEDED
    readRTC();
    if ( hours > 3 ) { // this check is done only if we are past the changeover time of 2/3 AM
      oldDST = EEPROM.read(DSTADDRESS);
      curDST = IsDst(month_day, month_nr, year_nr);
      if ( curDST != oldDST ) {
        if ( curDST == 1 ) { // we're in solar time, move backwards 1h
          Serial.println("Switching to DST! (+1h)");
          intHours = bcdToDec(hours);
          intHours = intHours + 1;
          hours = decToBcd(intHours);
        } else { // we're in DST, move forward 1h
          Serial.println("Reverting back to solar time (-1h).");
          intHours = bcdToDec(hours);
          intHours = intHours - 1;
          hours = decToBcd(intHours);
          
        }

          Wire.beginTransmission(0x68);
          //il primo byte stabilisce il registro
          //iniziale da scivere
          Wire.write((byte)0x00);
          //specifico il tempo e la data
          Wire.write(seconds); //1° byte SECONDI da 0x00 a 0x59 -- skipping ahead of two seconds
          Wire.write(minutes); //2° byte MINUTI da 0x00 a 0x59
          Wire.write((byte)0x80 | hours); //3° byte ORE da 0x00 a 0x24
          Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
          Wire.write(month_day); //5° byte GIORNO del mese da 0x00 a 0x31
          Wire.write(month_nr); //6° byte MESE da 0x00 a 0x12
          Wire.write(year_nr); //7° byte ANNO 0x00 a 0x99
          Wire.endTransmission();
      } // endif curDST != oldDST

      Serial.print("DST values old/cur: ");
      Serial.print(oldDST, DEC);
      Serial.print("/");
      Serial.println(curDST, DEC);
      
      EEPROM.update(DSTADDRESS, curDST);
    } // end if hour>3
    


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

  if ((myPosition == 3) && (MSDblank == 1) && (myBCD == 0)) { // blank MSD if zero
	myBCD = 11;
  }
  
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

    // send a signal to the latch so that the display loads the data
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

// check if we are in DST timeframe
// Returns 0 in solar time, returns 1 in DST
int IsDst(int day, int month, int year)
{

    int myday = bcdToDec(day);
    int mymonth = bcdToDec(month);
    int myyear = bcdToDec(year);
    int changeOverDays = DSTdays[myyear-16];
    int changeOverToSolar = (changeOverDays & 0x0F) + 25;
    int changeOverToDST = (changeOverDays >> 4) + 25;

    Serial.println(changeOverDays, HEX);
    Serial.println(changeOverToSolar, DEC);
    Serial.println(changeOverToDST, DEC);

    Serial.print("day: ");
    Serial.print(myday, DEC);
    Serial.print(" month: ");
    Serial.print(mymonth, DEC);
    Serial.print(" year: ");
    Serial.println(myyear, DEC);
    
    if (mymonth < 3 || mymonth > 10)  return 0; 
    if (mymonth > 3 && mymonth < 10)  return 1; 
    
    if ((mymonth == 3) && ( myday >= changeOverToDST )) return 1;
    if ((mymonth == 10) && ( myday < changeOverToSolar )) return 1;

    return 0; 
}

void readRTC() {
        //richiedo 7 byte dal dispositivo con
      //indirizzo 0x68
      Wire.beginTransmission(0x68);   // Initialize the Tx buffer
      Wire.write(0x00);            // Put slave register address in Tx buffer
      Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive

      //  Wire.requestFrom(0x68, 7, 1); // just date and time
      Wire.requestFrom(0x68, 7); // time, date and control byte
      //recupero i 7 byte relativi ai
      //corrispondenti registri
      seconds = Wire.read();
      minutes = Wire.read();
      hours = Wire.read();
      weekday = Wire.read();
      month_day = Wire.read();
      month_nr = Wire.read();
      year_nr = Wire.read();
      //control = Wire.read();

}


void printInt(int myPosition, int myBCD, int myDPh, int myDPl) {
  // unpack an integer into BCD and call printing functions
}

void oneSecondISR() {
  secondElapsed = 1;
}

void loop() {

  int digit;
  static unsigned int tempOn; // temporarily keep display powered even during daytime
  int j;
  int lowDigit;
  int highDigit;
  static int mainDP0;
  static int mainDP1;
  static int mainDP2;
  static int mainDP3;
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
    sensorValuePrev = sensorValue / 20; // keep 1/10th of the previous reading
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
      readRTC();
     } else {
      Wire.requestFrom(0x68, 1); // read just seconds for blinking
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

	// **** TODO TOTEST **** add here the drift correction routine that adds 2 seconds every day at 3 AM
		if ( (hours == 3) && (minutes == 0) && (seconds == 0) ) { // is it 3:00:00 AM?
          //attivo la comunicazione con il DS1307
          //l'indirizzo dell'RTC è 0x68
          Wire.beginTransmission(0x68);
          //il primo byte stabilisce il registro
          //iniziale da scivere
          Wire.write((byte)0x00);
          //specifico il tempo e la data
          Wire.write((byte)0x02); //1° byte SECONDI da 0x00 a 0x59 -- skipping ahead of two seconds
          Wire.write((byte)0x00); //2° byte MINUTI da 0x00 a 0x59
          Wire.write((byte)0x80 | hours); //3° byte ORE da 0x00 a 0x24
          Wire.write((byte)0x02); //4° byte GIORNO della settimana da 0x01 a 0x07
          Wire.write(month_day); //5° byte GIORNO del mese da 0x00 a 0x31
          Wire.write(month_nr); //6° byte MESE da 0x00 a 0x12
          Wire.write(year_nr); //7° byte ANNO 0x00 a 0x99
          Wire.endTransmission();
		}

	    new_year_nr = 0x03 & seconds;
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

    // turn off display if we are not setting date and time during daytime
    // and incoming light has not changed abruptly (sign of no movement)
    if (inSetMode == 0) {
      if ( tempOn == 0 ){
        if ( sensorValuePrev == ( sensorValue / 20) ) {
          if ( hours > 0x09 && hours < 0x18 ) {
            allOff = 1;
            tempOn = 0;
          }
        } else {
          tempOn = 1;
        }
      } else {
        tempOn++;
        if ( tempOn > 20000 ) {
          tempOn = 0;
        }
      }
    }
    
    printBCD(0, lowDigit, mainDP1, mainDP0);
    printBCD(2, highDigit, mainDP3, mainDP2);

} 




