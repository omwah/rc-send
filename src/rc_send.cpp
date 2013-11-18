/*
  Send commands using RCSwitch library 
  
  http://code.google.com/p/rc-switch/
  
*/

#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

}

#define BUFFER_SIZE 10
char incomingBytes[BUFFER_SIZE] = {0};   // for incoming serial data

void send_byte(char outgoing_byte)
{
    Serial.print(outgoing_byte);
    Serial.print(" (");
    Serial.print(outgoing_byte, DEC);
    Serial.print(") ");

    mySwitch.send(outgoing_byte, 8);
}

void loop() {
    Serial.setTimeout(10);
    // send data only when you receive data:
    if (Serial.available() > 0) {
        // read the incoming byte:
        //incomingBytes = Serial.read();
        int num_read = Serial.readBytes(incomingBytes, BUFFER_SIZE);

        // say what you got:
        if (num_read > 0) {
            Serial.print(num_read);
            Serial.print(" bytes: ");
            char last_byte = -1;
            for(int idx = 0; idx < num_read; idx++) {
                if(incomingBytes[idx] != last_byte) {
                    send_byte(incomingBytes[idx]);
                    last_byte = incomingBytes[idx];
                }
            }
            Serial.println("");
        }
    } else {
        if(digitalRead(4) == HIGH) {
            // Send a
            send_byte(97);
            //delay(50);
        } else if(digitalRead(5) == HIGH) {
            // Send w
            send_byte(119);
            //delay(50);
        } else if(digitalRead(6) == HIGH) {
            // Send s
            send_byte(115);
            //delay(50);
        } else if(digitalRead(7) == HIGH) {
            // Send d
            send_byte(100);
            //delay(50);
        }
    }
 
}
