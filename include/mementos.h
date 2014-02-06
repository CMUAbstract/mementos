/* See license.txt for licensing information. */

#ifndef __NEWMEM_H__
#define __NEWMEM_H__

#ifdef __MSPGCC__
#include <msp430.h>
#elif (defined(__clang__))
# if defined(__MSP430F2131__)
#  include <msp430f2131.h>
# elif defined(__MSP430F2132__)
#  include <msp430f2132.h>
# elif defined(__MSP430F1611__)
#  include <msp430f1611.h>
# elif defined(__MSP430F2618__)
#  include <msp430f2618.h>
# elif defined(__MSP430FR5969__)
#  include <msp430fr5969.h>
# else
#  error Missing or unsupported chip.
# endif
#else
#error Unsupported compiler; use either mspgcc or Clang.
#endif

#define MAINMEM_SEGSIZE 512
#define MEMENTOS_FLASH
#undef  MEMENTOS_FRAM

# if defined(__MSP430F2131__)
#  define FIRST_BUNDLE_SEG 0xFA00u
#  define SECOND_BUNDLE_SEG 0xFC00u
#  define TOPOFSTACK 0x0300
#  define STARTOFDATA 0x0200
# elif defined(__MSP430F2132__)
#  define FIRST_BUNDLE_SEG 0xFA00u
#  define SECOND_BUNDLE_SEG 0xFC00u
#  define TOPOFSTACK 0x0400
#  define STARTOFDATA 0x0200
# elif defined(__MSP430F1611__)
#  define FIRST_BUNDLE_SEG 0xFA00u
#  define SECOND_BUNDLE_SEG 0xFC00u
#  define TOPOFSTACK 0x3900
#  define STARTOFDATA 0x1100
# elif defined(__MSP430F2618__)
#  define FIRST_BUNDLE_SEG 0xFB00
#  define SECOND_BUNDLE_SEG 0xFD00
#  define TOPOFSTACK 0x3100
#  define STARTOFDATA 0x1100
# elif defined(__MSP430FR5969__)
#  define MEMENTOS_FRAM
#  undef MEMENTOS_FLASH
#  undef FIRST_BUNDLE_SEG /* FRAM has no segments */
#  undef SECOND_BUNDLE_SEG
#  undef MAINMEM_SEGSIZE
#  define TOPOFSTACK 0x2400
#  define STARTOFDATA 0x1C00
#  define FRAM_END 0x13FFF
#  define FRAM_FIRST_BUNDLE_SEG (FRAM_END+1 - (2*(TOPOFSTACK+1 - STARTOFDATA)))
#  define FRAM_SECOND_BUNDLE_SEG (FRAM_FIRST_BUNDLE_SEG + (TOPOFSTACK+1 - STARTOFDATA))
#  define ACTIVE_BUNDLE_PTR (FRAM_FIRST_BUNDLE_SEG - (sizeof(unsigned long)))
# else
#  error Unsupported hardware; see include/mementos.h.
# endif

#define RAM_SIZE (TOPOFSTACK+1 - STARTOFDATA)

#ifdef MEMENTOS_HARDWARE
# include "pinDefWISP4.1DL.h"
#endif // MEMENTOS_HARDWARE


#ifndef V_THRESH
/* in Haskell: map (\x -> (x, round $ 65536 * (x/3.0)/2.5)) [1.8,1.9..5.0] */
// #define V_THRESH 15729 // 1.8 volts
// #define V_THRESH 16602 // 1.9 volts
// #define V_THRESH 17476 // 2.0 volts
// #define V_THRESH 18350 // 2.1 volts
// #define V_THRESH 19224 // 2.2 volts
// #define V_THRESH 20098 // 2.3 volts
// #define V_THRESH 20972 // 2.4 volts
// #define V_THRESH 21845 // 2.5 volts
// #define V_THRESH 22719 // 2.6 volts
#define V_THRESH 23593 // 2.7 volts
// #define V_THRESH 24467 // 2.8 volts
// #define V_THRESH 25341 // 2.9 volts
// #define V_THRESH 26214 // 3.0 volts
// #define V_THRESH 27088 // 3.1 volts
// #define V_THRESH 27962 // 3.2 volts
// #define V_THRESH 28836 // 3.3 volts
// #define V_THRESH 29710 // 3.4 volts
// #define V_THRESH 30583 // 3.5 volts
// #define V_THRESH 31457 // 3.6 volts
// #define V_THRESH 32331 // 3.7 volts
// #define V_THRESH 33205 // 3.8 volts
// #define V_THRESH 34079 // 3.9 volts
// #define V_THRESH 34953 // 4.0 volts
// #define V_THRESH 35826 // 4.1 volts
// #define V_THRESH 36700 // 4.2 volts
// #define V_THRESH 37574 // 4.3 volts
// #define V_THRESH 38448 // 4.4 volts
// #define V_THRESH 39322 // 4.5 volts
// #define V_THRESH 40195 // 4.6 volts
// #define V_THRESH 41069 // 4.7 volts
// #define V_THRESH 41943 // 4.8 volts
// #define V_THRESH 42817 // 4.9 volts
// #define V_THRESH 43691 // 5.0 volts
#endif // V_THRESH

/* silly string functions for stringifying #define'd constants -- don't ask */
#define xstr(s) str(s)
#define str(s) #s

/* function prototypes */
void __mementos_checkpoint (void);
void __mementos_restore (unsigned long);
#ifdef MEMENTOS_FRAM
unsigned long __mementos_locate_next_bundle (void);
unsigned long __mementos_find_active_bundle (void);
#else
unsigned int __mementos_locate_next_bundle (unsigned int);
unsigned int __mementos_find_active_bundle (void);
#endif

void __mementos_atboot_cleanup (void);
void __mementos_inactive_cleanup (unsigned int);

#ifdef MEMENTOS_FRAM
void __mementos_fram_clear(unsigned long);
unsigned int __mementos_bundle_in_range (unsigned long);
#endif

#ifdef MEMENTOS_FLASH
void __mementos_erase_segment (unsigned int);
void __mementos_mark_segment_erase (unsigned int);
unsigned int __mementos_segment_is_empty (unsigned int);
unsigned int __mementos_segment_marked_erase (unsigned int);
unsigned int __mementos_bundle_in_range (unsigned int);
#endif

#ifdef MEMENTOS_LOGGING
# define MEMENTOS_STATUS_STARTING_CHECKPOINT 0x1
# define MEMENTOS_STATUS_COMPLETED_CHECKPOINT 0x2
# define MEMENTOS_STATUS_STARTING_RESTORATION 0x4
# define MEMENTOS_STATUS_STARTING_MAIN 0x8
# define MEMENTOS_STATUS_FINDING_BUNDLE 0x9
# define MEMENTOS_STATUS_DONE_FINDING_BUNDLE 0xA
# define MEMENTOS_STATUS_HELLO 0xB
# define MEMENTOS_STATUS_CHECKING_VOLTAGE 0xC
# define MEMENTOS_STATUS_DONE_CHECKING_VOLTAGE 0xD
# define MEMENTOS_STATUS_PROGRAM_COMPLETE 0xE
void __mementos_log_event (unsigned char);
#else
# define __mementos_log_event(x)
#endif // MEMENTOS_LOGGING

#ifdef MEMENTOS_HARDWARE
unsigned int __mementos_voltage_check (void);
#define VOLTAGE_CHECK __mementos_voltage_check()
#else
/* 0x01C0 is an (apparently) unused addr in peripheral memory on MSP430F2132, so
 * our simulator (mspsim) tracks accesses to it and returns energy data when
 * it's read. */
extern volatile unsigned int VOLTAGE_CHECK asm("0x01C0");
#endif // MEMENTOS_HARDWARE

#define MEMENTOS_MAGIC_NUMBER 0xBEADu

#ifdef MEMENTOS_TIMER
void Timer_A (void) __attribute__((interrupt(12))); // 0xFFEC for F1611 XXX
#define TIMER_INTERVAL 20000
#endif

#define MEMREF_UINT(x) (*((unsigned int*)(x)))
#define MEMREF_ULONG(x) (*((unsigned long*)(x)))

#ifdef MEMENTOS_FRAM
#  define REGISTER_BYTES (sizeof(unsigned long)) // bytes in a register
#  define BUNDLE_SIZE_REGISTERS 60 // (15 * REGISTER_BYTES)
#else
#  define REGISTER_BYTES (sizeof(unsigned int))
#  define BUNDLE_SIZE_REGISTERS 30 // (15 * REGISTER_BYTES)
#endif

#define BUNDLE_SIZE_HEADER 2     // stack size (1 byte) + dataseg size (1 byte)
#define BUNDLE_SIZE_MAGIC 2      // 2 bytes for MEMENTOS_MAGIC_NUMBER

// distances from start of bundle...
#define REGISTERS_OFFSET BUNDLE_SIZE_HEADER
#define STACK_OFFSET (BUNDLE_SIZE_HEADER + BUNDLE_SIZE_REGISTERS)

#define ROUND_TO_NEXT_EVEN(x) (((x)+1) & 0xFFFEu)

typedef unsigned char bool;

#endif /* __NEWMEM_H__ */
// vim:ft=c