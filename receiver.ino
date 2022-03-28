#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
RF24 radio(8, 10);
int relay[] = {3,4,5,6};

const byte address[6] = "00001";
int relaynum = 1;

void setup() {
  lcd.init();
  lcd.clear();
 
  pinMode (relay[0], OUTPUT);
  pinMode (relay[1], OUTPUT);
  pinMode (relay[2], OUTPUT);
  pinMode (relay[3], OUTPUT);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN); 
  radio.startListening();

  digitalWrite (relay[0], LOW);
  digitalWrite (relay[1], LOW);
  digitalWrite (relay[2], LOW);
  digitalWrite (relay[3], LOW);

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("channel");
  lcd.setCursor(0,1);
  lcd.print(relaynum);
}

void loop() {
   if (radio.available()) {
    int num;
    radio.read(&num, sizeof(num));  
    if(num == 4) {
      relaynum++;
      if (relaynum == 5) relaynum = 1;
      lcd.setCursor(0,1);
      lcd.print(relaynum);
    }
    else if(num == 3){
      relaynum--;
      if (relaynum == 0) relaynum = 4;
      lcd.setCursor(0,1);
      lcd.print(relaynum);
    }
    else if(num == 1) {
      digitalWrite (relay[relaynum-1],HIGH);
      lcd.setCursor(9,0);
      lcd.print("O N");
    }
    else if(num == 2) {
      digitalWrite (relay[relaynum-1],LOW);
      lcd.setCursor(9,0);
      lcd.print("OFF");
    }
    else{
      lcd.setCursor(9,0);
      lcd.print("n o");
    }
   }
}
