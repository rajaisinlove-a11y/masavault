# Transfer Cancellation and Errors

The transfer boundary exposes an atomic cancellation token and stable error classes: offline, timeout, API, rate-limited, permission, corrupt, missing chunk, and cancelled. Network workers should check cancellation between bounded I/O operations; UI code must display the safe message rather than raw stack traces.
