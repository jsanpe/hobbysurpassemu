#include "SoftwareSerialWithHalfDuplex.h"

#define comPin 4

SoftwareSerialWithHalfDuplex escPort(comPin,comPin,false,false);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Starting...");
  // put your setup code here, to run once:
  escPort.begin(820);
  delay(500);
  Serial.println("NOW!");
}

void handshake() {
  //Handshake
  escPort.write(0x1C);
  escPort.write(0xFF);
}

void loghex(int x) {
  Serial.print("[0x");
  Serial.print(x, HEX);
  Serial.println("]");
}

void logunexpected(int val, int status) {
  Serial.print("Unexpected value ");
  Serial.print(val, HEX);
  Serial.print(" in status ");
  Serial.println(status, HEX);
}

int wait_receive(){
  while(!escPort.available()){delay(50);}
  int a = int(escPort.read());
  loghex(a);
  return a;
}

void flush_buffer(){
  while(escPort.available()) {
    escPort.read();
  }
}

int status = 0;

int queryParam(byte param) {
  escPort.write(0x1C);
  escPort.write(param);
  escPort.write((byte)0x00);
  //wait for 0x1b followed by the value
  int b2 = wait_receive();
  if(b2!=0x1B) {logunexpected(b2, status);return -2;}
  b2 = wait_receive();
  return b2;
}

int setParam(byte param, byte value) {
  escPort.write(0x1B);
  escPort.write(param);
  escPort.write(value);
  //wait for 0x1b followed by the value
  int b2 = wait_receive();
  if(b2!=0x1B) {logunexpected(b2, status);b2=0;}
  else b2 = wait_receive();
  return b2==0x0a?1:0;
}


void loop() {
  Serial.print("Status: ");
  Serial.println(status);

  if (status == 0) { //waiting handshake byte 1
    int b = wait_receive();
    if(b==0x1B) status = 1;
    else logunexpected(b,status);
  }
  else if (status == 1) { //Waiting handshake byte 2
    int b = wait_receive();
    if(b==0xff) {
      handshake();
      status = 2;
    }
    else {
      logunexpected(b,status);
      status = 0;
    }
  }
  else if (status == 2) { //Handshake finished, we can read and write!
    flush_buffer();
    //Wait for user command
    Serial.print("Enter command: ");
    while (Serial.available()==0) {}
    String s = Serial.readString();
    if (s.c_str()[0]=='q') {
      byte param = s.c_str()[1]-'0';
      Serial.print("Query individual param ");
      Serial.println(param);
      int value = queryParam(param);
      Serial.print("The value is ");
      Serial.println(value);
    } 
    else if (s.c_str()[0]=='Q') {
      Serial.print("Query all params");
      for(byte param = 0;param<5;param++) {
        int value = queryParam(param);
        Serial.print("The value for param ");
        Serial.print(param);
        Serial.print(" is ");
        Serial.println(value, HEX);
      }
    }
    else if (s.c_str()[0]=='w') {
      byte param = s.c_str()[1]-'0';
      byte value = s.c_str()[2]-'0';
      Serial.print("Write param ");
      Serial.print(param);
      Serial.print(" with value ");
      Serial.print(value);
      int e = setParam(param, value);
      if(e==0) Serial.println("Error setting param");
    }
  }

}
