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
        
        BaseAST(size_t lineNumber)
        : d_lineNumber(lineNumber)
        {
        }
        
        virtual ~BaseAST() {}
        
        virtual void accept(ASTVisitor *visitor) const = 0;
        
        size_t lineNumber() const
        {
            return d_lineNumber;
        }
    
        
    private:
        
        size_t d_lineNumber;
        
    };
}

#endif /* BaseAST_hpp */
