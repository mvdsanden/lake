//
//  CodeGenVisitor.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "CodeGenVisitor.hpp"

#include "ConstDefAST.hpp"
#include "FunctionBlockAST.hpp"
#include "FunctionPrototypeAST.hpp"
#include "FunctionDefAST.hpp"
#include "TypeAST.hpp"

#include <unordered_map>
#include <stack>
#include <memory>
#include <iostream>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"

namespace lake {
    class CodeGenVisitorImpl
    : public CodeGenVisitor
    {
        
        class Value
        {
            TypeInfo const *d_typeInfo;
            llvm::Value *d_value;
        public:
            
            Value(TypeInfo const *typeInfo, llvm::Value *value)
            : d_typeInfo(typeInfo), d_value(value)
            {
                
            }
            
            ~Value()
            {
            }
            
            TypeInfo const *typeInfo() const
            {
                return d_typeInfo;
            }
            
            llvm::Value *value() const
            {
                return d_value;
            }
            
        };

        class Function
        {
            TypeInfo const *d_typeInfo;
            llvm::Function *d_function;
            std::vector<std::pair<std::string, TypeInfo const *>> d_args;
        public:
            
            Function(TypeInfo const *typeInfo,
                     llvm::Function *function,
                     std::vector<std::pair<std::string, TypeInfo const *>> &&args)
            : d_typeInfo(typeInfo), d_function(function), d_args(std::move(args))
            {
            }
            
            ~Function()
            {
            }
            
            TypeInfo const *typeInfo() const
            {
                return d_typeInfo;
            }
            
            llvm::Function *function() const
            {
                return d_function;
            }
            
            std::vector<std::pair<std::string, TypeInfo const *>> const &args() const
            {
                return d_args;
            }
            
        };
        
        class Scope
        {
            Scope *d_parent;
            typedef std::unordered_map<std::string, std::shared_ptr<Value>> NamedValuesMap;
            typedef std::unordered_map<std::string, std::shared_ptr<Function>> NamedFunctionsMap;
            NamedValuesMap d_namedValuesMap;
            NamedFunctionsMap d_namedFunctionsMap;
        public:
            
            Scope(Scope *parent = nullptr)
            : d_parent(parent)
            {
            }
            
            ~Scope()
            {
                /*
                for (auto &i : d_namedValuesMap) {
                    if (i.second != nullptr) {
                        delete i.second;
                    }
                }
                 */
            }
            
            Value *getNamedValue(std::string const &name)
            {
                auto i = d_namedValuesMap.find(name);
                if (i != d_namedValuesMap.end()) {
                    return i->second.get();
                }
                return (d_parent != nullptr?d_parent->getNamedValue(name):nullptr);
            }
            
            bool insertNamedValue(std::string const &name, Value *value)
            {
                return d_namedValuesMap.insert(std::make_pair(name, std::shared_ptr<Value>(value))).second;
            }

            Function *getNamedFunction(std::string const &name)
            {
                auto i = d_namedFunctionsMap.find(name);
                if (i != d_namedFunctionsMap.end()) {
                    return i->second.get();
                }
                return (d_parent != nullptr?d_parent->getNamedFunction(name):nullptr);
            }
            
            bool insertNamedFunction(std::string const &name, Function *funct)
            {
                return d_namedFunctionsMap.insert(std::make_pair(name, std::shared_ptr<Function>(funct))).second;
            }
            
        };
        
        std::shared_ptr<llvm::Module> d_module;
        
        llvm::IRBuilder<> d_builder;
        
        Scope d_scope;
        
        static void error(BaseAST const *node, std::string const &msg)
        {
            std::cerr << "error " << node->lineNumber() << ": " << msg << ".\n";
        }
        
    public:
        
        CodeGenVisitorImpl(std::shared_ptr<llvm::Module> const &module)
        : d_module(module), d_builder(llvm::getGlobalContext())
        {
        }
        
        virtual ~CodeGenVisitorImpl()
        {
        }
        
        class ValueVisitor
        : public ASTVisitor
        {
         
            TypeInfo const *d_typeInfo;
            llvm::Value *d_value;
            
        public:
            
            ValueVisitor(TypeInfo const *typeInfo)
            : d_typeInfo(typeInfo), d_value(nullptr)
            {
            }
            
            llvm::Value *value()
            {
                return d_value;
            }
            
            void visit(ConstValueAST<double> const *node)
            {
                d_value = d_typeInfo->createLLVMConstant(node->value());
            }
            
            void visit(ConstValueAST<std::string> const *node)
            {
                d_value = d_typeInfo->createLLVMConstant(node->value());
            }
            
            void visit(ConstValueAST<int8_t> const *node)
            {
                d_value = d_typeInfo->createLLVMConstant(static_cast<int64_t>(node->value()));
            }

            void visit(ConstValueAST<int64_t> const *node)
            {
                d_value = d_typeInfo->createLLVMConstant(node->value());
            }

            void visit(ConstValueAST<uint64_t> const *node)
            {
                d_value = d_typeInfo->createLLVMConstant(node->value());
            }
            
        };
        
        class TypeVisitor
        : public ASTVisitor
        {
            llvm::Type *d_type;
            TypeInfo const *d_typeInfo;
        public:
            
            TypeVisitor()
            : d_type(nullptr), d_typeInfo(nullptr)
            {
            }
            
            virtual void visit(TypeAST const *node)
            {
                d_typeInfo = node->typeInfo();
                d_type = d_typeInfo->getLLVMType();
            }
            
            llvm::Type *type() const
            {
                return d_type;
            }
            
            TypeInfo const *typeInfo() const
            {
                return d_typeInfo;
            }
            
        };
        
        void visit(ConstDefAST const *node)
        {
            TypeVisitor typeVis;
            node->typeAndName()->type()->accept(&typeVis);
            
            ValueVisitor valueVis(typeVis.typeInfo());
            node->value()->accept(&valueVis);
            
            /*
            llvm::Value *value = d_builder.CreateAlloca(typeVis.type());
            
            if (value == nullptr) {
                error(node, "error creating an allocated valued");
            }
            
            // For now a mutated variable.
            // TODO: make const definition.
            d_builder.CreateStore(value, valueVis.value());
            */
            
            d_scope.insertNamedValue(node->typeAndName()->name()->name(), new Value(typeVis.typeInfo(), valueVis.value()));
        }
        
        class ExpressionVisitor
        : public ASTVisitor
        {
            Scope &d_scope;
            llvm::IRBuilder<> &d_builder;
            llvm::Value *d_value;
            TypeInfo const *d_typeInfo;
        public:
            
            ExpressionVisitor(Scope &scope, llvm::IRBuilder<> &builder)
            : d_scope(scope), d_builder(builder), d_value(nullptr), d_typeInfo(nullptr)
            {
            }
            
            virtual ~ExpressionVisitor()
            {
            }
            
            void visit(BinOpExpressionAST const *node)
            {
                ExpressionVisitor lhs(d_scope, d_builder), rhs(d_scope, d_builder);
                
                node->lhs()->accept(&lhs);
                node->rhs()->accept(&rhs);
                
                d_typeInfo = lhs.d_typeInfo;
                
                switch (node->op()) {
                    case '+': d_value = d_builder.CreateAdd(lhs.d_value, rhs.d_value); break;
                    case '-': d_value = d_builder.CreateSub(lhs.d_value, rhs.d_value); break;
                    case '*': d_value = d_builder.CreateMul(lhs.d_value, rhs.d_value); break;
                    case '/':
                        if (lhs.d_value->getType()->isFPOrFPVectorTy()) {
                            d_value = d_builder.CreateFDiv(lhs.d_value, rhs.d_value, "div");
                        } else if (d_typeInfo->isSigned()) {
                            d_value = d_builder.CreateSDiv(lhs.d_value, rhs.d_value, "div");
                        } else {
                            d_value = d_builder.CreateUDiv(lhs.d_value, rhs.d_value, "div");
                        }
                        break;
                        
                    default:
                        error(node, "unknown binary operator");
                        break;
                }
            }
            
#define LAKE_CODEGEN_VISITOR_EXPRESSION_VISITOR_CONST_EXPRESSION(TYPE)\
            void visit(ConstExpressionAST<TYPE> const *node)\
            {\
                d_typeInfo = node->typeInfo();\
                d_value = d_typeInfo->createLLVMConstant(node->value());\
            }
            
            LAKE_ALL_CONSTANT_TYPES(LAKE_CODEGEN_VISITOR_EXPRESSION_VISITOR_CONST_EXPRESSION);
            
            void visit(VarExpressionAST const *node)
            {
                Value *value = d_scope.getNamedValue(node->name()->name());
                
                if (value == nullptr) {
                    error(node, node->name()->name() + ": unknown variable");
                }
                
                d_typeInfo = value->typeInfo();
                d_value = value->value();
            }
            
            void visit(CallExpressionAST const *node)
            {
                Function *function = d_scope.getNamedFunction(node->name()->name());
                
                if (function == nullptr) {
                    error(node, node->name()->name() + ": unknown function");
                }
                
                d_typeInfo = function->typeInfo();
                
                llvm::Function *func = function->function();
                
                if (func->arg_size() != node->args().size()) {
                    error(node, node->name()->name() + ": incorrect number of arguments");
                }
                
                // TODO: function argument type checking and implicit casts!
                std::vector<llvm::Value*> args;
                for (auto &i : node->args()) {
                    ExpressionVisitor vis(d_scope, d_builder);
                    i->accept(&vis);
                    args.push_back(vis.d_value);
                }
                
                d_value = d_builder.CreateCall(func, args, "calltmp");
            }
            
            void visit(ReturnExpressionAST const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->rhs()->accept(&vis);
                d_typeInfo = vis.d_typeInfo;
                d_builder.CreateRet(vis.d_value);
            }
            
        };
        
        class BlockVisitor
        : public ASTVisitor
        {
            llvm::IRBuilder<> &d_builder;
            
            Function *d_function;
            
            Scope d_scope;
            
        public:
            
            BlockVisitor(Scope &parentScope, llvm::IRBuilder<> &builder, Function *function)
            : d_scope(parentScope), d_builder(builder), d_function(function)
            {
                llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", function->function());
                d_builder.SetInsertPoint(BB);
                
                // Record the function arguments in the NamedValues map.
                
                auto i = d_function->args().begin();
                for (auto j = d_function->function()->args().begin(); i != d_function->args().end() && j != d_function->function()->args().end(); ++i, ++j) {
                    d_scope.insertNamedValue(i->first, new Value(i->second, &*j));
                }
                
            }
            
            virtual ~BlockVisitor()
            {
                // TODO: verify and create return value (close block?).
            }

            void visit(BinOpExpressionAST const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }
            
            void visit(ConstExpressionAST<int8_t> const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }

            void visit(ConstExpressionAST<int64_t> const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }

            void visit(ConstExpressionAST<uint64_t> const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }

            
            void visit(ConstExpressionAST<double> const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }
            
            void visit(ConstExpressionAST<std::string> const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }
            
            void visit(VarExpressionAST const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }
            
            void visit(CallExpressionAST const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }
            
            void visit(ReturnExpressionAST const *node)
            {
                ExpressionVisitor vis(d_scope, d_builder);
                node->accept(&vis);
            }
            
        };
        
        class FunctionVisitor
        : public ASTVisitor
        {
            
            Scope &d_scope;
            
            llvm::IRBuilder<> &d_builder;
            
            std::shared_ptr<llvm::Module> &d_module;
            
            Function *d_function;
            
        public:
            
            FunctionVisitor(Scope &scope,
                            std::shared_ptr<llvm::Module> &module,
                            llvm::IRBuilder<> &builder)
            : d_scope(scope), d_module(module), d_builder(builder) {}
            
            void visit(FunctionPrototypeAST const *node)
            {
                TypeVisitor typeVis;
                node->typeAndName()->type()->accept(&typeVis);
                
                std::vector<llvm::Type*> llvmTypes;
                std::vector<std::pair<std::string, TypeInfo const *>> args;
                
                for (auto &i : node->args()) {
                    TypeVisitor vis;
                    i->type()->accept(&vis);
                    args.push_back(std::make_pair(i->name()->name(), vis.typeInfo()));
                    llvmTypes.push_back(vis.type());
                }
                
                llvm::FunctionType *type = llvm::FunctionType::get(typeVis.type(), llvmTypes, false);
                
                llvm::Function *func = llvm::Function::Create(type,
                                                              llvm::Function::ExternalLinkage,
                                                              node->typeAndName()->name()->name(),
                                                              d_module.get());
                
                d_function = new Function(typeVis.typeInfo(), func, std::move(args));
                
                size_t idx = 0;
                for (auto &i : func->args()) {
                    i.setName(node->args()[idx++]->name()->name());
                }
                
                d_scope.insertNamedFunction(node->typeAndName()->name()->name(), d_function);
            }
            
            void visit(FunctionBlockAST const *node)
            {
                BlockVisitor vis(d_scope, d_builder, d_function);
                
                for (auto &i : node->expressions()) {
                    i->accept(&vis);
                }
            }
            
            Function *function() const
            {
                return d_function;
            }
            
        };
        
        void visit(FunctionDefAST const *node)
        {
            FunctionVisitor vis(d_scope, d_module, d_builder);
            node->prototype()->accept(&vis);
            node->block()->accept(&vis);
        }
    
        
        void visit(ReturnExpressionAST const *node)
        {
            error(node, "return expression illegal outside of function block definition");
        }
        
    };
    
    
    std::shared_ptr<CodeGenVisitor> CodeGenVisitor::create(std::shared_ptr<llvm::Module> const &module)
    {
        return std::make_shared<CodeGenVisitorImpl>(module);
    }
}

