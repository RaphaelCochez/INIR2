// Function to decode gas type
String decodeGasType(byte hexcode) {
    switch (hexcode) {
        case 0:
            return "Methane (CH4)";
        case 3:
            return "Carbon Dioxide (CO2)";
        default:
            return "Reserved or Unknown Gas Type";
    }
}

// Function to decode sensor type
String decodeSensorType(uint32_t hexcode) { // Use uint32_t for 32-bit values
    switch (hexcode) {
        case 0x00000017:
            return "INIR-CD";
        case 0x0000001A:
            return "INIR-ME";
        default:
            return "Unknown Sensor Type";
    }
}

// Function to decode acknowledgement
int decodeAcknowledgement(uint32_t hexcode) { // Use uint32_t for 32-bit values
    if (hexcode == 0x5B414B5D) { // Acknowledged
        return 1;
    } else if (hexcode == 0x5B4E415D) { // Not acknowledged
        return 0;
    } else {
        return -1; // Unknown or invalid
    }
}
