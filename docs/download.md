# Download Engine

The Telegram client now exposes `getFile` resolution and a disk-streamed `download_to` operation. `Downloader` keeps transfer I/O outside the UI and never loads the remote response into memory. URL/file-ID resolution and chunk reconstruction are separate tasks.
