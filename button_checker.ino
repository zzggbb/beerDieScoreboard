#define TAP_TIMEOUT 250
#define IGNORE_TIMEOUT 300
#define N_BUTTONS 4
#define USB Serial
#define USB_BAUD 115200
#define TELEM Serial1
#define TELEM_BAUD 57600

// pin numbers connected to each button
const uint8_t pins[N_BUTTONS] = {20, 21, 22, 23};

// tap states
enum {CHILLIN, SINGLE, DOUBLE};

// button states
enum {UNPRESSED=0x00, PRESSED=0xFF};

unsigned int tap_times[N_BUTTONS] = {0};
uint8_t tap_states[N_BUTTONS] = {CHILLIN};
uint8_t button_states[N_BUTTONS] = {UNPRESSED};
uint8_t histories[N_BUTTONS] = {0};

void setup() {
  USB.begin(USB_BAUD);
  TELEM.begin(TELEM_BAUD);
  while(!TELEM);
  for (uint8_t i=0; i<N_BUTTONS; i++)
    pinMode(pins[i], INPUT_PULLUP);
}

void loop() {
  for (uint8_t i=0; i<N_BUTTONS; i++) {
    uint8_t reading = digitalRead(pins[i]);
    histories[i] = ((histories[i] << 1) | !reading);

    if (tap_states[i] == SINGLE && (millis() - tap_times[i] > TAP_TIMEOUT)) {
      tap_states[i] = CHILLIN;
      TELEM.printf("%d,%d\n", i+1, SINGLE);
      USB.printf("single press on button %d\n", i+1);
    }

    if (button_states[i] == PRESSED && histories[i] == UNPRESSED) {
      // falling edge
      button_states[i] = UNPRESSED;
    }

    if (button_states[i] == UNPRESSED && histories[i] == PRESSED) {
      // rising edge
      button_states[i] = PRESSED;
      switch (tap_states[i]) {
        case CHILLIN:
          tap_states[i] = SINGLE;
          tap_times[i] = millis();
          break;

        case SINGLE:
          tap_states[i] = DOUBLE;
          TELEM.printf("%d,%d\n", DOUBLE);
          USB.printf("double tap on button %d\n", i+1);
          break;

        case DOUBLE:
          tap_states[i] = SINGLE;
          tap_times[i] = millis();
          break;
      }
    }
  }
}
