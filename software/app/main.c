#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include "sys/alt_stdio.h"
#include "unistd.h"
#include "sys/alt_sys_init.h"
#include "io.h"

#include "sys/alt_irq.h"

#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"

#include "opencores_i2c.h"
#include "opencores_i2c_regs.h"

volatile enum axis{X, Y, Z} print_ax =X;

volatile alt_u16 value_x = 0, value_y = 0, value_z = 0;

alt_u16 complement_a_2(alt_u16 val);
void gsensor_i2c_single_write(alt_u8 register_addr, alt_u8 data);
alt_u8 gsensor_i2c_single_read(alt_u8 register_addr);
alt_u16 gsensor_i2c_multiple_read(alt_u8 register_addr);
alt_u16 gsensor_to_bcd(alt_u16 axes);
void gsensor_print();
void gsensor_measurement();
static void isr_handler_timer(void* context, alt_u32 id);
static void isr_handler_bp(void* context, alt_u32 id);


int main(){
	
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_3_BASE, 0b01);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_3_BASE, 0b01);
    alt_irq_register(PIO_3_IRQ, NULL, (void*)isr_handler_bp);
	
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0b0111);
    alt_irq_register(TIMER_0_IRQ, NULL, (void*)isr_handler_timer);

    I2C_init(OPENCORES_I2C_0_BASE, 50000000, 400000);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1d, 1);
    
    gsensor_i2c_single_read(0x00);
    gsensor_i2c_single_write(0x1E, 0);
    gsensor_i2c_single_write(0x1F, 0);
    gsensor_i2c_single_write(0x20, 0);

    while(1){}
    return 0;
}

alt_u16 complement_a_2(alt_u16 val){
    return (val ^ 0xffff) + 1;
}

void gsensor_i2c_single_write(alt_u8 register_addr, alt_u8 data){
    I2C_start(OPENCORES_I2C_0_BASE, 0x1d, 0);
    I2C_write(OPENCORES_I2C_0_BASE, register_addr, 0);
    I2C_write(OPENCORES_I2C_0_BASE, data, 1);
}

alt_u8 gsensor_i2c_single_read(alt_u8 register_addr){
    I2C_start(OPENCORES_I2C_0_BASE, 0x1d, 0);
    I2C_write(OPENCORES_I2C_0_BASE, register_addr, 0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1d, 1);
    return I2C_read(OPENCORES_I2C_0_BASE, 1);
}

alt_u16 gsensor_i2c_multiple_read(alt_u8 register_addr){
    alt_u8  lsb;
    alt_u16 msb;

    I2C_start(OPENCORES_I2C_0_BASE, 0x1d, 0);
    I2C_write(OPENCORES_I2C_0_BASE, register_addr, 0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1d, 1);
    lsb= I2C_read(OPENCORES_I2C_0_BASE, 0);
    msb= I2C_read(OPENCORES_I2C_0_BASE, 1);

    return (msb<<8) + (lsb);
}

alt_u16 gsensor_to_bcd(alt_u16 axes){
    alt_u8 sign = 0, dig1, dig2;

    if (axes >= 512){
		axes = complement_a_2(axes);
        sign = 1;        
    }

    axes = (axes*100)/230;

    dig2 = (axes/10)%10;
    dig1 = (axes/100)%10;

    return (sign<<8) + (dig1<<4) + (dig2);
}

void gsensor_print(){
    switch (print_ax){
    case X:
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, gsensor_to_bcd(value_x)); 
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, gsensor_to_bcd(value_x)>>4);  
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, gsensor_to_bcd(value_x)>>8);
		
        return;

    case Y:
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, gsensor_to_bcd(value_y)); 
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, gsensor_to_bcd(value_y)>>4);  
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, gsensor_to_bcd(value_y)>>8);
		
        return;

    case Z:
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, gsensor_to_bcd(value_z)); 
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, gsensor_to_bcd(value_z)>>4);  
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, gsensor_to_bcd(value_z)>>8);
		
        return;

    default:
        return;
    }
}

void gsensor_measurement(){
    x_val= gsensor_i2c_multiple_read(0x32);
    y_val= gsensor_i2c_multiple_read(0x34);
    z_val= gsensor_i2c_multiple_read(0x36);
}


static void isr_handler_timer(void* context, alt_u32 id){
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0b00);

    gsensor_measurement();
    gsensor_print();
}

static void isr_handler_bp(void* context, alt_u32 id){
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_3_BASE, 0b01);
	
    switch (print_ax){
    case X:
        print_ax= Y;
        return;
    case Y:
        print_ax= Z;
        return;
    case Z:
        print_ax= X;
        return;
    default:
        print_ax= X;
        return;
    }
}