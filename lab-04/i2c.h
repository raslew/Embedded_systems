#ifndef _I2C_H_
#define _I2C_H_

// I2C Read/Write flags
#define I2C_R	1
#define I2C_W	0
#define CONTROL_BYTE 0xA0


void i2c_init(void);

void i2c_start(void);
void i2c_stop(void);

uint8_t i2c_get_status(void);
void i2c_meaningful_status(uint8_t status);

void i2c_xmit_addr(uint8_t address, uint8_t rw);
void i2c_xmit_byte(uint8_t data);

void eeprom_wait_until_write_complete(void);

uint8_t eeprom_read_byte(uint8_t addr);
void eeprom_write_byte(uint8_t addr, uint8_t data);

void eeprom_write_page(uint8_t addr, uint8_t *data, uint8_t len);

void eeprom_sequential_read(uint8_t *buf, uint8_t start_addr, uint8_t len);

void eeprom_slicer(uint8_t eep_adr3, uint8_t *fname, uint8_t len_array);

#endif // _I2C_H_
