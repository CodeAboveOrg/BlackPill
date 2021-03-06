/*****************************************************************
 * Copyright (c) 2020 CodeAbove.
 * All rights reserved.
 *
 * This software component is licensed by CodeAbove under
 * BSD 3-Clause license, the 'License'. You may not use
 * this file except in compliance with the License. You
 * may obtain a copy of the License at:
 *                  opensource.org/licenses/BSD-3-Clause
 ****************************************************************/
#include "Flash.h"

void Flash::SetLatency(WaitStates ws)
{
    ACR.bits.LATENCY = ws;

    while(ACR.bits.LATENCY != ws) {}
}