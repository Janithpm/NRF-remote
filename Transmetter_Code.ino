/*............Read Me......................................................................................................................................................................

                                                                          ___TRANSMETTER____
                                                            for Multicopters, aircraft, and all type of RC Vehicles.
                                                            Main Processer : Arduino NANO.
                                                            nRF24L01+ 2.4GHz Transceiver ( Worldwide 2.4GHz ISM Band ).
                                                            working Voltage : 5V to 9V.
            Left Joystick  :-
              X value   : JL_x , {AnalogPin 1 - JLX}
              Y value   : JL_y , {AnalogPin 2 - JLY}
              PushButton   : leftPushB [ Default : OFF (0) ] , {DigitalPin 8 - JLS}

            Right Joystick  :-
              X value   : JR_x , {AnalogPin 3 - JRX}
              Y value   : JR_y  ,{AnalogPin 4 - JRY}
              PushButton   : rightPushB[ Default : OFF (0) ] , {DigitalPin 21 - JRS}

            Toggle Switches (Clockwise. Starting at next to the processer):-
              Toggle switch 1  : tSwitch_1 , Default status : OFF - (0) , ON - (1) , {DigitalPin 2 - TS1}
              Toggle switch 2  : tSwitch_2 , Default status : OFF - (0) , ON - (1) , {DigitalPin 3 - TS2}
              Toggle switch 3  : tSwitch_3 , Default status : OFF - (0) , ON - (1) , {DigitalPin 4 - TS3}
              Toggle switch 4  : tSwitch_4 , Default status : OFF - (0) , ON - (1) , {DigitalPin 5 - TS4}

            protenciamitter(VR) : vR , Resister value : 0 Ohm to 550 Ohm , Default maping Range  : 1000 to 2000 {AnalogPin 0 - PM}

            LED Indicaters  :-
               RED LED  : redLed , {DigitalPin 6 - RLED}
               BLUE LED : blueLed , {DigitalPin 7 - BLED}

            nRF24L01+ Pinout :-
               CE  : DigitalPin 9 - cePin
               CES : DigitalPin 10 - cesPin


                                                                         Hardware And Software by,
                                                                           JANITH MADARASINGHE.
  ........................................................................................................................................................................................
*/

#include <SPI.h>
#include<RF24.h>

int JLX = A1;
int JLY = A2;
int JRX = A3;
int JRY = A4;
int TS[] = {3, 4, 5};
int activateTS = 2;
int PM = A0;
int RLED = 6;
int BLED = 7;

int JL_x;
int JL_y;
int JR_x;
int JR_y;
int tSwitch[] = {0, 0, 0};
int activateSwitch;
int vR;
int redLed;
int blueLed;

RF24 radio (9, 10);
byte address[][6] = {"ADR00001"};
int data[8];

void setup() {

  Serial.begin(9600);
  radio.begin();
  radio.setChannel(124);
  radio.openWritingPipe(address[0]);
  radio.stopListening();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);

  pinMode(RLED, OUTPUT);
  pinMode(BLED, OUTPUT);

  for (int i = 2; i < 6; i ++) {
    pinMode(i, INPUT);
  }

  startingLight();

}

void loop() {

  activateSwitch = digitalRead(activateTS);

  for (int i = 0; i < 3; i++) {
    tSwitch[i] = digitalRead(TS[i]);
  }

  vR = analogRead(PM);
  if (vR > 550) {
    vR = 550;
  } else if (vR < 0) {
    vR = 0;
  }

  JL_x = analogRead(JLX);
  JL_y = analogRead(JLY);

  JR_x = analogRead(JRX);
  JR_y = analogRead(JRY);

  for (int i = 0; i < 3; i++) {
    data[i] = tSwitch[i];
  }

  data[3] = vR;
  data[4] = JL_x;
  data[5] = JL_y;
  data[6] = JR_x;
  data[7] = JR_y;


  if (activateSwitch == 1) {
    radio.write(&data, sizeof(data));
  } else {
    digitalWrite(RLED, HIGH);
    delay(50);
    digitalWrite(RLED, LOW);
    delay(50);
  }

  ConectionLight();
  printing();

  delay(50);
}

void startingLight() {

  boolean Red = true;

  for (int i = 1; i <= 8 ; i++) {
    if (Red) {
      digitalWrite(RLED, HIGH);
      delay(50);
      digitalWrite(RLED, LOW);
      delay(50);
    }
    else {
      digitalWrite(BLED, HIGH);
      delay(50);
      digitalWrite(BLED, LOW);
      delay(50);
    }
    Red = !Red;
  }
}

void ConectionLight() {

  if (activateSwitch == 1 && tSwitch[0] == 1) {
    digitalWrite(BLED, HIGH);
    digitalWrite(RLED, LOW);
  }
  else {
    digitalWrite(RLED, HIGH);
    digitalWrite(BLED, LOW);
  }
}

void printing() {
  Serial.print("Activate :"); Serial.print(" "); Serial.print(activateSwitch); Serial.print("   ");
  Serial.print("V R :"); Serial.print(" "); Serial.print(vR); Serial.print("   ");
  Serial.print("TS1 :"); Serial.print(" "); Serial.print(tSwitch[0]); Serial.print("   ");
  Serial.print("TS2 :"); Serial.print(" "); Serial.print(tSwitch[1]); Serial.print("   ");
  Serial.print("TS3 :"); Serial.print(" "); Serial.print(tSwitch[2]); Serial.print("   ");
  Serial.print("J.L_X :"); Serial.print(" "); Serial.print(JL_x); Serial.print("   ");
  Serial.print("J.L_Y :"); Serial.print(" "); Serial.print(JL_y); Serial.print("   ");
  Serial.print("J.R_X :"); Serial.print(" "); Serial.print(JR_x); Serial.print("   ");
  Serial.print("J.R_Y :"); Serial.print(" "); Serial.print(JR_y); Serial.println("   ");


}
