//
//  FunctionDefAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef FunctionDefAST_hpp
#define FunctionDefAST_hpp

#include "BaseAST.hpp"
#include "FunctionPrototypeAST.hpp"
#include "FunctionBlockAST.hpp"

namespace lake {
    class FunctionDefAST
    : public BaseAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);

        FunctionDefAST(std::unique_ptr<FunctionPrototypeAST> &&prototype,
                       std::unique_ptr<FunctionBlockAST> &&block);
        
        std::unique_ptr<FunctionPrototypeAST> const &prototype() const;
        
        std::unique_ptr<FunctionBlockAST> const &block() const;
        
    private:
        
        std::unique_ptr<FunctionPrototypeAST> d_prototype;
        std::unique_ptr<FunctionBlockAST> d_block;
        
    };
}

#endif /* FunctionDefAST_hpp */
