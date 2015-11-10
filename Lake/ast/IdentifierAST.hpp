//
//  IdentifierAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef IdentifierAST_hpp
#define IdentifierAST_hpp

#include "BaseAST.hpp"
#include "ASTVisitor.hpp"

#include <string>

namespace lake {
    class IdentifierAST
    : public BaseAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
                
        IdentifierAST(size_t lineNumber, std::string const &name);
        
        virtual ~IdentifierAST();
        
        std::string const &name() const;
        
    private:
        
        std::string d_name;
        
    };
}

#endif /* IdentifierAST_hpp */
