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

#ifndef IROHA_SHARED_MODEL_SIGNATORIES_RESPONSE_HPP
#define IROHA_SHARED_MODEL_SIGNATORIES_RESPONSE_HPP

#include <boost/assert.hpp>
#include "crypto/hash.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/primitive.hpp"
#include "interfaces/visitor_apply_for_all.hpp"
#include "model/queries/responses/signatories_response.hpp"
#include "utils/string_builder.hpp"

namespace shared_model {
  namespace interface {
    /**
     * Container of asset, for fetching data.
     */
    class SignatoriesResponse
        : public Primitive<SignatoriesResponse,
                           iroha::model::SignatoriesResponse> {
     public:
      /**
       * @return All public keys attached to account
       */
      virtual types::PublicKeyCollectionType keys() const = 0;

      /**
       * Stringify the data.
       * @return string representation of data.
       */
      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("SignatoriesResponse")
            .append("signatories")
            .appendAll(keys(), [](auto &key) { return key->toString(); })
            .finalize();
      }

      /**
       * @return true if the data are same.
       */
      bool operator==(const ModelType &rhs) const override {
        return keys() == rhs.keys();
      }

      /**
       * Makes old model.
       * @return An allocated old model of signatories response.
       */
      OldModelType *makeOldModel() const override {
        OldModelType *oldModel = new OldModelType();
        //        BOOST_ASSERT(decltype(oldModel->keys)::value_type::size()
        //                     == publicKeys().size());
        // for (size_t )
        return oldModel;
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_SIGNATORIES_RESPONSE_HPP