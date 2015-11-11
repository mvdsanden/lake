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
#include "TypeAST.hpp"
#include "TypeAndNameAST.hpp"

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
            
            std::unique_ptr<TypeAndNameAST> typeAndName = parseTypeAndName(lexer);
            
            if (lexer.token() != '=') {
                return error(lexer, "expected '='");
            }
            
            lexer.next();
            switch (lexer.token()) {
                case Lexer::TOK_CONST_INT:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<int64_t>(lexer.lineNumber(), lexer.intValue()));
                    break;

                case Lexer::TOK_CONST_UINT:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<uint64_t>(lexer.lineNumber(), lexer.uintValue()));
                    break;
                    
                case Lexer::TOK_CONST_DOUBLE:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<double>(lexer.lineNumber(), lexer.doubleValue()));
                    break;
                    
                case Lexer::TOK_CONST_LITERAL:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<int8_t>(lexer.lineNumber(), lexer.literalValue()));
                    break;
                    
                case Lexer::TOK_CONST_STRING:
                    value = std::unique_ptr<ConstAST>(new ConstValueAST<std::string>(lexer.lineNumber(), lexer.stringValue()));
                    break;
                    
                default:
                    error(lexer, "expected const");
                    break;
            };
            
            lexer.next();
            if (lexer.token() != ';') {
                error(lexer, "expected ';'");
            }
            
            return std::unique_ptr<BaseAST>(new ConstDefAST(lexer.lineNumber(), std::move(typeAndName), std::move(value)));
        }

        std::unique_ptr<BaseAST> parseFunc(Lexer &lexer)
        {
            auto decl = parseFuncDecl(lexer);
            
            if (lexer.token() == ';') {
                return std::move(decl);
            }
            
            if (lexer.token() == '{') {
                auto blk = parseFuncBlock(lexer);
                return std::unique_ptr<FunctionDefAST>(new FunctionDefAST(lexer.lineNumber(), std::move(decl), std::move(blk)));
            //} else if (lexer.token() == '=') {
                
            } else {
                return error(lexer, "expected '{' or '='");
            }
        }
        
        std::unique_ptr<TypeAndNameAST> parseTypeAndName(Lexer &lexer)
        {

            std::unique_ptr<TypeAST> type;
            
            switch (lexer.token()) {
                case Lexer::TOK_TYPE_INT_8: type = std::unique_ptr<TypeAST>(new NativeTypeAST<int8_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_INT_16: type = std::unique_ptr<TypeAST>(new NativeTypeAST<int16_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_INT_32: type = std::unique_ptr<TypeAST>(new NativeTypeAST<int32_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_INT_64: type = std::unique_ptr<TypeAST>(new NativeTypeAST<int64_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_UINT_8: type = std::unique_ptr<TypeAST>(new NativeTypeAST<uint8_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_UINT_16: type = std::unique_ptr<TypeAST>(new NativeTypeAST<uint16_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_UINT_32: type = std::unique_ptr<TypeAST>(new NativeTypeAST<uint32_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_UINT_64: type = std::unique_ptr<TypeAST>(new NativeTypeAST<uint64_t>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_FLOAT: type = std::unique_ptr<TypeAST>(new NativeTypeAST<float>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_DOUBLE: type = std::unique_ptr<TypeAST>(new NativeTypeAST<double>(lexer.lineNumber())); break;
                case Lexer::TOK_TYPE_STRING: type = std::unique_ptr<TypeAST>(new NativeTypeAST<std::string>(lexer.lineNumber())); break;

                default:
                    error(lexer, "expected type specifier");
                    return nullptr;
            };
            
            lexer.next();

            if (lexer.token() != Lexer::TOK_IDENTIFIER) {
                error(lexer, "expected identifier");
                return nullptr;
            }
            
            std::unique_ptr<IdentifierAST> name(new IdentifierAST(lexer.lineNumber(), lexer.identifierValue()));
            lexer.next();
            
            return std::unique_ptr<TypeAndNameAST>(new TypeAndNameAST(lexer.lineNumber(), std::move(type), std::move(name)));
        }
        
        std::unique_ptr<FunctionPrototypeAST> parseFuncDecl(Lexer &lexer)
        {
            lexer.next();
            
            auto typeName = parseTypeAndName(lexer);
            
            if (lexer.token() != '(') {
                error(lexer, "expected '('");
                return nullptr;
            }
            
            lexer.next();
            
            std::vector<std::unique_ptr<TypeAndNameAST>> args;
            
            if (lexer.token() != ')') {
                
                while (true) {
                    
                    auto argTypeName = parseTypeAndName(lexer);
                    
                    args.push_back(std::move(argTypeName));
                    
                    if (lexer.token() == ')') {
                        lexer.next();
                        break;
                    } else if (lexer.token() == ',') {
                        lexer.next();
                    } else {
                        error(lexer, "expected ',' or ')'");
                        break;
                    }
                    
                }
            } else {
                lexer.next();
            }
            
            return std::unique_ptr<FunctionPrototypeAST>(new FunctionPrototypeAST(lexer.lineNumber(), std::move(typeName), std::move(args)));
        }
        
        std::unique_ptr<FunctionBlockAST> parseFuncBlock(Lexer &lexer)
        {
            std::vector<std::unique_ptr<ExpressionAST>> expressionList;
            
            lexer.next();
            
            while (lexer.token() != '}') {
                
                if (lexer.token() == Lexer::TOK_RETURN) {
                    expressionList.push_back(parseReturnExpression(lexer));
                } else {
                    expressionList.push_back(parseExpression(lexer));
                }
                
                if (lexer.token() != ';') {
                    error(lexer, "expected ';'");
                } else {
                    lexer.next();
                }
            }
            
            return std::unique_ptr<FunctionBlockAST>(new FunctionBlockAST(lexer.lineNumber(), std::move(expressionList)));
        }

        std::unique_ptr<ExpressionAST> parsePrimaryExpression(Lexer &lexer)
        {
            switch (lexer.token()) {
                case Lexer::TOK_CONST_INT:
                    return parseIntExpression(lexer);

                case Lexer::TOK_CONST_UINT:
                    return parseUIntExpression(lexer);

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

        std::unique_ptr<ExpressionAST> parseIntExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<int64_t>(lexer.lineNumber(), lexer.intValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseUIntExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<uint64_t>(lexer.lineNumber(), lexer.uintValue()));
            lexer.next();
            return std::move(expr);
        }

        
        std::unique_ptr<ExpressionAST> parseDoubleExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<double>(lexer.lineNumber(), lexer.doubleValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseLiteralExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<int8_t>(lexer.lineNumber(), lexer.literalValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseStringExpression(Lexer &lexer)
        {
            auto expr = std::unique_ptr<ExpressionAST>(new ConstExpressionAST<std::string>(lexer.lineNumber(), lexer.stringValue()));
            lexer.next();
            return std::move(expr);
        }

        std::unique_ptr<ExpressionAST> parseIdentifierExpression(Lexer &lexer)
        {
            auto name = std::unique_ptr<IdentifierAST>(new IdentifierAST(lexer.lineNumber(), lexer.identifierValue()));
            
            lexer.next();
            
            if (lexer.token() != '(') {
                return std::unique_ptr<ExpressionAST>(new VarExpressionAST(lexer.lineNumber(), std::move(name)));
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
            
            return std::unique_ptr<ExpressionAST>(new CallExpressionAST(lexer.lineNumber(), std::move(name), std::move(args)));
        }

        std::unique_ptr<ExpressionAST> parseReturnExpression(Lexer &lexer)
        {
            lexer.next();
            
            auto rhs = parseExpression(lexer);
            
            return std::unique_ptr<ExpressionAST>(new ReturnExpressionAST(lexer.lineNumber(), std::move(rhs)));
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
                
                lhs = std::unique_ptr<ExpressionAST>(new BinOpExpressionAST(lexer.lineNumber(), op, std::move(lhs), std::move(rhs)));
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