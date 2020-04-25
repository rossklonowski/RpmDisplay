/*
Name: Ross Klonowski
Date: 04/24/2020
Description: Interface with a 4 pin fan with tach output. 
The RPM and PWM will be displayed on a 16 x 2 led display.
4 Pin Fan Connector:
    Black : GND
    Red : +12V(Works with 5V from Arduino)
    Yellow : Tachymeter
    Blue: PWM Control

Connections that must be made:
For Fan:
    PWM Output : 3
    External Interrupt 2 (Cannot change)
    Connect a pull up Resistor(10K) to Tach output
For LED Display:
    rs : 27
    en : 26
    d4 : 25
    d5 : 23
    d6 : 24
    d7 : 22;

See for wiring connections: 
    http://www.themakersworkbench.com/content/tutorial/reading-pc-fan-rpm-arduino
*/

#include <LiquidCrystal.h>

template<class T> inline Print &operator <<(Print &obj,T arg) { 
    obj.print(arg);
    return obj; 
}

//LCD Initialization
const int rs = 27 en = 26, d4 = 25, d5 = 23, d6 = 24, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Global Variables
int ticks = 0;
int rpmCalc;
String message;

// Pin Definitons
int pwmPin = 3; 
int TACH = 2; 

typedef struct{
    char fantype;
    unsigned int fanDivider; 
} fanspec;

fanspec fanspace[3]={{0,1},{1,2},{2,8}}; //{num of hall effect sensors, divider}
char fan = 1; // Selection can be 0, 1, or 2 for whichever fan type you have
 
void rpm(){ //Increment number of passes on hall effect sensor
    ticks++;
}
 
void setup(){ 
    Serial.begin(9600);
    lcd.begin(16, 2);                     // LCD Initialization
    lcd.setCursor(0, 0);
    
    pinMode(pwmPin, OUTPUT);
    pinMode(TACH, INPUT_PULLUP);          // Pin Initialization
    analogWrite(pwmPin, 255);              // 0=0%, 64=25%, 127=50%, 255=100%
    attachInterrupt(0 , rpm, RISING);     // 0 indicates Digital Pin 2 will look for Rising Edges 
}
 
void loop (){
    lcd.setCursor(0, 0);
    ticks = 0;      // Reset passes count
    interrupts();   // Enable Interrupts
    delay(1000);    // Wait a second to capture rotations
    noInterrupts(); // Disable Interrupts to proceed with synchronous instructions

    rpmCalc = ((ticks * 60) / fanspace[fan].fanDivider);

    Serial << rpmCalc << " rpm"; // Print to Console
    
    lcd.clear();                 //Clear last displayed message
    message = String(String(rpmCalc) + " rpm");
    lcd.print(message);
} 
