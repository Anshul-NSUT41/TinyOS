#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

// Public API
void keyboard_init(void);
char keyboard_getchar(void);
char keyboard_read(void);

#endif