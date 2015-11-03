//
//  ASTVisitor.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ASTVisitor_hpp
#define ASTVisitor_hpp

#include <string>

#define LAKE_VISITOR_ACCEPT(V) virtual void accept(V *visitor) { visitor->visit(this); }

namespace lake {
    
    template <class T> class ConstValueAST;
    template <class T> class ConstExpressionAST;
    class IdentifierAST;
    class FunctionPrototypeAST;
    class ConstDefAST;
    class BinOpExpressionAST;
    class VarExpressionAST;
    class CallExpressionAST;
    class FunctionDefAST;
    class FunctionBlockAST;
    
    class ASTVisitor
    {
    public:
        
        virtual void visit(ConstValueAST<double> const *node) = 0;
        
        virtual void visit(ConstValueAST<std::string> const *node) = 0;
        
        virtual void visit(ConstValueAST<int> const *node) = 0;
        
        virtual void visit(FunctionPrototypeAST const *node) = 0;
        
        virtual void visit(IdentifierAST const *node) = 0;
        
        virtual void visit(ConstDefAST const *node) = 0;
        
        virtual void visit(BinOpExpressionAST const *node) = 0;
        
        virtual void visit(ConstExpressionAST<double> const *node) = 0;
        
        virtual void visit(ConstExpressionAST<std::string> const *node) = 0;
        
        virtual void visit(ConstExpressionAST<int> const *node) = 0;

        virtual void visit(VarExpressionAST const *node) = 0;
        
        virtual void visit(CallExpressionAST const *node) = 0;
        
        virtual void visit(FunctionDefAST const *node) = 0;
        
        virtual void visit(FunctionBlockAST const *node) = 0;
        
    };
    
    template <class T>
    class ASTVisitable
    {
    public:
        
        virtual void accept(ASTVisitor *visitor)
        {
            visitor->visit(static_cast<T*>(this));
        }
        
    };
    
}

#endif /* ASTVisitor_hpp */
