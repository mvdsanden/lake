//
//  NativeTypeAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/10/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef NativeTypeAST_hpp
#define NativeTypeAST_hpp

#include "TypeAST.hpp"
#include "ASTVisitor.hpp"

namespace lake {
    
    template <class T, size_t TypeID>
    class NativeTypeAST
    : public TypeAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        typedef T ValueType;
        const size_t ID = TypeID;
        
        NativeTypeAST(size_t lineNumber)
        : TypeAST(lineNumber)
        {
        }
        
        virtual size_t typeId() const
        {
            return ID;
        }
        
    private:
    };
    
}

#endif /* NativeTypeAST_hpp */
