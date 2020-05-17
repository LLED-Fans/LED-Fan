//
// Created by Lukas Tenbrink on 05.05.20.
//

#ifndef LED_FAN_ESP32SPI_H
#define LED_FAN_ESP32SPI_H

#include "Setup.h"

#include <FastLED.h>
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

template <uint8_t _DATA_PIN, uint8_t _CLOCK_PIN, uint32_t _CLOCK_SPEED, spi_host_device_t host>
class ESP32SPI {
public:
    int bufferSize = SPI_BUFFER_SIZE;

    uint8_t* buffer;

    SPI_settings_t SPI_settings = {};

    int transactionLength = 0;

    spi_transaction_t transactions[2];
    int currentTransaction = 0;

    void init() {
        esp_err_t err;

        spi_bus_config_t buscfg = {};
        buscfg.miso_io_num = -1;
        buscfg.mosi_io_num = _DATA_PIN;
        buscfg.sclk_io_num = _CLOCK_PIN;
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        buscfg.max_transfer_sz = bufferSize;

        spi_device_interface_config_t devcfg = {};
        // Honestly, no clue what _CLOCK_SPEED is, so just use our own
        devcfg.clock_speed_hz = LED_CLOCK_SPEED_MHZ * 1000 * 1000;
        devcfg.mode = 0; //SPI mode 0
        devcfg.spics_io_num = -1; //CS pin
        devcfg.queue_size = 1; //Not sure if needed
        devcfg.command_bits = 0;
        devcfg.address_bits = 0;

        SPI_settings.host = HSPI_HOST;
        SPI_settings.dma_chan = 2;
        SPI_settings.buscfg = buscfg;
        SPI_settings.devcfg = devcfg;

        err = spi_bus_initialize(SPI_settings.host, &SPI_settings.buscfg, SPI_settings.dma_chan);
        ESP_ERROR_CHECK(err);

        //Attach the Accel to the SPI bus
        err = spi_bus_add_device(SPI_settings.host, &SPI_settings.devcfg, &SPI_settings.spi);
        ESP_ERROR_CHECK(err);

        // Allocate DMA memory
        buffer = reinterpret_cast<uint8_t *>(heap_caps_malloc(bufferSize, MALLOC_CAP_DMA));
        if (!buffer)
            // Probably not enough DMA memory
            throw std::bad_exception();
    }

    void inline select() __attribute__((always_inline)) {
        // No need to wipe; we overwrite anyway
//        memset(buffer, 0, LED_DMA_BUFFER_SIZE);

        // Don't wait since there is no way to check if it's already complete
// queue_trans will wait if there's no space
//        if (transaction) {
//            // wait for last transaction to finish, if need be
//            auto err = spi_device_get_trans_result(SPI_settings.spi, &transaction, portMAX_DELAY);
//            ESP_ERROR_CHECK(err);
//        }

        transactionLength = 0;
    }

    void inline release() __attribute__((always_inline)) {
        if (transactionLength > bufferSize)
            // Oh god, we fucked up
            // If you get this error, re-calculate your buffer size
            throw std::bad_exception();

        spi_transaction_t *transaction = transactions + currentTransaction;
        memset(transaction, 0, sizeof(*transaction));
        transaction->length = transactionLength * 8; //length is in bits
        transaction->tx_buffer = buffer;

        auto err = spi_device_queue_trans(SPI_settings.spi, transaction, portMAX_DELAY);
        ESP_ERROR_CHECK(err);

        // Cycle to the other transaction
        currentTransaction = (currentTransaction + 1) % 2;
    }

    static void waitFully() { } // No need to wait lol

    void inline writeByte(uint8_t b) __attribute__((always_inline)) {
        buffer[transactionLength++] = b;
    }

    void inline writeWord(uint16_t w) __attribute__((always_inline)) {
        *reinterpret_cast<uint16_t *>(&buffer[transactionLength]) = w;
        transactionLength += 2;
    }
};

#ifdef SPI_ESP32_HARDWARE_SPI_HOST
template<uint32_t SPI_SPEED>\
class SPIOutput<SPI_DATA, SPI_CLOCK, SPI_SPEED> : public ESP32SPI<SPI_DATA, SPI_CLOCK, SPI_SPEED, SPI_ESP32_HARDWARE_SPI_HOST> {};
#endif

#define ESPHardwareSPIOutput(_SPI_HOST, _SPI_CLOCK, _SPI_DATA)\
template<uint32_t SPI_SPEED>\
class SPIOutput<_SPI_DATA, _SPI_CLOCK, SPI_SPEED> : public ESP32SPI<_SPI_DATA, _SPI_CLOCK, SPI_SPEED, _SPI_HOST> {};

ESPHardwareSPIOutput(VSPI_HOST, 18, 23)
ESPHardwareSPIOutput(HSPI_HOST, 14, 13)

#endif //LED_FAN_ESP32SPI_H
