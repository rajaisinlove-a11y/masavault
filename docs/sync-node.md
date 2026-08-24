# Encrypted Sync Nodes

`sync::Node` models a node ID, `prevId`, timestamp, and operation entries. Nodes serialize as compact JSON and use the sync crypto boundary for encryption. A node is not assumed to contain a complete database; entries are independent operations.
