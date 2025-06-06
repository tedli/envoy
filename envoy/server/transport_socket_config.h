#pragma once

#include <string>

#include "envoy/config/core/v3/health_check.pb.h"
#include "envoy/config/typed_config.h"
#include "envoy/event/dispatcher.h"
#include "envoy/init/manager.h"
#include "envoy/local_info/local_info.h"
#include "envoy/network/transport_socket.h"
#include "envoy/secret/secret_manager.h"
#include "envoy/server/factory_context.h"
#include "envoy/singleton/manager.h"
#include "envoy/ssl/context_manager.h"
#include "envoy/stats/scope.h"
#include "envoy/thread_local/thread_local.h"
#include "envoy/upstream/cluster_manager.h"

#include "source/common/protobuf/protobuf.h"

namespace Envoy {
namespace Server {
namespace Configuration {

using TransportSocketFactoryContext = GenericFactoryContext;
using TransportSocketFactoryContextPtr = std::unique_ptr<TransportSocketFactoryContext>;

class TransportSocketConfigFactory : public Config::TypedFactory {
public:
  ~TransportSocketConfigFactory() override = default;
};

/**
 * Implemented by each transport socket used for upstream connections. Registered via class
 * RegisterFactory.
 */
class UpstreamTransportSocketConfigFactory : public virtual TransportSocketConfigFactory {
public:
  /**
   * Create a particular transport socket factory implementation.
   * @param config const Protobuf::Message& supplies the config message for the transport socket
   *        implementation.
   * @param context TransportSocketFactoryContext& supplies the transport socket's context.
   * @return absl::StatusOr<Network::UpstreamTransportSocketFactoryPtr> the transport socket factory
   * instance or error status. The returned TransportSocketFactoryPtr should not be nullptr.
   *
   * @throw EnvoyException if the implementation is unable to produce a factory with the provided
   *        parameters.
   */
  virtual absl::StatusOr<Network::UpstreamTransportSocketFactoryPtr>
  createTransportSocketFactory(const Protobuf::Message& config,
                               TransportSocketFactoryContext& context) PURE;

  std::string category() const override { return "envoy.transport_sockets.upstream"; }
};

/**
 * Implemented by each transport socket used for downstream connections. Registered via class
 * RegisterFactory.
 */
class DownstreamTransportSocketConfigFactory : public virtual TransportSocketConfigFactory {
public:
  /**
   * Create a particular downstream transport socket factory implementation.
   * @param server_names const std::vector<std::string>& the names of the server. This parameter is
   *        currently used by SNI implementation to know the expected server names.
   * @param config const Protobuf::Message& supplies the config message for the transport socket
   *        implementation.
   * @param context TransportSocketFactoryContext& supplies the transport socket's context.
   * @return absl::StatusOr<Network::DownstreamTransportSocketFactoryPtr> the transport socket
   * factory instance. The returned TransportSocketFactoryPtr should not be nullptr.
   *
   * @throw EnvoyException if the implementation is unable to produce a factory with the provided
   *        parameters.
   */
  virtual absl::StatusOr<Network::DownstreamTransportSocketFactoryPtr>
  createTransportSocketFactory(const Protobuf::Message& config,
                               TransportSocketFactoryContext& context,
                               const std::vector<std::string>& server_names) PURE;

  std::string category() const override { return "envoy.transport_sockets.downstream"; }
};

} // namespace Configuration
} // namespace Server
} // namespace Envoy
