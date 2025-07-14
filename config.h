#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ========== BAUDRATES ==========
#define USB_BAUDRATE        115200
#define INIR_BAUDRATE       38400  // INIR2-sensoren (CO2/CH4)
#define PS1_BAUDRATE        9600   // PS1 NO2/SO2 sensoren

// ========== UART PIN DEFINITIES ==========

// UART0 – communicatie met moedermodule
#define UART0_TX_PIN        1
#define UART0_RX_PIN        3

// UART1 – CO₂-sensor (INIR2-CD100)
#define UART1_TX_PIN        26
#define UART1_RX_PIN        25

// UART2 – CH₄-sensor (INIR2-ME5)
#define UART2_TX_PIN        15
#define UART2_RX_PIN        4

// SoftwareSerial A – NO₂-sensor (PS1-NO2)
#define SOFT_A_TX_PIN       32
#define SOFT_A_RX_PIN       33

// SoftwareSerial B – SO₂-sensor (PS1-SO2)
#define SOFT_B_TX_PIN       27
#define SOFT_B_RX_PIN       14

// ========== DEBUG MODE ==========
#define DEBUG_MODE          true

#endif
