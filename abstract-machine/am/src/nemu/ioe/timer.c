#include <am.h>
#include <nemu.h>

uint64_t UPTIME;
void __am_timer_init() {
  UPTIME = inl(RTC_ADDR) + 1000000 * inl(RTC_ADDR + 4);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = inl(RTC_ADDR) + 1000000 * inl(RTC_ADDR + 4) - UPTIME;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
  outl(RTC_ADDR, rtc->year);
}
