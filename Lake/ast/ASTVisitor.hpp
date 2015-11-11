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

//#include "TypeAST.hpp"

#define LAKE_VISITOR_ACCEPT(V) virtual void accept(V *visitor) const { visitor->visit(this); }

namespace lake {
    
    template <class T> class ConstValueAST;
    template <class T> class ConstExpressionAST;
    template <class T> class NativeTypeAST;
    class BaseAST;
    class IdentifierAST;
    class FunctionPrototypeAST;
    class ConstAST;
    class ConstDefAST;
    class ExpressionAST;
    class BinOpExpressionAST;
    class VarExpressionAST;
    class CallExpressionAST;
    class FunctionDefAST;
    class FunctionBlockAST;
    class ReturnExpressionAST;
    class TypeAndNameAST;
    class TypeAST;
    
    class ASTVisitor
    {
    public:
        
        virtual void visitNotHandled(BaseAST const *node, std::string const &type);
        
        virtual void visit(ConstAST const *node);
        
        virtual void visit(ConstValueAST<double> const *node);
        
        virtual void visit(ConstValueAST<std::string> const *node);
        
        virtual void visit(ConstValueAST<int8_t> const *node);
        
        virtual void visit(ConstValueAST<int64_t> const *node);
        
        virtual void visit(ConstValueAST<uint64_t> const *node);
        
        virtual void visit(FunctionPrototypeAST const *node);
        
        virtual void visit(IdentifierAST const *node);
        
        virtual void visit(ConstDefAST const *node);
        
        virtual void visit(BinOpExpressionAST const *node);
        
        virtual void visit(ExpressionAST const *node);
        
        virtual void visit(ConstExpressionAST<int8_t> const *node);
        
        virtual void visit(ConstExpressionAST<int64_t> const *node);
        
        virtual void visit(ConstExpressionAST<uint64_t> const *node);
        
        virtual void visit(ConstExpressionAST<double> const *node);
        
        virtual void visit(ConstExpressionAST<std::string> const *node);

        virtual void visit(VarExpressionAST const *node);
        
        virtual void visit(CallExpressionAST const *node);
        
        virtual void visit(FunctionDefAST const *node);
        
        virtual void visit(FunctionBlockAST const *node);
        
        virtual void visit(ReturnExpressionAST const *node);
        
        virtual void visit(TypeAndNameAST const *node);
        
        virtual void visit(TypeAST const *node);
        
        virtual void visit(NativeTypeAST<int8_t> const *node);
        
        virtual void visit(NativeTypeAST<int16_t> const *node);
        
        virtual void visit(NativeTypeAST<int32_t> const *node);
        
        virtual void visit(NativeTypeAST<int64_t> const *node);

        virtual void visit(NativeTypeAST<uint8_t> const *node);
        
        virtual void visit(NativeTypeAST<uint16_t> const *node);
        
        virtual void visit(NativeTypeAST<uint32_t> const *node);
        
        virtual void visit(NativeTypeAST<uint64_t> const *node);
        
        virtual void visit(NativeTypeAST<float> const *node);
        
        virtual void visit(NativeTypeAST<double> const *node);

        virtual void visit(NativeTypeAST<std::string> const *node);
        
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
