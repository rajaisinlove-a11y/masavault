# Transfer Progress

The progress engine maintains bounded completed/total byte counts and computes instantaneous average throughput and ETA from a monotonic clock. Snapshots are thread-safe and suitable for periodic UI polling.
