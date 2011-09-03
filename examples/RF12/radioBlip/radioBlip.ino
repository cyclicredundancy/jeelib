// Send out a radio packet every minute, consuming as little power as possible
// 2010-08-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>

static long payload;

// this must be added since we're using the watchdog for low-power waiting
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
    rf12_initialize(4, RF12_868MHZ, 5);
    rf12_sleep(RF12_SLEEP);
    
    // stay off  3 minutes after reset, to make sure that power is stable
    for (byte i = 0; i < 3; ++i)
        Sleepy::loseSomeTime(60000);
}

void loop() {
    ++payload;

    rf12_sleep(RF12_WAKEUP);
    
    while (!rf12_canSend())
        rf12_recvDone();
    
    rf12_sendStart(0, &payload, sizeof payload);
    // set the sync mode to 2 if the fuses are still the Arduino default
    // mode 3 (full powerdown) can only be used with 258 CK startup fuses
    rf12_sendWait(2);
    
    rf12_sleep(RF12_SLEEP);
    Sleepy::loseSomeTime(60000);
}
