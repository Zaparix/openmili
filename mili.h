#include <cstdlib>
#include <iostream>
#include <string.h>
#include <cmath>

#include <RF24/RF24.h>

#include "PL1167_nRF24.h"
#include "MiLightRadio.h"

using namespace std;

void setup_mili();
void switch_state(int zone, bool state);
void set_color(uint8_t zone, uint8_t color);
uint8_t from_rgb(int r, int g, int b);
void set_brightness(uint8_t zone, int brightness);
void send(uint8_t* packet);
