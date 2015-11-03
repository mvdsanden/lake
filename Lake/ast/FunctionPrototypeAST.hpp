//
//  FunctionPrototypeAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef FunctionPrototypeAST_hpp
#define FunctionPrototypeAST_hpp

#include "BaseAST.hpp"
#include "ASTVisitor.hpp"

#include <vector>
#include <memory>

namespace lake {
    
    class IdentifierAST;
    
    class FunctionPrototypeAST
    : public BaseAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
                
        FunctionPrototypeAST(std::unique_ptr<IdentifierAST> &&name,
                             std::vector<std::unique_ptr<IdentifierAST>> &&args);
        
        virtual ~FunctionPrototypeAST();
        
        std::unique_ptr<IdentifierAST> const &name() const;
        
        std::vector<std::unique_ptr<IdentifierAST>> const &args() const;
        
    private:
        
        std::unique_ptr<IdentifierAST> d_name;
        
        std::vector<std::unique_ptr<IdentifierAST>> d_args;
        
    };
    
}

#endif /* FunctionPrototypeAST_hpp */
