#include <RCSwitch.h>
#include <LiquidCrystal_I2C.h>

RCSwitch mySwitch = RCSwitch();
LiquidCrystal_I2C lcd(0x27, 16, 2);

String temporaryValue ;
int temperature = 0 ;
int humidite = 0 ;
long oldDataTime;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      /*      Serial.print("Received ");
            Serial.print( mySwitch.getReceivedValue() );
            Serial.print(" / ");
            Serial.print( mySwitch.getReceivedBitlength() );
            Serial.print("bit ");
            Serial.print("Protocol: ");
            Serial.println( mySwitch.getReceivedProtocol() );*/
    }

    // get data and split values
    // on recupere la data et on separe les valeurs
    temporaryValue = String(mySwitch.getReceivedValue());
    if (temporaryValue.substring(0, 3) == "999") {
      temperature = temporaryValue.substring(3).toInt() ;
    }
    if (temporaryValue.substring(0, 3) == "888") {
      humidite = temporaryValue.substring(3).toInt() ;
    }

    // reset 433
    mySwitch.resetAvailable();

    oldDataTime = millis();

  }
  // control if we lost information , 10 second
  // On controle si on a perdu le signal depuis plus de 10secondes
  if (millis() > (oldDataTime + 10000) ) {
    temperature = 999 ;
    humidite = 999 ;
  }

  // print value
  // on affiche les valeurs
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humidite: ");
  lcd.print(humidite);

  delay(2000);
  lcd.clear();
}
