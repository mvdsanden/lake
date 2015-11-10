//
//  FunctionBlockAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef FunctionBlockAST_hpp
#define FunctionBlockAST_hpp

#include "BaseAST.hpp"
#include "ExpressionAST.hpp"
#include "IdentifierAST.hpp"
#include "ASTVisitor.hpp"

#include <vector>

namespace lake {
    
    class ExpressionAST;
    
    class FunctionBlockAST
    : public BaseAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        FunctionBlockAST(size_t lineNumber, std::vector<std::unique_ptr<ExpressionAST>> &&expressions)
        : BaseAST(lineNumber), d_expressions(std::move(expressions))
        {
        }
        
        std::vector<std::unique_ptr<ExpressionAST>> const &expressions() const
        {
            return d_expressions;
        }
        
    private:
        
        std::vector<std::unique_ptr<ExpressionAST>> d_expressions;
        
    };
}

#endif /* FunctionBlockAST_hpp */
