//
//  FunctionPrototypeAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "FunctionPrototypeAST.hpp"
#include "IdentifierAST.hpp"
#include "TypeAndNameAST.hpp"

#include <unordered_map>

using namespace lake;

FunctionPrototypeAST::FunctionPrototypeAST(size_t lineNumber,
                                           std::unique_ptr<TypeAndNameAST> &&typeAndName,
                                           std::vector<std::unique_ptr<TypeAndNameAST>> &&args)
: BaseAST(lineNumber), d_typeAndName(std::move(typeAndName)), d_args(std::move(args))
{
    
}

FunctionPrototypeAST::~FunctionPrototypeAST()
{
    
}

std::unique_ptr<TypeAndNameAST> const &FunctionPrototypeAST::typeAndName() const
{
    return d_typeAndName;
}

std::vector<std::unique_ptr<TypeAndNameAST>> const &FunctionPrototypeAST::args() const
{
    return d_args;
}

