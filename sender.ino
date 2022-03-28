#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <APDS9960.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define APDS9960_INT 0 
LiquidCrystal_I2C lcd(0x27,16,2); 
RF24 radio(8, 10); 
const int numpin = 3;
const int ONpin = 4;
const int OFFpin = 5;
SoftwareSerial BTSerial(6,7);
APDS9960 apds = APDS9960();

int flag = 0;
int relaynum = 1;

const byte address[6] = "00001";

void setup() {
  BTSerial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  lcd.init();
  lcd.clear();
  
  pinMode(APDS9960_INT, INPUT);
  pinMode(numpin,INPUT_PULLUP);
  pinMode(ONpin,INPUT_PULLUP);
  pinMode(OFFpin,INPUT_PULLUP);
  
  Serial.begin(9600);
  apds.init();

  if ( apds.enableGestureSensor(true) ) {
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("channel");
    lcd.setCursor(0,1);
    lcd.print(relaynum);
  } else {
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Something wrong!");
  }

}

void loop() {
  if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP: {
        Serial.println("UP");
        const char sigup = 1;
        radio.write(&sigup,sizeof(sigup));
        lcd.setCursor(9,0);
        lcd.print("O N");
        break;}
      case DIR_DOWN:{
        Serial.println("DOWN");
        const int sigdown = 2;
        radio.write(&sigdown,sizeof(sigdown));
        lcd.setCursor(9,0);
        lcd.print("OFF");
        break;}
      case DIR_LEFT:{
        Serial.println("LEFT");
        const int sigleft = 3;
        radio.write(&sigleft,sizeof(sigleft));
        relaynum--;
        if (relaynum == 0) relaynum = 4;
        lcd.setCursor(0,1);
        lcd.print(relaynum);
        break;}
      case DIR_RIGHT:{
        Serial.println("RIGHT");
        const int sigright = 4;
        radio.write(&sigright,sizeof(sigright));
        relaynum++;
        if (relaynum == 5) relaynum = 1;
        lcd.setCursor(0,1);
        lcd.print(relaynum);
        break;}
      case DIR_NEAR:{
        Serial.println("NEAR");
        const int signear = 5;
        radio.write(&signear,sizeof(signear));
        break;}
      case DIR_FAR:{
        Serial.println("FAR");
        const int sigfar = 6;
        radio.write(&sigfar,sizeof(sigfar));
        break;}
      default:
        Serial.println("NONE");
        const int signone = 7;
        radio.write(&signone,sizeof(signone));
    }
  }

  else if(BTSerial.available()){
  char data = BTSerial.read();

   if(data == '1'){
    const char BTON = 1;
    radio.write(&BTON,sizeof(BTON));
    lcd.setCursor(9,0);
    lcd.print("O N");
   }
   else if(data == '2'){
    const int BTOFF = 2;
    radio.write(&BTOFF,sizeof(BTOFF));
    lcd.setCursor(9,0);
    lcd.print("OFF");
   }
   else if(data == '4'){
    const char BTPLUS = 4;
    radio.write(&BTPLUS,sizeof(BTPLUS));
    relaynum++;
    if (relaynum == 5) relaynum = 1;
    lcd.setCursor(0,1);
    lcd.print(relaynum);
   } 
 }
  else{
  if(digitalRead(numpin) == LOW){
    delay(10);
    while(digitalRead(numpin) == LOW); 
    Serial.println("RIGHT");
    const char pinRG = 4;
    radio.write(&pinRG,sizeof(pinRG));
    relaynum++;
    if (relaynum == 5) relaynum = 1;
    lcd.setCursor(0,1);
    lcd.print(relaynum);
  }
  if(digitalRead(OFFpin) == LOW){
    delay(10);
    while(digitalRead(OFFpin) == LOW); 
    Serial.println("DOWN");
    const int pindown = 2;
    radio.write(&pindown,sizeof(pindown));
    lcd.setCursor(9,0);
    lcd.print("OFF");
  }
  if(digitalRead(ONpin) == LOW){
    delay(10);
    while(digitalRead(ONpin) == LOW); 
    Serial.println("UP");
    const char pinup = 1;
    radio.write(&pinup,sizeof(pinup));
    lcd.setCursor(9,0);
    lcd.print("O N");
  }
  }
 
 
}
