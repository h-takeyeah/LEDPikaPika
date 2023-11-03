void setup() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, HIGH);
  digitalWrite(PIN_LED_G, HIGH);
  digitalWrite(PIN_LED_B, HIGH);
}

void loop() {
  digitalWrite(PIN_LED_R, LOW);
  delay(200);
  digitalWrite(PIN_LED_R, HIGH);
  delay(200);

  digitalWrite(PIN_LED_G, LOW);
  delay(200);
  digitalWrite(PIN_LED_G, HIGH);
  delay(200);

  digitalWrite(PIN_LED_B, LOW);
  delay(200);
  digitalWrite(PIN_LED_B, HIGH);
  delay(200);
}
