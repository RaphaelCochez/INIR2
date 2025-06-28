HardwareSerial inirSerial(PA3, PA2);  // RX, TX = USART2

void setup() {
  Serial.begin(9600);
  inirSerial.begin(38400);

  Serial.println("Initialiseren INIR sensor...");
  delay(500);

  sendCommand('C');  // Configuratiemodus
  waitAck();

  // sendCommand('I');  // ← optioneel, mag weg

  sendCommand('B');  // Zet sensor in Engineering Mode
  waitAck();

  Serial.println("Sensor staat in meetmodus (Engineering Mode).");
}

void loop() {
  if (inirSerial.available()) {
    String line = inirSerial.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      Serial.println("RX: " + line);
    }
  }
}

// Hulp: commando sturen
void sendCommand(char cmd) {
  String command = "[" + String(cmd) + "]";
  inirSerial.print(command);
  Serial.println(">> Stuur: " + command);
}

// Hulp: wacht op ACK
void waitAck() {
  String response = "";
  unsigned long start = millis();

  while (millis() - start < 2000) {
    if (inirSerial.available()) {
      char c = inirSerial.read();
      response += c;
      if (c == ']') break;
    }
  }

  response.trim();
  Serial.println("<< Antwoord: " + response);
}