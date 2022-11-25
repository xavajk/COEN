/*
    This code was written to support the book, "ARM Assembly for Embedded Applications",
    by Daniel W. Lewis. Permission is granted to freely share this software provided
    that this notice is not removed. This software is intended to be used with a run-time
    library adapted by the author from the STM Cube Library for the 32F429IDISCOVERY 
    board and available for download from http://www.engr.scu.edu/~dlewis/book3.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "library.h"
#include "graphics.h"
#include "touch.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

// Functions to be implemented in Assembly:

uint32_t __attribute__((weak)) Time2Msecs(uint32_t hour, uint32_t mins, uint32_t secs)
    {
    return 1000 * (secs + 60*(mins + 60*hour)) ;
    }

void __attribute__((weak)) Msecs2Time(uint32_t msec, uint32_t *hour, uint32_t *mins, uint32_t *secs)
    {
    uint32_t h, m, s = msec / 1000 ;

    *hour = h = s/3600 ;
    s = s - 3600*h ;
    *mins = m = s/60 ;
    *secs = s - 60*m;
    }

#pragma GCC pop_options

// Public fonts defined in run-time library
typedef struct
    {
    const uint8_t *         table ;
    const uint16_t          Width ;
    const uint16_t          Height ;
    } sFONT ;

extern sFONT                Font8, Font12, Font16, Font20, Font24 ;
static char *               months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
static char *               weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"} ;

// Private functions defined in this file
static void                 Delay(uint32_t msec) ;
static void                 DigitalTime(int hour, int mins, int secs) ;
static void                 DrawClockFace(int xctr, int yctr) ;
static void                 DrawClockHand(int hand, float radians, int xctr, int yctr) ;
static void                 DrawInnerFace(char *name, struct tm *tm, int xctr, int yctr) ;
static uint32_t             ElapsedMsec(void) ;
static void                 Error(char *function, char *format, ...) ;
static uint32_t             GetTimeout(uint32_t msec) ;
static void                 LEDs(int grn_on, int red_on) ;
static void                 SanityCheck(void) ;
static void                 SetFontSize(sFONT *Font) ;

#define CPU_CLOCK_SPEED_MHZ 168
#define PI                  3.14159

#define ENTRIES(a)          (sizeof(a)/sizeof(a[0]))

#define HEADER_ROWS         48
#define FOOTER_ROWS         16

#define CLOCK_XCTR          (XPIXELS/2)
#define CLOCK_YCTR          (HEADER_ROWS + (YPIXELS - HEADER_ROWS - FOOTER_ROWS)/2 - 20)
#define CLOCK_RADIUS        85
#define CLOCK_BGND          0xFFFFECB3
#define ERASE_RADIUS        (CLOCK_RADIUS - 23)

#define HAND_SECS           0
#define HAND_MINS           1
#define HAND_HOUR           2

#define ANALOG_FONT         Font16
#define DATE_FONT           Font12
#define DIGITAL_FONT        Font20

int main()
    {
    uint32_t start, msec, hour ;
    struct tm tm ;

    InitializeHardware(NULL, "Lab 8D: Time of Day") ;
    SanityCheck() ;

    memset(&tm, 0, sizeof(tm)) ;
    sscanf(__TIME__, "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec) ;
    sscanf(__DATE__, "%*s %d %d", &tm.tm_mday, &tm.tm_year) ;
    for (tm.tm_mon = 0; tm.tm_mon < 12; tm.tm_mon++)
        {
        if (strncmp(months[tm.tm_mon], __DATE__, 3) == 0) break ;
        }
    tm.tm_year -= 1900 ;
    mktime(&tm) ;

    DrawClockFace(CLOCK_XCTR, CLOCK_YCTR) ;
    start = Time2Msecs(hour = tm.tm_hour, tm.tm_min, tm.tm_sec) ;
    while (1)
        {
        msec = (start + ElapsedMsec()) % (1000*24*60*60) ;
        Msecs2Time(msec, (uint32_t *) &tm.tm_hour, (uint32_t *) &tm.tm_min, (uint32_t *) &tm.tm_sec) ;

        if (tm.tm_hour < hour)
            {
            tm.tm_mday++ ;
            mktime(&tm) ;
            }
        hour = tm.tm_hour ;

        DrawInnerFace("GCC Time", &tm, CLOCK_XCTR, CLOCK_YCTR) ;
        DrawClockHand(HAND_SECS, PI*tm.tm_sec/3.00E1, CLOCK_XCTR, CLOCK_YCTR) ;
        DrawClockHand(HAND_MINS, PI*msec/1.80E6, CLOCK_XCTR, CLOCK_YCTR) ;
        DrawClockHand(HAND_HOUR, PI*msec/2.16E7, CLOCK_XCTR, CLOCK_YCTR) ;

        DigitalTime(tm.tm_hour, tm.tm_min, tm.tm_sec) ;
        Delay(1000) ;
        }

    return 0 ;
    }

static void DrawInnerFace(char *name, struct tm *tm, int xctr, int yctr)
    {
    char *weekday, date[7] ;
    int xpos ;

    SetForeground(COLOR_WHITE) ;
    FillCircle(xctr, yctr, ERASE_RADIUS) ;

    SetForeground(COLOR_BLACK) ;
    SetBackground(COLOR_WHITE) ;

    SetFontSize(&ANALOG_FONT) ;
    xpos = xctr - strlen(name)*ANALOG_FONT.Width/2 ;
    DisplayStringAt(xpos, yctr - 30, name) ;

    SetFontSize(&DATE_FONT) ;
    weekday = weekdays[tm->tm_wday] ;
    xpos = xctr - strlen(weekday)*DATE_FONT.Width/2 ;
    DisplayStringAt(xpos, yctr + 21, weekday) ;

    sprintf(date, "%s %d", months[tm->tm_mon], tm->tm_mday) ;
    xpos = xctr - strlen(date)*DATE_FONT.Width/2 ;
    DisplayStringAt(xctr - strlen(date)*DATE_FONT.Width/2, yctr + 33, date) ;

    // Paint axis of hands
    SetForeground(COLOR_BLACK) ;
    FillCircle(xctr, yctr, 4) ;
    }

static void DigitalTime(int hour, int mins, int secs)
    {
    int xpos, ypos ;
    char digital[14], *ampm ;

    ampm = (hour >= 12) ? "PM" : "AM" ;
    if (hour == 0) hour = 12 ;
    else if (hour > 12) hour -= 12 ;
    sprintf(digital, " %2u:%02u:%02u %s ", (unsigned) hour, (unsigned) mins, (unsigned) secs, ampm) ;
    xpos = (XPIXELS - DIGITAL_FONT.Width*strlen(digital)) / 2 ;
    ypos = (YPIXELS - FOOTER_ROWS - DIGITAL_FONT.Height - 20) ;
    SetFontSize(&DIGITAL_FONT) ;
    DisplayStringAt(xpos, ypos, digital) ;
    }

static void DrawClockFace(int xctr, int yctr)
    {
    sFONT *font = &ANALOG_FONT ;
    struct { int x, y ; } nudge[] =
        {
        {-font->Width,      +2                  },    // 1
        {-font->Width-4,    -font->Height/5     },    // 2
        {-3*font->Width/2,  -font->Height/2     },    // 3
        {-font->Width-3,    -2*font->Height/3   },    // 4
        {-font->Width,      -font->Height+1     },    // 5
        {-font->Width/2,    -font->Height       },    // 6
        {-1,                 -font->Height+1    },    // 7
        {+4,                -2*font->Height/3   },    // 8
        {+font->Width/2,    -font->Height/2,    },    // 9
        {-1,                -font->Height/5     },    // 10
        {-3,                +2                  },    // 11
        {-font->Width,      +4                  }     // 12
        } ;        
    char digits[3] ;
    int hour ;

    SetForeground(COLOR_BLACK) ;
    FillCircle(xctr, yctr, CLOCK_RADIUS+2) ;
    SetForeground(CLOCK_BGND) ;
    FillCircle(xctr, yctr, CLOCK_RADIUS) ;

    SetForeground(COLOR_BLACK) ;
    SetBackground(CLOCK_BGND) ;
    SetFontSize(font) ;
    for (hour = 1; hour <= 12; hour++)
        {
        float radians ;
        int degrees, x, y ;

        degrees = ((360/12) * hour + 270) % 360 ;
        radians = PI*(degrees/180.0) ;
        x = cosf(radians) * CLOCK_RADIUS ;
        y = sinf(radians) * CLOCK_RADIUS ;
        x += nudge[hour-1].x ;
        y += nudge[hour-1].y ;
        sprintf(digits, "%d", hour) ;
        DisplayStringAt(xctr + x, yctr + y, digits) ;
        }
    SetForeground(COLOR_BLACK) ;
    FillCircle(xctr, yctr, ERASE_RADIUS+1) ;
    SetForeground(COLOR_WHITE) ;
    FillCircle(xctr, yctr, ERASE_RADIUS) ;
    }

static void DrawClockHand(int hand, float radians, int xctr, int yctr)
    {
    static int maxrad[] = {ERASE_RADIUS, ERASE_RADIUS-1, 3*ERASE_RADIUS/4} ;

    int x1, x2, x3, y1, y2, y3 ;
    int maxlen = maxrad[hand] ;
    int minlen = maxlen - 12 ;
    float rad1, rad2, rad3, drad ;

    x1 = xctr + maxlen * sinf(radians) ;
    y1 = yctr - maxlen * cosf(radians) ;
    DrawLine(xctr, yctr, x1, y1) ;
    if (hand == HAND_SECS) return ;

    drad = (hand == HAND_MINS) ? 0.122 : 0.163 ;
    
    rad1 = radians - drad ;
    rad2 = radians ;
    rad3 = radians + drad ;

    x1 = 0.5 + xctr + sinf(rad1) * minlen ;
    x2 = 0.5 + xctr + sinf(rad2) * maxlen ;
    x3 = 0.5 + xctr + sinf(rad3) * minlen ;

    y1 = 0.5 + yctr - cosf(rad1) * minlen ;
    y2 = 0.5 + yctr - cosf(rad2) * maxlen ;
    y3 = 0.5 + yctr - cosf(rad3) * minlen ;

    SetForeground(COLOR_BLACK) ;
    FillTriangle(x1, x2, x3, y1, y2, y3) ;
    }

static void SetFontSize(sFONT *Font)
    {
    extern void BSP_LCD_SetFont(sFONT *) ;
    BSP_LCD_SetFont(Font) ;
    }

static uint32_t ElapsedMsec(void)
    {
    static uint32_t prev_tics = 0 ;
    static uint32_t msec = 0 ;
    uint32_t curr_tics ;

    curr_tics = GetClockCycleCount() ;
    msec += (curr_tics - prev_tics) / (1000 * CPU_CLOCK_SPEED_MHZ) ;
    prev_tics = curr_tics ;
    return msec ;
    }

static uint32_t GetTimeout(uint32_t msec)
    {
    uint32_t cycles = 1000 * msec * CPU_CLOCK_SPEED_MHZ ;
    return GetClockCycleCount() + cycles ;
    }

static void Delay(uint32_t msec)
    {
    uint32_t timeout = GetTimeout(msec) ;
    while ((int) (timeout - GetClockCycleCount()) > 0) ;
    }

static void SanityCheck(void)
    {
    uint32_t msec, hour, mins, secs ;
    struct tm tm ;
    time_t now ;

    msec = 1000 * ((GetRandomNumber() % 86400000) / 1000) ;
    Msecs2Time(msec, &hour, &mins, &secs) ;
    now = (time_t) (msec / 1000) ;
    tm = *localtime(&now) ;

    if (hour != tm.tm_hour) Error("Msecs2Time", "hour(%u ms) != %u", msec, tm.tm_hour) ;
    if (mins != tm.tm_min ) Error("Msecs2Time", "mins(%u ms) != %u", msec, tm.tm_min) ;
    if (secs != tm.tm_sec ) Error("Msecs2Time", "secs(%u ms) != %u", msec, tm.tm_sec) ;

    if (Time2Msecs(hour, mins, secs) != msec) Error("Time2Msecs", "%02u:%02u:%02u != %u ms", hour, mins, secs, msec) ;
    LEDs(1, 0) ;
    }

static void Error(char *function, char *format, ...)
    {
#   define GFXROW1              54
#   define GFXROWN              215
#   define GFXROWS              (GFXROWN - GFXROW1 + 1)
#   define MAX(a,b)             ((a) > (b) ? (a) : (b))
#   define ERR_FONT             Font12
#   define ERR_BRDR_COLOR       COLOR_BLACK
#   define ERR_BGND_COLOR       COLOR_RED
#   define ERR_FGND_COLOR       COLOR_WHITE
    uint32_t width, row, col, len1, len2 ;
    char line1[100], line2[100] ;
    va_list args ;

    sprintf(line1, "Function %s:", function) ;
    len1 = strlen(line1) ;

    va_start(args, format) ;
    vsprintf(line2, format, args) ;
    va_end(args) ;
    len2 = strlen(line2) ;

    width = ERR_FONT.Width * (MAX(len1, len2) + 2) ;

    col = (XPIXELS - width) / 2 ;
    row = GFXROW1 + GFXROWS / 2 ;

    SetFontSize(&ERR_FONT) ;
    SetColor(ERR_BGND_COLOR) ;
    FillRect(col + 1, row, width - 1, 3*ERR_FONT.Height) ;
    SetColor(ERR_BRDR_COLOR) ;
    DrawRect(col, row, width, 3*ERR_FONT.Height) ;
    row += ERR_FONT.Height/2 ;

    SetForeground(ERR_FGND_COLOR) ;
    SetBackground(ERR_BGND_COLOR) ;
    DisplayStringAt(col + ERR_FONT.Width, row, line1) ;
    row += ERR_FONT.Height ;
    DisplayStringAt(col + ERR_FONT.Width, row, line2) ;

    LEDs(0, 1) ;
    for (;;) ;
    }

static void LEDs(int grn_on, int red_on)
    {
    static uint32_t * const pGPIOG_MODER    = (uint32_t *) 0x40021800 ;
    static uint32_t * const pGPIOG_ODR      = (uint32_t *) 0x40021814 ;
    
    *pGPIOG_MODER |= (1 << 28) | (1 << 26) ;    // output mode
    *pGPIOG_ODR &= ~(3 << 13) ;         // both off
    *pGPIOG_ODR |= (grn_on ? 1 : 0) << 13 ;
    *pGPIOG_ODR |= (red_on ? 1 : 0) << 14 ;
    }

