# Telegram Bot API Client

Task 08 adds a transport boundary backed by libcurl with timeouts and HTTP status capture for `getMe` and `getChat`. Tokens are held only in memory by `BotClient`; callers must redact response bodies and errors. Multipart uploads, JSON parsing, retries, and channel validation are later tasks.
