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
    
    while (true) {
        
        auto ast = lake::Parser::instance().parse(ll);
        
        if (!ast) {
            break;
        }
            
        auto visitor = lake::ASTPrintVisitor::create(std::cout);
            
        ast->accept(visitor.get());

    }
    
    return 0;
}
