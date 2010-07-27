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

/**
 *  @file usart.c
 *
 *  @brief USART control routines
 */

/* USART1 = APB2 72 mhz
 * USART2 = APB1 36 mhz
 * USART3 = APB1 36 mhz */

#include <stdlib.h>
#include "libmaple.h"
#include "rcc.h"
#include "nvic.h"
#include "usart.h"

#define USART1_BASE         0x40013800
#define USART2_BASE         0x40004400
#define USART3_BASE         0x40004800

static uint8 buf[64];

struct usart_dev usart_dev_table[] = {
   [USART1] = {
      .base = (usart_port*)USART1_BASE,
      .rcc_dev_num = RCC_USART1,
      .nvic_dev_num = NVIC_USART1
   },
   [USART2] = {
      .base = (usart_port*)USART2_BASE,
      .rcc_dev_num = RCC_USART2,
      .nvic_dev_num = NVIC_USART2
   },
   [USART3] = {
      .base = (usart_port*)USART3_BASE,
      .rcc_dev_num = RCC_USART3,
      .nvic_dev_num = NVIC_USART3
   },
};

void USART1_IRQHandler(void) {
    /* Read the data  */
//    ring_buf1.buf[ring_buf1.tail++] = (uint8)(((usart_port*)(USART1_BASE))->DR);
//    ring_buf1.tail %= USART_RECV_BUF_SIZE;
}

/* Don't overrun your buffer, seriously  */
void USART2_IRQHandler(void) {
    /* Read the data  */
//    ring_buf2.buf[ring_buf2.tail++] = (uint8)(((usart_port*)(USART2_BASE))->DR);
//    ring_buf2.tail %= USART_RECV_BUF_SIZE;
}
/* Don't overrun your buffer, seriously  */
void USART3_IRQHandler(void) {
    /* Read the data  */
//    ring_buf3.buf[ring_buf3.tail++] = (uint8)(((usart_port*)(USART3_BASE))->DR);
//    ring_buf3.tail %= USART_RECV_BUF_SIZE;
    rb_insert(&usart_dev_table[USART3].rb,(uint8)(((usart_port*)(USART3_BASE))->DR));
}

/**
 *  @brief Enable a USART in single buffer transmission mode, multibuffer
 *  receiver mode.
 *
 *  @param usart_num USART to be initialized
 *  @param baud Baud rate to be set at
 *  @param recvBuf buf buffer for receiver
 *  @param len size of recvBuf
 *
 *  @sideeffect Turns on the specified USART
 */
void usart_init(uint8 usart_num, uint32 baud) {
    usart_port *port;
    ring_buffer *ring_buf;
    uint8 *buf;

    uint32 clk_speed;
    uint32 integer_part;
    uint32 fractional_part;
    uint32 tmp;

    port = usart_dev_table[usart_num].base;
    rcc_enable_device(usart_dev_table[usart_num].rcc_dev_num);
    nvic_enable_interrupt(usart_dev_table[usart_num].nvic_dev_num);

    /* usart1 is mad fast  */
    clk_speed = (usart_num == USART1) ? 72000000UL : 36000000UL;

    /* Initialize ring buffer  */
    rb_init(&usart_dev_table[usart_num].rb, 64, buf);

    /* Set baud rate  */
    integer_part = ((25 * clk_speed) / (4 * baud));
    tmp = (integer_part / 100) << 4;

    fractional_part = integer_part - (100 * (tmp >> 4));
    tmp |= (((fractional_part * 16) + 50) / 100) & ((uint8)0x0F);

    port->BRR = (uint16)tmp;

    port->CR1 = USART_TE          |    // transmitter enable
                USART_RE          |    // receiver enable
                USART_RXNEIE;          // receive interrupt enable


    /* Enable the USART and set mode to 8n1 */
    port->CR1 |= USART_UE;
}


/**
 *  @brief Turn off a USART.
 *
 *  @param USART to be disabled
 *
 *  @sideeffect Turns off the specified USART
 */
void usart_disable(uint8 usart_num) {
    usart_port *port = usart_dev_table[usart_num].base;

    /* TC bit must be high before disabling the usart  */
    while ((port->SR & USART_TC) == 0)
        ;

    /* Disable UE */
    port->CR1 = 0;

    /* Clean up buffer */
    usart_clear_buffer(usart_num);
}


/**
 *  @brief Print a null terminated string to the specified USART
 *
 *  @param[in] usart_num USART to send on
 *  @param[in] str String to send
 */
void usart_putstr(uint8 usart_num, const char* str) {
    char ch;

    while((ch = *(str++)) != '\0') {
        usart_putc(usart_num, ch);
    }
}

/**
 *  @brief Print an unsigned integer to the specified usart
 *
 *  @param[in] usart_num usart to send on
 *  @param[in] val number to print
 */
void usart_putudec(uint8 usart_num, uint32 val) {
    char digits[12];
    int i;

    i = 0;
    do {
        digits[i++] = val % 10 + '0';
        val /= 10;
    } while (val > 0);
    while (--i >= 0) {
        usart_putc(usart_num, digits[i]);
    }
}




