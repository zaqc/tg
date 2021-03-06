#ifndef _ALTERA_HPS_0_H_
#define _ALTERA_HPS_0_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'soc_system' in
 * file '/home/zaqc/work/DE0Clean/de0_nano_mtl2_lxde/soc_system.sopcinfo'.
 */

/*
 * This file contains macros for module 'hps_0' and devices
 * connected to the following masters:
 *   h2f_axi_master
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'pio_reset', class 'altera_avalon_pio'
 * The macros are prefixed with 'PIO_RESET_'.
 * The prefix is the slave descriptor.
 */
#define PIO_RESET_COMPONENT_TYPE altera_avalon_pio
#define PIO_RESET_COMPONENT_NAME pio_reset
#define PIO_RESET_BASE 0x0
#define PIO_RESET_SPAN 128
#define PIO_RESET_END 0x7f
#define PIO_RESET_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_RESET_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_RESET_CAPTURE 0
#define PIO_RESET_DATA_WIDTH 1
#define PIO_RESET_DO_TEST_BENCH_WIRING 0
#define PIO_RESET_DRIVEN_SIM_VALUE 0
#define PIO_RESET_EDGE_TYPE NONE
#define PIO_RESET_FREQ 50000000
#define PIO_RESET_HAS_IN 0
#define PIO_RESET_HAS_OUT 1
#define PIO_RESET_HAS_TRI 0
#define PIO_RESET_IRQ_TYPE NONE
#define PIO_RESET_RESET_VALUE 0

/*
 * Macros for device 'pio_sync', class 'altera_avalon_pio'
 * The macros are prefixed with 'PIO_SYNC_'.
 * The prefix is the slave descriptor.
 */
#define PIO_SYNC_COMPONENT_TYPE altera_avalon_pio
#define PIO_SYNC_COMPONENT_NAME pio_sync
#define PIO_SYNC_BASE 0x10
#define PIO_SYNC_SPAN 128
#define PIO_SYNC_END 0x8f
#define PIO_SYNC_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_SYNC_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_SYNC_CAPTURE 0
#define PIO_SYNC_DATA_WIDTH 1
#define PIO_SYNC_DO_TEST_BENCH_WIRING 0
#define PIO_SYNC_DRIVEN_SIM_VALUE 0
#define PIO_SYNC_EDGE_TYPE NONE
#define PIO_SYNC_FREQ 50000000
#define PIO_SYNC_HAS_IN 0
#define PIO_SYNC_HAS_OUT 1
#define PIO_SYNC_HAS_TRI 0
#define PIO_SYNC_IRQ_TYPE NONE
#define PIO_SYNC_RESET_VALUE 0

/*
 * Macros for device 'touch_i2c', class 'oc_i2c_master'
 * The macros are prefixed with 'TOUCH_I2C_'.
 * The prefix is the slave descriptor.
 */
#define TOUCH_I2C_COMPONENT_TYPE oc_i2c_master
#define TOUCH_I2C_COMPONENT_NAME touch_i2c
#define TOUCH_I2C_BASE 0x20
#define TOUCH_I2C_SPAN 32
#define TOUCH_I2C_END 0x3f
#define TOUCH_I2C_IRQ 2

/*
 * Macros for device 'channal_cntrl', class 'channal_cntrl'
 * The macros are prefixed with 'CHANNAL_CNTRL_'.
 * The prefix is the slave descriptor.
 */
#define CHANNAL_CNTRL_COMPONENT_TYPE channal_cntrl
#define CHANNAL_CNTRL_COMPONENT_NAME channal_cntrl
#define CHANNAL_CNTRL_BASE 0x40
#define CHANNAL_CNTRL_SPAN 8
#define CHANNAL_CNTRL_END 0x47

/*
 * Macros for device 'alt_vip_vfr_0', class 'alt_vip_vfr'
 * The macros are prefixed with 'ALT_VIP_VFR_0_'.
 * The prefix is the slave descriptor.
 */
#define ALT_VIP_VFR_0_COMPONENT_TYPE alt_vip_vfr
#define ALT_VIP_VFR_0_COMPONENT_NAME alt_vip_vfr_0
#define ALT_VIP_VFR_0_BASE 0x100
#define ALT_VIP_VFR_0_SPAN 1024
#define ALT_VIP_VFR_0_END 0x4ff

/*
 * Macros for device 'intterrupt_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'INTTERRUPT_PIO_'.
 * The prefix is the slave descriptor.
 */
#define INTTERRUPT_PIO_COMPONENT_TYPE altera_avalon_pio
#define INTTERRUPT_PIO_COMPONENT_NAME intterrupt_pio
#define INTTERRUPT_PIO_BASE 0x200
#define INTTERRUPT_PIO_SPAN 128
#define INTTERRUPT_PIO_END 0x27f
#define INTTERRUPT_PIO_IRQ 0
#define INTTERRUPT_PIO_BIT_CLEARING_EDGE_REGISTER 0
#define INTTERRUPT_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define INTTERRUPT_PIO_CAPTURE 1
#define INTTERRUPT_PIO_DATA_WIDTH 1
#define INTTERRUPT_PIO_DO_TEST_BENCH_WIRING 0
#define INTTERRUPT_PIO_DRIVEN_SIM_VALUE 0
#define INTTERRUPT_PIO_EDGE_TYPE FALLING
#define INTTERRUPT_PIO_FREQ 50000000
#define INTTERRUPT_PIO_HAS_IN 1
#define INTTERRUPT_PIO_HAS_OUT 0
#define INTTERRUPT_PIO_HAS_TRI 0
#define INTTERRUPT_PIO_IRQ_TYPE EDGE
#define INTTERRUPT_PIO_RESET_VALUE 0

/*
 * Macros for device 'led_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'LED_PIO_'.
 * The prefix is the slave descriptor.
 */
#define LED_PIO_COMPONENT_TYPE altera_avalon_pio
#define LED_PIO_COMPONENT_NAME led_pio
#define LED_PIO_BASE 0x10000
#define LED_PIO_SPAN 128
#define LED_PIO_END 0x1007f
#define LED_PIO_BIT_CLEARING_EDGE_REGISTER 0
#define LED_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_PIO_CAPTURE 0
#define LED_PIO_DATA_WIDTH 8
#define LED_PIO_DO_TEST_BENCH_WIRING 0
#define LED_PIO_DRIVEN_SIM_VALUE 0
#define LED_PIO_EDGE_TYPE NONE
#define LED_PIO_FREQ 50000000
#define LED_PIO_HAS_IN 0
#define LED_PIO_HAS_OUT 1
#define LED_PIO_HAS_TRI 0
#define LED_PIO_IRQ_TYPE NONE
#define LED_PIO_RESET_VALUE 0

/*
 * Macros for device 'button_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'BUTTON_PIO_'.
 * The prefix is the slave descriptor.
 */
#define BUTTON_PIO_COMPONENT_TYPE altera_avalon_pio
#define BUTTON_PIO_COMPONENT_NAME button_pio
#define BUTTON_PIO_BASE 0x10010
#define BUTTON_PIO_SPAN 128
#define BUTTON_PIO_END 0x1008f
#define BUTTON_PIO_IRQ 1
#define BUTTON_PIO_BIT_CLEARING_EDGE_REGISTER 1
#define BUTTON_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define BUTTON_PIO_CAPTURE 1
#define BUTTON_PIO_DATA_WIDTH 2
#define BUTTON_PIO_DO_TEST_BENCH_WIRING 0
#define BUTTON_PIO_DRIVEN_SIM_VALUE 0
#define BUTTON_PIO_EDGE_TYPE ANY
#define BUTTON_PIO_FREQ 50000000
#define BUTTON_PIO_HAS_IN 1
#define BUTTON_PIO_HAS_OUT 0
#define BUTTON_PIO_HAS_TRI 0
#define BUTTON_PIO_IRQ_TYPE EDGE
#define BUTTON_PIO_RESET_VALUE 0

/*
 * Macros for device 'dipsw_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'DIPSW_PIO_'.
 * The prefix is the slave descriptor.
 */
#define DIPSW_PIO_COMPONENT_TYPE altera_avalon_pio
#define DIPSW_PIO_COMPONENT_NAME dipsw_pio
#define DIPSW_PIO_BASE 0x10040
#define DIPSW_PIO_SPAN 128
#define DIPSW_PIO_END 0x100bf
#define DIPSW_PIO_IRQ 3
#define DIPSW_PIO_BIT_CLEARING_EDGE_REGISTER 1
#define DIPSW_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DIPSW_PIO_CAPTURE 1
#define DIPSW_PIO_DATA_WIDTH 4
#define DIPSW_PIO_DO_TEST_BENCH_WIRING 0
#define DIPSW_PIO_DRIVEN_SIM_VALUE 0
#define DIPSW_PIO_EDGE_TYPE ANY
#define DIPSW_PIO_FREQ 50000000
#define DIPSW_PIO_HAS_IN 1
#define DIPSW_PIO_HAS_OUT 0
#define DIPSW_PIO_HAS_TRI 0
#define DIPSW_PIO_IRQ_TYPE EDGE
#define DIPSW_PIO_RESET_VALUE 0

/*
 * Macros for device 'data_receiver', class 'data_receiver'
 * The macros are prefixed with 'DATA_RECEIVER_'.
 * The prefix is the slave descriptor.
 */
#define DATA_RECEIVER_COMPONENT_TYPE data_receiver
#define DATA_RECEIVER_COMPONENT_NAME data_receiver
#define DATA_RECEIVER_BASE 0x20000
#define DATA_RECEIVER_SPAN 65536
#define DATA_RECEIVER_END 0x2ffff


#endif /* _ALTERA_HPS_0_H_ */
