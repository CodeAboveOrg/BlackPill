/*****************************************************************
 * Copyright (c) 2021 CodeAbove.
 * All rights reserved.
 *
 * This software component is licensed by CodeAbove under
 * BSD 3-Clause license, the 'License'. You may not use
 * this file except in compliance with the License. You
 * may obtain a copy of the License at:
 *                  opensource.org/licenses/BSD-3-Clause
 ****************************************************************/
#include "Heartbeat.h"
#include "Drivers/Led/Led.h"

Heartbeat::Heartbeat(Led& led)
: led(led)
{

}

void Heartbeat::Perform(void)
{
    led.Toggle();
}
