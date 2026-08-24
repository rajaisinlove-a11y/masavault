# Sync Log Database

`sync::LogStore` provides a thread-safe append-only boundary for recovered sync-log records and pending-log enumeration. Persistent SQL mapping and uploaded-state tracking are deferred to the database/sync integration layers.
