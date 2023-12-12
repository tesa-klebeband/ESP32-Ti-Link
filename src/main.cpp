#include <Arduino.h>

#define TIP 23
#define RING 22
#define TIMEOUT_RX_US 500
#define TIMEOUT_TX_US 500000
#define LED_RX_TMT_PIN BUILTIN_LED
#define LED_TX_TMT_PIN BUILTIN_LED
#define LED_RX_PIN BUILTIN_LED
#define LED_TX_PIN BUILTIN_LED
#define RETRY_PIN 0
bool error_level;

uint8_t getByte() {
    uint8_t data = 0;
    error_level = 0;
    int time_us = 0;
    for (int i = 0; i < 8; i++) {
        time_us = 0;
        while(digitalRead(TIP) & digitalRead(RING)) {
            if (time_us > TIMEOUT_RX_US) {
                error_level = 1;
                digitalWrite(LED_RX_PIN, LOW);
                return 0;
            }
            delayMicroseconds(1);
            time_us++;
        }
        digitalWrite(LED_RX_PIN, HIGH);
        time_us = 0;
        if (digitalRead(RING) == 0) {
            data |= 1 << i;
            pinMode(TIP, OUTPUT);
            digitalWrite(TIP, LOW);
            while(!digitalRead(RING)) {
                if (time_us > TIMEOUT_RX_US) {
                    digitalWrite(LED_RX_TMT_PIN, HIGH);
                    while(digitalRead(RETRY_PIN));
                    digitalWrite(LED_RX_TMT_PIN, LOW);
                }
                delayMicroseconds(1);
                time_us++;
            }
            digitalWrite(TIP, HIGH);
            pinMode(TIP, INPUT_PULLUP);
        } else {
            pinMode(RING, OUTPUT);
            digitalWrite(RING, LOW);
            while(!digitalRead(TIP)) {
                if (time_us > TIMEOUT_RX_US) {
                    digitalWrite(LED_RX_TMT_PIN, HIGH);
                    while(digitalRead(RETRY_PIN));
                    digitalWrite(LED_RX_TMT_PIN, LOW);
                }
                delayMicroseconds(1);
                time_us++;
            }
            digitalWrite(RING, HIGH);
            pinMode(RING, INPUT_PULLUP);
        }
    }
    digitalWrite(LED_RX_PIN, LOW);
    return data;
}

void sendByte(uint8_t data) {
    int time_us;
    error_level = 0;
    for (int i = 0; i < 8; i++) {
        time_us = 0;
        while(!digitalRead(TIP) || !digitalRead(RING)) {
            if (time_us > TIMEOUT_TX_US) {
                error_level = 1;
                digitalWrite(LED_TX_PIN, LOW);
                return;
            }
            delayMicroseconds(1);
            time_us++;
        }
        digitalWrite(LED_TX_PIN, HIGH);
        time_us = 0;
        if ((data >> i) & 1) {
            pinMode(RING, OUTPUT);
            digitalWrite(RING, LOW);
            while(digitalRead(TIP)) {
                if (time_us > TIMEOUT_TX_US) {
                    digitalWrite(LED_TX_TMT_PIN, HIGH);
                    while(digitalRead(RETRY_PIN));
                    digitalWrite(LED_TX_TMT_PIN, LOW);
                }
                delayMicroseconds(1);
                time_us++;
            }
            pinMode(RING, INPUT_PULLUP);
            time_us = 0;
            while(!digitalRead(TIP)) {
                if (time_us > TIMEOUT_TX_US) {
                    digitalWrite(LED_TX_TMT_PIN, HIGH);
                    while(digitalRead(RETRY_PIN));
                    digitalWrite(LED_TX_TMT_PIN, LOW);
                }
                delayMicroseconds(1);
                time_us++;
            }
        } else {
            pinMode(TIP, OUTPUT);
            digitalWrite(TIP, LOW);
            while(digitalRead(RING)) {
                if (time_us > TIMEOUT_TX_US) {
                    digitalWrite(LED_TX_TMT_PIN, HIGH);
                    while(digitalRead(RETRY_PIN));
                    digitalWrite(LED_TX_TMT_PIN, LOW);
                }
                delayMicroseconds(1);
                time_us++;
            }
            pinMode(TIP, INPUT_PULLUP);
            time_us = 0;
            while(!digitalRead(RING)) {
                if (time_us > TIMEOUT_TX_US) {
                    digitalWrite(LED_TX_TMT_PIN, HIGH);
                    while(digitalRead(RETRY_PIN));
                    digitalWrite(LED_TX_TMT_PIN, LOW);
                }
                delayMicroseconds(1);
                time_us++;
            }
        }
    }
    digitalWrite(LED_TX_PIN, LOW);
}

void setup() {
    Serial.begin(9600);

    pinMode(TIP, INPUT_PULLUP);
    pinMode(RING, INPUT_PULLUP);
    pinMode(LED_RX_TMT_PIN, OUTPUT);
    pinMode(LED_TX_TMT_PIN, OUTPUT);
    pinMode(LED_RX_PIN, OUTPUT);
    pinMode(LED_TX_PIN, OUTPUT);
    pinMode(RETRY_PIN, INPUT_PULLUP);
}

void loop() {
    while (1) {
        uint8_t data = getByte();
        if (!error_level) {
            Serial.write(data);
        } else {
            break;
        }
    }

    while (Serial.available()) {
        sendByte(Serial.read());
    }
}