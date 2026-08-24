# Compatible Chunk Engine

Compatibility mode uses exactly 4 MiB (`4 * 1024 * 1024`) chunks, including a final short chunk. Input is streamed from disk and each part is hashed with SHA-256. Chunk indexes are zero-based and total count is explicit; empty files produce zero chunks.
