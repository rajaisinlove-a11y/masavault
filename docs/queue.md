# Upload Queue and Concurrency

`TransferQueue` provides bounded worker concurrency, FIFO job submission, graceful draining, and deterministic shutdown. It is transport-neutral; task state persistence and cancellation are layered on top in later tasks.
