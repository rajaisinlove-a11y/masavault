# Per-chunk Bot Assignment

Chunk assignment consumes the shared `BotPool` dynamically and records the selected bot alias on each chunk. Assignment stops when no enabled bot is available; the caller owns release/retry policy.
