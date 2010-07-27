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
 *  @file usart.h
 *
 *  @brief USART Definitions
 */

#ifndef _USART_H_
#define _USART_H_

#include "ring_buffer.h"

#ifdef __cplusplus
extern "C"{
#endif

#define USART_UE            BIT(13)
#define USART_M             BIT(12)
#define USART_TE            BIT(3)
#define USART_RE            BIT(2)
#define USART_RXNEIE        BIT(5)       // read data register not empty interrupt enable
#define USART_TXE           BIT(7)
#define USART_TC            BIT(6)

#define USART_STOP_BITS_1   BIT_MASK_SHIFT(0b0, 12)
#define USART_STOP_BITS_05  BIT_MASK_SHIFT(0b01, 12)
#define USART_STOP_BITS_2   BIT_MASK_SHIFT(0b02, 12)
#define USART_STOP_BITS_15  BIT_MASK_SHIFT(0b02, 12)

#define USART_RECV_BUF_SIZE 64

enum {
   USART1 = 0x0,
   USART2,
   USART3,
   USART4,
   USART5,
};

typedef struct usart_port {
    volatile uint32 SR;       // Status register
    volatile uint32 DR;       // Data register
    volatile uint32 BRR;      // Baud rate register
    volatile uint32 CR1;      // Control register 1
    volatile uint32 CR2;      // Control register 2
    volatile uint32 CR3;      // Control register 3
    volatile uint32 GTPR;     // Guard time and prescaler register
} usart_port;

//typedef struct usart_ring_buf {
//    uint32 head;
//    uint32 tail;
//    uint8 buf[USART_RECV_BUF_SIZE];
//} usart_ring_buf;


struct usart_dev {
   usart_port *base;
   ring_buffer *buf;
   const uint8 rcc_dev_num;
   const uint8 nvic_dev_num;
};

extern const struct usart_dev usart_dev_table[];

static inline void usart_putc(uint8 usart_num, uint8 byte) {
    usart_port *port = usart_dev_table[usart_num].base;

    port->DR = byte;

    /* Wait for transmission to complete  */
    while ((port->SR & USART_TXE) == 0)
        ;
}

static inline uint8 usart_getc(uint8 usart_num) {
    return rb_remove(usart_dev_table[usart_num].buf);
}

static inline uint32 usart_data_available(uint8 usart_num) {
    return rb_full_count(usart_dev_table[usart_num].buf);
}

static inline void usart_clear_buffer(uint8 usart_num) {
    rb_reset(usart_dev_table[usart_num].buf);
}


//void usart_putc(uint8 usart_num, uint8 ch);

void usart_init(uint8 usart_num, uint32 baud);
void usart_disable(uint8 usart_num);
void usart_putstr(uint8 usart_num, const char*);
void usart_putudec(uint8 usart_num, uint32 val);
//uint32 usart_data_available(uint8 usart_num);
//uint8 usart_getc(uint8 usart_num);
//void usart_clear_buffer(uint8 usart_num);

#ifdef __cplusplus
} // extern "C"
#endif


#endif

