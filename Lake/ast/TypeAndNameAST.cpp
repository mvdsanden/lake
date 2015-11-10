//
//  TypeAndNameAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/10/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "TypeAndNameAST.hpp"
#include "TypeAST.hpp"
#include "IdentifierAST.hpp"

using namespace lake;

TypeAndNameAST::TypeAndNameAST(size_t lineNumber,
                               std::unique_ptr<TypeAST> &&type,
                               std::unique_ptr<IdentifierAST> &&name)
: BaseAST(lineNumber), d_type(std::move(type)), d_name(std::move(name))
{
}

TypeAndNameAST::~TypeAndNameAST()
{
}

std::unique_ptr<TypeAST> const &TypeAndNameAST::type() const
{
    return d_type;
}

std::unique_ptr<IdentifierAST> const &TypeAndNameAST::name() const
{
    return d_name;
}
