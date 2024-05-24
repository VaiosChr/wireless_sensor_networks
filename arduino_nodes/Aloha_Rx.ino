#include <RF22.h>
#include <RF22Router.h>

#define MY_ADDRESS 9
#define NODE_ADDRESS_1 21
#define NODE_ADDRESS_2 13
#define NODE_ADDRESS_3 14
RF22Router rf22(MY_ADDRESS);

void setup() {
  Serial.begin(9600);

  if (!rf22.init())
    Serial.println("RF22 init failed");

  if (!rf22.setFrequency(440.0))
    Serial.println("setFrequency Fail");

  rf22.setTxPower(RF22_TXPOW_20DBM);
  rf22.setModemConfig(RF22::GFSK_Rb125Fd125);
  rf22.addRouteTo(NODE_ADDRESS_1, NODE_ADDRESS_1);
  rf22.addRouteTo(NODE_ADDRESS_2, NODE_ADDRESS_2);
  rf22.addRouteTo(NODE_ADDRESS_3, NODE_ADDRESS_3);
}


void loop() {
  uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
  char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];

  memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);

  uint8_t len = sizeof(buf);
  uint8_t from;
  int value1 = 0, value2 = 0;
  int room = 0;
  String id = "";

  if (rf22.recvfromAck(buf, &len, &from)) {
    buf[len] = '\0';
    memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
    char* token = strtok(incoming, ",");
    if(token != NULL) {
      room = atoi(token);
    }
    if(token != NULL) {
      token = strtok(NULL, ",");
      if(token != NULL) {
        id = token;
      }
    }
    Serial.print("room = ");
    Serial.print(room);
    Serial.print(", tag_id = ");
    Serial.print(id);
    Serial.print("\n");
  }
}