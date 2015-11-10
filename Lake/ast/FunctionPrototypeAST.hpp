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
    class TypeAndNameAST;
    
    class FunctionPrototypeAST
    : public BaseAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
                
        FunctionPrototypeAST(size_t lineNumber,
                             std::unique_ptr<TypeAndNameAST> &&typeAndName,
                             std::vector<std::unique_ptr<TypeAndNameAST>> &&args);
        
        virtual ~FunctionPrototypeAST();
        
        std::unique_ptr<TypeAndNameAST> const &typeAndName() const;
        
        std::vector<std::unique_ptr<TypeAndNameAST>> const &args() const;
        
    private:
        
        std::unique_ptr<TypeAndNameAST> d_typeAndName;
        
        std::vector<std::unique_ptr<TypeAndNameAST>> d_args;
        
    };
    

    
}

#endif /* FunctionPrototypeAST_hpp */
