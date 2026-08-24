# Desktop File-Browser Integration

Task 34 establishes a UI-independent `tgcloud://` URI boundary for file-browser integration. The architecture is feasible for Thunar, but Thunar itself is not installed in this environment. A production Thunar-visible mount requires either a GVfs backend extension or FUSE mount process; those should be implemented at the desktop platform layer without coupling Telegram logic into widgets.
