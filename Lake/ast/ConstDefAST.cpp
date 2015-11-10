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
                         std::unique_ptr<TypeAndNameAST> &&typeAndName,
                         std::unique_ptr<ConstAST> &&value)
: BaseAST(lineNumber), d_typeAndName(std::move(typeAndName)), d_value(std::move(value))
{
}

ConstDefAST::~ConstDefAST()
{
    
}

std::unique_ptr<TypeAndNameAST> const &ConstDefAST::typeAndName() const
{
    return d_typeAndName;
}

std::unique_ptr<ConstAST> const &ConstDefAST::value() const
{
    return d_value;
}
