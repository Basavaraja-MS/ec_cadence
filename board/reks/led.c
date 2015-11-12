/* Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Power/Battery LED control for Reks
 */

#include "charge_state.h"
#include "chipset.h"
#include "console.h"
#include "extpower.h"
#include "gpio.h"
#include "hooks.h"
#include "led_common.h"
#include "pwm.h"
#include "registers.h"
#include "util.h"

#define CPRINTF(format, args...) cprintf(CC_PWM, format, ## args)
#define CPRINTS(format, args...) cprints(CC_PWM, format, ## args)

static int led_debug;

const enum ec_led_id supported_led_ids[] = {
	EC_LED_ID_POWER_LED, EC_LED_ID_BATTERY_LED};
const int supported_led_ids_count = ARRAY_SIZE(supported_led_ids);

enum led_color {
	LED_OFF = 0,
	LED_RED,
	LED_AMBER,
	LED_GREEN,

	/* Number of colors, not a color itself */
	LED_COLOR_COUNT
};

/* Brightness vs. color, in the order of off, red, amber, and green */
static const uint8_t color_brightness[LED_COLOR_COUNT][3] = {
	/* {Red, Amber, Green}, */
	[LED_OFF]   = {  0,   0,   0},
	[LED_RED]   = {  0,   0, 100},
	[LED_AMBER] = {  0, 100,   0},
	[LED_GREEN] = {100,   0,   0},
};

/**
 * Set LED color
 *
 * @param color		Enumerated color value
 */
static void set_color(enum led_color color)
{
	pwm_set_duty(PWM_CH_LED_RED, color_brightness[color][0]);
	pwm_set_duty(PWM_CH_LED_BLUE, color_brightness[color][1]);
	pwm_set_duty(PWM_CH_LED_GREEN, color_brightness[color][2]);
}

void led_get_brightness_range(enum ec_led_id led_id, uint8_t *brightness_range)
{
	brightness_range[EC_LED_COLOR_RED] = 100;
	brightness_range[EC_LED_COLOR_BLUE] = 100;
	brightness_range[EC_LED_COLOR_GREEN] = 100;
}

int led_set_brightness(enum ec_led_id led_id, const uint8_t *brightness)
{
	pwm_set_duty(PWM_CH_LED_RED, brightness[EC_LED_COLOR_RED]);
	pwm_set_duty(PWM_CH_LED_BLUE, brightness[EC_LED_COLOR_BLUE]);
	pwm_set_duty(PWM_CH_LED_GREEN, brightness[EC_LED_COLOR_GREEN]);
	return EC_SUCCESS;
}


static void led_init(void)
{
	/* Configure GPIOs */
	gpio_config_module(MODULE_PWM_LED, 1);

	/*
	 * Enable PWMs and set to 0% duty cycle.  If they're disabled,
	 * seems to ground the pins instead of letting them float.
	 */
	pwm_enable(PWM_CH_LED_RED, 1);
	pwm_enable(PWM_CH_LED_GREEN, 1);
	pwm_enable(PWM_CH_LED_BLUE, 1);

	set_color(LED_OFF);
}
DECLARE_HOOK(HOOK_INIT, led_init, HOOK_PRIO_DEFAULT);

/**
 * Called by hook task every 250 ms
 */
static void led_tick(void)
{
	static unsigned ticks;
	int chstate = charge_get_state();

	ticks++;

	/* If we don't control the LED, nothing to do */
	if (!led_auto_control_is_enabled(EC_LED_ID_BATTERY_LED))
		return;

	/* If charging error, blink orange, 25% duty cycle, 4 sec period */
	if (chstate == PWR_STATE_ERROR) {
		set_color((ticks % 16) < 4 ? LED_AMBER : LED_OFF);
		return;
	}

	/* If charge-force-idle, blink green, 50% duty cycle, 2 sec period */
	if (chstate == PWR_STATE_IDLE &&
	   (charge_get_flags() & CHARGE_FLAG_FORCE_IDLE)) {
		set_color((ticks & 0x4) ? LED_GREEN : LED_OFF);
		return;
	}

	/* If the system is charging, solid orange */
	if (chstate == PWR_STATE_CHARGE) {
		set_color(LED_AMBER);
		return;
	}

	/* If AC connected and fully charged (or close to it), solid green */
	if (chstate == PWR_STATE_CHARGE_NEAR_FULL ||
	   chstate == PWR_STATE_IDLE) {
		set_color(LED_GREEN);
		return;
	}

	/* Otherwise, system is off and AC not connected, LED off */
	set_color(LED_OFF);
}
DECLARE_HOOK(HOOK_TICK, led_tick, HOOK_PRIO_DEFAULT);

static void dump_pwm_channels(void)
{
	int ch;
	for (ch = 0; ch < 4; ch++) {
		CPRINTF("channel = %d\n", ch);
		CPRINTF("0x%04X 0x%04X 0x%04X\n",
			MEC1322_PWM_CFG(ch),
			MEC1322_PWM_ON(ch),
			MEC1322_PWM_OFF(ch));
	}
}
/******************************************************************/
/* Console commands */
static int command_led_color(int argc, char **argv)
{
	if (argc > 1) {
		if (!strcasecmp(argv[1], "debug")) {
			led_debug ^= 1;
			CPRINTF("led_debug = %d\n", led_debug);
		} else if (!strcasecmp(argv[1], "off")) {
			set_color(LED_OFF);
		} else if (!strcasecmp(argv[1], "red")) {
			set_color(LED_RED);
		} else if (!strcasecmp(argv[1], "green")) {
			set_color(LED_GREEN);
		} else if (!strcasecmp(argv[1], "amber")) {
			set_color(LED_AMBER);
		} else {
			/* maybe handle charger_discharge_on_ac() too? */
			return EC_ERROR_PARAM1;
		}
	}

	if (led_debug == 1)
		dump_pwm_channels();
	return EC_SUCCESS;
}
DECLARE_CONSOLE_COMMAND(ledcolor, command_led_color,
			"[debug|red|green|amber|off]",
			"Change LED color",
			NULL);
