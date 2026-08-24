# Sync Conflict Detection and Merge

Conflicts are detected when two operations target the same table/primary key but carry different payloads. The current deterministic merge policy selects the latest timestamp; equal timestamps prefer the local operation. This policy is explicit and can be replaced when additional Android evidence is recovered.
