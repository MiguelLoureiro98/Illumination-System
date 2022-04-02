const int n = 10;

void setup(){
    Serial.begin(9600);
}

void loop(){
  int LDR;

  LDR = analogRead(A0);
  Serial.println(LDR);

  delay(100);
}
