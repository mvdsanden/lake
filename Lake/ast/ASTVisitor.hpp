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

#include "TypeAST.hpp"

#define LAKE_VISITOR_ACCEPT(V) virtual void accept(V *visitor) const { visitor->visit(this); }

namespace lake {
    
    template <class T> class ConstValueAST;
    template <class T> class ConstExpressionAST;
    template <class T, size_t TypeID> class NativeTypeAST;
    class IdentifierAST;
    class FunctionPrototypeAST;
    class ConstDefAST;
    class BinOpExpressionAST;
    class VarExpressionAST;
    class CallExpressionAST;
    class FunctionDefAST;
    class FunctionBlockAST;
    class ReturnExpressionAST;
    class TypeAndNameAST;
    
    class ASTVisitor
    {
    public:
        
        virtual void visit(ConstValueAST<double> const *node) {};
        
        virtual void visit(ConstValueAST<std::string> const *node) {};
        
        virtual void visit(ConstValueAST<int8_t> const *node) {};
        
        virtual void visit(ConstValueAST<int64_t> const *node) {};
        
        virtual void visit(ConstValueAST<uint64_t> const *node) {};
        
        virtual void visit(FunctionPrototypeAST const *node) {};
        
        virtual void visit(IdentifierAST const *node) {};
        
        virtual void visit(ConstDefAST const *node) {};
        
        virtual void visit(BinOpExpressionAST const *node) {};
        
        virtual void visit(ConstExpressionAST<int8_t> const *node) {};
        
        virtual void visit(ConstExpressionAST<int64_t> const *node) {};
        
        virtual void visit(ConstExpressionAST<uint64_t> const *node) {};
        
        virtual void visit(ConstExpressionAST<double> const *node) {};
        
        virtual void visit(ConstExpressionAST<std::string> const *node) {};

        virtual void visit(VarExpressionAST const *node) {};
        
        virtual void visit(CallExpressionAST const *node) {};
        
        virtual void visit(FunctionDefAST const *node) {};
        
        virtual void visit(FunctionBlockAST const *node) {};
        
        virtual void visit(ReturnExpressionAST const *node) {};
        
        virtual void visit(TypeAndNameAST const *node) {};
        
        virtual void visit(TypeInt8AST const *node) {};
        
        virtual void visit(TypeInt16AST const *node) {};
        
        virtual void visit(TypeInt32AST const *node) {};
        
        virtual void visit(TypeInt64AST const *node) {};

        virtual void visit(TypeUInt8AST const *node) {};
        
        virtual void visit(TypeUInt16AST const *node) {};
        
        virtual void visit(TypeUInt32AST const *node) {};
        
        virtual void visit(TypeUInt64AST const *node) {};
        
        virtual void visit(TypeFloatAST const *node) {};
        
        virtual void visit(TypeDoubleAST const *node) {};

        virtual void visit(TypeStringAST const *node) {};
        
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
