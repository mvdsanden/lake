//
//  ASTVisitor.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "ASTVisitor.hpp"

#include "ConstAST.hpp"
#include "FunctionBlockAST.hpp"
#include "FunctionDefAST.hpp"
#include "FunctionPrototypeAST.hpp"
#include "IdentifierAST.hpp"
#include "ConstDefAST.hpp"
#include "ExpressionAST.hpp"
#include "TypeAST.hpp"
#include "TypeAndNameAST.hpp"
#include "NativeTypeAST.hpp"

#include <iostream>
#include <cassert>

using namespace lake;

void ASTVisitor::visitNotHandled(BaseAST const *node, std::string const &type)
{
    std::cerr << "ASTVisitor: warning: visit not handled: " << type << ".\n";
    abort();
}

#define LAKE_VISIT_DEFAULT_IMPL(Type) void ASTVisitor::visit(Type const *node) { visitNotHandled(node, #Type); };

LAKE_VISIT_DEFAULT_IMPL(ConstValueAST<double>);

LAKE_VISIT_DEFAULT_IMPL(ConstValueAST<std::string>);

LAKE_VISIT_DEFAULT_IMPL(ConstValueAST<int8_t>);

LAKE_VISIT_DEFAULT_IMPL(ConstValueAST<int64_t>);

LAKE_VISIT_DEFAULT_IMPL(ConstValueAST<uint64_t>);

LAKE_VISIT_DEFAULT_IMPL(FunctionPrototypeAST);

LAKE_VISIT_DEFAULT_IMPL(IdentifierAST);

LAKE_VISIT_DEFAULT_IMPL(ConstDefAST);

LAKE_VISIT_DEFAULT_IMPL(BinOpExpressionAST);

LAKE_VISIT_DEFAULT_IMPL(ConstExpressionAST<int8_t>);

LAKE_VISIT_DEFAULT_IMPL(ConstExpressionAST<int64_t>);

LAKE_VISIT_DEFAULT_IMPL(ConstExpressionAST<uint64_t>);

LAKE_VISIT_DEFAULT_IMPL(ConstExpressionAST<double>);

LAKE_VISIT_DEFAULT_IMPL(ConstExpressionAST<std::string>);

LAKE_VISIT_DEFAULT_IMPL(VarExpressionAST);

LAKE_VISIT_DEFAULT_IMPL(CallExpressionAST);

LAKE_VISIT_DEFAULT_IMPL(FunctionDefAST);

LAKE_VISIT_DEFAULT_IMPL(FunctionBlockAST);

LAKE_VISIT_DEFAULT_IMPL(ReturnExpressionAST);

LAKE_VISIT_DEFAULT_IMPL(TypeAndNameAST);

LAKE_VISIT_DEFAULT_IMPL(TypeInt8AST);

LAKE_VISIT_DEFAULT_IMPL(TypeInt16AST);

LAKE_VISIT_DEFAULT_IMPL(TypeInt32AST);

LAKE_VISIT_DEFAULT_IMPL(TypeInt64AST);

LAKE_VISIT_DEFAULT_IMPL(TypeUInt8AST);

LAKE_VISIT_DEFAULT_IMPL(TypeUInt16AST);

LAKE_VISIT_DEFAULT_IMPL(TypeUInt32AST);

LAKE_VISIT_DEFAULT_IMPL(TypeUInt64AST);

LAKE_VISIT_DEFAULT_IMPL(TypeFloatAST);

LAKE_VISIT_DEFAULT_IMPL(TypeDoubleAST);

LAKE_VISIT_DEFAULT_IMPL(TypeStringAST);
