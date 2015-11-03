//
//  Parser.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include "BaseAST.hpp"

#include <memory>

namespace lake {
    
    class Lexer;
    
    class Parser
    {
    public:
        
        static Parser &instance();
        
        virtual ~Parser() {}
        
        virtual std::unique_ptr<BaseAST> parse(Lexer &lexer) = 0;
        
    private:
        
    };
}

#endif /* Parser_hpp */
