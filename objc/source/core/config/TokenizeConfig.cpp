/*
 * Tencent is pleased to support the open source community by making
 * WCDB available.
 *
 * Copyright (C) 2017 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <WCDB/TokenizeConfig.hpp>

namespace WCDB {

std::shared_ptr<Config>
TokenizeConfig::configWithName(const std::list<std::string> &names)
{
    return std::shared_ptr<Config>(new TokenizeConfig(names));
}

TokenizeConfig::TokenizeConfig(const std::list<std::string> &names)
    : Config("tokenize", TokenizeConfig::order), m_names(names)
{
}

bool TokenizeConfig::invoke(Handle *handle) const
{
    for (const std::string &name : m_names) {
        const unsigned char *address = FTS::Modules::shared()->getAddress(name);
        NoCopyData data((unsigned char *) &address, sizeof(unsigned char *));

        //Setup Tokenize
        static const StatementSelect s_fts3Tokenizer =
            StatementSelect().select(Expression::function(
                "fts3_tokenizer", {BindParameter(1), BindParameter(2)}));
        if (handle->prepare(s_fts3Tokenizer)) {
            handle->bindText(name.c_str(), 1);
            handle->bindBLOB(data, 2);
            bool result = handle->step();
            handle->finalize();
            return result;
        }
    }
    return true;
}

} //namespace WCDB