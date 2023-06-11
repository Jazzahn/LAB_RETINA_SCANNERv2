#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define LATCH 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

int GREEN_LED = 4;
int RED_LED = 3;

int phaseState = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  SPI.begin();

  mfrc522.PCD_Init();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(LATCH, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(LATCH, HIGH);
  phaseState = 1;
}

void loop() {
  
  if(mfrc522.PICC_IsNewCardPresent()) {
    unsigned long uid = getID();
    if(uid != -1){
     Serial.print("Debug - Card detected, UID: "); Serial.println(uid);
    }
    if (uid == 23874) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      phaseState = 5;
    }
    if (uid == 24386) {
      digitalWrite(GREEN_LED, HIGH);
      delay(5000);
      digitalWrite(GREEN_LED, LOW);
      phaseState = 2;
    }
  }

  if(Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    if(data == "3") {
      phaseState = 4;
      Serial.println("Debug - Latch Opened");
      digitalWrite(LATCH, LOW);
      delay(500);
      digitalWrite(LATCH, HIGH);
    }
    if(data == "6") {
      reset();
      Serial.println("Debug - Puzzle Reset");
    }
  }
  Serial.println(phaseState);
}


unsigned long getID(){
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return -1;
  }
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}


void reset() {
  phaseState = 1;
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(LATCH, HIGH);
}
