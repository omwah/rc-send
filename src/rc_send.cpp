/*
  Send commands using RCSwitch library 
  
  http://code.google.com/p/rc-switch/
  
*/

#include <Arduino.h>
#include <RCSwitch.h>
#include <LiquidCrystal.h>

// Analog pin for ADC key reading
#define ADC_PIN 0

// Digital pin for RC output
#define RC_PIN 13

#define BUFFER_SIZE 10
char incoming_serial[BUFFER_SIZE] = {0};   // for incoming serial data

#define NUM_KEYS 5
int adc_key_val[NUM_KEYS] = {50, 200, 400, 600, 800 };

RCSwitch mySwitch = RCSwitch();
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

// Cycle through speed commands
#define SPEED_LOW 49
#define SPEED_HIGH 52
char speed = SPEED_LOW;

void setup() {

    Serial.begin(9600);

    // Transmitter is connected to Arduino Pin #10  
    mySwitch.enableTransmit(RC_PIN);

    // Initialize lcd and ADC keys
    lcd.clear(); 
    lcd.begin(16, 2);
    lcd.setCursor(0,0); 
    lcd.print("RC Send");
}


// Convert ADC value to key number
int get_adc_key(unsigned int input)
{
    int k;

    for (k = 0; k < NUM_KEYS; k++) {
        if (input < adc_key_val[k]) {
            return k;
        }
    }

    if (k >= NUM_KEYS) k = -1;  // No valid key pressed
    return k;
}

void display_byte(char outgoing_byte)
{
    lcd.setCursor(0, 1);
    lcd.print(outgoing_byte);
    lcd.print(" (");
    lcd.print(outgoing_byte, DEC);
    lcd.print(") ");
}

void send_byte(char outgoing_byte)
{
    display_byte(outgoing_byte);
    mySwitch.send(outgoing_byte, 8);
}

void loop() {
    Serial.setTimeout(10);
    // send data only when you receive data:
    if (Serial.available() > 0) {
        // read the incoming byte:
        int num_read = Serial.readBytes(incoming_serial, BUFFER_SIZE);

        // say what you got:
        if (num_read > 0) {
            Serial.print(num_read);
            Serial.print(" bytes: ");
            char last_byte = -1;
            for(int idx = 0; idx < num_read; idx++) {
                if(incoming_serial[idx] != last_byte) {
                    send_byte(incoming_serial[idx]);
                    last_byte = incoming_serial[idx];
                }
            }
            Serial.println("");
        }
    } else {
        int key = get_adc_key(analogRead(ADC_PIN));
        switch (key) {
        case 0:
            // Send d - right
            send_byte(100);
            break;
        case 1:
            // Send w - up
            send_byte(119);
            break;
        case 2:
            // Send s - down
            send_byte(115);
            break;
        case 3:
            // Send a - left
            send_byte(97);
            break;
        case 4:
            send_byte(speed);
            speed++;
            if (speed > SPEED_HIGH) speed = SPEED_LOW;
            break;
        }
    }
}
