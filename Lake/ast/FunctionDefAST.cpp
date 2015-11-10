//
//  FunctionDefAST.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "FunctionDefAST.hpp"

using namespace lake;

FunctionDefAST::FunctionDefAST(size_t lineNumber,
                               std::unique_ptr<FunctionPrototypeAST> &&prototype,
                               std::unique_ptr<FunctionBlockAST> &&block)
: BaseAST(lineNumber), d_prototype(std::move(prototype)), d_block(std::move(block))
{
    
}

std::unique_ptr<FunctionPrototypeAST> const &FunctionDefAST::prototype() const
{
    return d_prototype;
}

std::unique_ptr<FunctionBlockAST> const &FunctionDefAST::block() const
{
    return d_block;
}
