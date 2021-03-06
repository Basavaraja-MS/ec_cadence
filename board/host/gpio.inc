/* -*- mode:c -*-
 *
 * Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

GPIO_INT(LID_OPEN,             PIN(0, 0), GPIO_INT_BOTH, lid_interrupt)
GPIO_INT(POWER_BUTTON_L,       PIN(0, 0), GPIO_INT_BOTH, power_button_interrupt)
GPIO_INT(AC_PRESENT,           PIN(0, 0), GPIO_INT_BOTH, extpower_interrupt)
GPIO_INT(BUTTON_VOLUME_DOWN_L, PIN(0, 0), GPIO_INT_BOTH, button_interrupt)
GPIO_INT(BUTTON_VOLUME_UP,     PIN(0, 0), GPIO_INT_BOTH, button_interrupt)
GPIO_INT(CHARGE_DONE,          PIN(0, 0), GPIO_INT_BOTH, inductive_charging_interrupt)

GPIO(EC_INT_L,             PIN(0, 0), 0)
GPIO(WP,                   PIN(0, 0), 0)
GPIO(ENTERING_RW,          PIN(0, 0), 0)
GPIO(PCH_BKLTEN,           PIN(0, 0), 0)
GPIO(ENABLE_BACKLIGHT,     PIN(0, 0), 0)

/* Inductive charging */
GPIO(CHARGE_EN,            PIN(0, 0), 0)
GPIO(BASE_CHG_VDD_EN,      PIN(0, 0), 0)
