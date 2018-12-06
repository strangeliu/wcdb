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

#include <WCDB/Assertion.hpp>
#include <WCDB/Syntax.h>

namespace WCDB {

namespace Syntax {

#pragma mark - Identifier
Identifier::Type TableOrSubquery::getType() const
{
    return type;
}

String TableOrSubquery::getDescription() const
{
    std::ostringstream stream;
    switch (switcher) {
    case Switch::Table:
        stream << schema << "." << tableOrFunction;
        if (!alias.empty()) {
            stream << " AS " << alias;
        }
        switch (indexType) {
        case IndexType::NotSet:
            break;
        case IndexType::Indexed:
            stream << " INDEXED BY " << index;
            break;
        case IndexType::NotIndexed:
            stream << " NOT INDEXED";
            break;
        }
        break;
    case Switch::Function:
        stream << schema << "." << tableOrFunction << "(" << expressions << ")";
        if (!alias.empty()) {
            stream << " AS " << alias;
        }
        break;
    case Switch::TableOrSubqueries:
        stream << "(" << tableOrSubqueries << ")";
        break;
    case Switch::JoinClause:
        stream << "(" << joinClause << ")";
        break;
    case Switch::Select:
        stream << "(" << select << ")";
        if (!alias.empty()) {
            stream << " AS " << alias;
        }
        break;
    }
    return stream.str();
}

void TableOrSubquery::iterate(const Iterator& iterator, void* parameter)
{
    Identifier::iterate(iterator, parameter);
    switch (switcher) {
    case Switch::Table:
        schema.iterate(iterator, parameter);
        break;
    case Switch::Function:
        schema.iterate(iterator, parameter);
        listIterate(expressions, iterator, parameter);
        break;
    case Switch::TableOrSubqueries:
        listIterate(tableOrSubqueries, iterator, parameter);
        break;
    case Switch::JoinClause:
        joinClause.iterate(iterator, parameter);
        break;
    case Switch::Select:
        select.iterate(iterator, parameter);
        break;
    }
}

} // namespace Syntax

} // namespace WCDB