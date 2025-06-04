#include "INIR2.h"

// Constructor
INIRSensor::INIRSensor(int rx, int tx, unsigned long baud) : rxPin(rx), txPin(tx), baudRate(baud) {
  sensorSerial = new SoftwareSerial(rxPin, txPin);
  sensorSerial->begin(baudRate);
}

// Send a command to the sensor
bool INIRSensor::sendCommand(const String& command) {
  if (!command.startsWith("[") || !command.endsWith("]")) {
    Serial.println("Error: Invalid command format.");
    return false;
  }

  // Send the command
  sensorSerial->print(command);
  delay(100); // Wait for response

  // Read and validate the response
  String response = readResponse();
  if (response == "[AK]") {
    return true;
  } else if (response == "[NA]") {
    return false;
  } else {
    Serial.println("Unexpected response: " + response);
    return false;
  }
}

// Read response from the sensor
String INIRSensor::readResponse(unsigned long timeout) {
  String response = "";
  unsigned long startTime = millis();

  // Read data from the sensor within the timeout
  while (millis() - startTime < timeout) {
    if (sensorSerial->available()) {
      response += (char)sensorSerial->read();
    }
  }

  return response;
}

// Enter Configuration Mode
bool INIRSensor::enterConfigurationMode() {
  return sendCommand("[C]");
}

// Enter Engineering Mode
bool INIRSensor::enterEngineeringMode() {
  return sendCommand("[B]");
}

// Read Settings in Configuration Mode
SensorSettings INIRSensor::readSettings() {
  SensorSettings settings;

  // Send the Read Settings command
  if (!sendCommand("[I]")) {
    Serial.println("Failed to read settings.");
    return settings;
  }

  // Read and parse the response
  String response = readResponse();
  if (response.startsWith("[") && response.endsWith("]")) {
    settings.sensorType = strtol(response.substring(1, 9).c_str(), nullptr, 16);
    settings.gasType = strtol(response.substring(9, 17).c_str(), nullptr, 16);
    settings.concRange = strtol(response.substring(17, 25).c_str(), nullptr, 16) / 1000000.0;
    settings.span = strtol(response.substring(33, 41).c_str(), nullptr, 16) / 1000000.0;
    settings.zero = strtol(response.substring(41, 49).c_str(), nullptr, 16) / 1000000.0;
    settings.offset = strtol(response.substring(49, 57).c_str(), nullptr, 16) / 1000000.0;
    settings.calibrationTemperature = strtol(response.substring(57, 65).c_str(), nullptr, 16) / 10.0;
    settings.baudRate = strtol(response.substring(65, 73).c_str(), nullptr, 16);
    settings.firmwareVersion = strtol(response.substring(73, 81).c_str(), nullptr, 16);
    settings.serialNumber = strtol(response.substring(81, 89).c_str(), nullptr, 16);
    settings.crc = strtol(response.substring(105, 113).c_str(), nullptr, 16);
  } else {
    Serial.println("Invalid response format.");
  }

  return settings;
}
