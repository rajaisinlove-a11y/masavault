# Chunked Download Reconstruction

Reconstruction sorts chunks by their explicit logical index, rejects gaps/duplicates, and concatenates part files directly to disk. Telegram message order is never used. Large objects remain streamed and memory-bounded.
