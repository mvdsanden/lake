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
            node->name()->accept(this);
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
            d_stream << "(CONSTANT (";
            node->name()->accept(this);
            d_stream << ") (";
            node->value()->accept(this);
            d_stream << "))\n";
        }
        
    };
    
    std::unique_ptr<ASTPrintVisitor> ASTPrintVisitor::create(std::ostream &stream)
    {
        return std::unique_ptr<ASTPrintVisitorImpl>(new ASTPrintVisitorImpl(stream));
    }
    
}