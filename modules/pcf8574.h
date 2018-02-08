/*
 * PCF8574.h
 *
 *  Created on: 28-07-2014
 *      Author: Bartek
 */

#ifndef PCF8574_H_
#define PCF8574_H_

#define PCF8574_READ_ERROR		0xFF

uint8_t pcf8574_read_port();
uint8_t pcf8574_read_pin( uint8_t pin_num );

void pcf8574_set_port_dir_output(uint8_t pin_mask);
void pcf8574_set_port_dir_input(uint8_t pin_mask);
void pcf8574_set_port_high(uint8_t pin_mask);
void pcf8574_set_port_low(uint8_t pin_mask);

#endif /* PCF8574_H_ */
