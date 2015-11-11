//
//  TypeAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/10/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef TypeAST_hpp
#define TypeAST_hpp

#include "BaseAST.hpp"
#include "Type.hpp"
#include "ASTVisitor.hpp"

namespace lake {
    
    class TypeAST
    : public BaseAST
    {
    public:
        
        TypeAST(size_t lineNumber, TypeInfo const *typeInfo)
        : BaseAST(lineNumber), d_typeInfo(typeInfo)
        {
        }
        
        virtual ~TypeAST()
        {            
        }
        
        TypeInfo const *typeInfo() const
        {
            return d_typeInfo;
        }
        
    private:
        
        TypeInfo const *d_typeInfo;
        
    };
    
    template <class _T>
    class NativeTypeAST
    : public TypeAST
    {
    public:
        
        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        NativeTypeAST(size_t lineNumber)
        : TypeAST(lineNumber, TypeInfo::getTypeInfo<_T>())
        {
        }
        
    };
    
}

#endif /* TypeAST_hpp */
