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
        
        ExpressionAST(size_t lineNumber)
        : BaseAST(lineNumber)
        {
        }
        
    };
    
    class BinOpExpressionAST
    : public ExpressionAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        BinOpExpressionAST(size_t lineNumber,
                           int op,
                           std::unique_ptr<ExpressionAST> &&lhs,
                           std::unique_ptr<ExpressionAST> &&rhs)
        : ExpressionAST(lineNumber), d_op(op), d_lhs(std::move(lhs)), d_rhs(std::move(rhs))
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
        
        ConstExpressionAST(size_t lineNumber, T const &value)
        : ExpressionAST(lineNumber), d_value(value)
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
        
        VarExpressionAST(size_t lineNumber, std::unique_ptr<IdentifierAST> &&name)
        : ExpressionAST(lineNumber), d_name(std::move(name))
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
        
        CallExpressionAST(size_t lineNumber,
                          std::unique_ptr<IdentifierAST> &&name,
                          std::vector<std::unique_ptr<ExpressionAST>> &&args)
        : ExpressionAST(lineNumber), d_name(std::move(name)), d_args(std::move(args))
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
    
    class ReturnExpressionAST
    : public ExpressionAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        ReturnExpressionAST(size_t lineNumber, std::unique_ptr<ExpressionAST> &&rhs)
        : ExpressionAST(lineNumber), d_rhs(std::move(rhs))
        {
            
        }
        
        std::unique_ptr<ExpressionAST> const &rhs() const
        {
            return d_rhs;
        }
        
    private:
        std::unique_ptr<ExpressionAST> d_rhs;
    };

    
}

#endif /* ExpressionAST_hpp */
