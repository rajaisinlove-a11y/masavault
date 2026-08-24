# Settings, security, and diagnostics

Task 39. This is the desktop *model*, not a painted window.

## Settings

`SettingsModel` stores only non-secret preferences:

- data directory
- theme (`system` / `light` / `dark`)
- max concurrent transfers (1–16)
- diagnostics on/off

Bot tokens, sync passwords, and backup passwords stay out of this struct. Setup still uses **aliases** only (`docs/setup.md`). OS secret storage is a later platform step.

## Security

`tgcloud::security::redact` strips Telegram bot-token shapes (`digits:AA…`) from any string.

Creating or opening a `.link` must show:

> Shared manifests may contain sensitive Telegram access information. Protect the share password.

That warning is `tgcloud::security::link_share_warning()`.

## Diagnostics

`DiagnosticsModel` keeps subsystem lines (`telegram`, `sync`, `transfer`, …). Every line is redacted on insert and again on export. The export is what a user can copy into a bug report.

Never attach raw API bodies to a diagnostic line.
