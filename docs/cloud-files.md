# `cloud_files` Compatibility Layer

`compat::CloudFiles` preserves the logical cloud-file boundary and supports upsert by the recovered logical UUID plus lookup by Telegram file ID. It delegates persistence to the database abstraction, keeping UI and transport independent. Full Android schema migration remains a later compatibility task.
