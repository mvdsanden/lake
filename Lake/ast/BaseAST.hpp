//
//  BaseAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef BaseAST_hpp
#define BaseAST_hpp

#include <memory>

namespace lake {
    
    class ASTVisitor;
    
    class BaseAST {
    public:
        
        virtual ~BaseAST() {}
        
        virtual void accept(ASTVisitor *visitor) = 0;
        
        
    };
}

#endif /* BaseAST_hpp */
