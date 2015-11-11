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

#include <iostream>
#include <cassert>

using namespace lake;

void ASTVisitor::visitNotHandled(BaseAST const *node, std::string const &type)
{
    std::cerr << "ASTVisitor: warning: visit not handled: " << type << ".\n";
    abort();
}

#define LAKE_VISIT_DEFAULT_IMPL(Type) void ASTVisitor::visit(Type const *node) { visitNotHandled(node, #Type); };
#define LAKE_VISIT_FW_IMPL(From, To) void ASTVisitor::visit(From const *node) { visit(static_cast<To const*>(node)); };

LAKE_VISIT_DEFAULT_IMPL(ConstAST);

LAKE_VISIT_FW_IMPL(ConstValueAST<double>, ConstAST);

LAKE_VISIT_FW_IMPL(ConstValueAST<std::string>, ConstAST);

LAKE_VISIT_FW_IMPL(ConstValueAST<int8_t>, ConstAST);

LAKE_VISIT_FW_IMPL(ConstValueAST<int64_t>, ConstAST);

LAKE_VISIT_FW_IMPL(ConstValueAST<uint64_t>, ConstAST);

LAKE_VISIT_DEFAULT_IMPL(FunctionPrototypeAST);

LAKE_VISIT_DEFAULT_IMPL(IdentifierAST);

LAKE_VISIT_DEFAULT_IMPL(ConstDefAST);

LAKE_VISIT_DEFAULT_IMPL(ExpressionAST);

LAKE_VISIT_FW_IMPL(BinOpExpressionAST, ExpressionAST);

LAKE_VISIT_FW_IMPL(ConstExpressionAST<int8_t>, ExpressionAST);

LAKE_VISIT_FW_IMPL(ConstExpressionAST<int64_t>, ExpressionAST);

LAKE_VISIT_FW_IMPL(ConstExpressionAST<uint64_t>, ExpressionAST);

LAKE_VISIT_FW_IMPL(ConstExpressionAST<double>, ExpressionAST);

LAKE_VISIT_FW_IMPL(ConstExpressionAST<std::string>, ExpressionAST);

LAKE_VISIT_FW_IMPL(VarExpressionAST, ExpressionAST);

LAKE_VISIT_FW_IMPL(CallExpressionAST, ExpressionAST);

LAKE_VISIT_DEFAULT_IMPL(FunctionDefAST);

LAKE_VISIT_DEFAULT_IMPL(FunctionBlockAST);

LAKE_VISIT_DEFAULT_IMPL(ReturnExpressionAST);

LAKE_VISIT_DEFAULT_IMPL(TypeAndNameAST);

LAKE_VISIT_DEFAULT_IMPL(TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<int8_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<int16_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<int32_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<int64_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<uint8_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<uint16_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<uint32_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<uint64_t>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<float>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<double>, TypeAST);

LAKE_VISIT_FW_IMPL(NativeTypeAST<std::string>, TypeAST);
