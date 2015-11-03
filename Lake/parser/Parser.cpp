//
//  Parser.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Parser.hpp"

#include "Lexer.hpp"

#include "ConstAST.hpp"
#include "ConstDefAST.hpp"

#include <iostream>

namespace lake {
    
    
    class ParserImpl
    : public Parser
    {
    public:
        
        virtual std::unique_ptr<BaseAST> parse(Lexer &lexer)
        {
            lexer.next();
            
            switch (lexer.token()) {
                case Lexer::TOK_EOF:
                    return nullptr;
                    
                case Lexer::TOK_CONST:
                    return parseConstDef(lexer);
                    
                default:
                    return error(lexer, "expected const, var or func");
                    
            }
            
            
        }
        
    private:
        
        std::unique_ptr<BaseAST> error(Lexer &lexer, std::string const &str)
        {
            std::cerr << "Error " << lexer.lineNumber() << ": " << str << ".\n";
            return nullptr;
        }
        
        std::unique_ptr<BaseAST> parseConstDef(Lexer &lexer)
        {
            std::unique_ptr<ConstAST> value;
            
            lexer.next();
            
            if (lexer.token() != Lexer::TOK_IDENTIFIER) {
                return error(lexer, "expected identifier");
            }
            
            std::unique_ptr<IdentifierAST> name(new IdentifierAST(lexer.identifierValue()));
            
            lexer.next();
            if (lexer.token() != '=') {
                return error(lexer, "expected '='");
            }
            
            lexer.next();
            switch (lexer.token()) {
                case Lexer::TOK_CONST_DOUBLE:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<double>(lexer.doubleValue()));
                    break;
                    
                case Lexer::TOK_CONST_LITERAL:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<int>(lexer.literalValue()));
                    break;
                    
                case Lexer::TOK_CONST_STRING:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<std::string>(lexer.stringValue()));
                    break;
                    
                default:
                    error(lexer, "expected const");
                    break;
            };
            
            lexer.next();
            if (lexer.token() != ';') {
                error(lexer, "expected ';'");
            }
            
            return std::unique_ptr<BaseAST>(new ConstDefAST(std::move(name), std::move(value)));
        }
        
    };
    
    
    Parser &Parser::instance()
    {
        static ParserImpl s_instance;
        return s_instance;
    }
    
}