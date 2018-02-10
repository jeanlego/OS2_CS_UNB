// monitor.h -- Defines the interface for monitor.h
//              From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "error.h"
#include "global.h"

// Write a byte out to the specified port.
void outb(unsigned short port, unsigned char value);

// Read a byte out to the specified port.
unsigned char inb(unsigned short port);

// Read a byte out to the specified port.
unsigned short inw(unsigned short port);

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(const char *c);

void monitor_write_dec(unsigned int n);

void monitor_write_hex(unsigned int n);

#endif // MONITOR_H
