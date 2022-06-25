#include <16F877A.h>
#include <lcd4bit.h>
#device ADC = 16

#FUSES NOWDT
#FUSES NOBROWNOUT
#FUSES NOLVP

#use delay(crystal = 20000000, restart_wdt)
#use i2c(master, fast, sda = pin_c4, scl = pin_c3) // su dung giao tiep i2c cho DS1307
#include <lcd4bit.c>                               // dinh kem file giao tiep LCD che do 4 BIT
#include <dht11.h>
#define k2 pin_A4 // nut nhan set thoi gian
#define k3 pin_A3 // nut nhan tang
#define k4 pin_A2 // nut nhan giam
#define k5 pin_A1 // nut nhan set bao thuc
#define k6 pin_B4 // bam gio

#define coi pin_c0 // ngo ra kich cho chuong bao thuc
#define led pin_a0
#use fast_io(B)

#FUSES HS
#FUSES NOPROTECT
#FUSES NOLVP
