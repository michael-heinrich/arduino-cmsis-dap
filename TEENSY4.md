# Teensy 4.0 CMSIS-DAP adapter

This firmware turns a Teensy 4.0 into a CMSIS-DAP v1 probe using USB Raw HID.
It supports SWD and JTAG; SWD is the normal choice for Cortex-M targets.

## Build and flash

Create the local build environment once:

```bash
uv venv .venv
uv pip install --python .venv/bin/python platformio
```

Build the Teensy 4.0 firmware:

```bash
.venv/bin/pio run -e teensy40
```

If `.pio-core` was removed or PlatformIO refreshed the Teensy framework, apply
the repository-local USB descriptor patch before building:

```bash
scripts/apply-teensy4-descriptor-patch.sh
```

Upload with a connected Teensy 4.0:

```bash
.venv/bin/pio run -e teensy40 -t upload
```

The adapter enumerates as **Teensy CMSIS-DAP**. The patch changes only the
framework copy below this repository's `.pio-core`; it does not modify Arduino,
Teensyduino, or PlatformIO files used by any other repository.

## Wiring

Connect a common ground between Teensy and target. All Teensy 4.0 GPIO is 3.3 V
only: do not connect the debug signals to a 5 V target without level shifting.

| Signal | Teensy 4.0 physical pin number |
| --- | ---: |
| SWDIO | 19 |
| SWCLK | 20 |
| nRESET (optional) | 23 |
| TDO (JTAG only) | 21 |
| TDI (JTAG only) | 22 |
| GND | any GND pin |

For OpenOCD, use a CMSIS-DAP interface configuration, for example:

```bash
openocd -f interface/cmsis-dap.cfg -f target/<target>.cfg
```

Start with a conservative SWD speed (for example 1 MHz) until the wiring and
target connection are verified.
