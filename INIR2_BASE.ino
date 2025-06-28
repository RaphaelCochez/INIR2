#include <Arduino.h>
// STM32 USART2 op PA3 = RX, PA2 = TX
HardwareSerial inirSerial(PA3, PA2);  // RX, TX = USART2

// Struct om sensorwaarden op te slaan
typedef struct {
  uint32_t start_char;
  uint32_t gas_concentration;
  uint32_t faults;
  uint32_t sensor_temp;
  uint32_t ref_1s_avg;
  uint32_t active_1s_avg;
  uint32_t crc;
  uint32_t crc_complement;
  uint32_t end_char;
} SensorDataFrame_t;

SensorDataFrame_t myData;

void setup() {
  Serial.begin(9600);           // USB debug
  inirSerial.begin(38400);      // Sensor UART (PA3, PA2)

  Serial.println("Initialiseren INIR sensor...");
  delay(500);

  sendCommand('C');  // Configuratiemodus
  waitAck();

  sendCommand('B');  // Engineering mode
  waitAck();

  Serial.println("Sensor staat in meetmodus (Engineering Mode).");
}

// Hulp: stuur commando naar sensor
void sendCommand(char cmd) {
  String command = "[" + String(cmd) + "]";
  inirSerial.print(command);
  Serial.println(">> Stuur: " + command);
}

// Hulp: wacht op antwoord (ACK)
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

// MAIN LOOP — lees sensor en parse naar struct
void loop() {
  static int lineIndex = 0;
  static String lines[9];

  if (inirSerial.available()) {
    String line = inirSerial.readStringUntil('\n');
    line.trim();

    if (line.length() > 0) {
      Serial.println("RX: " + line);

      if (lineIndex < 9) {
        lines[lineIndex++] = line;
      }

      if (lineIndex == 9) {
        updateSensorData(lines);
        lineIndex = 0;
      }
    }
  }
}

// Verwerk ontvangen hex-regels in struct
void updateSensorData(String lines[]) {
  myData.start_char        = strtoul(lines[0].c_str(), NULL, 16);
  myData.gas_concentration = strtoul(lines[1].c_str(), NULL, 16);
  myData.faults            = strtoul(lines[2].c_str(), NULL, 16);
  myData.sensor_temp       = strtoul(lines[3].c_str(), NULL, 16);
  myData.ref_1s_avg        = strtoul(lines[4].c_str(), NULL, 16);
  myData.active_1s_avg     = strtoul(lines[5].c_str(), NULL, 16);
  myData.crc               = strtoul(lines[6].c_str(), NULL, 16);
  myData.crc_complement    = strtoul(lines[7].c_str(), NULL, 16);
  myData.end_char          = strtoul(lines[8].c_str(), NULL, 16);

  // Debug overzicht
  Serial.println("------ SensorDataFrame ---------");
  Serial.print("Gas concentratie: ");  Serial.println(myData.gas_concentration);
  Serial.print("Fouten:           0x"); Serial.println(myData.faults, HEX);
  Serial.print("Temp (raw):       ");  Serial.println(myData.sensor_temp);
  Serial.print("Ref avg:          ");  Serial.println(myData.ref_1s_avg);
  Serial.print("Actief avg:       ");  Serial.println(myData.active_1s_avg);
  Serial.print("CRC:              0x"); Serial.println(myData.crc, HEX);
  Serial.print("CRC ~:            0x"); Serial.println(myData.crc_complement, HEX);
  Serial.println("--------------------------------");
}