#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

/*
 Original Description: Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */
 
 /*
 Daniel's Modified Description: Serial to LCD display
 
 This code is written with the Adafruit LCD 2 color I2C display library,
 it takes a string of up to 64 characters as input over serial, and then
 outputs it to the display. If the string doesn't fit on the first two
 lines, the display can be scrolled using the up and down buttons onboard
 the display. The select button toggles between two modes- clearing 1 sec
 after the message is recieved, and waiting until the next message is 
 recieved to clear the display.
 
 */


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield(); //Define the display
String display1= "";
String display2 = "";
String display3 = "";
String display4 = "";
String temp; //this var is used to store the text while displaying clear mode.
int inputStringLength;
boolean button_state = true;
int delay_type;
int top_line = 0; //The top line on the screen, used for scrolling, starts @ 0 like the display

void setup() {
  lcd.begin(16,2);
  lcd.blink();
  lcd.print("SYSTEM ONLINE");
  lcd.setCursor(0,1);
  lcd.print("V2.5 Djbeadle");
  delay(1000);
  lcd.clear();
  // initialize serial:
  Serial.begin(19200);
  Serial.print("System Online");
  Serial.print("Ver. 2.5 by Daniel Beadle");
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

}

uint8_t i=0;
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    if (button_state == true) { //Clears the display if set that way
      lcd.clear();
    }
    trim_and_split();
    lcd.setCursor(0,0);
    lcd.print(display1);
    lcd.setCursor(0,1);
    lcd.print(display2);
    Serial.println("----------");
    if(button_state==true){
      Serial.println("WAIT MODE");
    }
    if(button_state==false){
      Serial.println("CLEAR MODE"+ button_state);
    }
    Serial.println("inputString: " + inputString);
    Serial.println("display1: " + display1);
    Serial.println("display2: " + display2);
    Serial.println("display3: " + display3);
    Serial.println("display4: " + display4);
    if (button_state == false){
      delay(1000);
      lcd.clear();
    }
    inputString = "";
    stringComplete = false;
  }
  
  
  
  uint8_t buttons = lcd.readButtons();
  if (buttons & BUTTON_SELECT) {
    if (button_state == true){
      button_state = false; // In false mode, the display will clear after 1 second
      // temp = display2.substring(9,16); //get the text that will be replaced temporarily
      lcd.setCursor(8,1); 
      lcd.print("CLEARING");
      delay(500);
      lcd.clear();
      // lcd.print(temp); //put the text that was replaced back
    }
    else {
      button_state = true; // In true mode, the display will wait until the next message
      // temp = display2.substring(10,16); //get the text that will be replaced temporarily
      lcd.setCursor(9,1); 
      lcd.print("WAITING");
      delay(500);
      lcd.clear();
      // lcd.print(temp); //put the text that was replaced back
    }
  }
  
  if (buttons & BUTTON_DOWN) {  //Scrolling down!
    switch(top_line) {
      case 0:  //button is pressed while displaying lines 1 & 2
        top_line = top_line + 1;
        lcd.clear();
        delay(100);
        lcd.setCursor(0,0);
        lcd.print(display2);  //hides line 1, displays lines 2 & 3
        lcd.setCursor(0,1);
        lcd.print(display3);
        break;
      case 1:  //button is pressed while displaying lines 2 & 3
        top_line = top_line + 1;
        lcd.clear();
        delay(100);
        lcd.setCursor(0,0);
        lcd.print(display3);
        lcd.setCursor(0,1);
        lcd.print(display4);
        break;
    }
  }
  if (buttons & BUTTON_UP) {  //Scrolling up!
    switch(top_line) {
      case 2:  //button is pressed while displaying lines 3 & 4
        top_line = top_line - 1;
        lcd.clear();
        delay(100);
        lcd.setCursor(0,0);
        lcd.print(display2);  //hides line 4, displays lines 2 & 3
        lcd.setCursor(0,1);
        lcd.print(display3);
        break;
      case 1:  //button is pressed while displaying lines 2 & 3
        top_line = top_line - 1;
        lcd.clear();
        delay(100);
        lcd.setCursor(0,0);
        lcd.print(display1); // Hides line 3, displays lines 1 & 2
        lcd.setCursor(0,1);
        lcd.print(display2);
        break;
    }
  }  

}  

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      top_line = 0; //Resets the scrolling back to the top
      clear_display_vars();
    } 
  }
}


