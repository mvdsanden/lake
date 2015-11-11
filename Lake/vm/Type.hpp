//
//  Type.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/11/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Type_hpp
#define Type_hpp

#include <cstddef>
#include <cstdint>

#include <string>
#include <memory>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

#define LAKE_ALL_CONSTANT_TYPES(x)\
x(int8_t);\
x(int64_t);\
x(uint64_t);\
x(double);\
x(std::string)

namespace lake {
    
    enum NativeTypes {
        NATIVE_TYPE_VOID,
        NATIVE_TYPE_INT_8,
        NATIVE_TYPE_INT_16,
        NATIVE_TYPE_INT_32,
        NATIVE_TYPE_INT_64,
        NATIVE_TYPE_UINT_8,
        NATIVE_TYPE_UINT_16,
        NATIVE_TYPE_UINT_32,
        NATIVE_TYPE_UINT_64,
        NATIVE_TYPE_FLOAT,
        NATIVE_TYPE_DOUBLE,
        NATIVE_TYPE_STRING,
    };
    
    template <class _T>
    struct TypeTraits {
        typedef _T InternalType;
        static const size_t typeId = NATIVE_TYPE_VOID;
        static const size_t internalByteSize = 0;
        static const size_t bitSize = 0;
        static const bool isVoid = true;
        static const bool isInteger = false;
        static const bool isSigned = false;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getVoidTy(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return nullptr; }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return nullptr; }
        static llvm::Value *createLLVMConstant(double value)
        { return nullptr; }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return nullptr; }
    };
    
    class TypeInfo
    {
    public:
        
        virtual size_t internalByteSize() const = 0;
        
        virtual size_t bitSize() const = 0;
        
        virtual bool isSigned() const = 0;
        
        virtual bool isInteger() const = 0;
        
        virtual bool isFloatingPoint() const = 0;
        
        virtual llvm::Type *getLLVMType() const = 0;
        
        virtual llvm::Value *createLLVMConstant(int8_t value) const
        {
            return createLLVMConstant(static_cast<int64_t>(value));
        }
        
        virtual llvm::Value *createLLVMConstant(int64_t value) const = 0;
        
        virtual llvm::Value *createLLVMConstant(uint64_t value) const = 0;
        
        virtual llvm::Value *createLLVMConstant(double value) const = 0;
        
        virtual llvm::Value *createLLVMConstant(std::string const &value) const = 0;
        
        template <class T>
        static TypeInfo const *getTypeInfo();
        
    };
    
    template <class _Traits>
    class NativeTypeInfo
    : public TypeInfo
    {
    public:
        
        virtual size_t internalByteSize() const
        {
            return _Traits::internalByteSize;
        }
        
        virtual size_t bitSize() const
        {
            return _Traits::bitSize;
        }
        
        virtual bool isSigned() const
        {
            return _Traits::isSigned;
        }
        
        virtual bool isInteger() const
        {
            return _Traits::isInteger;
        }
        
        virtual bool isFloatingPoint() const
        {
            return _Traits::isFloatingPoint;
        }
        
        virtual llvm::Type *getLLVMType() const
        {
            return _Traits::getLLVMType();
        }
        
        virtual llvm::Value *createLLVMConstant(int64_t value) const
        {
            return _Traits::createLLVMConstant(value);
        }
        
        virtual llvm::Value *createLLVMConstant(uint64_t value) const
        {
            return _Traits::createLLVMConstant(value);
        }
        
        virtual llvm::Value *createLLVMConstant(double value) const
        {
            return _Traits::createLLVMConstant(value);
        }
        
        virtual llvm::Value *createLLVMConstant(std::string const &value) const
        {
            return _Traits::createLLVMConstant(value);
        }

        
    };
    
    template <>
    struct TypeTraits<int8_t> {
        typedef int8_t InternalType;
        static const size_t typeId = NATIVE_TYPE_INT_8;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 8;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = true;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt8Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtol(value.c_str(), nullptr, 10), isSigned)); }
    };

    template <>
    struct TypeTraits<int16_t> {
        typedef int16_t InternalType;
        static const size_t typeId = NATIVE_TYPE_INT_16;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 16;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = true;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt16Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtol(value.c_str(), nullptr, 10), isSigned)); }
    };

    template <>
    struct TypeTraits<int32_t> {
        typedef int32_t InternalType;
        static const size_t typeId = NATIVE_TYPE_INT_32;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 32;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = true;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt32Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtol(value.c_str(), nullptr, 10), isSigned)); }
    };

    template <>
    struct TypeTraits<int64_t> {
        typedef int64_t InternalType;
        static const size_t typeId = NATIVE_TYPE_INT_64;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 64;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = true;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt64Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtoll(value.c_str(), nullptr, 10), isSigned)); }
    };

    template<>
    struct TypeTraits<uint8_t> {
        typedef uint8_t InternalType;
        static const size_t typeId = NATIVE_TYPE_UINT_8;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 8;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = false;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt8Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtoull(value.c_str(), nullptr, 10), isSigned)); }
    };
    
    template <>
    struct TypeTraits<uint16_t> {
        typedef uint16_t InternalType;
        static const size_t typeId = NATIVE_TYPE_UINT_16;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 16;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = false;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt16Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtoull(value.c_str(), nullptr, 10), isSigned)); }
    };
    
    template <>
    struct TypeTraits<uint32_t> {
        typedef uint32_t InternalType;
        static const size_t typeId = NATIVE_TYPE_UINT_32;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 32;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = false;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt32Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtoull(value.c_str(), nullptr, 10), isSigned)); }
    };
    
    template <>
    struct TypeTraits<uint64_t> {
        typedef uint64_t InternalType;
        static const size_t typeId = NATIVE_TYPE_UINT_64;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 64;
        static const bool isVoid = false;
        static const bool isInteger = true;
        static const bool isSigned = false;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt64Ty(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, value, isSigned)); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(bitSize, strtoull(value.c_str(), nullptr, 10), isSigned)); }
    };
    
    template <>
    struct TypeTraits<float> {
        typedef float InternalType;
        static const size_t typeId = NATIVE_TYPE_FLOAT;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 4;
        static const bool isVoid = false;
        static const bool isInteger = false;
        static const bool isSigned = false;
        static const bool isFloatingPoint = true;
        static llvm::Type *getLLVMType() { return llvm::Type::getFloatTy(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(static_cast<float>(value))); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(static_cast<float>(value))); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(static_cast<float>(value))); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(strtof(value.c_str(), nullptr))); }
    };

    // TODO: long double?
    template <>
    struct TypeTraits<double> {
        typedef double InternalType;
        static const size_t typeId = NATIVE_TYPE_DOUBLE;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = 8;
        static const bool isVoid = false;
        static const bool isInteger = false;
        static const bool isSigned = false;
        static const bool isFloatingPoint = true;
        static llvm::Type *getLLVMType() { return llvm::Type::getDoubleTy(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(static_cast<float>(value))); }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(static_cast<float>(value))); }
        static llvm::Value *createLLVMConstant(double value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(static_cast<float>(value))); }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(strtof(value.c_str(), nullptr))); }
    };
    
    template <>
    struct TypeTraits<std::string> {
        typedef float InternalType;
        static const size_t typeId = NATIVE_TYPE_STRING;
        static const size_t internalByteSize = sizeof(InternalType);
        static const size_t bitSize = sizeof(void*) * 8;
        static const bool isVoid = false;
        static const bool isInteger = false;
        static const bool isSigned = false;
        static const bool isFloatingPoint = false;
        static llvm::Type *getLLVMType() { return llvm::Type::getInt8PtrTy(llvm::getGlobalContext()); }
        static llvm::Value *createLLVMConstant(int64_t value)
        { return nullptr; }
        static llvm::Value *createLLVMConstant(uint64_t value)
        { return nullptr; }
        static llvm::Value *createLLVMConstant(double value)
        { return nullptr; }
        static llvm::Value *createLLVMConstant(std::string const &value)
        { return llvm::ConstantDataArray::getString(llvm::getGlobalContext(), value); }
    };
    
    template <class T>
    TypeInfo const *TypeInfo::getTypeInfo()
    {
//        static std::unique_ptr<TypeInfo> _instance(new NativeTypeInfo<TypeTraits<T>>);
        static NativeTypeInfo<TypeTraits<T>> _instance;
        return &_instance;//.get();
    }

}

#endif /* Type_hpp */
