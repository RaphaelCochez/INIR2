#include "INIR2.h"

// Create the sensor object
INIRSensor sensor(10, 11, 38400); // RX: 10, TX: 11

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Sensor...");

  // Step 1: Enter Configuration Mode
  if (!sensor.enterConfigurationMode()) {
    Serial.println("Failed to enter Configuration Mode.");
    while (true); // Halt execution if configuration mode fails
  }

  // Step 2: Read and Print Settings
  Serial.println("Reading Sensor Settings...");
  SensorSettings settings = sensor.readSettings();

  // Print Settings
  Serial.println("Sensor Settings:");
  Serial.print("Sensor Type: ");
  Serial.println(settings.sensorType, HEX);
  Serial.print("Gas Type: ");
  Serial.println(settings.gasType, HEX);
  Serial.print("Concentration Range: ");
  Serial.print(settings.concRange);
  Serial.println(" ppm");
  Serial.print("Span: ");
  Serial.print(settings.span);
  Serial.println(" ppm");
  Serial.print("Zero: ");
  Serial.print(settings.zero);
  Serial.println(" ppm");
  Serial.print("Offset: ");
  Serial.print(settings.offset);
  Serial.println(" ppm");
  Serial.print("Calibration Temperature: ");
  Serial.print(settings.calibrationTemperature);
  Serial.println(" K");
  Serial.print("Baud Rate: ");
  Serial.println(settings.baudRate);
  Serial.print("Firmware Version: ");
  Serial.println(settings.firmwareVersion);
  Serial.print("Serial Number: ");
  Serial.println(settings.serialNumber, HEX);
  Serial.print("CRC: ");
  Serial.println(settings.crc, HEX);

  // Step 3: Enter Engineering Mode for Continuous Data
  if (!sensor.enterEngineeringMode()) {
    Serial.println("Failed to enter Engineering Mode.");
    while (true); // Halt execution if Engineering Mode fails
  }
  Serial.println("Sensor is now in Engineering Mode.");
}

void loop() {
  // Read PPM values in the current mode (Engineering Mode)
  String ppmResponse = sensor.readResponse();

  // Parse the gas concentration in PPM from the response
  if (ppmResponse.startsWith("[") && ppmResponse.endsWith("]")) {
    float ppmValue = strtol(ppmResponse.substring(1, 9).c_str(), nullptr, 16) / 1000000.0; // Convert HEX to float
    Serial.print("Gas Concentration (PPM): ");
    Serial.print(ppmValue);
    Serial.println(" ppm");
  } else {
    Serial.println("Invalid response received.");
  }

  delay(1000); // Wait 1 second before the next reading
}
