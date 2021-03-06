#pragma once

#include <string>

#include "envoy/network/transport_socket.h"
#include "envoy/ssl/context_manager.h"

#include "common/protobuf/protobuf.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Context passed to transport socket factory to access server resources.
 */
class TransportSocketFactoryContext {
public:
  virtual ~TransportSocketFactoryContext() {}

  /**
   * @return Ssl::ContextManager& the SSL context manager
   */
  virtual Ssl::ContextManager& sslContextManager() PURE;

  /**
   * @return Stats::Scope& the transport socket's stats scope.
   */
  virtual Stats::Scope& statsScope() const PURE;
};

class TransportSocketConfigFactory {
public:
  virtual ~TransportSocketConfigFactory() {}

  /**
   * Create a particular transport socket factory implementation.
   * @param config const Protobuf::Message& supplies the config message for the transport socket
   *        implementation.
   * @param context TransportSocketFactoryContext&  supplies the transport socket's context.
   * @return Network::TransportSocketFactoryPtr the transport socket factory instance. The returned
   *         TransportSocketFactoryPtr should not be nullptr.
   *
   * @throw EnvoyException if the implementation is unable to produce a factory with the provided
   *        parameters.
   */
  virtual Network::TransportSocketFactoryPtr
  createTransportSocketFactory(const Protobuf::Message& config,
                               TransportSocketFactoryContext& context) PURE;

  /**
   * @return ProtobufTypes::MessagePtr create empty config proto message. The transport socket
   *         config, which arrives in an opaque google.protobuf.Struct message, will be converted
   *         to JSON and then parsed into this empty proto.
   */
  virtual ProtobufTypes::MessagePtr createEmptyConfigProto() PURE;

  /**
   * @return std::string the identifying name for a particular TransportSocketFactoryPtr
   *         implementation produced by the factory.
   */
  virtual std::string name() const PURE;
};

class UpstreamTransportSocketConfigFactory : public virtual TransportSocketConfigFactory {};

} // namespace Configuration
} // namespace Server
} // namespace Envoy
