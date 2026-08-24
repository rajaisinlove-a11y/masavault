# Retry and Rate Limits

`retry::Policy` bounds transient retries, uses exponential backoff, and honors a server-provided Retry-After delay when present. Permanent errors and exhausted attempts are not retried. Jitter and persistent rate-limit state belong to transport orchestration.
