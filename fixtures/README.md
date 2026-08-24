# Test fixtures

These are **synthetic**. They follow the field names recovered from the Android app, but they are not the user's real `.link`, sync-node, or 252-chunk backup.

Real Android artifacts live only in the private `apkforensic` repo and must not be copied here.

| File | What it is |
|---|---|
| `synthetic-share.json` | One logical file, two chunks, two uploaders |
| `synthetic-sync-node.json` | One INSERT into `cloud_files` with a `prevId` |

A 252-chunk manifest is **generated in the test** so the repo stays small.
