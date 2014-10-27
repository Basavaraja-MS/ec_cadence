/* Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
/* tiny substitute of the runtime layer */

#include "clock.h"
#include "common.h"
#include "cpu.h"
#include "debug.h"
#include "registers.h"
#include "task.h"
#include "timer.h"
#include "util.h"

volatile uint32_t last_event;

timestamp_t get_time(void)
{
	timestamp_t t;

	t.le.lo = STM32_TIM32_CNT(2);
	t.le.hi = 0;
	return t;
}

void force_time(timestamp_t ts)
{
	STM32_TIM32_CNT(2) = ts.le.lo;
}

void udelay(unsigned us)
{
	unsigned t0 = STM32_TIM32_CNT(2);
	while ((STM32_TIM32_CNT(2) - t0) < us)
		;
}

void task_enable_irq(int irq)
{
	CPU_NVIC_EN(0) = 1 << irq;
}

void task_disable_irq(int irq)
{
	CPU_NVIC_DIS(0) = 1 << irq;
}

void task_clear_pending_irq(int irq)
{
	CPU_NVIC_UNPEND(0) = 1 << irq;
}

uint32_t task_set_event(task_id_t tskid, uint32_t event, int wait)
{
	last_event = event;

	return 0;
}

void tim2_interrupt(void)
{
	STM32_TIM_DIER(2) = 0; /* disable match interrupt */
	task_clear_pending_irq(STM32_IRQ_TIM2);
	last_event = TASK_EVENT_TIMER;
}
DECLARE_IRQ(STM32_IRQ_TIM2, tim2_interrupt, 1);

static void config_hispeed_clock(void)
{
	/* Ensure that HSI8 is ON */
	if (!(STM32_RCC_CR & (1 << 1))) {
		/* Enable HSI */
		STM32_RCC_CR |= 1 << 0;
		/* Wait for HSI to be ready */
		while (!(STM32_RCC_CR & (1 << 1)))
			;
	}
	/* PLLSRC = HSI, PLLMUL = x12 (x HSI/2) = 48Mhz */
	STM32_RCC_CFGR = 0x00288000;
	/* Enable PLL */
	STM32_RCC_CR |= 1 << 24;
	/* Wait for PLL to be ready */
	while (!(STM32_RCC_CR & (1 << 25)))
			;

	/* switch SYSCLK to PLL */
	STM32_RCC_CFGR = 0x00288002;
	/* wait until the PLL is the clock source */
	while ((STM32_RCC_CFGR & 0xc) != 0x8)
		;
}

void runtime_init(void)
{
	/* put 1 Wait-State for flash access to ensure proper reads at 48Mhz */
	STM32_FLASH_ACR = 0x1001; /* 1 WS / Prefetch enabled */

	config_hispeed_clock();

	rtc_init();
}

/*
 * minimum delay to enter stop mode
 * STOP_MODE_LATENCY: max time to wake up from STOP mode with regulator in low
 * power mode is 5 us + PLL locking time is 200us.
 * SET_RTC_MATCH_DELAY: max time to set RTC match alarm. if we set the alarm
 * in the past, it will never wake up and cause a watchdog.
 */
#define STOP_MODE_LATENCY 300   /* us */
#define SET_RTC_MATCH_DELAY 200 /* us */

uint32_t task_wait_event(int timeout_us)
{
	uint32_t evt;
	timestamp_t t0;
	uint32_t rtc0, rtc0ss, rtc1, rtc1ss;
	int rtc_diff;

	asm volatile("cpsid i");
	/* the event already happened */
	if (last_event || !timeout_us) {
		evt = last_event;
		last_event = 0;

		asm volatile("cpsie i ; isb");
		return evt;
	}

	/* set timeout on timer */
	if (timeout_us < 0) {
		asm volatile ("wfi");
	} else if (timeout_us <= (STOP_MODE_LATENCY + SET_RTC_MATCH_DELAY)) {
		STM32_TIM32_CCR1(2) = STM32_TIM32_CNT(2) + timeout_us;
		STM32_TIM_SR(2) = 0; /* clear match flag */
		STM32_TIM_DIER(2) = 2; /*  match interrupt */

		asm volatile("wfi");

		STM32_TIM_DIER(2) = 0; /* disable match interrupt */
	} else {
		t0 = get_time();

		/* set deep sleep bit */
		CPU_SCB_SYSCTRL |= 0x4;

		set_rtc_alarm(0, timeout_us - STOP_MODE_LATENCY,
			      &rtc0, &rtc0ss);

		asm volatile("wfi");

		CPU_SCB_SYSCTRL &= ~0x4;

		config_hispeed_clock();

		/* fast forward timer according to RTC counter */
		reset_rtc_alarm(&rtc1, &rtc1ss);
		rtc_diff = get_rtc_diff(rtc0, rtc0ss, rtc1, rtc1ss);
		t0.val = t0.val + rtc_diff;
		force_time(t0);
	}

	asm volatile("cpsie i ; isb");

	/* note: interrupt that woke us up will run here */

	evt = last_event;
	last_event = 0;
	return evt;
}

void cpu_reset(void)
{
	/* Disable interrupts */
	asm volatile("cpsid i");
	/* reboot the CPU */
	CPU_NVIC_APINT = 0x05fa0004;
	/* Spin and wait for reboot; should never return */
	while (1)
		;
}

/**
 * Default exception handler, which reports a panic.
 *
 * Declare this as a naked call so we can extract the real LR and SP.
 */
void exception_panic(void) __attribute__((naked));
void exception_panic(void)
{
	asm volatile(
#ifdef CONFIG_DEBUG_PRINTF
		"mov r0, %0\n"
		"mov r3, sp\n"
		"ldr r1, [r3, #6*4]\n" /* retrieve exception PC */
		"ldr r2, [r3, #5*4]\n" /* retrieve exception LR */
		"bl debug_printf\n"
#endif
		"b cpu_reset\n"
	: : "r"("PANIC PC=%08x LR=%08x\n\n"));
}

void panic_reboot(void)
{ /* for div / 0 */
	debug_printf("DIV0 PANIC\n\n");
	cpu_reset();
}

/* --- stubs --- */
void __hw_timer_enable_clock(int n, int enable)
{ /* Done in hardware init */ }

void usleep(unsigned us)
{ /* Used only as a workaround */ }