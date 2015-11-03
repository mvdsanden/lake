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
#include "FunctionPrototypeAST.hpp"
#include "ExpressionAST.hpp"
#include "FunctionBlockAST.hpp"
#include "FunctionDefAST.hpp"

#include <iostream>
#include <vector>

namespace lake {
    
    
    class ParserImpl
    : public Parser
    {
    public:
        
        ParserImpl()
        {
            d_binaryOpPrecedenceTable['<'] = 10;
            d_binaryOpPrecedenceTable['+'] = 20;
            d_binaryOpPrecedenceTable['-'] = 20;
            d_binaryOpPrecedenceTable['*'] = 40;
            d_binaryOpPrecedenceTable['/'] = 40;
        }
        
        virtual std::unique_ptr<BaseAST> parse(Lexer &lexer)
        {
            lexer.next();
            
            switch (lexer.token()) {
                case Lexer::TOK_EOF:
                    return nullptr;
                    
                case Lexer::TOK_CONST:
                    return parseConstDef(lexer);
                    
                case Lexer::TOK_FUNC:
                    return parseFunc(lexer);
                    
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

        std::unique_ptr<BaseAST> parseFunc(Lexer &lexer)
        {
            auto decl = parseFuncDecl(lexer);
            
            if (lexer.token() == ';') {
                return std::move(decl);
            }
            
            if (lexer.token() == '{') {
                auto blk = parseFuncBlock(lexer);
                return std::unique_ptr<FunctionDefAST>(new FunctionDefAST(std::move(decl), std::move(blk)));
            //} else if (lexer.token() == '=') {
                
            } else {
                return error(lexer, "expected '{' or '='");
            }
        }
        
        std::unique_ptr<FunctionPrototypeAST> parseFuncDecl(Lexer &lexer)
        {
            lexer.next();
            
            if (lexer.token() != Lexer::TOK_IDENTIFIER) {
                error(lexer, "expected identifier");
                return nullptr;
            }
            
            std::unique_ptr<IdentifierAST> name(new IdentifierAST(lexer.identifierValue()));
            
            lexer.next();
            if (lexer.token() != '(') {
                error(lexer, "expected '('");
                return nullptr;
            }
            
            lexer.next();
            
            std::vector<std::unique_ptr<IdentifierAST>> args;
            while (lexer.token() != ')') {
                
                if (lexer.token() != Lexer::TOK_IDENTIFIER) {
                    args.push_back(nullptr);
                    error(lexer, "expected identifier");
                    lexer.next();
                } else {
                    args.push_back(std::unique_ptr<IdentifierAST>(new IdentifierAST(lexer.identifierValue())));
                    lexer.next();
                }
                
                if (lexer.token() == ',') {
                    lexer.next();
                }
                
            }
            
            lexer.next();
            
            return std::unique_ptr<FunctionPrototypeAST>(new FunctionPrototypeAST(std::move(name), std::move(args)));
        }
        
        std::unique_ptr<FunctionBlockAST> parseFuncBlock(Lexer &lexer)
        {
            std::vector<std::unique_ptr<ExpressionAST>> expressionList;
            
            lexer.next();
            
            while (lexer.token() != '}') {
                expressionList.push_back(parseExpression(lexer));
                
                if (lexer.token() != ';') {
                    error(lexer, "expected ';'");
                } else {
                    lexer.next();
                }
            }
            
            return std::unique_ptr<FunctionBlockAST>(new FunctionBlockAST(std::move(expressionList)));
        }

        std::unique_ptr<ExpressionAST> parsePrimaryExpression(Lexer &lexer)
        {
            switch (lexer.token()) {
                case Lexer::TOK_CONST_DOUBLE:
                    return parseDoubleExpression(lexer);
                    
                case Lexer::TOK_CONST_LITERAL:
                    return parseLiteralExpression(lexer);
                    
                case Lexer::TOK_CONST_STRING:
                    return parseStringExpression(lexer);
                    
                case Lexer::TOK_IDENTIFIER:
                    return parseIdentifierExpression(lexer);
                    
                default:
                    error(lexer, "expected primary expression");
                    return nullptr;
            }
        }
        
        std::unique_ptr<ExpressionAST> parseDoubleExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<double>(lexer.doubleValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseLiteralExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<int>(lexer.literalValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseStringExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<std::string>(lexer.stringValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseIdentifierExpression(Lexer &lexer)
        {
            auto name = std::unique_ptr<IdentifierAST>(new IdentifierAST(lexer.identifierValue()));
            
            lexer.next();
            
            if (lexer.token() != '(') {
                return std::unique_ptr<ExpressionAST>(new VarExpressionAST(std::move(name)));
            }
            
            lexer.next();
            
            std::vector<std::unique_ptr<ExpressionAST>> args;
            
            while (lexer.token() != ')') {
                
                args.push_back(parseExpression(lexer));
                
                if (lexer.token() == ',') {
                    lexer.next();
                }
                
            }
            
            lexer.next();
            
            return std::unique_ptr<ExpressionAST>(new CallExpressionAST(std::move(name), std::move(args)));
        }
        
        std::unique_ptr<ExpressionAST> parseExpression(Lexer &lexer)
        {
            auto lhs = parsePrimaryExpression(lexer);
            
            if (!lhs) {
                return nullptr;
            }
            
            return parseBinOpRHS(lexer, 0, std::move(lhs));
        }
        
        std::unique_ptr<ExpressionAST> parseBinOpRHS(Lexer &lexer, int precedence, std::unique_ptr<ExpressionAST> lhs)
        {
            
            while (true) {
                
                int tokenPrecedence = getOperatorPrecedence(lexer.token());
                
                if (tokenPrecedence < precedence) {
                    return lhs;
                }
                
                int op = lexer.token();
                lexer.next();
                
                auto rhs = parsePrimaryExpression(lexer);
                
                if (!rhs) {
                    return nullptr;
                }
                
                int nextTokenPrecedence = getOperatorPrecedence(lexer.token());
                
                if (tokenPrecedence < nextTokenPrecedence) {
                    rhs = parseBinOpRHS(lexer, tokenPrecedence+1, std::move(rhs));
                    
                    if (!rhs) {
                        return nullptr;
                    }
                }
                
                lhs = std::unique_ptr<ExpressionAST>(new BinOpExpressionAST(op, std::move(lhs), std::move(rhs)));
            }
            
        }
        
        int getOperatorPrecedence(int token)
        {
            auto i = d_binaryOpPrecedenceTable.find(token);
            return (i == d_binaryOpPrecedenceTable.end()?-1:i->second);
        }
        
        std::unordered_map<int, int> d_binaryOpPrecedenceTable;
        
    };
    
    
    Parser &Parser::instance()
    {
        static ParserImpl s_instance;
        return s_instance;
    }
    
}