/*
 * Copyright (C) 2018 Burak Akguel, Christian Gesse, Fabian Ruhland, Filip Krakowski, Michael Schoettner
 * Heinrich-Heine University
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <cstdint>
#include "lib/libc/sprintf.h"
#include "lib/StrFormat.h"

static void _putc(char * strout, const char & character, int & n) {
    strout[n++] = character;
    strout[n] = '\0';
}

static void _puts(char* strout, const char* str, int & n) {
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        strout[n++] = str[i];
    }

    strout[n] = '\0';
}

static int _vprintf(char* strout, const char* format, int & n, va_list &args) {
    const char* traverse;
    uint32_t uint;
    int32_t sint;
    char *s;
    unsigned char padding = 0;
    
    for(traverse = format; *traverse != '\0'; traverse++) {

        while( *traverse != '%' ) {

            if (*traverse == '\0') return n;

            _putc(strout, *traverse, n);
            traverse++;
        }

        traverse++;

        if (*traverse == '0') {
            traverse++;
            padding = (unsigned char) *traverse - '0';
            traverse++;
        }

        switch (*traverse) {
            case 'c' :
                sint = va_arg(args, int);
                _putc(strout, (char) sint, n);
                break;
            case 'd' :
                sint = va_arg(args, int32_t);
                if (sint < 0) {
                    uint = (uint32_t) -sint;
                    _putc(strout, '-', n);
                } else {
                    uint = (uint32_t) sint;
                }
                _puts(strout, StrFormat::convert(uint, 10, padding), n);
                break;
            case 'u' :
                uint = va_arg(args, uint32_t);
                _puts(strout, StrFormat::convert(uint, 10, padding), n);
                break;
            case 'o':
                uint = va_arg(args, uint32_t);
                _puts(strout, StrFormat::convert(uint, 8, padding), n);
                break;
            case 's':
                s = va_arg(args, char*);
                _puts(strout, s, n);
                break;
            case 'x':
                uint = va_arg(args, uint32_t);
                _puts(strout, StrFormat::convert(uint, 16, padding), n);
                break;
            case 'b':
                uint = va_arg(args, uint32_t);
                _puts(strout, StrFormat::convert(uint, 2, padding), n);
                break;
            default:
                break;
        }
    }
    return n;
}

int sprintf ( char * str, const char * format, ... ) {
    int n = 0;
    va_list args;
    va_start(args, format);
    n = sprintf(str, format, args);
    va_end(args);
    return n;
}

int sprintf ( char * str, const char * format, va_list &args ) {
    int n = 0;
    _vprintf(str, format, n, args);
    return n;
}
