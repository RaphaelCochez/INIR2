#ifndef INIR2_H
#define INIR2_H

#include <SoftwareSerial.h>

// Structure for holding sensor settings
struct SensorSettings {
  uint32_t sensorType;
  uint32_t gasType;
  float concRange;
  float span;
  float zero;
  float offset;
  float calibrationTemperature;
  uint32_t baudRate;
  uint32_t firmwareVersion;
  uint32_t serialNumber;
  uint32_t crc; // CRC validation
};

class INIRSensor {
private:
  SoftwareSerial* sensorSerial;
  int rxPin, txPin;
  unsigned long baudRate;

public:
  // Constructor
  INIRSensor(int rx, int tx, unsigned long baud);

  // Methods
  bool sendCommand(const String& command);
  String readResponse(unsigned long timeout = 500);

  bool enterConfigurationMode();
  bool enterEngineeringMode();
  SensorSettings readSettings();

  bool validateCRC(const String& data, uint32_t receivedCRC);
};

#endif
