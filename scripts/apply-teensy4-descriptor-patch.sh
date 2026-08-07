#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
framework_dir="$repo_dir/.pio-core/packages/framework-arduinoteensy"
descriptor="$framework_dir/cores/teensy4/usb_desc.h"
patch_file="$repo_dir/patches/teensy4-cmsis-dap-descriptor.patch"

if [[ ! -f "$descriptor" ]]; then
  echo "Teensy framework is missing. Run .venv/bin/pio pkg install or .venv/bin/pio run first." >&2
  exit 1
fi

if grep -Fq "'C','M','S','I','S','-','D','A','P'" "$descriptor"; then
  echo "Teensy CMSIS-DAP USB descriptor is already patched."
  exit 0
fi

git -C "$repo_dir" apply --check --ignore-space-change --directory=.pio-core/packages/framework-arduinoteensy "$patch_file"
git -C "$repo_dir" apply --ignore-space-change --directory=.pio-core/packages/framework-arduinoteensy "$patch_file"
echo "Patched $descriptor"
