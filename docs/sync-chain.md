# Sync-Chain Traversal

Chain traversal indexes node IDs, follows `prevId` from a discovered head, reverses the result to oldest-first order, and rejects missing links or cycles. Nodes remain operation batches rather than whole-database snapshots.
