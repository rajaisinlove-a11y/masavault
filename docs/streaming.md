# Chunk-aware Media Streaming

The range planner converts a requested byte offset and length into only the required logical chunks and intra-chunk ranges. It supports seeking and bounded buffering without loading the complete media object into memory. Telegram fetch and media-player adapters remain separate.
