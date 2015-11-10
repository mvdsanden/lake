//
//  TypeAndNameAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/10/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef TypeAndNameAST_hpp
#define TypeAndNameAST_hpp

#include "BaseAST.hpp"
#include "ASTVisitor.hpp"

namespace lake {
    
    class TypeAST;
    
    class TypeAndNameAST
    : public BaseAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        TypeAndNameAST(size_t lineNumber,
                       std::unique_ptr<TypeAST> &&type,
                       std::unique_ptr<IdentifierAST> &&name);
        
        virtual ~TypeAndNameAST();
        
        std::unique_ptr<TypeAST> const &type() const;
        
        std::unique_ptr<IdentifierAST> const &name() const;
        
    private:
        
        std::unique_ptr<TypeAST> d_type;
        
        std::unique_ptr<IdentifierAST> d_name;
        
    };
    
}

#endif /* TypeAndNameAST_hpp */
