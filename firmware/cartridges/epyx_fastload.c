/*
 * Copyright (c) 2019-2020 Kim Jørgensen
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
static uint16_t epyx_cycles;

/*************************************************
* C64 bus read callback
*************************************************/
static inline bool epyx_read_handler(uint8_t control, uint16_t addr)
{
    if (!(control & C64_ROML))
    {
        epyx_cycles = 512;
        c64_data_write(crt_ptr[addr & 0x1fff]);
        return true;
    }

    if (!(control & C64_IO2))
    {
        c64_data_write(crt_ptr[addr & 0x1fff]);
        return true;
    }

    if (!(control & C64_IO1))
    {
        epyx_cycles = 512;
        c64_crt_control(STATUS_LED_ON|CRT_PORT_8K);
    }
    else if (epyx_cycles && --epyx_cycles == 0)
    {
        // Disable cartridge
        c64_crt_control(STATUS_LED_OFF|CRT_PORT_NONE);
    }

    return false;
}

/*************************************************
* C64 bus write callback
*************************************************/
static inline void epyx_write_handler(uint8_t control, uint16_t addr, uint8_t data)
{
    // No write support
}

static void epyx_init(void)
{
    c64_crt_control(STATUS_LED_ON|CRT_PORT_8K);
}

C64_BUS_HANDLER(epyx)
