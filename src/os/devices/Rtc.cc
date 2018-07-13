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

#include <kernel/interrupts/IntDispatcher.h>
#include <kernel/interrupts/Pic.h>
#include <kernel/services/TimeService.h>

Logger &Rtc::log = Logger::get("RTC");

Rtc::Rtc() : registerPort(0x70), dataPort(0x71) {

}

void Rtc::plugin() {
    Cpu::disableInterrupts();

    // Disable NMIs
    uint8_t oldValue = registerPort.inb();
    registerPort.outb(static_cast<uint8_t>(oldValue | 0x80u));

    log.trace("Disabled NMIs");

    // Enable 'Update interrupts': An Interrupt will be triggered after every RTC-update.
    registerPort.outb(STATUS_REGISTER_B);
    oldValue = dataPort.inb();

    registerPort.outb(STATUS_REGISTER_B);
    dataPort.outb(static_cast<uint8_t>(oldValue | 0x10u));

    // Set the periodic interrupt rate.
    registerPort.outb(STATUS_REGISTER_A);
    oldValue = dataPort.inb();

    registerPort.outb(STATUS_REGISTER_A);
    dataPort.outb(static_cast<uint8_t>((oldValue & 0xF0u) | RTC_RATE));

    // Read Register C. This will clear data-flag. As long as this flag is set,
    // the RTC won't trigger any interrupts.
    registerPort.outb(STATUS_REGISTER_C);
    dataPort.inb();

    log.trace("Setup RTC");

    IntDispatcher::getInstance().assign(40, *this);
    Pic::getInstance()->allow(Pic::Interrupt::RTC);

    log.trace("Registered RTC interrupt handler");

    // Enable NMIs
    oldValue = registerPort.inb();
    registerPort.outb(static_cast<uint8_t>(oldValue & 0x7Fu));

    log.trace("Enabled NMIs");

    Cpu::enableInterrupts();
}

void Rtc::trigger() {

    registerPort.outb(STATUS_REGISTER_B);
    char bcd = !(dataPort.inb() & 0x04);

    registerPort.outb(STATUS_REGISTER_B);
    char twelveHour = !(dataPort.inb() & 0x02);

    registerPort.outb(STATUS_REGISTER_C);
    dataPort.inb();

    registerPort.outb(SECONDS_REGISTER);
    currentDate.seconds = dataPort.inb();

    registerPort.outb(MINUTES_REGISTER);
    currentDate.minutes = dataPort.inb();

    registerPort.outb(HOURS_REGISTER);
    currentDate.hours = dataPort.inb();

    registerPort.outb(DAY_OF_MONTH_REGISTER);
    currentDate.dayOfMonth = dataPort.inb();

    registerPort.outb(MONTH_REGISTER);
    currentDate.month = dataPort.inb();

    registerPort.outb(YEAR_REGISTER);
    currentDate.year = dataPort.inb();

    registerPort.outb(CENTURY_REGISTER);
    currentDate.year += dataPort.inb() * 100;

    if(bcd) {
        currentDate.seconds = static_cast<uint8_t>((currentDate.seconds & 0x0F) + ((currentDate.seconds / 16) * 10));
        currentDate.minutes = static_cast<uint8_t>((currentDate.minutes & 0x0F) + ((currentDate.minutes / 16) * 10));
        currentDate.hours = static_cast<uint8_t>(((currentDate.hours & 0x0F) + (((currentDate.hours & 0x70) / 16) * 10)) | (currentDate.hours & 0x80));
        currentDate.dayOfMonth = static_cast<uint8_t>((currentDate.dayOfMonth & 0x0F) + ((currentDate.dayOfMonth / 16) * 10));
        currentDate.month = static_cast<uint8_t>((currentDate.month & 0x0F) + ((currentDate.month / 16) * 10));
        currentDate.year = static_cast<uint16_t>((currentDate.year & 0x0F) + ((currentDate.year / 16) * 10));
    }

    if(twelveHour && (currentDate.hours & 0x80)) {
        currentDate.hours = static_cast<uint8_t>(((currentDate.hours & 0x7F) + 12) % 24);
    }
}

bool Rtc::checkForData() {
    registerPort.outb(STATUS_REGISTER_C);
    return (dataPort.inb() & 0x10) == 0x10;
}

Rtc::Date Rtc::getCurrentDate() {
    return Rtc::currentDate;
}
