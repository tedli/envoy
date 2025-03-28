#include "source/extensions/upstreams/http/generic/config.h"

#include "test/mocks/router/mocks.h"
#include "test/mocks/upstream/thread_local_cluster.h"
#include "test/test_common/utility.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace Upstreams {
namespace Http {
namespace Generic {

using ::testing::NiceMock;

class GenericGenericConnPoolFactoryTest : public ::testing::Test {
public:
  GenericGenericConnPoolFactoryTest() = default;

protected:
  NiceMock<Upstream::MockThreadLocalCluster> thread_local_cluster_;
  Upstream::ResourcePriority priority_ = Upstream::ResourcePriority::Default;
  Upstream::HostConstSharedPtr host_;
  GenericGenericConnPoolFactory factory_;
  ProtobufTypes::MessagePtr message_{new Envoy::ProtobufWkt::Struct()};
};

TEST_F(GenericGenericConnPoolFactoryTest, CreateValidHttpConnPool) {
  EXPECT_TRUE(factory_.createGenericConnPool(
      nullptr, thread_local_cluster_, Router::GenericConnPoolFactory::UpstreamProtocol::HTTP,
      priority_, Envoy::Http::Protocol::Http2, nullptr, *message_));
}

TEST_F(GenericGenericConnPoolFactoryTest, CreateValidTcpConnPool) {
  EXPECT_TRUE(factory_.createGenericConnPool(
      nullptr, thread_local_cluster_, Router::GenericConnPoolFactory::UpstreamProtocol::TCP,
      priority_, Envoy::Http::Protocol::Http2, nullptr, *message_));
}

TEST_F(GenericGenericConnPoolFactoryTest, CreateValidUdpConnPool) {
  EXPECT_TRUE(factory_.createGenericConnPool(thread_local_cluster_.lb_.host_, thread_local_cluster_,
                                             Router::GenericConnPoolFactory::UpstreamProtocol::UDP,
                                             priority_, Envoy::Http::Protocol::Http2, nullptr,
                                             *message_));
}

TEST_F(GenericGenericConnPoolFactoryTest, InvalidConnPool) {
  // Passes an invalid UpstreamProtocol and check a nullptr is returned.
  EXPECT_FALSE(factory_.createGenericConnPool(
      nullptr, thread_local_cluster_,
      static_cast<Router::GenericConnPoolFactory::UpstreamProtocol>(0xff), priority_,
      Envoy::Http::Protocol::Http2, nullptr, *message_));
}

} // namespace Generic
} // namespace Http
} // namespace Upstreams
} // namespace Extensions
} // namespace Envoy
