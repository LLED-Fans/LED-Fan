//
// Created by Lukas Tenbrink on 05.07.20.
//

#include "Apa102Renderer.h"
#include <algorithm>
#include "Setup.h"

Apa102Renderer::Apa102Renderer(size_t pixelCount, size_t overflowWall) : Renderer(pixelCount, overflowWall) {
    _endBoundary = pixelCount / 32 * 4 + 1;
    _startBoundary = 4;
    bufferSize = pixelCount * 4 + _startBoundary + _endBoundary;

    esp_err_t err;

    spi_bus_config_t buscfg = {};
    buscfg.miso_io_num = -1;
    buscfg.mosi_io_num = LED_DATA_PIN;
    buscfg.sclk_io_num = LED_CLOCK_PIN;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = bufferSize;

    spi_device_interface_config_t devcfg = {};
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
    if (!buffer) {
        Serial.println("Failed to allocate SPI buffer; possibly too little DMA memory available?");
        exit(1);
    }
    _prepareBuffer();
}

void Apa102Renderer::_prepareBuffer() {
    // Prepare buffer - start and end boundary are
    // actually all 0, so we might as well just fill the whole buffer
    memset(buffer, 0, bufferSize);
}


void Apa102Renderer::_flush() {
    uint32_t boundary = 0b11100000;
    uint32_t emptyBoundary = uint32_t(0xff);

    uint8_t maxBrightness = 0b00011111;
    int _255e3 = 255 * 255 * 255;

    auto intBuffer = reinterpret_cast<uint32_t*>(buffer);

    unsigned int i = 0, c = _startBoundary / 4;
    while(i < pixelCount * 3) {
        uint32_t r_r = _rgbOutput[i++], g_r = _rgbOutput[i++], b_r = _rgbOutput[i++];
        uint32_t peakBrightness = std::max(std::max(r_r, g_r), b_r);

        if (peakBrightness == 0) {
            intBuffer[c++] = emptyBoundary;
            continue;
        }

        uint32_t brightness = ((peakBrightness - 1) / 255 * maxBrightness / _255e3) + 1;

        uint32_t rescaler = _255e3 * brightness / maxBrightness;

        // We may have rounding errors coming out at 256
        uint32_t r = std::min(r_r / rescaler, uint32_t(255));
        uint32_t g = std::min(g_r / rescaler, uint32_t(255));
        uint32_t b = std::min(b_r / rescaler, uint32_t(255));

        intBuffer[c++] = (r << 24) | (g << 16) | (b << 8) | boundary | brightness;
    }

    // Flush transaction

    spi_transaction_t *transaction = transactions + currentTransaction;
    memset(transaction, 0, sizeof(*transaction));
    transaction->length = bufferSize * 8; //length is in bits
    transaction->tx_buffer = buffer;

    auto err = spi_device_queue_trans(SPI_settings.spi, transaction, portMAX_DELAY);
    ESP_ERROR_CHECK(err);

    // Cycle to the other transaction
    currentTransaction = (currentTransaction + 1) % 2;
}

uint8_t Apa102Renderer::getMaxDynamicColorRescale() const {
    return _maxDynamicColorRescale;
}

void Apa102Renderer::setMaxDynamicColorRescale(uint8_t maxDynamicColorRescale) {
    _maxDynamicColorRescale = std::max(uint8_t(1), maxDynamicColorRescale);
}
