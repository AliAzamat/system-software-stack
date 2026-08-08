#!/usr/bin/env bash
# Verify a boot image's signature before it is allowed to deploy. This is the
# pipeline enforcement of the chain of trust declared in boot.cfg and checked by
# the bootloader at power-on — defense in depth.
set -euo pipefail

IMAGE="$1"                 # e.g. build/Image
SIG="$1.sig"               # detached signature alongside the image
PUBKEY="keys/dpu_kernel.pub"

if [[ ! -f "$SIG" ]]; then
    echo "FAIL: no signature for $IMAGE" >&2
    exit 1                  # no signature is an automatic refusal
fi

# openssl returns non-zero if the signature does not match image+key.
if openssl dgst -sha256 -verify "$PUBKEY" -signature "$SIG" "$IMAGE"; then
    echo "OK: $IMAGE signature verified"
else
    echo "FAIL: $IMAGE signature INVALID — refusing to deploy" >&2
    exit 1
fi
