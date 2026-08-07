/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#include <RTL.h>
#include <stdint.h>
#include <string.h>

//#include "rl_usb.h"
// #include "version.h"
#include "DAP_config.h"
#include "uart.h"
#include "DAP.h"

// Retrieve the in-memory SWD trace.  Each entry is request, ACK, and the
// little-endian data word observed or written during that transfer.
uint32_t DAP_ProcessVendorCommand(uint8_t *request, uint8_t *response) {
    if (*request == ID_DAP_Vendor0) {
        *response++ = ID_DAP_Vendor0;
        return SWD_TraceRead(response) + 1;
    }

    if (*request == ID_DAP_Vendor1) {
        SWD_TraceClear();
        *response = ID_DAP_Vendor1;
        return 1;
    }

    *response = ID_DAP_Invalid;
    return 1;
}
