//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Shield LED Example
/// @note
///              ______________
///             |              |
///             |         SW[] |
///             |[]RST         |
///             |         AREF |--
///             |          GND |--
///             |           13 |--X LED
///             |           12 |--
///             |           11 |--
///           --| 3.3V      10 |--
///           --| 5V         9 |--
///           --| GND        8 |--
///           --| GND          |
///           --| Vin        7 |--
///             |            6 |--
///           --| A0         5 |--
///           --| A1    ( )  4 |--
///           --| A2         3 |--X THING_RX
///           --| A3  ____   2 |--X THING_TX
///           --| A4 |    |  1 |--
///           --| A5 |    |  0 |--
///             |____|    |____|
///                  |____|
///
//*****************************************************************************
#include <SoftwareSerial.h>   //TODO need to set due to some weird wire language linker, should we absorb this whole library into smartthings
#include <SmartThings.h>
#include <LiquidCrystal.h>

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
#define PIN_LED         13
#define PIN_THING_RX    3
#define PIN_THING_TX    2

SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor

bool isDebugEnabled;    // enable or disable debug in this example
int stateLED;           // state to track last set value of LED
String lcd_defaultline1; // first line in LCD

//*****************************************************************************
// LCD                | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
byte organizer[8] = {
        B00000,
        B01010,
        B10001,
        B10101,
        B10101,
        B10001,
        B01010,
        B00000
};
//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup()
{
  // setup default state of global variables
  isDebugEnabled = true;
  stateLED = 0;                 // matches state of hardware pin set below
 
  // setup hardware pins
  pinMode(PIN_LED, OUTPUT);     // define PIN_LED as an output
  digitalWrite(PIN_LED, LOW);   // set value to LOW (off) to match stateLED=0

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
   
  }
//*****************************************************************************
// LCD FUNCTIONS    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

  lcd.createChar(0, organizer);
  lcd.begin(16, 2);
  lcd_defaultline1 = "SmartThings:";
  // Print a message to the LCD.
  lcd.print(lcd_defaultline1);
  lcd.setCursor(0,1);
  lcd.print("Welcome");
}
//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void on()
{
  stateLED = 1;                 // save state as 1 (on)
  digitalWrite(PIN_LED, HIGH);  // turn LED on
  smartthing.shieldSetLED(1, 1, 1);
  smartthing.send("on");        // send message to cloud
}

//*****************************************************************************
void off()
{
  stateLED = 0;                 // set state to 0 (off)
  digitalWrite(PIN_LED, LOW);   // turn LED off
  smartthing.shieldSetLED(0, 0, 0);
  smartthing.send("off");       // send message to cloud
}

//*****************************************************************************
void hello()
{
  Serial.println("brobasaur");
  smartthing.send("off");
  smartthing.send("colors!");
  smartthing.shieldSetLED(1, 0, 0);
  delay(200);
  smartthing.shieldSetLED(0, 1, 0);
  delay(200);
  smartthing.shieldSetLED(0, 0, 1);
  delay(200);
  smartthing.shieldSetLED(1, 1, 0);
  delay(200);
  smartthing.shieldSetLED(1, 1, 1);
  delay(200);
  smartthing.shieldSetLED(1, 0, 1);
  delay(200);
  smartthing.shieldSetLED(0, 1, 1);
  delay(200);
  smartthing.shieldSetLED(3, 2, 1);
  delay(200);
  smartthing.shieldSetLED(1, 2, 3);
  delay(200);
  smartthing.shieldSetLED(2, 2, 4);
  delay(200);
  smartthing.shieldSetLED(4, 3, 1);
     delay(200);
  smartthing.shieldSetLED(0, 0, 0);
  smartthing.send("off");
}
//*****************************************************************************
void loop()
{
  // run smartthing logic
  smartthing.run();
}

//*****************************************************************************
void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
    //lcd.setCursor(0,1);
    //lcd.print(message);
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function
  if (message.equals("on"))
  {
    on();
      lcd.clear();
      lcd.print(lcd_defaultline1);
      lcd.setCursor(0,1);
      lcd.print("Lights ON ");

  }
  else if (message.equals("off"))
  {
    off();
      lcd.clear();
      lcd.print(lcd_defaultline1);
      lcd.setCursor(0,1);
      lcd.print("Lights OFF");
  }
  else if (message.equals("hello"))
  {
      lcd.clear();
      lcd.print(lcd_defaultline1);
      lcd.setCursor(0,1);
      lcd.print("Dance ON");
      hello();
      lcd.setCursor(0,1);
      lcd.print("Dance OFF");
      delay (1000);
      lcd.clear();
      lcd.print(lcd_defaultline1);
      lcd.setCursor(0,1);
      lcd.print("Lights OFF");
   }
}
