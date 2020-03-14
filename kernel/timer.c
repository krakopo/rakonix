/* Programmable Interval Timer (PIT) */

#include "kernel/low_level.h"
#include "kernel/idt.h"

/* Ticks per second */
const unsigned int TICK_HZ = 100;

/*
 * Keep track of the number of ticks the system has been running for.
 * For a 32-bit unsigned int we'll overflow after about 1 year and 4 months.
 * Years before overflow = 0xffffffff / TICK_HZ / 3600 / 24 / 365
 */
unsigned int timer_ticks = 0;

/* Program the PIT to fire at a specific frequency */
void timer_set_freq(unsigned int hz)
{
  unsigned int divisor = 1193180 / hz;     /* Calculate our divisor */
  port_byte_out(0x43, 0x36);               /* Set our command byte 0x36 */
  port_byte_out(0x40, divisor & 0xFF);     /* Set low byte of divisor */
  port_byte_out(0x40, divisor >> 8);       /* Set high byte of divisor */
}

/* Handle an interrupt from the PIT */
void timer_handler(struct isr_params *isrp)
{
  timer_ticks++;
}

/* Install the timer into the IDT */
void timer_install()
{
  timer_set_freq(TICK_HZ);
  irq_install_custom_handler(0, timer_handler);
}

void timer_wait(unsigned int ticks_to_wait)
{
  unsigned int end_wait_ticks = timer_ticks + ticks_to_wait;
  while (timer_ticks < end_wait_ticks);
}

unsigned int seconds_to_ticks(unsigned int seconds)
{
  return seconds * TICK_HZ;
}

unsigned int microseconds_to_ticks(unsigned int microseconds)
{
  return (unsigned int) ((microseconds / 1000000.0) * TICK_HZ);
}
