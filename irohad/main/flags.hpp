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

#ifndef IROHA_FLAGS_HPP_
#define IROHA_FLAGS_HPP_

#include <CLI/CLI.hpp>
#include "main/common.hpp"
#include "util/network.hpp"

using namespace std::literals::string_literals;

inline void addPeerFlags(CLI::App *p,
                         iroha::config::Torii &torii,
                         iroha::config::Cryptography &crypto) {
  p->add_option("--host"s,                      /* option's name */
                torii.host,                     /* bind to this variable */
                "Peer's address to listen on"s, /* description */
                true /* use initial value as default */)
      ->envname("IROHA_HOST"s)
      ->group("Peer"s)
      ->check(iroha::network::is_host_valid);

  p->add_option("--port"s, torii.port, "Peer's port to listen on"s, true)
      ->envname("IROHA_PORT"s)
      ->check(CLI::Range(1, 65535))
      ->group("Peer"s);

  p->add_option(
       "--pubkey"s, crypto.public_key, "Path to peer's public key"s, false)
      ->required()
      ->envname("IROHA_PUBKEY"s)
      ->group("Peer"s)
      ->check(CLI::ExistingFile);

  p->add_option(
       "--privkey"s, crypto.private_key, "Path to peer's private key"s, false)
      ->required()
      ->envname("IROHA_PRIVKEY"s)
      ->group("Peer"s)
      ->check(CLI::ExistingFile);
}

inline void addPostgresFlags(CLI::App *p, iroha::config::Postgres &postgres) {
  p->add_option("--pghost"s, postgres.host, "PostgreSQL database host. "s, true)
      ->envname("IROHA_PGHOST"s)
      ->group("PostgreSQL"s)
      ->check(iroha::network::is_host_valid);

  p->add_option("--pgport"s, postgres.port, "PostgreSQL database port."s, true)
      ->envname("IROHA_PGPORT"s)
      ->check(CLI::Range(1, 65535))
      ->group("PostgreSQL"s);

  p->add_option(
       "--pgdatabase"s, postgres.database, "PostgreSQL database name"s, true)
      ->envname("IROHA_PGDATABASE"s)
      ->group("PostgreSQL"s);

  p->add_option("--pguser"s, postgres.username, "PostgreSQL username"s, false)
      ->required()
      ->envname("IROHA_PGUSER"s)
      ->group("PostgreSQL"s);

  p->add_option(
       "--pgpassword"s, postgres.password, "PostgreSQL password"s, false)
      ->required()
      ->envname("IROHA_PGPASSWORD"s)
      ->group("PostgreSQL"s);
}

inline void addRedisFlags(CLI::App *p, iroha::config::Redis &redis) {
  p->add_option("--rdhost"s, redis.host, "Redis database host"s, true)
      ->envname("IROHA_RDHOST"s)
      ->group("Redis"s)
      ->check(iroha::network::is_host_valid);

  p->add_option("--rdport"s, redis.port, "Redis database port"s, true)
      ->envname("IROHA_RDPORT"s)
      ->check(CLI::Range(1, 65535))
      ->group("Redis"s);
}

#endif  //  IROHA_FLAGS_HPP_