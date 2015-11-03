//
//  ExpressionAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ExpressionAST_hpp
#define ExpressionAST_hpp

#include "BaseAST.hpp"
#include "IdentifierAST.hpp"
#include "ASTVisitor.hpp"

#include <vector>

namespace lake {
    
    class ExpressionAST
    : public BaseAST
    {
    public:
        
        
    };
    
    class BinOpExpressionAST
    : public ExpressionAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        BinOpExpressionAST(int op,
                           std::unique_ptr<ExpressionAST> &&lhs,
                           std::unique_ptr<ExpressionAST> &&rhs)
        : d_op(op), d_lhs(std::move(lhs)), d_rhs(std::move(rhs))
        {
        }

        int op() const
        {
            return d_op;
        }
        
        std::unique_ptr<ExpressionAST> const &lhs() const
        {
            return d_lhs;
        }
        
        std::unique_ptr<ExpressionAST> const &rhs() const
        {
            return d_rhs;
        }
        
    private:
        
        int d_op;
        
        std::unique_ptr<ExpressionAST> d_lhs;
        
        std::unique_ptr<ExpressionAST> d_rhs;
        
    };
    
    template <class T>
    class ConstExpressionAST
    : public ExpressionAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        ConstExpressionAST(T const &value)
        : d_value(value)
        {
        }
        
        T const &value() const
        {
            return d_value;
        }
        
    private:
        
        T d_value;
        
    };
    
    class VarExpressionAST
    : public ExpressionAST
    {
    public:

        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        VarExpressionAST(std::unique_ptr<IdentifierAST> &&name)
        : d_name(std::move(name))
        {
        }
        
        std::unique_ptr<IdentifierAST> const &name() const
        {
            return d_name;
        }
        
    private:
        
        std::unique_ptr<IdentifierAST> d_name;
        
    };

    class CallExpressionAST
    : public ExpressionAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        CallExpressionAST(std::unique_ptr<IdentifierAST> &&name,
                          std::vector<std::unique_ptr<ExpressionAST>> &&args)
        : d_name(std::move(name)), d_args(std::move(args))
        {
        }
        
        std::unique_ptr<IdentifierAST> const &name() const
        {
            return d_name;
        }
        
        std::vector<std::unique_ptr<ExpressionAST>> const &args() const
        {
            return d_args;
        }
        
    private:
        
        std::unique_ptr<IdentifierAST> d_name;
        
        std::vector<std::unique_ptr<ExpressionAST>> d_args;
        
    };

    
}

#endif /* ExpressionAST_hpp */
