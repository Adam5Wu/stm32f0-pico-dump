/*
 * Copyright (C) 2017 Obermaier Johannes
 * Copyright (C) 2022 Lucas Teske
 *
 * This Source Code Form is subject to the terms of the MIT License.
 * If a copy of the MIT License was not distributed with this file,
 * you can obtain one at https://opensource.org/licenses/MIT
 */

#include <Arduino.h>

extern "C" {
    #include "main.h"
    #include "reader.h"
}

// STM32 target flash memory size in bytes
uint32_t size = 65536;

// Usually the STM32F0x starts here.
// If you're trying to dump another series check the datasheet.
uint32_t flashAddress = 0x08000000;

void setup() {
    swdStatus_t status;
    Serial.begin(115200);

    pinMode(TARGET_RESET_Pin, OUTPUT);
    pinMode(TARGET_PWR_Pin, OUTPUT);
    pinMode(SWDIO_Pin, OUTPUT);
    pinMode(SWCLK_Pin, OUTPUT);

    targetInit();
    digitalWrite(LED1_Pin, HIGH);
    while(!Serial.available()) {
        delay(1000);
        Serial.println("Send anything to start...");
    }
    Serial.printf("Starting from %08x...", flashAddress);

    uint32_t flashData = 0;
    for (uint32_t i = 0; i < size; i+=4) {
        flashData = 0;
        status = extractFlashData(flashAddress + i, &flashData);
        if (status != swdStatusOk) {
            Serial.printf("\r\nError reading: %d\r\n", status);
            break;
        }
        Serial.printf("%s%02x%02x%02x%02x", (i & 0xF)? " " : "\r\n",
                       flashData & 0xFF, (flashData >> 8) & 0xFF,
                       (flashData >> 16) & 0xFF, (flashData >> 24) & 0xFF);
    }
    Serial.println("\r\nDONE");
}

void loop() {

}