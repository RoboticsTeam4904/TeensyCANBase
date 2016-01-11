#include <FlexCAN.h>

int led = 13;
FlexCAN CANbus(1000000);
static CAN_message_t msg, rxmsg;
static uint8_t hex[17] = "0123456789abcdef";

int txCount, rxCount;
unsigned int txTimer, rxTimer;


// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while ( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working >> 4 ] );
    Serial.write( hex[ working & 15 ] );
  }
}


CAN_filter_t filter;
// -------------------------------------------------------------
void setup(void)
{
  filter.id = 0x222;
  CANbus.begin(filter);
  //CANbus.setFilter(0x222, 0);
  pinMode(led, OUTPUT);

  delay(1000);
  Serial.println(F("Hello Teensy 3.1 CAN Test."));


}


// -------------------------------------------------------------
void loop(void)
{
  while ( CANbus.read(rxmsg) ) {
    if (rxmsg.id == 0x222) {
      Serial.print("T: ");
      Serial.print(millis());
      Serial.print(" ID: 0x");
      hexDump( sizeof(rxmsg.id), (uint8_t *)&rxmsg.id );
      Serial.print(" MSG: 0x");
      hexDump( sizeof(rxmsg.buf), (uint8_t *)&rxmsg.buf );
      Serial.print(" ASCII: ");
      for ( int i = 0; i < sizeof(rxmsg.buf); i++) {
        Serial.write(rxmsg.buf[i]);
      }
      Serial.println();

      msg.len = 8;
      msg.id = 0x222;
      for ( int idx = 0; idx < 8; ++idx ) {
        msg.buf[idx] = '1' + idx;
      }
      CANbus.write(msg);
    }
  }

  delay(1);
}

