#include <am.h>
#include <nemu.h>
uint64_t UPTIME;
void __am_timer_init() {
  UPTIME = ((uint64_t)inl(RTC_ADDR) << 32) + inl(RTC_ADDR + 4);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = ((uint64_t)inl(RTC_ADDR) << 32) + inl(RTC_ADDR + 4) - UPTIME;
  // ioe_write(AM_TIMER_RTC, NULL);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
