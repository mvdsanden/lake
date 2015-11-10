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

namespace lake {
    
    class TypeAST
    : public BaseAST
    {
    public:
        
        enum NativeTypes {
            NATIVE_VOID,
            NATIVE_INT_8,
            NATIVE_INT_16,
            NATIVE_INT_32,
            NATIVE_INT_64,
            NATIVE_UINT_8,
            NATIVE_UINT_16,
            NATIVE_UINT_32,
            NATIVE_UINT_64,
            NATIVE_FLOAT,
            NATIVE_DOUBLE,
            NATIVE_STRING,
        };
        
        TypeAST(size_t lineNumber)
        : BaseAST(lineNumber)
        {
        }
        
    };
    
    template <class T, size_t TypeID> class NativeTypeAST;
 
    typedef NativeTypeAST<int8_t, TypeAST::NATIVE_INT_8> TypeInt8AST;
    typedef NativeTypeAST<int16_t, TypeAST::NATIVE_INT_16> TypeInt16AST;
    typedef NativeTypeAST<int32_t, TypeAST::NATIVE_INT_32> TypeInt32AST;
    typedef NativeTypeAST<int64_t, TypeAST::NATIVE_INT_64> TypeInt64AST;
    typedef NativeTypeAST<uint8_t, TypeAST::NATIVE_UINT_8> TypeUInt8AST;
    typedef NativeTypeAST<uint16_t, TypeAST::NATIVE_UINT_16> TypeUInt16AST;
    typedef NativeTypeAST<uint32_t, TypeAST::NATIVE_UINT_32> TypeUInt32AST;
    typedef NativeTypeAST<uint64_t, TypeAST::NATIVE_UINT_64> TypeUInt64AST;
    typedef NativeTypeAST<float, TypeAST::NATIVE_FLOAT> TypeFloatAST;
    typedef NativeTypeAST<double, TypeAST::NATIVE_DOUBLE> TypeDoubleAST;
    typedef NativeTypeAST<std::string, TypeAST::NATIVE_STRING> TypeStringAST;
    
}

#endif /* TypeAST_hpp */
