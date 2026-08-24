# Direct Upload

`BotClient::send_document` performs a multipart Bot API `sendDocument` request with a chat ID, local file path, and optional caption. libcurl streams the file from disk; it is not loaded into RAM. The returned response preserves HTTP status and response body for the later structured API/error layer.
