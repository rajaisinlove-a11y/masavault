#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
namespace tgcloud::model {
using Id = std::int64_t;
struct ChunkRecord { std::uint32_t index{}; std::uint32_t total{}; std::uint64_t size{}; std::string sha256; Id telegram_message_id{}; std::string telegram_file_id; std::string telegram_file_unique_id; std::string uploader_alias; };
struct CloudFile { Id local_id{}; std::string file_uuid; std::string name; std::string mime_type; std::uint64_t size_bytes{}; std::int64_t uploaded_at{}; std::string checksum; Id telegram_message_id{}; std::string telegram_file_id; std::string telegram_file_unique_id; std::vector<ChunkRecord> chunks; };
enum class TransferState { queued, active, failed, completed, cancelled };
struct TransferTask { Id local_id{}; std::string path; std::string display_name; std::uint64_t size_bytes{}; TransferState state{TransferState::queued}; std::uint64_t completed_bytes{}; std::string error; std::vector<std::uint32_t> completed_chunks; };
enum class SyncOperation { insert, update, remove };
struct SyncLog { std::string log_id; std::int64_t timestamp{}; std::string device_id; SyncOperation operation{SyncOperation::insert}; std::string table_name; std::string primary_key; std::string data_json; std::optional<std::string> previous_data_json; std::string checksum; };
}
