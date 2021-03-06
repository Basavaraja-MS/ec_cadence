/* -*- mode:c -*-
 *
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

GPIO_INT(LID_OPEN,          PIN(27), GPIO_INT_BOTH | GPIO_PULL_UP, lid_interrupt)
GPIO_INT(AC_PRESENT,        PIN(30), GPIO_INT_BOTH, extpower_interrupt)
GPIO_INT(WP_L,              PIN(33), GPIO_INT_BOTH,        switch_interrupt)
/* Buffered power button input from PMIC / ROP_EC_PWR_BTN_L_R */
GPIO_INT(POWER_BUTTON_L,    PIN(35), GPIO_INT_BOTH, power_button_interrupt)
/* RSMRST from PMIC */
GPIO_INT(RSMRST_L_PGOOD,    PIN(63),  GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PCH_SLP_S4_L,      PIN(200), GPIO_INT_BOTH | GPIO_PULL_UP, power_signal_interrupt)
GPIO_INT(PCH_SLP_S3_L,      PIN(206), GPIO_INT_BOTH | GPIO_PULL_UP, power_signal_interrupt)
GPIO_INT(PCH_SLP_S0_L,      PIN(141), GPIO_INT_BOTH, power_signal_interrupt_S0)
GPIO_INT(PCH_SLP_SUS_L,     PIN(12),  GPIO_INT_BOTH | GPIO_PULL_UP, power_signal_interrupt)
GPIO_INT(VOLUME_UP_L,       PIN(31),  GPIO_INT_BOTH | GPIO_PULL_UP, button_interrupt)
GPIO_INT(VOLUME_DOWN_L,     PIN(47),  GPIO_INT_BOTH | GPIO_PULL_UP, button_interrupt)
GPIO_INT(PMIC_INT_L,        PIN(50),  GPIO_INT_FALLING, power_signal_interrupt)
GPIO_INT(PD_MCU_INT,        PIN(122), GPIO_INT_FALLING, pd_mcu_interrupt)
GPIO_INT(USB_C0_VBUS_WAKE_L,PIN(152), GPIO_INT_BOTH, vbus0_evt)
GPIO_INT(USB_C1_VBUS_WAKE_L,PIN(123), GPIO_INT_BOTH, vbus1_evt)
GPIO_INT(USB_C0_BC12_INT_L, PIN(124), GPIO_INT_FALLING, usb0_evt)
GPIO_INT(USB_C1_BC12_INT_L, PIN(145), GPIO_INT_FALLING, usb1_evt)
#ifdef CONFIG_LOW_POWER_IDLE
GPIO_INT(UART0_RX,          PIN(162), GPIO_INT_BOTH_DSLEEP | GPIO_PULL_UP, uart_deepsleep_interrupt) /* UART0 RX input */
#endif
/* Puesdo G3 */
GPIO(G3_SLEEP_EN,           PIN(55),  GPIO_OUT_LOW)

GPIO(PD_RST_L,              PIN(130), GPIO_ODR_HIGH)
GPIO(USB2_OTG_ID,           PIN(13),  GPIO_ODR_HIGH)
/* I2C pins - these will be reconfigured for alternate function below */
GPIO(I2C0_0_SCL,            PIN(15),  GPIO_INPUT)
GPIO(I2C0_0_SDA,            PIN(16),  GPIO_INPUT)
GPIO(I2C0_1_SCL,            PIN(134), GPIO_INPUT)
GPIO(I2C0_1_SDA,            PIN(17),  GPIO_INPUT)
GPIO(I2C1_SCL,              PIN(22),  GPIO_INPUT)
GPIO(I2C1_SDA,              PIN(23),  GPIO_INPUT)
GPIO(I2C2_SCL,              PIN(20),  GPIO_INPUT)
GPIO(I2C2_SDA,              PIN(21),  GPIO_INPUT)
GPIO(I2C3_SCL,              PIN(24),  GPIO_INPUT)
GPIO(I2C3_SDA,              PIN(25),  GPIO_INPUT)
GPIO(PCH_SCI_L,             PIN(26),  GPIO_ODR_HIGH)

GPIO(USB1_ENABLE,           PIN(36),  GPIO_OUT_HIGH)
GPIO(USB2_ENABLE,           PIN(67),  GPIO_OUT_HIGH)
GPIO(ENTERING_RW,           PIN(41),  GPIO_OUT_LOW)
GPIO(PCH_SMI_L,             PIN(44),  GPIO_ODR_HIGH)
GPIO(PCH_PWRBTN_L,          PIN(45),  GPIO_OUT_HIGH)
GPIO(USB_C0_DP_HPD,         PIN(46),  GPIO_OUT_LOW)
GPIO(USB_C1_DP_HPD,         PIN(51),  GPIO_OUT_LOW)
GPIO(CPU_PROCHOT,           PIN(52),  GPIO_OUT_LOW)
GPIO(ENABLE_TOUCHPAD,       PIN(53),  GPIO_OUT_LOW)
GPIO(BAT_PRESENT_L,         PIN(56),  GPIO_INPUT)
GPIO(USB_PD_WAKE,           PIN(60),  GPIO_OUT_LOW)
/* When asserted, ME does not lock security descriptor */
GPIO(PCH_SEC_DISABLE_L,     PIN(65),  GPIO_ODR_HIGH)
GPIO(PCH_WAKE_L,            PIN(66),  GPIO_ODR_HIGH)
GPIO(USB2_OTG_VBUSSENSE,    PIN(105), GPIO_ODR_HIGH)
GPIO(PCH_ACOK,              PIN(110), GPIO_OUT_LOW)
/* Interrupts from accelerometer / gyro -- not yet implemented */
GPIO(LID_ACCEL_INT,         PIN(161), GPIO_INPUT | GPIO_PULL_DOWN)  /* Lid ACCEL sensor interrupt to EC */
GPIO(GYRO_INT,              PIN(127), GPIO_INPUT | GPIO_PULL_DOWN)  /* Lid Gyro sensor interrupt to EC */
GPIO(BASE_ACCEL_INT,        PIN(147), GPIO_INPUT | GPIO_PULL_DOWN)  /* Base ACCEL sensor interrupt to EC */
GPIO(GYRO_DRDY,             PIN(133), GPIO_INPUT | GPIO_PULL_DOWN)  /* Lid Gyro sensor DRDY interrupt to EC */
GPIO(WLAN_OFF_L,            PIN(132), GPIO_OUT_HIGH)
/* RCIN# line to PCH for 8042 emulation */
GPIO(PCH_RCIN_L,            PIN(135), GPIO_ODR_HIGH)
/* Fan PWM output - NC / testing only */
GPIO(NC_136,                PIN(136), GPIO_INPUT | GPIO_PULL_UP)    /* Reserved for KBD Backlight */

/* Bring down all rails but RTC rail */
GPIO(LDO_EN,                PIN(211), GPIO_OUT_LOW)
GPIO(PCH_RSMRST_L,          PIN(143), GPIO_OUT_LOW)
/* prochot input from devices */
GPIO(PLATFORM_EC_PROCHOT,   PIN(151), GPIO_INPUT)
GPIO(USB_C0_5V_EN,          PIN(154), GPIO_OUT_LOW)
GPIO(USB_C1_5V_EN,          PIN(204), GPIO_OUT_LOW)
GPIO(USB_C0_CHARGE_EN_L,    PIN(64),  GPIO_OUT_LOW)
GPIO(USB_C1_CHARGE_EN_L,    PIN(210), GPIO_OUT_LOW)
/* Reserved for USB-A ILIM */
GPIO(USB_ILIM_SEL,          PIN(11),  GPIO_INPUT | GPIO_PULL_UP)
GPIO(FAN_PWR_DIS_L,         PIN(160), GPIO_OUT_HIGH)
GPIO(PCH_RTCRST_L,          PIN(163), GPIO_ODR_HIGH)

GPIO(PMIC_SLP_SUS_L,        PIN(201), GPIO_OUT_LOW)
GPIO(ENABLE_BACKLIGHT,      PIN(202), GPIO_OUT_HIGH)
GPIO(PP3300_WLAN_EN,        PIN(203), GPIO_OUT_HIGH)
GPIO(CHG_1A5_EN,            PIN(157), GPIO_OUT_HIGH)
GPIO(BOARD_VERSION1,        PIN(6),   GPIO_INPUT)
GPIO(BOARD_VERSION2,        PIN(7),   GPIO_INPUT)
GPIO(BOARD_VERSION3,        PIN(10),  GPIO_INPUT)
GPIO(PVT_CS0,               PIN(146), GPIO_ODR_HIGH)
GPIO(SYS_RESET_L,           PIN(121), GPIO_ODR_HIGH)

GPIO(NC_150,                PIN(150), GPIO_INPUT | GPIO_PULL_UP)
GPIO(CHARGE_LED1,           PIN(155), GPIO_OUT_LOW)
GPIO(CHARGE_LED2,           PIN(156), GPIO_OUT_LOW)

/* Alternate functions GPIO definitions */

/* GPIO162(UART_RX),  GPIO165(UART_TX) */
ALTERNATE(PIN_MASK(16, 0x24), 1, MODULE_UART, 0)

/* KB pins */
/* KB ROW - GPIO000-GPIO005 */
ALTERNATE(PIN_MASK(0,  0x3f), 3, MODULE_KEYBOARD_SCAN, GPIO_KB_OUTPUT)
/* KB ROW - GPIO100-GPIO104, GPIO106-GPIO107 */
ALTERNATE(PIN_MASK(10, 0xdf), 3, MODULE_KEYBOARD_SCAN, GPIO_KB_OUTPUT)
/* KB COL - GPIO032 */
ALTERNATE(PIN_MASK(3,  0x04), 3, MODULE_KEYBOARD_SCAN, GPIO_KB_INPUT)
/* KB COL - GPIO040, GPIO42-GPIO43 */
ALTERNATE(PIN_MASK(4,  0x0d), 3, MODULE_KEYBOARD_SCAN, GPIO_KB_INPUT)
/* KB COL - GPIO125-GPIO126 */
ALTERNATE(PIN_MASK(12, 0x60), 2, MODULE_KEYBOARD_SCAN, GPIO_KB_INPUT)
/* KB COL - GPIO142, GPIO144 */
ALTERNATE(PIN_MASK(14, 0x14), 3, MODULE_KEYBOARD_SCAN, GPIO_KB_INPUT)

/* LPC pins */
/* LPC_CLK_RUN_L - GPIO014 */
ALTERNATE(PIN_MASK(1,  0x10), 1, MODULE_LPC, 0)
/* LAD[0:3] - GPIO111-GPIO114, SERIRQ - GPIO115, PCI_CLK - GPIO117 */
ALTERNATE(PIN_MASK(11, 0xbe), 1, MODULE_LPC, 0)
/* LRESET# - GPIO116 */
ALTERNATE(PIN_MASK(11, 0x40), 1, MODULE_LPC, GPIO_INT_BOTH)
/* LFRAME# - GPIO120 */
ALTERNATE(PIN_MASK(12, 0x01), 1, MODULE_LPC, 0)

/* SPI pins */
/* MOSI - GPIO054 */
ALTERNATE(PIN_MASK(5,  0x10), 1, MODULE_SPI, 0)
/* MISO - GPIO164 */
ALTERNATE(PIN_MASK(16, 0x10), 1, MODULE_SPI, 0)
/* PVT_SCLK - GPIO153 */
ALTERNATE(PIN_MASK(15, 0x08), 1, MODULE_SPI, 0)

/* I2C pins */
/* I2C0_0 CLK - GPIO015, I2C0_0 DAT - GPIO016, I2C0_1 DAT - GPIO017 */
ALTERNATE(PIN_MASK(1,  0xe0), 2, MODULE_I2C, GPIO_ODR_HIGH)
/* I2C{1,2,3} CLK / DAT - GPIO020-GPIO025*/
ALTERNATE(PIN_MASK(2,  0x3f), 2, MODULE_I2C, GPIO_ODR_HIGH)
/* I2C0_1 CLK - GPIO134 */
ALTERNATE(PIN_MASK(13, 0x10), 2, MODULE_I2C, GPIO_ODR_HIGH)

/* ADC pins */
/* ADC1 - GPIO057 / PPVAR_BOOSTIN_SENSE */
ALTERNATE(PIN_MASK(5,  0x80), 1, MODULE_ADC, GPIO_ANALOG)
/* ADC3 - GPIO061 / IADP_ACMON_BMON. ADC4 - GPIO062 / PMON_PSYS */
ALTERNATE(PIN_MASK(6,  0x06), 1, MODULE_ADC, GPIO_ANALOG)

/* VCC1_RST# - GPIO131 */
ALTERNATE(PIN_MASK(13, 0x02), 1, MODULE_PMU, 0)
/* nRESET_OUT - GPIO121 */
ALTERNATE(PIN_MASK(12, 0x02), 1, MODULE_PMU, 0)

/* RPM-PWM for FAN */
/* TACH2PWM_OUT - GPIO34 */
ALTERNATE(PIN_MASK(3,  0x10), 3, MODULE_PWM, 0)
/* TACH2PWM_IN - GPIO140 */
ALTERNATE(PIN_MASK(14, 0x01), 3, MODULE_PWM, 0)
