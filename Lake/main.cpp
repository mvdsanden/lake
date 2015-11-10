//
//  main.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Lexer.hpp"
#include "Parser.hpp"
#include "ASTPrintVisitor.hpp"
#include "CodeGenVisitor.hpp"

#include "llvm/IR/LLVMContext.h"

#include <iostream>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    lake::Lexer ll(STDIN_FILENO);
    
    /*
    ll.next();
    
    while (ll.token() != lake::Lexer::TOK_EOF) {
        
        std::cout << "Token: " << ll.token() << " \"" << ll.stringValue() << "\"\n.";
        
        ll.next();
    }
     */
    auto module = std::make_shared<llvm::Module>("my cool jit", llvm::getGlobalContext());

    auto logVisitor = lake::ASTPrintVisitor::create(std::cout);
    auto codeGenVisitor = lake::CodeGenVisitor::create(module);
    
    while (true) {
        
        auto ast = lake::Parser::instance().parse(ll);
        
        if (!ast) {
            break;
        }
            

            
        ast->accept(logVisitor.get());
        ast->accept(codeGenVisitor.get());

        std::cout.flush();
        
    }
    
    module->dump();
    
    return 0;
}
