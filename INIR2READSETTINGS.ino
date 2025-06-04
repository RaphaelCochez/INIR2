#include <SoftwareSerial.h>

// Define RX and TX pins for the INIR2 sensor
#define INIR2_RX_PIN 16
#define INIR2_TX_PIN 17

// Set up SoftwareSerial for the INIR2 sensor
SoftwareSerial inirSerial(INIR2_RX_PIN, INIR2_TX_PIN);

void setup() {
    Serial.begin(9600);       // Initialize Serial Monitor
    inirSerial.begin(38400);  // Initialize INIR2 sensor UART
    Serial.println("Initializing INIR2 Sensor...");

    initializeINIR2();  // Run initialization sequence
}

void loop() {
    // Continuously read sensor data after initialization
    if (inirSerial.available()) {
        String response = inirSerial.readStringUntil('\n'); // Read UART response
        Serial.println("Sensor Data: " + response);
    }
    delay(100); // Small delay for readability
}


// Function to initialize the INIR2 sensor
void initializeINIR2() {
    // Step 1: Enter Configuration Mode
    sendCommand('C');
    if (!readResponse()) {
        Serial.println("Error: Failed to enter Configuration Mode.");
        return;
    }

    // Step 2: Read back all settings
    sendCommand('I');
    if (!readResponse()) {
        Serial.println("Error: Failed to read settings.");
        return;
    }

    // Step 3: Enter Engineering Mode
    sendCommand('B');
    if (!readResponse()) {
        Serial.println("Error: Failed to enter Engineering Mode.");
        return;
    }

    Serial.println("Initialization complete. Sensor is operational.");
}

// Function to send a command to the INIR2 sensor
void sendCommand(char command) {
    String cmd = "[" + String(command) + "]";
    inirSerial.print(cmd); // Send command via UART
    Serial.print("Command Sent: ");
    Serial.println(cmd);
}

// Function to read a response from the sensor
bool readResponse() {
    String response = "";
    unsigned long startTime = millis();

    // Wait for response with a timeout
    while ((millis() - startTime) < 2000) { // 2-second timeout
        if (inirSerial.available()) {
            response += (char)inirSerial.read();
        }
    }

    // Print the raw response for debugging
    Serial.println("Sensor Response: " + response);

    

    // Validate response (implement CRC or ACK checks here if necessary)
    return response.length() > 0;
}
