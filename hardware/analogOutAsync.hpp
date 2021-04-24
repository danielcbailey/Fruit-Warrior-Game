/*
 * Analog Out Async Class
 *
 * A driver for the DAC peripheral that takes advantage of hardware
 * DMA channels to allow the processor to not be concerned with each
 * sample.
 *
 * (c) 2021 Daniel Cooper
 */

#ifndef ANALOG_OUT_ASYNC_INCLUDED
#define ANALOG_OUT_ASYNC_INCLUDED

#include <stdint.h>
#include "dma.h"

class AnalogOutAsync {
    private:

    DMA_CHANNEL* dma;


    public:

    AnalogOutAsync(); // Can only be on one pin, so no pin required.

    /**
     * Writes the contents of buf to the analog out pin
     * @param buf The buffer containing samples
     * @param size The number of bytes the buffer contains
     * @param rate The samples per second to output
     */
    void write_u16(uint16_t* buf, int size, int rate);

    inline bool isFinished() { return isDMAFinished(this->dma); }
};

#endif //ANALOG_OUT_ASYNC_INCLUDED