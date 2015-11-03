//
//  Lexer.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#include <string>
#include <unordered_map>

namespace lake {
    
    class Lexer {
    public:
        
        enum Token {
            
            TOK_EOF = -1000,
            TOK_CONST,
            TOK_FUNC,
            TOK_VAR,
            
            TOK_CONST_DOUBLE,
            TOK_CONST_STRING,
            TOK_CONST_LITERAL,
            
            TOK_IDENTIFIER,
            
        };

        Lexer(int fd);
        
        ~Lexer();
        
        void next();
        
        int token() const;
        
        double doubleValue() const;
        
        std::string stringValue() const;
        
        int literalValue() const;
        
        std::string identifierValue() const;
        
        size_t lineNumber() const;
        
    private:
        
        void skipLine();
        
        void lexIdentifier();
        
        void lexNumber();
        
        void lexString();
        
        void lexLiteral();
        
        void getch();
        
        void error(std::string const &str);
        
        int d_fd;
        
        char *d_buffer;
        
        size_t d_bufferSize;
        size_t d_bufferFill;
        size_t d_bufferPos;
        
        int d_ch;
        
        int d_token;
        
        std::string d_value;
        
        size_t d_lineNumber;
        
        std::unordered_map<std::string, Token> d_keywords;
        
    };
    
}

#endif /* Lexer_hpp */
