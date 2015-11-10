//
//  ConstDefAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "ConstDefAST.hpp"

using namespace lake;


ConstDefAST::ConstDefAST(size_t lineNumber,
                         std::unique_ptr<IdentifierAST> &&name,
                         std::unique_ptr<ConstAST> &&value)
: BaseAST(lineNumber), d_name(std::move(name)), d_value(std::move(value))
{
}

ConstDefAST::~ConstDefAST()
{
    
}

std::unique_ptr<IdentifierAST> const &ConstDefAST::name() const
{
    return d_name;
}

std::unique_ptr<ConstAST> const &ConstDefAST::value() const
{
    return d_value;
}
