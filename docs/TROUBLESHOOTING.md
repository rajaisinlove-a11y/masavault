# Troubleshooting

| Symptom | Likely cause | What to do |
|---|---|---|
| `cmake: command not found` | Toolchain missing | `sudo apt-get install cmake g++ libssl-dev libcurl4-openssl-dev` |
| Configure fails on `transfer_center_test.cpp` | Old clone from before the UI-test restore | `git pull` — those tests live on `main` after `e923b55` |
| `compat_matrix_test` cannot open fixtures | Wrong working directory | CTest sets cwd to the repo root. Run the binary from the repo root if you invoke it by hand |
| Live test prints `SKIP` | `TG_BOT_1` / `TG_CHAT` unset | Expected. Unit/integration tests stay secret-free |
| Live test FAIL | Bot revoked, not in the channel, or network | `getMe` then `getChat` with curl; add the bot as channel admin |
| Encrypted store / sync / `.link` “authentication failed” | Wrong password | Not corruption. Re-enter the password |
| Reconstruct “missing or duplicate chunk” | Gap in indexes or missing `N.part` | Chunk **index** is authoritative, not Telegram message order |
| Token appears in a log | Bug | `tgcloud::security::redact` must wrap every diagnostic string |
| Want a windowed app | No toolkit was chosen | Next work: Qt 6 or GTK4 bound to `desktop/ui/*` models |

Diagnostics export is secret-free by design (`Settings` / `DiagnosticsModel`). If you see a `digits:AA…` token in an export, treat it as a defect.
