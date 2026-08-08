# DPU stack runbook

## Boot health (serial console on ttyAMA0)
- Healthy boot shows: `DRAM up` → `kernel verified; jumping` → kernel banner.
- Halt at `FATAL: DRAM init failed`: firmware fail-stop — the board has a memory
  fault. RMA it; there is no software fix.
- Halt at `SECURE BOOT: kernel signature INVALID`: the bootloader refused an
  unsigned/tampered kernel. Re-sign with the correct key and rebuild — never
  disable verification to "get it booting."

## Runtime health (the resource manager)
- `dmesg | grep dpu_resmgr` shows queue count and major number on load.
- `-EBUSY` from DPU_ALLOC_QUEUE: all hardware queues are allocated. A client
  leaked a queue (took one without DPU_FREE_QUEUE). Audit clients.
- `-EINVAL` from DPU_FREE_QUEUE: a client passed an out-of-range queue id — a
  client bug the driver correctly refused. Fix the caller.
- The whole manager freezes, every client stuck: a handler returned while holding
  `rm_lock` (a missed unlock = deadlock). Audit the lock/unlock paths.

## Telemetry / SLA (the monitor)
- `dpu_monitor.py` prints throughput every second; `DEGRADED`/`ALERT` means
  packets/sec fell below the floor — check engine temperature and queue saturation.
- Monitor can't read `/dev/dpu_resmgr`: the module isn't loaded, or read() returned
  EFAULT. Check `dmesg` and that the device node exists.

## Deploy
- CI must be green: Build, Test, AND Verify signature all passed.
- Never deploy an artifact that skipped the `Verify signature` stage.
- Roll back by re-deploying the previous *signed* artifact; never hand-patch on device.

## Golden rules
- Never `rmmod -f` the resource manager while clients hold queues — undefined state.
- Never disable `secure_boot = enforce` in production to work around a signing issue.
- Never deploy a build that wasn't produced in the pinned container.
