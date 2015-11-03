//
//  FunctionPrototypeAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "FunctionPrototypeAST.hpp"
#include "IdentifierAST.hpp"

#include <unordered_map>

using namespace lake;

FunctionPrototypeAST::FunctionPrototypeAST(std::unique_ptr<IdentifierAST> &&name,
                                           std::vector<std::unique_ptr<IdentifierAST>> &&args)
: d_name(std::move(name)), d_args(std::move(args))
{
    
}

FunctionPrototypeAST::~FunctionPrototypeAST()
{
    
}

std::unique_ptr<IdentifierAST> const &FunctionPrototypeAST::name() const
{
    return d_name;
}

std::vector<std::unique_ptr<IdentifierAST>> const &FunctionPrototypeAST::args() const
{
    return d_args;
}
