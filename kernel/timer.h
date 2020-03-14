#ifndef TIMER_H
#define TIMER_H
void timer_wait(unsigned int ticks_to_wait);
unsigned int seconds_to_ticks(unsigned int seconds);
unsigned int microseconds_to_ticks(unsigned int microseconds);
#endif /* TIMER_H */
