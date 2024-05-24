#include <SPI.h>
#include <MFRC522.h>
#include <RF22.h>
#include <RF22Router.h>
 
#define ROOM 1

#define BUZZER 5
#define LED 6

#define RFID 4
#define RST_RFID 3

#define MY_ADDRESS 21
#define DESTINATION_ADDRESS_1 9

MFRC522 mfrc522(RFID, RST_RFID);
RF22Router rf22(MY_ADDRESS);
boolean successful_packet = false;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  SPI.begin();
  Serial.println();

  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  if (!rf22.init())
    Serial.println("RF22 init failed");
  if (!rf22.setFrequency(440.0))
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM);
  rf22.setModemConfig(RF22::GFSK_Rb125Fd125);

  rf22.addRouteTo(DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_1);
}

void loop() {
  digitalWrite(SS, HIGH);
  digitalWrite(RFID, LOW);
  String id = read_RFID();

  if(id != "") {
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    Serial.println(id);

    digitalWrite(SS, LOW);
    digitalWrite(RFID, HIGH);
    data_sent(id);
    delay(800);

    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    delay(200);
  }
}

void data_sent(String id) {
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];

  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  sprintf(data_read, "%d,%s", ROOM, id.c_str());
  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);

  successful_packet = false;
  while (!successful_packet) {
    if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS_1) != RF22_ROUTER_ERROR_NONE) {
      Serial.println("sendtoWait failed");
      delay(50);    
    }
    else {
      Serial.println("sendtoWait Successful");
      successful_packet=true;
    }
  }
}

String read_RFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return "";
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return "";
  }
  
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      content += "0";
    }
    content += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.println();
  content.toUpperCase();
  return content.substring(1);
} 