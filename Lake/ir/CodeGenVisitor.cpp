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

#include <unordered_map>
#include <stack>

#include <iostream>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"

namespace lake {
    class CodeGenVisitorImpl
    : public CodeGenVisitor
    {
        
        class Scope
        {
            Scope *d_parent;
            typedef std::unordered_map<std::string, llvm::Value *> NamedValuesMap;
            typedef std::unordered_map<std::string, llvm::Function *> NamedFunctionsMap;
            NamedValuesMap d_namedValuesMap;
            NamedFunctionsMap d_namedFunctionsMap;
        public:
            
            Scope(Scope *parent = nullptr)
            : d_parent(parent)
            {
            }
            
            ~Scope()
            {
                for (auto &i : d_namedValuesMap) {
                    delete i.second;
                }
            }
            
            llvm::Value *getNamedValue(std::string const &name)
            {
                auto i = d_namedValuesMap.find(name);
                if (i != d_namedValuesMap.end()) {
                    return i->second;
                }
                return (d_parent != nullptr?d_parent->getNamedValue(name):nullptr);
            }
            
            bool insertNamedValue(std::string const &name, llvm::Value *value)
            {
                return d_namedValuesMap.insert(std::make_pair(name, value)).second;
            }

            llvm::Function *getNamedFunction(std::string const &name)
            {
                auto i = d_namedFunctionsMap.find(name);
                if (i != d_namedFunctionsMap.end()) {
                    return i->second;
                }
                return (d_parent != nullptr?d_parent->getNamedFunction(name):nullptr);
            }
            
            bool insertNamedFunction(std::string const &name, llvm::Function *funct)
            {
                return d_namedFunctionsMap.insert(std::make_pair(name, funct)).second;
            }
            
        };
        
        std::shared_ptr<llvm::Module> d_module;
        
        Scope d_scope;
        
        static void error(BaseAST const *node, std::string const &msg)
        {
            std::cerr << "error " << node->lineNumber() << ": " << msg << ".\n";
        }
        
    public:
        
        CodeGenVisitorImpl(std::shared_ptr<llvm::Module> const &module)
        : d_module(module)
        {
        }
        
        virtual ~CodeGenVisitorImpl()
        {
        }
        
        class ValueVisitor
        : public ASTVisitor
        {
            
            llvm::Value *d_value;
            
        public:
            
            llvm::Value *value()
            {
                return d_value;
            }
            
            void visit(ConstValueAST<double> const *node)
            {
                d_value = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(node->value()));
            }
            
            void visit(ConstValueAST<std::string> const *node)
            {
                d_value = llvm::ConstantDataArray::getString(llvm::getGlobalContext(), node->value());
            }
            
            void visit(ConstValueAST<int> const *node)
            {
                d_value = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, node->value(), true));
            }
            
        };
        
        
        void visit(IdentifierAST const *node)
        {
            
        }
        
        void visit(ConstDefAST const *node)
        {
            ValueVisitor vis;
            node->value()->accept(&vis);
            d_scope.insertNamedValue(node->name()->name(), vis.value());
        }
        
        class ExpressionVisitor
        : public ASTVisitor
        {
            Scope &d_scope;
            llvm::IRBuilder<> &d_builder;
            llvm::Value *d_value;
        public:
            
            ExpressionVisitor(Scope &scope, llvm::IRBuilder<> &builder)
            : d_scope(scope), d_builder(builder)
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
                
                switch (node->op()) {
                    case '+': d_builder.CreateAdd(lhs.d_value, rhs.d_value); break;
                    case '-': d_builder.CreateSub(lhs.d_value, rhs.d_value); break;
                    case '*': d_builder.CreateMul(lhs.d_value, rhs.d_value); break;
                    case '/': d_builder.CreateSDiv(lhs.d_value, rhs.d_value); break;
                        
                    default:
                        error(node, "unknown binary operator");
                        break;
                }
            }
            
            void visit(ConstExpressionAST<double> const *node)
            {
                d_value = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(node->value()));
            }
            
            void visit(ConstExpressionAST<std::string> const *node)
            {
                d_value = llvm::ConstantDataArray::getString(llvm::getGlobalContext(), node->value());
            }
            
            void visit(ConstExpressionAST<int> const *node)
            {
                d_value = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, node->value(), true));
            }
            
            void visit(VarExpressionAST const *node)
            {
                d_value = d_scope.getNamedValue(node->name()->name());
                
                if (d_value == nullptr) {
                    error(node, node->name()->name() + ": unknown variable");
                }
            }
            
            void visit(CallExpressionAST const *node)
            {
                llvm::Function *func = d_scope.getNamedFunction(node->name()->name());
                
                if (func == nullptr) {
                    error(node, node->name()->name() + ": unknown function");
                }
                
                if (func->arg_size() != node->args().size()) {
                    error(node, node->name()->name() + ": incorrect number of arguments");
                }
                
                std::vector<llvm::Value*> args;
                for (auto &i : node->args()) {
                    ExpressionVisitor vis(d_scope, d_builder);
                    i->accept(&vis);
                    args.push_back(vis.d_value);
                }
                
                d_value = d_builder.CreateCall(func, args, "calltmp");
            }
            
        };
        
        class BlockVisitor
        : public ASTVisitor
        {
            llvm::IRBuilder<> d_builder;
            
            llvm::Function *d_function;
            
            Scope d_scope;
            
        public:
            
            BlockVisitor(Scope &parentScope, llvm::Function *function)
            : d_scope(parentScope), d_builder(llvm::getGlobalContext()), d_function(function)
            {
                llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", function);
                d_builder.SetInsertPoint(BB);
                
                // Record the function arguments in the NamedValues map.
                for (auto &Arg : d_function->args()) {
                    d_scope.insertNamedValue(Arg.getName(), &Arg);
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
            
            void visit(ConstExpressionAST<int> const *node)
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

            
        };
        
        class FunctionVisitor
        : public ASTVisitor
        {
            
            Scope &d_scope;
            
            llvm::Function *d_function;
            
        public:
            
            FunctionVisitor(Scope &scope)
            : d_scope(scope) {}
            
            void visit(FunctionPrototypeAST const *node)
            {
                
            }
            
            void visit(FunctionBlockAST const *node)
            {
                BlockVisitor vis(d_scope, d_function);
                
                for (auto &i : node->expressions()) {
                    i->accept(&vis);
                }
            }
            
        };
        
        void visit(FunctionDefAST const *node)
        {
            FunctionVisitor vis(d_scope);
            node->prototype()->accept(&vis);
            node->block()->accept(&vis);
        }
    
        
        void visit(ReturnExpressionAST const *node)
        {
            
        }
        
    };
    
    
    std::shared_ptr<CodeGenVisitor> CodeGenVisitor::create(std::shared_ptr<llvm::Module> const &module)
    {
        return std::make_shared<CodeGenVisitorImpl>(module);
    }
}

