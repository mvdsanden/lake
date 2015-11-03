//
//  FunctionPrototypeAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "FunctionPrototypeAST.hpp"

#include <unordered_map>

using namespace lake;

FunctionPrototypeAST::FunctionPrototypeAST(std::shared_ptr<IdentifierAST> const &name,
                                           std::vector<std::shared_ptr<IdentifierAST>> &&args)
: d_name(name), d_args(args)
{
    
}

FunctionPrototypeAST::~FunctionPrototypeAST()
{
    
}

std::shared_ptr<IdentifierAST> const &FunctionPrototypeAST::name() const
{
    return d_name;
}

std::vector<std::shared_ptr<IdentifierAST>> const &FunctionPrototypeAST::args() const
{
    return d_args;
}
