/* *****************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#include "libmaple.h"
#include "flash.h"

#define FLASH_BASE                      0x40022000
#define FLASH_ACR                       FLASH_BASE

#define FLASH_WRITE_ACR(val)            __write(FLASH_ACR, val)
#define FLASH_READ_ACR()                __read(FLASH_ACR)

#define ACR_PRFTBE                      BIT(4)
#define ACR_PRFTBE_ENABLE               BIT(4)

/* flash wait states  */
#define ACR_LATENCY                     (0x7)
#define ACR_LATENCY_0                   (0x0)
#define ACR_LATENCY_1                   (0x1)
#define ACR_LATENCY_2                   (0x2)

static uint32 flash_wait_states[2] = {
   ACR_LATENCY_0,
   ACR_LATENCY_1,
   ACR_LATENCY_2
};

void flash_enable_prefetch(void) {
   uint32 val = FLASH_READ_ACR();

   val |= ACR_PRFTBE_ENABLE;

   FLASH_WRITE_ACR(val);
}

void flash_set_latency(uint32 wait_states) {
   uint32 val = FLASH_READ_ACR();

   val &= ~ACR_LATENCY;
   val |= flash_wait_states[wait_states];

   FLASH_WRITE_ACR(val);
}
