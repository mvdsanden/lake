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
#include "TypeAndNameAST.hpp"

namespace lake {
    
    class ConstDefAST
    : public BaseAST
    {
    public:

        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        ConstDefAST(size_t lineNumber,
                    std::unique_ptr<TypeAndNameAST>  &&typeAndName,
                    std::unique_ptr<ConstAST> &&value);
        
        virtual ~ConstDefAST();

        std::unique_ptr<TypeAndNameAST> const &typeAndName() const;
        
        std::unique_ptr<ConstAST> const &value() const;
        
    private:
        std::unique_ptr<TypeAndNameAST> d_typeAndName;
        std::unique_ptr<ConstAST> d_value;
    };
    
}

#endif /* ConstDefAST_hpp */
