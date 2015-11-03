//
//  ASTPrintVisitor.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ASTPrintVisitor_hpp
#define ASTPrintVisitor_hpp

#include "ASTVisitor.hpp"

#include <iosfwd>

namespace lake {
    
    class ASTPrintVisitor
    : public ASTVisitor
    {
    public:
        
        static std::unique_ptr<ASTPrintVisitor> create(std::ostream &stream);
        
    };
}

#endif /* ASTPrintVisitor_hpp */
