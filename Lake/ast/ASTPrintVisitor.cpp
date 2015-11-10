//
//  ASTPrintVisitor.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "ASTPrintVisitor.hpp"

#include "ConstAST.hpp"
#include "ConstDefAST.hpp"
#include "IdentifierAST.hpp"
#include "FunctionPrototypeAST.hpp"
#include "ConstDefAST.hpp"
#include "ExpressionAST.hpp"
#include "FunctionBlockAST.hpp"
#include "FunctionDefAST.hpp"
#include "TypeAndNameAST.hpp"
#include "TypeAST.hpp"

#include <iostream>

namespace lake {
    
    class ASTPrintVisitorImpl
    : public ASTPrintVisitor
    {
        
        std::ostream &d_stream;
        
    public:
        
        ASTPrintVisitorImpl(std::ostream &stream)
        : d_stream(stream)
        {
        }
        
        virtual void visit(ConstValueAST<double> const *node)
        {
            if (node) {
                d_stream << node->value();
            } else {
                d_stream << "NULL";
            }

        }
        
        virtual void visit(ConstValueAST<std::string> const *node)
        {
            if (node) {
                d_stream << node->value();
            } else {
                d_stream << "NULL";
            }
        }
        
        virtual void visit(ConstValueAST<int> const *node)
        {
            if (node) {
                d_stream << node->value();
            } else {
                d_stream << "NULL";
            }
        }
        
        virtual void visit(FunctionPrototypeAST const *node)
        {
            d_stream << "(FUNCTION (";
            node->typeAndName()->accept(this);
            d_stream << ")";
            
            for (auto i = node->args().begin(); i != node->args().end(); ++i) {
                d_stream << " (";
                (*i)->accept(this);
                d_stream << ")";
            }
            
            d_stream << ")\n";
        }
        
        virtual void visit(IdentifierAST const *node)
        {
            if (node) {
                d_stream << node->name();
            } else {
                d_stream << "NULL";
            }
        }
        
        virtual void visit(ConstDefAST const *node)
        {
            d_stream << "(";
            node->name()->accept(this);
            d_stream << ")";
            node->value()->accept(this);
        }
        
        virtual void visit(BinOpExpressionAST const *node)
        {
            if (isprint(node->op())) {
                d_stream << "(" << static_cast<char>(node->op()) << " (";
            } else {
                d_stream << "(" << node->op() << " (";
            }
            node->lhs()->accept(this);
            d_stream << ") (";
            node->rhs()->accept(this);
            d_stream << "))";
        }
        
        virtual void visit(ConstExpressionAST<double> const *node)
        {
            d_stream << "(double)" << node->value();
        }
        
        virtual void visit(ConstExpressionAST<std::string> const *node)
        {
            d_stream << "(string)" << node->value();
        }
        
        virtual void visit(ConstExpressionAST<int> const *node)
        {
            d_stream << "(int)" << node->value();
        }
        
        virtual void visit(VarExpressionAST const *node)
        {
            d_stream << "(VAR (";
            node->name()->accept(this);
            d_stream << "))";
        }
        
        virtual void visit(CallExpressionAST const *node)
        {
            d_stream << "(CALL (";
            node->name()->accept(this);
            d_stream << ")";
            
            for (auto i = node->args().begin(); i != node->args().end(); ++i) {
                d_stream << " (";
                (*i)->accept(this);
                d_stream << ")";
            }
            
            d_stream << ")";
        }
        
        virtual void visit(FunctionDefAST const *node)
        {
            d_stream << "(DEF (";
            node->prototype()->accept(this);
            d_stream << ") (";
            node->block()->accept(this);
            d_stream << ")\n";
        }
        
        virtual void visit(FunctionBlockAST const *node)
        {
            d_stream << "(BLOCK";
            for (auto i = node->expressions().begin(); i != node->expressions().end(); ++i) {
                d_stream << " (";
                (*i)->accept(this);
                d_stream << ")";
            }
            d_stream << ")\n";
        }

        virtual void visit(ReturnExpressionAST const *node)
        {
            d_stream << "(RETURN (";
            node->rhs()->accept(this);
            d_stream << "))\n";
        }
        
        virtual void visit(TypeAndNameAST const *node)
        {
            node->type()->accept(this);
            node->name()->accept(this);
        }
        
        virtual void visit(TypeInt8AST const *node)
        {
            d_stream << "(INT8)";
        }
        
        virtual void visit(TypeInt16AST const *node)
        {
            d_stream << "(INT16)";
        }
        
        virtual void visit(TypeInt32AST const *node)
        {
            d_stream << "(INT32)";
        }
        
        virtual void visit(TypeInt64AST const *node)
        {
            d_stream << "(INT64)";
        }
        
        virtual void visit(TypeUInt8AST const *node)
        {
            d_stream << "(UINT8)";
        }
        
        virtual void visit(TypeUInt16AST const *node)
        {
            d_stream << "(UINT16)";
        }
        
        virtual void visit(TypeUInt32AST const *node)
        {
            d_stream << "(UINT32)";
        }
        
        virtual void visit(TypeUInt64AST const *node)
        {
            d_stream << "(UINT64)";
        }
        
        virtual void visit(TypeFloatAST const *node)
        {
            d_stream << "(FLOAT)";
        }
        
        virtual void visit(TypeDoubleAST const *node)
        {
            d_stream << "(DOUBLE)";
        }
        
        virtual void visit(TypeStringAST const *node)
        {
            d_stream << "(STRING)";
        }
        
    };
    
    std::unique_ptr<ASTPrintVisitor> ASTPrintVisitor::create(std::ostream &stream)
    {
        return std::unique_ptr<ASTPrintVisitorImpl>(new ASTPrintVisitorImpl(stream));
    }
    
}