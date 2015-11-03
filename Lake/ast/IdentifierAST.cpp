//
//  IdentifierAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "IdentifierAST.hpp"

#include <unordered_map>

using namespace lake;

IdentifierAST::IdentifierAST(std::string const &name)
: d_name(name)
{
}

IdentifierAST::~IdentifierAST()
{
}


std::string const &IdentifierAST::name() const
{
    return d_name;
}