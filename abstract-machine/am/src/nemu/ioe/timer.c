#include <am.h>
#include <nemu.h>
uint64_t UPTIME;
void __am_timer_init() {
  UPTIME = inl(0x48) + inl(0x4c) * 1000000;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = inl(0x48) + inl(RTC_ADDR + 0x4c) * 1000000;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
