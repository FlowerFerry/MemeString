#ifndef MEGO_TEST_TIME_EXTRA_H_INCLUDED
#define MEGO_TEST_TIME_EXTRA_H_INCLUDED

#include <mego/util/std/time.h>
#include <minunit.h>

#include <limits.h>
#include <string.h>
#include <time.h>

/* Known reference: 2023-02-28 12:00:00 UTC  ==  1677585600 */
#define REF_TIME   ((mgu_time_t)1677585600)
#define REF_TS     ((mgu_timestamp_t)1677585600000LL)

/* ---- mgu_timestamp_get / mgu_timeval_get ---- */

MU_TEST(ut_timestamp_get_positive)
{
    mgu_timestamp_t ts = mgu_timestamp_get();
    mu_assert(ts > 0, "mgu_timestamp_get should return a positive value");
}

MU_TEST(ut_timeval_get_positive)
{
    mgu_timeval_t tv = mgu_timeval_get();
    mu_assert(tv > 0, "mgu_timeval_get should return a positive value");
}

/* ---- mgu_gmtime_s ---- */

MU_TEST(ut_gmtime_s_known)
{
    struct tm result;
    mgu_time_t t = REF_TIME;
    struct tm *p = mgu_gmtime_s(&t, &result);
    mu_assert(p != NULL, "mgu_gmtime_s should not return NULL");
    mu_assert(result.tm_year == 2023 - 1900, "gmtime_s: year mismatch");
    mu_assert(result.tm_mon  == 2 - 1,       "gmtime_s: month mismatch");
    mu_assert(result.tm_mday == 28,           "gmtime_s: day mismatch");
    mu_assert(result.tm_hour == 12,           "gmtime_s: hour mismatch");
    mu_assert(result.tm_min  == 0,            "gmtime_s: minute mismatch");
    mu_assert(result.tm_sec  == 0,            "gmtime_s: second mismatch");
}

MU_TEST(ut_gmtime_s_null_time)
{
    struct tm result;
    struct tm *p = mgu_gmtime_s(NULL, &result);
    mu_assert(p == NULL, "mgu_gmtime_s(NULL, ...) should return NULL");
}

/* ---- mgu_localtime_s ---- */

MU_TEST(ut_localtime_s_smoke)
{
    struct tm result;
    mgu_time_t t = REF_TIME;
    struct tm *p = mgu_localtime_s(&t, &result);
    mu_assert(p != NULL, "mgu_localtime_s should not return NULL for a valid time");
}

MU_TEST(ut_localtime_s_null_time)
{
    struct tm result;
    struct tm *p = mgu_localtime_s(NULL, &result);
    mu_assert(p == NULL, "mgu_localtime_s(NULL, ...) should return NULL");
}

/* ---- mgu_asctime_s ---- */

MU_TEST(ut_asctime_s_basic)
{
    struct tm gtm = {0};
    char buf[26];
    mgu_time_t t = REF_TIME;
    mgu_gmtime_s(&t, &gtm);
    mgec_t ec = mgu_asctime_s(buf, sizeof(buf), &gtm);
    mu_assert(ec == 0, "mgu_asctime_s should succeed");
    mu_assert(strlen(buf) > 0, "mgu_asctime_s: output should be non-empty");
}

MU_TEST(ut_asctime_s_small_buf)
{
    struct tm gtm = {0};
    char buf[4];
    mgu_time_t t = REF_TIME;
    mgu_gmtime_s(&t, &gtm);
    mgec_t ec = mgu_asctime_s(buf, sizeof(buf), &gtm);
    mu_assert(ec != 0, "mgu_asctime_s with too-small buffer should fail");
}

#if !MG_OS__WIN_AVAIL
MU_TEST(ut_asctime_s_null_buf)
{
    struct tm gtm = {0};
    mgu_time_t t = REF_TIME;
    mgu_gmtime_s(&t, &gtm);
    mgec_t ec = mgu_asctime_s(NULL, 26, &gtm);
    mu_assert(ec != 0, "mgu_asctime_s NULL buf should fail");
}
#endif

/* ---- mgu_ctime_s ---- */

MU_TEST(ut_ctime_s_basic)
{
    char buf[26];
    mgu_time_t t = REF_TIME;
    mgec_t ec = mgu_ctime_s(buf, sizeof(buf), &t);
    mu_assert(ec == 0, "mgu_ctime_s should succeed");
    mu_assert(strlen(buf) > 0, "mgu_ctime_s: output should be non-empty");
}

MU_TEST(ut_ctime_s_small_buf)
{
    char buf[4];
    mgu_time_t t = REF_TIME;
    mgec_t ec = mgu_ctime_s(buf, sizeof(buf), &t);
    mu_assert(ec != 0, "mgu_ctime_s with too-small buffer should fail");
}

#if !MG_OS__WIN_AVAIL
MU_TEST(ut_ctime_s_null_buf)
{
    mgu_time_t t = REF_TIME;
    mgec_t ec = mgu_ctime_s(NULL, 26, &t);
    mu_assert(ec != 0, "mgu_ctime_s NULL buf should fail");
}
#endif

/* ---- timezone functions ---- */

MU_TEST(ut_minute_timezone_range)
{
    int tz = mgu_minute_timezone();
    if (tz == INT_MIN) {
        mu_fail("mgu_minute_timezone returned INT_MIN (system error)");
    }
    mu_assert(tz >= -720 && tz <= 840,
              "mgu_minute_timezone out of valid range [-720, +840]");
}

MU_TEST(ut_timezone_minute_alias)
{
    int a = mgu_minute_timezone();
    int b = mgu_timezone_minute();
    mu_assert(a == b, "mgu_timezone_minute should equal mgu_minute_timezone");
}

MU_TEST(ut_hour_timezone_derived)
{
    int min_tz  = mgu_minute_timezone();
    int hour_tz = mgu_hour_timezone();
    if (min_tz == INT_MIN) {
        mu_assert(hour_tz == INT_MIN,
                  "mgu_hour_timezone should be INT_MIN when minute_timezone is INT_MIN");
        return;
    }
    mu_assert(hour_tz == min_tz / 60,
              "mgu_hour_timezone should equal mgu_minute_timezone / 60");
}

MU_TEST(ut_timezone_hour_alias)
{
    mu_assert(mgu_timezone_hour() == mgu_hour_timezone(),
              "mgu_timezone_hour should equal mgu_hour_timezone");
}

/* ---- time / timestamp conversion ---- */

MU_TEST(ut_time_to_ts_roundtrip)
{
    mgu_time_t t = REF_TIME;
    mgu_timestamp_t ts = mgu_time_to_ts(t);
    mu_assert(ts == REF_TS, "mgu_time_to_ts: unexpected value");
    mu_assert(mgu_timestamp_to_time(ts) == t, "mgu_timestamp_to_time roundtrip failed");
}

MU_TEST(ut_timestamp_get_ms)
{
    mu_assert(mgu_timestamp_get_ms(1677585600123LL) == 123,
              "mgu_timestamp_get_ms: expected 123");
    mu_assert(mgu_timestamp_get_ms(0LL) == 0,
              "mgu_timestamp_get_ms(0): expected 0");
    mu_assert(mgu_timestamp_get_ms(999LL) == 999,
              "mgu_timestamp_get_ms(999): expected 999");
    mu_assert(mgu_timestamp_get_ms(1000LL) == 0,
              "mgu_timestamp_get_ms(1000): expected 0");
}

MU_TEST(ut_gmtime_from_timestamp_ms)
{
    struct tm result;
    int ms = -1;
    struct tm *p = mgu_gmtime_from_timestamp(1677585600123LL, &result, &ms);
    mu_assert(p != NULL, "mgu_gmtime_from_timestamp should not return NULL");
    mu_assert(ms == 123, "mgu_gmtime_from_timestamp: ms should be 123");
    mu_assert(result.tm_year == 2023 - 1900, "gmtime_from_timestamp: year mismatch");
    mu_assert(result.tm_mon  == 2 - 1,       "gmtime_from_timestamp: month mismatch");
    mu_assert(result.tm_mday == 28,           "gmtime_from_timestamp: day mismatch");
}

MU_TEST(ut_gmtime_from_ts_alias)
{
    struct tm r1, r2;
    int ms1 = -1, ms2 = -1;
    mgu_gmtime_from_timestamp(REF_TS, &r1, &ms1);
    mgu_gmtime_from_ts(REF_TS, &r2, &ms2);
    mu_assert(r1.tm_year == r2.tm_year &&
              r1.tm_mon  == r2.tm_mon  &&
              r1.tm_mday == r2.tm_mday &&
              r1.tm_hour == r2.tm_hour &&
              ms1 == ms2,
              "mgu_gmtime_from_ts should produce same result as mgu_gmtime_from_timestamp");
}

MU_TEST(ut_timestamp_from_gmtime_roundtrip)
{
    struct tm gtm = {0};
    mgu_time_t t = REF_TIME;
    mgu_gmtime_s(&t, &gtm);
    mgu_timestamp_t ts = mgu_timestamp_from_gmtime(&gtm, 0);
    mu_assert(ts == REF_TS, "mgu_timestamp_from_gmtime roundtrip failed");
}

/* ---- mgu_timestamp_round_to_day ---- */
/*
 * REF_TS = 1677585600000 = 2023-02-28 12:00:00.000 UTC
 * round_down → 2023-02-28 00:00:00 UTC = 1677542400000
 * round_up   → 2023-03-01 00:00:00 UTC = 1677628800000
 */
MU_TEST(ut_timestamp_round_to_day_down)
{
    mgu_timestamp_t result = mgu_timestamp_round_to_day(REF_TS, mgu_round_down);
    mu_assert(result == 1677542400000LL,
              "round_to_day down: expected 1677542400000");
}

MU_TEST(ut_timestamp_round_to_day_up)
{
    mgu_timestamp_t result = mgu_timestamp_round_to_day(REF_TS, mgu_round_up);
    mu_assert(result == 1677628800000LL,
              "round_to_day up: expected 1677628800000");
}

MU_TEST(ut_timestamp_round_to_day_already_midnight)
{
    /* 2023-02-28 00:00:00 UTC = 1677542400 → round_down stays same */
    mgu_timestamp_t ts = 1677542400000LL;
    mgu_timestamp_t result = mgu_timestamp_round_to_day(ts, mgu_round_down);
    mu_assert(result == 1677542400000LL,
              "round_to_day down on midnight should stay same");
}

/* ---- mgu_timestamp_round_to_hour ---- */
/*
 * REF_TS = 1677585600000 = 2023-02-28 12:00:00.000 UTC  (already on the hour)
 * Use    = 1677587820000 = 2023-02-28 12:37:00.000 UTC
 * round_down(1h) → 2023-02-28 12:00:00 = 1677585600000
 * round_up(1h)   → 2023-02-28 13:00:00 = 1677589200000
 * 2023-02-28 12:37:00 UTC = 1677585600 + 37*60 = 1677585600 + 2220 = 1677587820
 */
#define REF_TS_37MIN ((mgu_timestamp_t)1677587820000LL)

MU_TEST(ut_timestamp_round_to_hour_down)
{
    mgu_timestamp_t result = mgu_timestamp_round_to_hour(REF_TS_37MIN, 1, mgu_round_down);
    mu_assert(result == 1677585600000LL,
              "round_to_hour(1) down: expected 1677585600000");
}

MU_TEST(ut_timestamp_round_to_hour_up)
{
    /* interval=1: tm_hour % 1 == 0 always, so round_up has no remainder to act on.
     * The implementation clears min/sec and returns the start of the current hour. */
    mgu_timestamp_t result = mgu_timestamp_round_to_hour(REF_TS_37MIN, 1, mgu_round_up);
    mu_assert(result == 1677585600000LL,
              "round_to_hour(1) up: expected 1677585600000 (interval=1 remainder always 0)");
}

MU_TEST(ut_timestamp_round_to_hour_already_on_hour)
{
    mgu_timestamp_t result = mgu_timestamp_round_to_hour(REF_TS, 1, mgu_round_down);
    mu_assert(result == REF_TS,
              "round_to_hour(1) down on exact hour should stay same");
}

/* ---- mgu_timestamp_round_to_minute ---- */
/*
 * REF_TS_37MIN = 2023-02-28 12:37:00 UTC
 * round_down(5) → 2023-02-28 12:35:00 = 1677585600 + 35*60 = 1677587700 → *1000
 * round_up(5)   → 2023-02-28 12:40:00 = 1677585600 + 40*60 = 1677588000 → *1000
 */
#define REF_TS_35MIN ((mgu_timestamp_t)1677587700000LL)
#define REF_TS_40MIN ((mgu_timestamp_t)1677588000000LL)

MU_TEST(ut_timestamp_round_to_minute_down)
{
    mgu_timestamp_t result = mgu_timestamp_round_to_minute(REF_TS_37MIN, 5, mgu_round_down);
    mu_assert(result == REF_TS_35MIN,
              "round_to_minute(5) down: expected 1677587700000");
}

MU_TEST(ut_timestamp_round_to_minute_up)
{
    mgu_timestamp_t result = mgu_timestamp_round_to_minute(REF_TS_37MIN, 5, mgu_round_up);
    mu_assert(result == REF_TS_40MIN,
              "round_to_minute(5) up: expected 1677588000000");
}

MU_TEST(ut_timestamp_round_to_minute_exact)
{
    /* Already on a 5-minute boundary: round_down stays, round_up stays */
    mgu_timestamp_t result_down = mgu_timestamp_round_to_minute(REF_TS_35MIN, 5, mgu_round_down);
    mgu_timestamp_t result_up   = mgu_timestamp_round_to_minute(REF_TS_35MIN, 5, mgu_round_up);
    mu_assert(result_down == REF_TS_35MIN,
              "round_to_minute(5) down on boundary should stay same");
    mu_assert(result_up == REF_TS_35MIN,
              "round_to_minute(5) up on boundary should stay same");
}

/* ---- mgu_localtime_from_ts alias ---- */

MU_TEST(ut_localtime_from_ts_alias)
{
    struct tm r1, r2;
    int ms1 = -1, ms2 = -1;
    mgu_localtime_from_timestamp(REF_TS, &r1, &ms1);
    mgu_localtime_from_ts(REF_TS, &r2, &ms2);
    mu_assert(
        r1.tm_year == r2.tm_year && r1.tm_mon == r2.tm_mon &&
        r1.tm_mday == r2.tm_mday && r1.tm_hour == r2.tm_hour &&
        ms1 == ms2,
        "mgu_localtime_from_ts should match mgu_localtime_from_timestamp");
}

/* ---- suite ---- */

MU_TEST_SUITE(test_time_extra)
{
    MU_RUN_TEST(ut_timestamp_get_positive);
    MU_RUN_TEST(ut_timeval_get_positive);

    MU_RUN_TEST(ut_gmtime_s_known);
    MU_RUN_TEST(ut_gmtime_s_null_time);
    MU_RUN_TEST(ut_localtime_s_smoke);
    MU_RUN_TEST(ut_localtime_s_null_time);

    MU_RUN_TEST(ut_asctime_s_basic);
    MU_RUN_TEST(ut_asctime_s_small_buf);
#if !MG_OS__WIN_AVAIL
    MU_RUN_TEST(ut_asctime_s_null_buf);
#endif

    MU_RUN_TEST(ut_ctime_s_basic);
    MU_RUN_TEST(ut_ctime_s_small_buf);
#if !MG_OS__WIN_AVAIL
    MU_RUN_TEST(ut_ctime_s_null_buf);
#endif

    MU_RUN_TEST(ut_minute_timezone_range);
    MU_RUN_TEST(ut_timezone_minute_alias);
    MU_RUN_TEST(ut_hour_timezone_derived);
    MU_RUN_TEST(ut_timezone_hour_alias);

    MU_RUN_TEST(ut_time_to_ts_roundtrip);
    MU_RUN_TEST(ut_timestamp_get_ms);
    MU_RUN_TEST(ut_gmtime_from_timestamp_ms);
    MU_RUN_TEST(ut_gmtime_from_ts_alias);
    MU_RUN_TEST(ut_timestamp_from_gmtime_roundtrip);

    MU_RUN_TEST(ut_timestamp_round_to_day_down);
    MU_RUN_TEST(ut_timestamp_round_to_day_up);
    MU_RUN_TEST(ut_timestamp_round_to_day_already_midnight);

    MU_RUN_TEST(ut_timestamp_round_to_hour_down);
    MU_RUN_TEST(ut_timestamp_round_to_hour_up);
    MU_RUN_TEST(ut_timestamp_round_to_hour_already_on_hour);

    MU_RUN_TEST(ut_timestamp_round_to_minute_down);
    MU_RUN_TEST(ut_timestamp_round_to_minute_up);
    MU_RUN_TEST(ut_timestamp_round_to_minute_exact);

    MU_RUN_TEST(ut_localtime_from_ts_alias);
}

#undef REF_TIME
#undef REF_TS
#undef REF_TS_37MIN
#undef REF_TS_35MIN
#undef REF_TS_40MIN

#endif /* MEGO_TEST_TIME_EXTRA_H_INCLUDED */
