/*
    This code was written to support the book, "ARM Assembly for Embedded Applications",
    by Daniel W. Lewis. Permission is granted to freely share this software provided
    that this notice is not removed. This software is intended to be used with a run-time
    library adapted by the author from the STM Cube Library for the 32F429IDISCOVERY 
    board and available for download from http://www.engr.scu.edu/~dlewis/book3.
*/

// Maze creation adapted from code written by Jacek Wieczorek

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "library.h"
#include "graphics.h"
#include "touch.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

// Functions to be implemented in assembly
uint32_t __attribute__((weak)) GetNibble(void *nibbles, uint32_t which)
    {
    uint8_t byte ;

    byte = ((uint8_t *) nibbles)[which >> 1] ;
    if ((which & 1) == 1) byte >>= 4 ;
    return (uint32_t) (byte & 0b00001111) ;
    }

void __attribute__((weak)) PutNibble(void *nibbles, uint32_t which, uint32_t value)
    {
    uint8_t *pbyte ;

    pbyte = (uint8_t *) nibbles + (which >> 1) ;

    if ((which & 1) == 1)
        {
        *pbyte &= 0b00001111 ;
        *pbyte |= value << 4 ;      
        }
    else
        {
        *pbyte &= 0b11110000 ;
        *pbyte |= value ;
        }
    }

#pragma GCC pop_options

#define CPU_CLOCK_HZ        168E6           // Clock cycle count speed
#define SENSITIVITY         0.07            // Converts integer samples to degrees/sec
#define TILT_DEGREES        5.0         // Minimum tilt to cause ball to roll

#define DIR_UP              (1 << 0)
#define DIR_DOWN            (1 << 1)
#define DIR_LEFT            (1 << 2)
#define DIR_RIGHT           (1 << 3)
#define DIR_ALL4            (DIR_UP | DIR_DOWN | DIR_LEFT | DIR_RIGHT)
#define MAZE_WALL           (1 << 0)

#define WIN_XMIN            0   // Pixel coordinate of left edge of white display area
#define WIN_YMIN            48  // Pixel coordinate of top edge of white display area

#define WIN_XMAX            239 // Pixel coordinate of right edge of white display area
#define WIN_YMAX            303 // Pixel coordinate of bottom edge of white display area

#define WIN_WIDTH           (WIN_XMAX - WIN_XMIN + 1)
#define WIN_HEIGHT          (WIN_YMAX - WIN_YMIN + 1)

#define CELL_COLS           45  // Number of physical cell columns
#define CELL_ROWS           45  // Number of physical cell rows

#define CELL_PXLS           5   // Number of vertical or horizontal pixels per physical cell
#define CELL_INCR           4   // Number of physical cells per logical col or row

#define GOAL_PIXELS         ((CELL_INCR - 1)*CELL_PXLS)
#define BALL_RADIUS         (GOAL_PIXELS/2)

#define MAZE_WIDTH          (CELL_COLS * CELL_PXLS)
#define MAZE_HEIGHT         (CELL_ROWS * CELL_PXLS)

#define MAZE_XMIN           (WIN_XMIN + (WIN_WIDTH  - MAZE_WIDTH )/2)
#define MAZE_YMIN           (WIN_YMIN + (WIN_HEIGHT - MAZE_HEIGHT)/2 - 8)

#define MAZE_XMAX           (MAZE_XMIN + MAZE_WIDTH  - 1)
#define MAZE_YMAX           (MAZE_YMIN + MAZE_HEIGHT - 1)

#define BALL_XOFF           (MAZE_XMIN + (CELL_INCR/2)*CELL_PXLS + CELL_PXLS/2)
#define BALL_YOFF           (MAZE_YMIN + (CELL_INCR/2)*CELL_PXLS + CELL_PXLS/2)

typedef uint32_t            ROWCOL ;
typedef uint8_t             FLAGS ;
typedef enum {FALSE = 0, TRUE = 1} BOOL ;

#define ROW(rowcol)         ((uint16_t *) &rowcol)[0]
#define COL(rowcol)         ((uint16_t *) &rowcol)[1]

ROWCOL                      parent[CELL_ROWS][CELL_COLS] ;
FLAGS                       flags[(CELL_ROWS*CELL_COLS+1)/2] ;

#define WHICH(row,col)      ((col) + CELL_COLS*(row))
#define COUNT(a)            (sizeof(a)/sizeof(a[0]))

#define SENSITIVITY_250DPS  8.75
#define SENSITIVITY_500DPS  17.50
#define SENSITIVITY_2000DPS 70.00

const int                   GYRO_CTRL_REG1 = 0x20 ;
#define GYRO_DR1_FLAG       (1 << 7)
#define GYRO_DR0_FLAG       (1 << 6)
#define GYRO_BW1_FLAG       (1 << 5)
#define GYRO_BW0_FLAG       (1 << 4)
#define GYRO_PD_FLAG        (1 << 3)
#define GYRO_ZEN_FLAG       (1 << 2)
#define GYRO_YEN_FLAG       (1 << 1)
#define GYRO_XEN_FLAG       (1 << 0)

const int                   GYRO_CTRL_REG2 = 0x21 ;

const int                   GYRO_CTRL_REG3 = 0x22 ;

const int                   GYRO_CTRL_REG4 = 0x23 ;
#define GYRO_BDU_FLAG       (1 << 7)
#define GYRO_FS1_FLAG       (1 << 5)
#define GYRO_FS0_FLAG       (1 << 4)

const int                   GYRO_CTRL_REG5 = 0x24 ;
#define GYRO_OSEL1_FLAG     (1 << 1)
#define GYRO_HPEN_FLAG      (1 << 4)

const int                   GYRO_STAT_REG = 0x27 ;
#define GYRO_ZYXDA_FLAG     (1 << 3)

const int                   GYRO_DATA_REG = 0x28 ;

typedef struct
    {
    const uint8_t *         table ;
    const uint16_t          Width ;
    const uint16_t          Height ;
    } sFONT;

static int                  Blocked(int x, int y, int dx, int dy) ;
static void                 CalibrateGyroscope(float Offsets[]) ;
static ROWCOL               CreatePath(ROWCOL rowcol) ;
static void                 DrawMaze(void) ;
static float                FloatAbsVal(float x) ;
static void                 InitializeGyroscope(void) ;
static void                 InitializeMaze(void) ;
static void                 Instructions(void) ;
static void                 LEDs(int grn_on, int red_on) ;
static void                 MoveBall(float roll, float pitch) ;
static int                  PlayGame(float Offsets[]) ;
static void                 ReadAngularRate(float pfData[]) ;
static int                  SanityChecksOK(void) ;
static void                 SetFontSize(sFONT *Font) ;

// Library function prototypes for Gyro sensor (not included in library.h)
extern void                 GYRO_IO_Init(void) ;
extern void                 GYRO_IO_Write(uint8_t* data, uint8_t port, uint16_t bytes) ;
extern void                 GYRO_IO_Read(uint8_t* data, uint8_t port, uint16_t bytes) ;

extern sFONT                Font8, Font12, Font16, Font20, Font24 ;

static int                  x_ball, y_ball ;

int main(void)
    {
    float bias[3] ;

    InitializeHardware(HEADER, "Lab 7D: Mazes & Gyroscopes") ;
    InitializeGyroscope() ;
    CalibrateGyroscope(bias) ;
    SanityChecksOK() ;
    while (1)
        {
        InitializeMaze() ;
        DrawMaze() ;
        PlayGame(bias) ;
        }
    }

// Setup crucial cells
static void InitializeMaze(void)
    {
    ROWCOL frst, last ;
    int row, col ;

    memset(parent, 0, sizeof(parent)) ;
    memset(flags,  0, sizeof(flags)) ;

    for (row = 0; row < CELL_ROWS; row += CELL_INCR)
        {
        for (col = 0; col < CELL_COLS; col++)
            {
            PutNibble(flags, WHICH(row, col), MAZE_WALL) ;
            }
        }

    for (col = 0; col < CELL_COLS; col += CELL_INCR)
        {
        for (row = 0; row < CELL_ROWS; row++)
            {
            PutNibble(flags, WHICH(row, col), MAZE_WALL) ;
            }
        }

    for (row = 2; row < CELL_ROWS; row += CELL_INCR)
        {
        for (col = 2; col < CELL_COLS; col += CELL_INCR)
            {
            PutNibble(flags, WHICH(row, col), DIR_ALL4) ;
            }
        }

    // Setup starting cell
    ROW(frst) = COL(frst) = CELL_INCR/2 ;
    parent[CELL_INCR/2][CELL_INCR/2] = frst ;
    
    // Connect cells until start cell is reached and can't be left
    last = frst ;
    do
        {
        last = CreatePath(last) ;
        } while (last != frst) ;
    }

// Connects cell to random neighbor (if possible) and returns
// address of next cell that should be visited
static ROWCOL CreatePath(ROWCOL rowcol)
    {
    int which = WHICH(ROW(rowcol), COL(rowcol)) ;
    uint32_t nibble ;

    // While there are directions still unexplored
    while (((nibble = GetNibble(flags, which)) & DIR_ALL4) != 0)
        {
        int next_row, next_col, dir ;

        // Randomly pick one direction
        dir = 1 << (GetRandomNumber() % 4) ;
        
        // If it has already been explored - try again
        if ((nibble & dir) == 0) continue ;
        
        // Mark direction as explored
        PutNibble(flags, which, nibble & ~dir) ;
        
        // Depending on chosen direction
        next_row = ROW(rowcol) ;
        next_col = COL(rowcol) ;
        switch (dir)
            {
            // Check if it's possible to go right
            case DIR_RIGHT:
                if (COL(rowcol) + CELL_INCR < CELL_COLS) next_col = COL(rowcol) + CELL_INCR ;
                else continue ;
                break ;
            
            // Check if it's possible to go down
            case DIR_DOWN:
                if (ROW(rowcol) + CELL_INCR < CELL_ROWS) next_row = ROW(rowcol) + CELL_INCR ;
                else continue ;
                break ;
            
            // Check if it's possible to go left    
            case DIR_LEFT:
                if (COL(rowcol) >= CELL_INCR) next_col = COL(rowcol) - CELL_INCR ;
                else continue ;
                break ;
            
            // Check if it's possible to go up
            case DIR_UP:
                if (ROW(rowcol) >= CELL_INCR) next_row = ROW(rowcol) - CELL_INCR ;
                else continue ;
                break ;
            }
        
        // If destination is a linked cell already - abort
        if (parent[next_row][next_col] != 0) continue ;
            
        // Otherwise, adopt cell
        parent[next_row][next_col] = rowcol ;
            
        // Remove wall between cells
        if (next_row != ROW(rowcol))
            {
            int row, col, k ;

            row = ROW(rowcol) + (next_row - ROW(rowcol)) / 2 ;
            col = COL(rowcol) - (CELL_INCR/2 - 1) ;
            for (k = 0; k < CELL_INCR - 1; k++, col++)
                {
                PutNibble(flags, WHICH(row, col), 0) ;
                }
            }

        if (next_col != COL(rowcol))
            {
            int row, col, k ;

            row = ROW(rowcol) - (CELL_INCR/2 - 1) ;
            col = COL(rowcol) + (next_col - COL(rowcol)) / 2 ;
            for (k = 0; k < CELL_INCR - 1; k++, row++)
                {
                which = col + CELL_COLS*row ;
                PutNibble(flags, WHICH(row, col), 0) ;
                }
            }
            
        // Return address of the child cell
        ROW(rowcol) = next_row ;
        COL(rowcol) = next_col ;
        return rowcol ;
        }
    
    // If nothing more can be done here - return parent's address
    return parent[ROW(rowcol)][COL(rowcol)] ;
    }

static void DrawMaze(void)
    {
    int row, col, x, y ;

    ClearDisplay() ;
    SetColor(COLOR_BLACK) ;
    y = MAZE_YMIN ;
    for (row = 0; row < CELL_ROWS; row++)
        {
        x = MAZE_XMIN ;
        for (col = 0; col < CELL_COLS; col++)
            {
            if ((GetNibble(flags, WHICH(row, col)) & MAZE_WALL) != 0)
                {
                FillRect(x, y, CELL_PXLS, CELL_PXLS) ;
                }
            x += CELL_PXLS ;
            }
        y += CELL_PXLS ;
        }

    // Mark the start ...
    SetColor(COLOR_RED) ;
    FillCircle(BALL_XOFF, BALL_YOFF, BALL_RADIUS) ;

    // and destination
    SetColor(COLOR_GREEN) ;
    x = MAZE_XMIN + CELL_PXLS*CELL_COLS - CELL_INCR*CELL_PXLS ;
    y = MAZE_YMIN + CELL_PXLS*CELL_ROWS - CELL_INCR*CELL_PXLS ;
    FillRect(x, y, GOAL_PIXELS, GOAL_PIXELS) ;
    }

static int PlayGame(float bias[])
    {
    const float ball_sec = 0.035 ;
    const uint32_t ball_period = (uint32_t) (CPU_CLOCK_HZ * ball_sec + 0.5) ;
    const float text_sec = 0.250 ;
    const uint32_t text_period = (uint32_t) (CPU_CLOCK_HZ * text_sec + 0.5) ;
    const uint32_t samples_per_sec = 760 ;
    const float gyro_period = 1.0 / samples_per_sec ;
    float degr_roll, degr_ptch, dps[3] ;
    uint32_t now, text_timeout, ball_timeout ;
    uint8_t sts ;

    SetFontSize(&Font12) ;
    x_ball = y_ball = 0 ;
    degr_roll = degr_ptch = 0.0 ;
    text_timeout = now = GetClockCycleCount() ;
    ball_timeout = now + ball_period ;
    while (1)
        {
        char text[100] ;

        GYRO_IO_Read(&sts, GYRO_STAT_REG, sizeof(sts)) ;
        if ((sts & GYRO_ZYXDA_FLAG) != 0)
            {
            ReadAngularRate(dps) ;

            dps[0] -= bias[0] ;
            dps[1] -= bias[1] ;
            dps[2] -= bias[2] ;

            // Integrate to get angular position
            degr_ptch += gyro_period * dps[0] ;
            degr_roll += gyro_period * dps[1] ;
            }

        if ((int) (ball_timeout - GetClockCycleCount()) <= 0)
            {
            MoveBall(degr_roll, degr_ptch) ;
            ball_timeout += ball_period ;
            }

        if ((int) (text_timeout - GetClockCycleCount()) <= 0)
            {
            SetColor(COLOR_BLACK) ;
            sprintf(text, "Pitch: %4d", (int) degr_ptch) ;
            DisplayStringAt(MAZE_XMIN, WIN_YMAX - 16, text) ;
            sprintf(text, "Roll: %4d", (int) degr_roll) ;
            DisplayStringAt(MAZE_XMAX - 7*strlen(text), WIN_YMAX - 16, text) ;
            text_timeout += text_period ;
            }

        // Reset roll and pitch if user presses blue pushbutton
        if (PushButtonPressed())
            {
            WaitForPushButton() ;
            break ;
            }
        }

    return 0 ;
    }

static void MoveBall(float roll, float pitch)
    {
    int oldx = x_ball ;
    int oldy = y_ball ;

    if (FloatAbsVal(roll) > TILT_DEGREES && (y_ball % CELL_INCR) == 0)
        {
        int newx = x_ball + roll/TILT_DEGREES ;

        while (x_ball < newx)
            {
            if (Blocked(x_ball, y_ball, +CELL_INCR/2, 0)) break ; 
            if (x_ball + 1 > newx) break ;
            x_ball++ ;
            }

        while (x_ball > newx)
            {
            if (Blocked(x_ball, y_ball, -CELL_INCR/2, 0)) break ;
            if (x_ball - 1 < newx) break ;
            x_ball-- ;
            }
        }

    if (FloatAbsVal(pitch) > TILT_DEGREES && (x_ball % CELL_INCR) == 0)
        {
        int newy = y_ball + pitch/TILT_DEGREES ;

        while (y_ball < newy)
            {
            if (Blocked(x_ball, y_ball, 0, +CELL_INCR/2)) break ;
            if (y_ball + 1 > newy) break ;
            y_ball++ ;
            }

        while (y_ball > newy)
            {
            if (Blocked(x_ball, y_ball, 0, -CELL_INCR/2)) break ;
            if (y_ball - 1 < newy) break ;
            y_ball-- ;
            }
        }

    if (x_ball == oldx && y_ball == oldy) return ;

    // Erase old ball position
    SetColor(COLOR_WHITE) ;
    FillCircle(CELL_PXLS*oldx + BALL_XOFF, CELL_PXLS*oldy + BALL_YOFF, BALL_RADIUS) ;

    // Paint ball at new position
    SetColor(COLOR_RED) ;
    FillCircle(CELL_PXLS*x_ball + BALL_XOFF, CELL_PXLS*y_ball + BALL_YOFF, BALL_RADIUS) ;
    }

static int Blocked(int x, int y, int dx, int dy)
    {
    int row = y + CELL_INCR/2 ;
    int col = x + CELL_INCR/2 ;
    return (GetNibble(flags, WHICH(row+dy, col+dx)) & MAZE_WALL) != 0 ;
    }

static int SanityChecksOK(void)
    {
#   define  WORDS(a)    (sizeof(a)/sizeof(a[0]))
#   define  NIBBLES(a)  (4*WORDS(a))
    uint32_t storage[100], index, word, left , bugs ;

    for (int i = 0; i < WORDS(storage); i++) storage[i] = 0 ;

    bugs = 0 ;

    do index = GetRandomNumber() % NIBBLES(storage) ; while (index < 8) ;
    PutNibble(storage, index, 0xF) ;
    word = index / 8 ;
    left  = index % 8 ;
    if (storage[word] != (0xF << 4*left)) bugs |= 0x1 ;
    storage[word] = 0 ;

    do index = GetRandomNumber() % NIBBLES(storage) ; while (index < 8) ;
    word = index / 8 ;
    left  = index % 8 ;
    storage[word] = 0xF << 4*left ;
    if (GetNibble(storage, index) != 0xF) bugs |= 0x2 ;
    storage[word] = 0 ;

    LEDs(!bugs, bugs) ;
    if (!bugs) return 1 ;

    SetForeground(COLOR_WHITE) ;
    SetBackground(COLOR_RED) ;
    if (bugs & 0x1) DisplayStringAt(5, 50, (uint8_t *) " Bad Function PutNibble\n") ;
    if (bugs & 0x2) DisplayStringAt(5, 70, (uint8_t *) " Bad Function GetNibble\n") ;
    return 0 ;
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

static void InitializeGyroscope(void)
    {
    uint8_t tmpreg ;

    GYRO_IO_Init() ;

    // Enable full scale = 250 dps
    tmpreg = 0x00 ;
    GYRO_IO_Write(&tmpreg, GYRO_CTRL_REG4, sizeof(tmpreg)) ;

    // Enable X, Y and Z channels; 760 Hz ODR; Normal Mode
    tmpreg = GYRO_DR1_FLAG|GYRO_DR0_FLAG|GYRO_PD_FLAG|GYRO_XEN_FLAG|GYRO_YEN_FLAG|GYRO_ZEN_FLAG ;
    GYRO_IO_Write(&tmpreg, GYRO_CTRL_REG1, sizeof(tmpreg)) ;

    tmpreg = 0x10 ;
    GYRO_IO_Write(&tmpreg, GYRO_CTRL_REG5, sizeof(tmpreg));
    }

static void Instructions(void)
    {
    static char *msg[] =
        {
        "CALIBRATING GYRO",
        "----------------",
        "",
        "Place board on a",
        "level surface and",
        "press blue button",
        "",
        "Do not move board",
        "until maze appears"
        } ;
    uint32_t y ;

    SetFontSize(&Font16) ;
    y = 100 ;
    for (int line = 0; line < COUNT(msg); line++)
        {
        DisplayStringAt(20, y, (uint8_t *) msg[line]) ;
        y += Font16.Height ;
        }
    }

static void CalibrateGyroscope(float bias[])
    {
    float X_Bias, Y_Bias, Z_Bias ;
    const uint32_t samples = 1000 ;
    uint8_t sts ;

    Instructions() ;
    WaitForPushButton() ;

    X_Bias = Y_Bias = Z_Bias = 0.0 ;
    for (int i = 0; i < samples; i++)
        {
        do GYRO_IO_Read(&sts, GYRO_STAT_REG, sizeof(sts)) ;
        while ((sts & GYRO_ZYXDA_FLAG) == 0) ;
        ReadAngularRate(bias) ;
        X_Bias += bias[0] ;
        Y_Bias += bias[1] ;
        Z_Bias += bias[2] ;
        }

    /* Get offset value on X, Y and Z */
    bias[0] = X_Bias / samples ;
    bias[1] = Y_Bias / samples ;
    bias[2] = Z_Bias / samples ;
    }

static void ReadAngularRate(float pfData[])
    {
    const float sensitivity = SENSITIVITY_250DPS / 1000.0 ;
    int16_t tmpbuffer[3] ;
  
    GYRO_IO_Read((uint8_t *) tmpbuffer, GYRO_DATA_REG, 6) ;
    for (int i = 0; i < COUNT(tmpbuffer); i++)
        {
        *pfData++ = sensitivity * tmpbuffer[i]  ;
        }
    }

static float FloatAbsVal(float x)
    {
    asm
        (
        "VABS.F32   %[fp_reg],%[fp_reg]"
        : [fp_reg]  "+w"    (x)
        ) ;

    return x ;
    }

static void SetFontSize(sFONT *Font)
    {
    extern void BSP_LCD_SetFont(sFONT *) ;
    BSP_LCD_SetFont(Font) ;
    }

