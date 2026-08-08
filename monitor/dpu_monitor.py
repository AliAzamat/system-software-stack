#!/usr/bin/env python3
"""Poll the DPU resource manager and report throughput; alert on the budget."""
import time

DEVICE = "/dev/dpu_resmgr"
THROUGHPUT_FLOOR = 1_000_000   # packets/sec we must stay above

def read_throughput() -> int:
    # The driver's read() returns the current packets/sec as text.
    with open(DEVICE, "r") as f:
        return int(f.read().strip())

def is_degraded(pps: int) -> bool:
    return pps < THROUGHPUT_FLOOR

def main() -> None:
    while True:
        pps = read_throughput()
        status = "DEGRADED" if is_degraded(pps) else "OK"
        print(f"throughput={pps} pps status={status}", flush=True)
        if is_degraded(pps):
            # In production this would page on-call / emit a metric.
            print("ALERT: throughput below floor", flush=True)
        time.sleep(1.0)

if __name__ == "__main__":
    main()
