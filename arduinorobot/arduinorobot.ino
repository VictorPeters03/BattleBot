#include <analogWrite.h>

int rechtsVoor = 17;
int linksAchter = 18;
int linksVoor = 5;
int rechtsAchter = 16;

void setup() {
  // put your setup code here, to run once:
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(rechtsVoor, 200);
  analogWrite(linksVoor, 200);
  analogWrite(rechtsAchter, LOW);
  analogWrite(linksAchter, LOW); 
}
