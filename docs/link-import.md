# `.link` Import

The import boundary reads an encrypted `.link` from disk, decrypts it with the compatibility crypto parameters, and returns the manifest JSON for validation and subsequent file/chunk download orchestration. No sender SQLite database is required.
