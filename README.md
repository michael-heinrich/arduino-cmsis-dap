# Arduino CMSIS-DAP

Turn a Teensy 4.0 into a USB CMSIS-DAP v1 debug probe for ARM targets. The
probe supports SWD (the usual Cortex-M debug interface) and JTAG, and works
with OpenOCD and other tools that support CMSIS-DAP.

## What you need

- A Teensy 4.0 and a USB data cable.
- A separately powered 3.3 V ARM target with an SWD or JTAG debug header.
- Jumper wires. Keep SWD wires short, especially `SWCLK`.
- [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html).
- OpenOCD, if you will use OpenOCD to flash or debug the target.

The Teensy is powered from its USB connection. This firmware does **not**
power the target; power the target by its normal supply.

## Build and upload

From the repository root, download the PlatformIO packages:

```bash
pio pkg install -e teensy40
```

The USB product name must contain `CMSIS-DAP` for host tools to recognize the
probe. Apply the supplied patch after the framework package has been installed:

```bash
scripts/apply-teensy4-descriptor-patch.sh
```

Build and upload the firmware:

```bash
pio run -e teensy40
pio run -e teensy40 -t upload
```

If PlatformIO cannot automatically place the Teensy in programming mode, press
its physical **Program** button once while the upload command is waiting. The
probe then enumerates as **Teensy CMSIS-DAP**.

The patch only changes the repository-local `.pio-core` copy of the Teensy
framework. Re-run the patch after deleting `.pio-core` or after a framework
update; it is safe to run when already applied.

### Using an isolated PlatformIO install

If `pio` is not installed globally, one option is to create a project-local
environment with `uv`:

```bash
uv venv .venv
uv pip install --python .venv/bin/python platformio
.venv/bin/pio pkg install -e teensy40
scripts/apply-teensy4-descriptor-patch.sh
.venv/bin/pio run -e teensy40 -t upload
```

## Wiring

Always connect ground first. The Teensy 4.0 GPIO is **3.3 V only**: do not
connect any debug or reset signal to a 5 V target, and use a level shifter for
1.8 V (or other non-3.3 V) targets.

| Target debug signal | Teensy 4.0 pin | Notes |
| --- | ---: | --- |
| GND | Any GND pin | Required common ground |
| SWDIO | 19 | SWD data; JTAG `TMS` |
| SWCLK | 20 | SWD clock; JTAG `TCK` |
| nRESET | 23 | Optional, active-low target reset |
| TDO | 21 | JTAG only |
| TDI | 22 | JTAG only |

For SWD, only `GND`, `SWDIO`, and `SWCLK` are required. Connecting `nRESET`
allows the debugger to reset the target. Do not connect the target's power pin
to a Teensy signal pin. The reset output is driven at 3.3 V, so leave it
disconnected if the target reset line is not 3.3 V compatible.

For a typical 10-pin Cortex Debug connector, wire the connector's `SWDIO`,
`SWCLK`, `nRESET`, and `GND` pins to the corresponding signals above; the
connector's `VTref` indicates the target I/O voltage but is not used by this
firmware.

## Use with OpenOCD

With the target powered and wired, start with SWD at 1 MHz:

```bash
openocd \
  -f interface/cmsis-dap.cfg \
  -c "transport select swd" \
  -c "adapter speed 1000" \
  -f target/<target>.cfg
```

Replace `<target>.cfg` with the OpenOCD target configuration for your chip. For
example, a board using an STM32F4 can use `target/stm32f4x.cfg`. Once the probe
can reliably identify the target, increase `adapter speed` if desired.

If OpenOCD cannot find the probe, disconnect and reconnect the Teensy after
uploading, confirm that its USB name contains `CMSIS-DAP`, and check that the
descriptor patch was applied before the build. If it finds the probe but cannot
communicate with the target, verify the common ground, target power, signal
names, voltage compatibility, and short wiring before increasing clock speed.

## Legacy boards

The original firmware also contains support for Arduino Pro Micro and Teensy
3.2 boards. Their older Arduino IDE-specific instructions remain in
[`arduino-cmsis-dap.ino`](arduino-cmsis-dap.ino).

## License

The CMSIS-DAP firmware is licensed under the Apache License 2.0; see
[`LICENSE`](LICENSE).
