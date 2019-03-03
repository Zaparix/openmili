#include "mili.h"

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ);
PL1167_nRF24 sender(radio);
MiLightRadio milight(sender);

const uint8_t remote_id[3]  = { 0xB0, 0x3D, 0x9B };
static uint8_t outgoingPacket[7];
int sequence_number = 0;

void send(uint8_t* packet) {
  // build outgoing packet
  copy(remote_id, remote_id + 3, outgoingPacket);
  copy(packet, packet + 3, outgoingPacket + 3);
  outgoingPacket[6] = ++sequence_number;

  // dump packet
  for (int i = 0; i < 7; i++) {
    printf("%02X ", outgoingPacket[i]);
  }
  printf("sending...\n");

  // send packet with 10 resends
  milight.write(outgoingPacket, sizeof(outgoingPacket));
  for (int i = 0; i < 10; i++) {
    milight.resend();
    delay(10);
  }
}

void set_color(uint8_t zone, uint8_t color) {
  // milight zone code
  uint8_t zone_code = 0xB0 + zone;

  // build and send packet
  uint8_t packet[3] = { color, zone_code, 0x0F };
  send(packet);
}

uint8_t from_rgb(int r, int g, int b) {
  // convert to hue from HSV
  double _r = (double)r / 255.0;
  double _g = (double)g / 255.0;
  double _b = (double)b / 255.0;

  double c_max = max(_r, max(_g, _b));
  double c_min = min(_r, min(_g, _b));
  double delta = c_max - c_min;

  if (delta == 0) {
    return 0;
  }
  else if (c_max = _r) {
    return 60 * fmod((_g - _b) / delta, 6);
  }
  else if (c_max = _g) {
    return 60 * (((_b - _r) / delta) + 2);
  }
  else if (c_max = _b) {
    return 60 * (((_r - _g) / delta) + 4);
  }
}

void set_brightness(uint8_t zone, int brightness) {
  // lowest brightness: 10 / heightest: 96
  if (brightness < 10) brightness = 10;
  if (brightness > 96) brightness = 96;

  // calculate milight brightness code
  uint8_t b_percent = (30.0 / 100.0 * (double)brightness) - 1;
  uint8_t brightness_code = (0x90 - 0x08 * b_percent + zone);

  // build and sent packet
  uint8_t packet[3] = { 0x00, brightness_code, 0x0E };
  send(packet);
}

void switch_state(int zone, bool state) {
  // calculate on command for the zone
  uint8_t command = (1 + 2*zone);

  // add one for the off command, if state is false
  if (!state) {
    command++;
  }

  uint8_t packet[3] = { 0x00, 0x00, command };
  send(packet);
}

void setup_mili() {
  // initialize RF24 connection
  milight.begin();
}
