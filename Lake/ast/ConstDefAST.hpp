//
//  ConstDefAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ConstDefAST_hpp
#define ConstDefAST_hpp

#include "ConstAST.hpp"
#include "IdentifierAST.hpp"

namespace lake {
    
    class ConstDefAST
    : public BaseAST
    {
    public:

        virtual void accept(ASTVisitor *visitor)
        {
            visitor->visit(this);
        }
        
        ConstDefAST(std::unique_ptr<IdentifierAST>  &&name,
                    std::unique_ptr<ConstAST> &&value);
        
        virtual ~ConstDefAST();

        std::unique_ptr<IdentifierAST> const &name() const;
        
        std::unique_ptr<ConstAST> const &value() const;
        
    private:
        std::unique_ptr<IdentifierAST> d_name;
        std::unique_ptr<ConstAST> d_value;
    };
    
}

#endif /* ConstDefAST_hpp */
