#ifndef INIR2_SENSOR_H
#define INIR2_SENSOR_H

#include <Arduino.h>
#include <HardwareSerial.h>

// Struct voor het ruwe en afgeleide dataresultaat van de sensor
struct SensorDataFrame_t {
  uint32_t start_char;
  uint32_t gas_concentration;
  uint32_t faults;
  uint32_t sensor_temp;
  uint32_t ref_1s_avg;
  uint32_t active_1s_avg;
  uint32_t crc;
  uint32_t crc_complement;
  uint32_t end_char;

  float temperature_kelvin;
  float temperature_celsius;
  bool crc_valid;  // Toegevoegd: validatie na crc-check
};

class INIR2Sensor {
public:
  INIR2Sensor(HardwareSerial& serial);

  void setup();

  // Sensor modes activeren
  bool sendInitCommand();           // 'C' - Configuratiemodus
  bool sendReadConfigCommand();     // 'I' - Lees configuratie
  bool sendEngModeCommand();        // 'B' - Engineering mode
  bool sendOnDemandModeCommand();   // 'H' - On-demand meetmodus

  // Data ophalen
  bool requestData(SensorDataFrame_t& frame);

  // Laatst gemeten waarden uitlezen
  float getGas();                  // ppm
  float getTemperature();          // °C
  bool getCRCValid();              // true = dataframe had geldige CRC

private:
  HardwareSerial& _serial;
  SensorDataFrame_t _lastFrame;
  bool _lastCRCValid;

  // Helpers
  bool sendCommand(const String& cmd);
  bool parseDataFrame(const String& hexLine, SensorDataFrame_t& frame);

  float convertKelvinToCelsius(uint32_t kelvinTimes10);
  uint8_t calculateCRC(const SensorDataFrame_t& frame);
  uint8_t calculateCRCBytewise(uint32_t value);
};

#endif
