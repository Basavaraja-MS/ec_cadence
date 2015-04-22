/* Copyright (c) 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Battery pack vendor provided charging profile
 */

#include "battery.h"
#include "battery_smart.h"
#include "console.h"
#include "extpower.h"
#include "gpio.h"
#include "hooks.h"
#include "host_command.h"
#include "system.h"
#include "util.h"

/* FET ON/OFF cammand write to fet off register */
#define SB_FET_OFF	0x34
#define SB_FETOFF_DATA1	0x0000
#define SB_FETOFF_DATA2	0x1000
#define SB_FETON_DATA1	0x2000
#define SB_FETON_DATA2	0x4000
#define BATTERY_FETOFF	0x0100

static const struct battery_info info = {
	.voltage_max    = 8600,		/* mV */
	.voltage_normal = 7600,
	.voltage_min    = 6000,
	.precharge_current  = 230,	/* mA */
	.start_charging_min_c = 0,
	.start_charging_max_c = 60,
	.charging_min_c       = 0,
	.charging_max_c       = 60,
	.discharging_min_c    = -20,
	.discharging_max_c    = 60,
};

const struct battery_info *battery_get_info(void)
{
	return &info;
}

static void wakeup_deferred(void)
{
	int d;

	sb_read(SB_FET_OFF, &d);

	if (extpower_is_present() && (BATTERY_FETOFF == d)) {
		sb_write(SB_FET_OFF, SB_FETON_DATA1);
		sb_write(SB_FET_OFF, SB_FETON_DATA2);
	}
}
DECLARE_DEFERRED(wakeup_deferred);

static void wakeup(void)
{
	/*
	 * The deferred call ensures that wakeup_deferred is called from a
	 * task. This is required to talk to the battery over I2C.
	 */
	hook_call_deferred(wakeup_deferred, 0);
}
DECLARE_HOOK(HOOK_INIT, wakeup, HOOK_PRIO_DEFAULT);

static int cutoff(void)
{
	int rv;

	/* Ship mode command must be sent twice to take effect */
	rv = sb_write(SB_FET_OFF, SB_FETOFF_DATA1);

	if (rv != EC_SUCCESS)
		return rv;

	return sb_write(SB_FET_OFF, SB_FETOFF_DATA2);
}

int board_cut_off_battery(void)
{
	return cutoff();
}