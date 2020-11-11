#include <am.h>
#include <nemu.h>
#include <stdio.h>
uint64_t UPTIME;
void __am_timer_init() {
  UPTIME = inl(RTC_ADDR) + inl(RTC_ADDR + 4) * 1000000;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = inl(RTC_ADDR) + inl(RTC_ADDR + 4) * 1000000 - UPTIME;
  // ioe_write(AM_TIMER_RTC, NULL);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  printf("abcd\n");
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
