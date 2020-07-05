//
// Created by Lukas Tenbrink on 05.07.20.
//

#ifndef LED_FAN_SPITOOLS_H
#define LED_FAN_SPITOOLS_H

#include <HardwareSerial.h>

#include <driver/spi_common.h>
#include <driver/spi_master.h>

#include <cstdint>
#include <SPI.h>

typedef struct {
    spi_host_device_t host;
    spi_device_handle_t spi;
    int dma_chan;
    spi_device_interface_config_t devcfg;
    spi_bus_config_t buscfg;
} SPI_settings_t;

#endif //LED_FAN_SPITOOLS_H
