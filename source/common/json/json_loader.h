#pragma once

#include <string>

#include "envoy/json/json_object.h"

#include "source/common/common/statusor.h"
#include "source/common/protobuf/protobuf.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

namespace Envoy {
namespace Json {

class Factory {
public:
  /**
   * Constructs a Json Object from a string.
   */
  static absl::StatusOr<ObjectSharedPtr> loadFromString(const std::string& json);

  /**
   * Constructs a Json Object from a Protobuf struct.
   */
  static ObjectSharedPtr loadFromProtobufStruct(const ProtobufWkt::Struct& protobuf_struct);

  /*
   * Serializes a JSON string to a byte vector using the MessagePack serialization format.
   * If the provided JSON string is invalid, an empty vector will be returned.
   * See: https://github.com/msgpack/msgpack/blob/master/spec.md
   */
  static std::vector<uint8_t> jsonToMsgpack(const std::string& json);

  /*
   * Constructs a JSON string from a list of strings.
   */
  static const std::string listAsJsonString(const std::list<std::string>& items);
};

} // namespace Json
} // namespace Envoy
