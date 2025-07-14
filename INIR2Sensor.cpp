#include "INIR2Sensor.h"

INIR2Sensor::INIR2Sensor(HardwareSerial& serial) : _serial(serial), _lastCRCValid(false) {}


void INIR2Sensor::setup() {
  if (!sendInitCommand()) Serial.println("FOUT: Init-commando [C] mislukt");
  delay(100);
  if (!sendReadConfigCommand()) Serial.println("FOUT: Config-commando [I] mislukt");
  delay(100);
  if (!sendOnDemandModeCommand()) Serial.println("FOUT: On-Demand Mode [H] mislukt");
  delay(100);
}

// -------------------------
// COMMANDO FUNCTIES
// -------------------------
bool INIR2Sensor::sendInitCommand() {
  return sendCommand("[C]");
}

bool INIR2Sensor::sendReadConfigCommand() {
  return sendCommand("[I]");
}

bool INIR2Sensor::sendEngModeCommand() {
  return sendCommand("[B]");
}

bool INIR2Sensor::sendOnDemandModeCommand() {
  return sendCommand("[H]");
}

bool INIR2Sensor::sendCommand(const String& cmd) {
  _serial.flush();
  _serial.print(cmd);

  unsigned long startTime = millis();
  while (millis() - startTime < 1000) {
    if (_serial.available()) {
      String response = _serial.readStringUntil('\n');
      response.trim();

      if (response == "5b414b5d") return true;  // [AK]
      if (response == "5b4e415d") return false; // [NA]
    }
  }

  return false;
}

// -------------------------
// DATA OPHALEN EN PARSEN
// -------------------------
bool INIR2Sensor::requestData(SensorDataFrame_t& frame) {
  _serial.flush();
  _serial.print("[Q]");

  String fullFrame = "";
  int linesRead = 0;

  unsigned long startTime = millis();
  while (millis() - startTime < 1000) {
    if (_serial.available()) {
      String line = _serial.readStringUntil('\n');
      line.trim();

      if (line.length() == 0) continue; // skip lege lijnen

      //Serial.print("[DEBUG] RX: ");
      //Serial.println(line);

      if (line == "5b414b5d") continue; // ACK negeren

      fullFrame += line;
      linesRead++;

      if (linesRead == 9) {
        if (fullFrame.length() == 72 && parseDataFrame(fullFrame, frame)) {
          _lastFrame = frame;
          _lastCRCValid = frame.crc_valid;
          return true;
        } else {
          Serial.println("[FOUT] Ongeldig frame opgebouwd");
          return false;
        }
      }
    }
  }

  return false;
}


bool INIR2Sensor::parseDataFrame(const String& hexLine, SensorDataFrame_t& frame) {
  if (hexLine.length() != 72) return false;

  uint32_t fields[9];
  for (int i = 0; i < 9; ++i) {
    String part = hexLine.substring(i * 8, (i + 1) * 8);
    char buf[9];
    part.toCharArray(buf, 9);
    fields[i] = strtoul(buf, NULL, 16);
  }

  frame.start_char        = fields[0];
  frame.gas_concentration = fields[1];
  frame.faults            = fields[2];
  frame.sensor_temp       = fields[3];
  frame.ref_1s_avg        = fields[4];
  frame.active_1s_avg     = fields[5];
  frame.crc               = fields[6];
  frame.crc_complement    = fields[7];
  frame.end_char          = fields[8];

  frame.temperature_kelvin  = frame.sensor_temp / 10.0;
  frame.temperature_celsius = convertKelvinToCelsius(frame.sensor_temp);
  frame.crc_valid = ((frame.crc ^ frame.crc_complement) == 0xFFFFFFFF);

  return frame.crc_valid;
}

// -------------------------
// ACCESSORS
// -------------------------
float INIR2Sensor::getGas() {
  return _lastFrame.gas_concentration;
}

float INIR2Sensor::getTemperature() {
  return _lastFrame.temperature_celsius;
}

bool INIR2Sensor::getCRCValid() {
  return _lastCRCValid;
}

// -------------------------
// HELPER FUNCTIES
// -------------------------
float INIR2Sensor::convertKelvinToCelsius(uint32_t kelvinTimes10) {
  return (kelvinTimes10 / 10.0f) - 273.15f;
}

uint8_t INIR2Sensor::calculateCRC(const SensorDataFrame_t& frame) {
  uint8_t crc = 0;
  crc ^= calculateCRCBytewise(frame.gas_concentration);
  crc ^= calculateCRCBytewise(frame.faults);
  crc ^= calculateCRCBytewise(frame.sensor_temp);
  crc ^= calculateCRCBytewise(frame.ref_1s_avg);
  crc ^= calculateCRCBytewise(frame.active_1s_avg);
  return crc;
}

uint8_t INIR2Sensor::calculateCRCBytewise(uint32_t value) {
  uint8_t crc = 0;
  crc ^= (value & 0xFF);
  crc ^= ((value >> 8) & 0xFF);
  crc ^= ((value >> 16) & 0xFF);
  crc ^= ((value >> 24) & 0xFF);
  return crc;
}
