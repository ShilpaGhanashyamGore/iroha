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

#ifndef IROHA_SHARED_MODEL_QUERY_RESPONSE_HPP
#define IROHA_SHARED_MODEL_QUERY_RESPONSE_HPP

#include <boost/variant.hpp>
#include "interfaces/hashable.hpp"
#include "interfaces/query_responses/account_asset_response.hpp"
#include "interfaces/query_responses/account_response.hpp"
#include "interfaces/query_responses/asset_response.hpp"
#include "interfaces/query_responses/no_account_assets_error_response.hpp"
#include "interfaces/query_responses/no_account_error_response.hpp"
#include "interfaces/query_responses/no_asset_error_response.hpp"
#include "interfaces/query_responses/no_roles_error_response.hpp"
#include "interfaces/query_responses/no_signatories_error_response.hpp"
#include "interfaces/query_responses/not_supported_error_response.hpp"
#include "interfaces/query_responses/role_permissions.hpp"
#include "interfaces/query_responses/roles_response.hpp"
#include "interfaces/query_responses/signatories_response.hpp"
#include "interfaces/query_responses/stateful_failed_error_response.hpp"
#include "interfaces/query_responses/stateless_failed_error_response.hpp"
#include "interfaces/query_responses/transactions_response.hpp"
#include "model/query_response.hpp"

namespace shared_model {
  namespace interface {
    /**
     * Class QueryResponse(qr) provides container with concrete query responses
     * available in the system.
     * General note: this class is container for QRs but not a base class.
     * Architecture note: query responses should be attached to following query.
     * To perform it make QueryResponse hashable, thus, in hash() method
     * expects hash of the following query.
     */
    class QueryResponse
        : public Hashable<QueryResponse, iroha::model::QueryResponse> {
     private:
      /// Shortcut type for polymorphic wrapper
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

     public:
      /// Type of container with all concrete query response
      using QueryResponseVariantType =
          boost::variant<w<AccountAssetResponse>,
                         w<AccountResponse>,
                         w<AssetResponse>,
                         w<RolePermissionsResponse>,
                         w<RolesResponse>,
                         w<SignatoriesResponse>,
                         w<TransactionsResponse>,
                         w<StatelessFailedErrorResponse>,
                         w<StatefulFailedErrorResponse>,
                         w<NoAccountErrorResponse>,
                         w<NoAssetErrorResponse>,
                         w<NoRolesErrorResponse>,
                         w<NoAccountAssetsErrorResponse>,
                         w<NoSignatoriesErrorResponse>,
                         w<NotSupportedErrorResponse>>;

      /// Type of all available query responses
      using QueryResponseListType = QueryResponseVariantType::types;

      /**
       * @return reference to const variant with concrete qr
       */
      virtual const QueryResponseVariantType &get() const = 0;

      // ------------------------| Primitive override |-------------------------

      std::string toString() const override {
        return boost::apply_visitor(detail::ToStringVisitor(), get());
      }

      OldModelType *makeOldModel() const override {
        return boost::apply_visitor(
            detail::OldModelCreatorVisitor<OldModelType *>(), get());
      }

      bool operator==(const ModelType &rhs) const override {
        return this->get() == rhs.get();
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_QUERY_RESPONSE_HPP
