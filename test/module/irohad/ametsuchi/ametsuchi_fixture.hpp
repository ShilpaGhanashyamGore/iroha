/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_AMETSUCHI_FIXTURE_HPP
#define IROHA_AMETSUCHI_FIXTURE_HPP

#include <gtest/gtest.h>
#include <cpp_redis/cpp_redis>
#include <pqxx/pqxx>

#include "common/files.hpp"
#include "crypto/crypto.hpp"
#include "logger/logger.hpp"
#include "main/config/common.hpp"
#include "util/string.hpp"
#include "main/config/common.hpp"

using namespace std::literals::string_literals;
using iroha::string::util::from_string;


namespace iroha {
  namespace ametsuchi {
    class AmetsuchiConfig : public iroha::config::Config {
     public:
      AmetsuchiConfig() {
        load();
      }

      void load() override {
        db_.path = "/tmp/block_store";
        pg_.host = parseEnv("IROHA_POSTGRES_HOST", "localhost"s);
        pg_.port = parseEnv("IROHA_POSTGRES_PORT", 5432);
        pg_.username = parseEnv("IROHA_POSTGRES_USER", "postgres"s);
        pg_.password = parseEnv("IROHA_POSTGRES_PASSWORD", "mysecretpassword"s);
        redis_.host = parseEnv("IROHA_REDIS_HOST", "localhost"s);
        redis_.port = parseEnv("IROHA_REDIS_PORT", 6379);
      }
    };

    /**
     * Class with ametsuchi initialization
     */
    class AmetsuchiTest : public ::testing::Test {
     protected:
      virtual void SetUp() {
        auto log = logger::testLog("AmetsuchiTest");

        config_ = std::make_unique<AmetsuchiConfig>();

        mkdir(config_->blockStorage().path.c_str(),
              S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      }

      virtual void TearDown() {
        const auto drop = R"(
DROP TABLE IF EXISTS account_has_signatory;
DROP TABLE IF EXISTS account_has_asset;
DROP TABLE IF EXISTS role_has_permissions;
DROP TABLE IF EXISTS account_has_roles;
DROP TABLE IF EXISTS account_has_grantable_permissions;
DROP TABLE IF EXISTS account;
DROP TABLE IF EXISTS asset;
DROP TABLE IF EXISTS domain;
DROP TABLE IF EXISTS signatory;
DROP TABLE IF EXISTS peer;
DROP TABLE IF EXISTS role;
)";
        auto config = std::make_unique<AmetsuchiConfig>();

        pqxx::connection connection(config->postgres().options());
        pqxx::work txn(connection);
        txn.exec(drop);
        txn.commit();
        connection.disconnect();

        cpp_redis::redis_client client;
        client.connect(config->redis().host, config->redis().port);
        client.flushall();
        client.sync_commit();

        iroha::remove_all(config->blockStorage().path);
      }

      std::unique_ptr<iroha::config::Config> config_;
      iroha::config::Config::BlockStorage storage;
      iroha::config::Config::Postgres postgres;
      iroha::config::Config::Redis redis;
    };
  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_AMETSUCHI_FIXTURE_HPP
