# Chunk Manifest

The manifest adapter maps one logical `CloudFile` to explicit chunk records. It preserves logical chunk index, total, size, hash, Telegram identifiers, and uploader alias. Serialization is deterministic JSON for the current desktop boundary; exact Android field ordering/encryption remains fixture-gated.
