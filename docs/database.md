# Local Database Abstraction

Task 05 defines a UI-independent `database::Store` boundary with paginated file listing and CRUD operations. `MemoryStore` is a deterministic test implementation, not the production persistence engine. SQL/SQLCipher-backed storage is implemented in Task 06 and must preserve the same logical fields.
