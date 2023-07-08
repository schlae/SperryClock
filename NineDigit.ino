#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define DELAY_ANODE_DECAY 10
#define MUX_RATE (65535L - 16000000L/64/2000)

uint16_t displayContents[9] = {0};
uint8_t currentDigit = 0;

//    --0x01--
//   |        |
//  0x20     0x02
//   |        |
//    --0x40--
//   |        |
//  0x10     0x04
//   |        |
//    --0x08--
//              . (0x80) , (0x100)

uint8_t digs[6] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20};
uint8_t dig_start = 0;

const uint8_t digitLookup[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                           0x6d, 0x7d, 0x07, 0x7f, 0x6f};

//                                @     A     b     C     d     E     F     g
const uint8_t alphaLookup[32] = {0x5f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6F,
//                                H     I     J     K     L     M     n     O
                                 0x76, 0x06, 0x0e, 0x00, 0x38, 0x37, 0x54, 0x3f,
//                                P     Q     r     S     T     U     V     W
                                 0x73, 0x67, 0x50, 0x6d, 0x78, 0x3e, 0x3e, 0x00,
//                                X     y     Z     [     \     ]     ^     _
                                 0x00, 0x6e, 0x5b, 0x39, 0x64, 0x0f, 0x00, 0x08};

void printDigit(uint8_t value, uint8_t digit) {
  displayContents[digit] = (displayContents[digit] & 0x180) | digitLookup[value];
}

void printAlpha(char c, uint8_t digit) {
  if (c >= 0x40 && c <= 0x5F) {
    displayContents[digit] = (displayContents[digit] & 0x180) | alphaLookup[c-0x40];
  }
} 

// Write to MAX6920
void writeMAX(uint16_t value) {
  int i;
  for (i = 11; i >= 0; i--) {
    if ((value >> i) & 1) {
      PORTB |= _BV(3);
    } else {
      PORTB &= ~_BV(3);
    }
    PORTB |= _BV(5);
    PORTB &= ~_BV(5);
  }
  PORTB |= _BV(0);
  PORTB &= ~_BV(0);
}

void setupMAX() {
  DDRB |= _BV(0) | _BV(1) | _BV(3) | _BV(5);
}

void setupMux() {
  DDRC |= _BV(0) | _BV(1) | _BV(2) | _BV(3);
  DDRD |= _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6);
  PORTC = 0;
  PORTD = 0;
}

void clearDigit() 
{
  PORTC &= ~(_BV(0) | _BV(1) | _BV(2) | _BV(3));
  PORTD &= ~(_BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6));  
}

void doDigit(uint8_t digit) {
  if (digit <= 3) {
    PORTC |= _BV(digit);
  } else {
    PORTD |= _BV(digit - 2);
  }
}

ISR (TIMER1_OVF_vect)
{
  currentDigit++;
  if (currentDigit > 8) currentDigit = 0;
  clearDigit();
  delayMicroseconds(DELAY_ANODE_DECAY);
  writeMAX(~displayContents[currentDigit]);
  doDigit(currentDigit);
  TCNT1 = MUX_RATE;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupMux();
  setupMAX();
  TCNT1 = MUX_RATE;
  TCCR1A = 0x00; // No output pins
  TCCR1B = (1 << CS10) | (1 << CS11); // CLKIO / 64
  TIMSK1 = (1 << TOIE1);
  sei();

  for (int i = 0; i < 9; i++) {
    displayContents[i] = 0x1AA;
  }
}

void loop() {
  
  // put your main code here, to run repeatedly:
  delayMicroseconds(1000);
  #if 0
  char st[] = "@TUBETIME";
  for (int i = 0; i < 10; i++) {
    //printDigit(i, i);
    printAlpha(st[i], i);
  }
  #endif
  for (int i = 0; i < 10; i++) {
    displayContents[i] = digs[(dig_start + i) % 6];
  }
  dig_start++;
  delay(50);
}
