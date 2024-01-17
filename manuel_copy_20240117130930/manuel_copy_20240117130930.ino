const int E1 = 5; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction

void setup(void)
{
  int i;
  for(i=4;i<=5;i++)
    pinMode(i, OUTPUT);
  Serial.begin(19200);
  Serial.println("Run keyboard control");
  digitalWrite(E1,LOW);
  pinMode(2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    back_off(100);
}



long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void advance(char a)
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);
}

void back_off (char a)
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);
}

void M1_stop()
{
  digitalWrite(M1,HIGH);
  analogWrite(E1, 0);
}
