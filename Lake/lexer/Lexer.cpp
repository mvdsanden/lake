//
//  Lexer.cpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "Lexer.hpp"

#include <iostream>

#include <cstdlib>
#include <errno.h>

using namespace lake;

enum {
    
    DEFAULT_BUFFER_SIZE = 128,
    
};

Lexer::Lexer(int fd)
: d_fd(fd),
d_buffer(new char [ DEFAULT_BUFFER_SIZE ]),
d_bufferSize(DEFAULT_BUFFER_SIZE),
d_bufferFill(0),
d_bufferPos(0),
d_lineNumber(1)
{
    d_keywords["var"] = TOK_VAR;
    d_keywords["const"] = TOK_CONST;
    d_keywords["func"] = TOK_FUNC;
    d_keywords["return"] = TOK_RETURN;
    
    // Native types.
    d_keywords["int8_t"] = TOK_TYPE_INT_8;
    d_keywords["int16_t"] = TOK_TYPE_INT_16;
    d_keywords["int32_t"] = TOK_TYPE_INT_32;
    d_keywords["int64_t"] = TOK_TYPE_INT_64;
    d_keywords["uint8_t"] = TOK_TYPE_UINT_8;
    d_keywords["uint16_t"] = TOK_TYPE_UINT_16;
    d_keywords["uint32_t"] = TOK_TYPE_UINT_32;
    d_keywords["uint64_t"] = TOK_TYPE_UINT_64;
    d_keywords["float"] = TOK_TYPE_FLOAT;
    d_keywords["double"] = TOK_TYPE_DOUBLE;
    d_keywords["string"] = TOK_TYPE_STRING;
    
    getch();
}

Lexer::~Lexer()
{
    if (d_buffer != nullptr) {
        delete [] d_buffer;
    }
}

void Lexer::next()
{
    while (true) {
        if (d_ch == '\n') {
            ++d_lineNumber;
            getch();
            continue;
        } else if (d_ch == '#') {
            skipLine();
        } else if (isblank(d_ch)) {
            getch();
            continue;
        } else if (isalpha(d_ch) || d_ch == '_') {
            lexIdentifier();
            break;
        } else if (isnumber(d_ch) || d_ch == '.') {
            lexNumber();
            break;
        } else if (d_ch == '"') {
            lexString();
            break;
        } else if (d_ch == '\'') {
            lexLiteral();
            break;
        } else if (d_ch == EOF) {
            d_token = TOK_EOF;
            break;
        } else {
            d_token = d_ch;
            //error("unexpected character");
            getch();
            break;
        }
    }
}

int Lexer::token() const
{
    return d_token;
}

int64_t Lexer::intValue() const
{
    return strtoll(d_value.c_str(), nullptr, 10);
}

uint64_t Lexer::uintValue() const
{
    return strtoull(d_value.c_str(), nullptr, 10);
}

double Lexer::doubleValue() const
{
    return std::atof(d_value.c_str());
}

std::string Lexer::stringValue() const
{
    return d_value;
}

int Lexer::literalValue() const
{
    return d_value[0];
}

std::string Lexer::identifierValue() const
{
    return d_value;
}

size_t Lexer::lineNumber() const
{
    return d_lineNumber;
}

void Lexer::skipLine()
{
    while (d_ch != '\n' && d_ch != EOF) {
        getch();
    }

    if (d_ch == '\n') {
        ++d_lineNumber;
        getch();
    }
}

void Lexer::lexIdentifier()
{
    d_value.clear();
    while (isalnum(d_ch) || d_ch == '_') {
        d_value += d_ch;
        getch();
    }
    
    auto i = d_keywords.find(d_value);
    d_token = (i != d_keywords.end()?i->second:TOK_IDENTIFIER);
}

void Lexer::lexNumber()
{
    d_token = TOK_CONST_INT;
    
    d_value.clear();
    while (isnumber(d_ch)) {
        d_value += d_ch;
        getch();
    }

    if (d_ch == '.') {
        d_token = TOK_CONST_DOUBLE;
        d_value += '.';
        getch();
        while (isnumber(d_ch)) {
            d_value += d_ch;
            getch();
        }
    }
    
    if (d_ch == 'f') {
        d_token = TOK_CONST_DOUBLE;
        getch();
    } else if (d_ch == 'u') {
        d_token = TOK_CONST_UINT;
        getch();
    }
}

void Lexer::lexString()
{
    d_value.clear();
    d_token = TOK_CONST_STRING;
    
    getch();
    while (d_ch != '"') {
        if (d_ch == '\n') {
            error("multi-line string literal");
            break;
        } else if (d_ch == EOF) {
            error("end-of-file inside string literal");
            break;
        } else if (d_ch == '\\') {
            getch();
            
            switch (d_ch) {
                case 'n':
                    d_value += '\n';
                    break;

                case 'r':
                    d_value += '\r';
                    break;

                case 't':
                    d_value += '\t';
                    break;

                case '0':
                    d_value += '\0';
                    break;
                    
                default:
                    d_value += d_ch;
                    break;
            }
        } else {
            d_value += d_ch;
        }
        
        getch();
    }
    
    getch();
}

void Lexer::lexLiteral()
{
    d_value.clear();
    d_token = TOK_CONST_LITERAL;
    
    getch();
    
    if (!isascii(d_ch)) {
        error("invalid literal");
    }
    
    d_value += d_ch;
    
    getch();
    
    if (d_ch != '\'') {
        error("multi-character literal");
    }
    
    getch();
}

void Lexer::getch()
{
    if (d_bufferPos == d_bufferFill) {
        d_bufferPos = 0;
        d_bufferFill = 0;
        
        while (true) {
            ssize_t ret = read(d_fd, d_buffer, d_bufferSize);
            
            if (ret < 0) {
                if (errno == EAGAIN || errno == EINTR) {
                    continue;
                }
                
                error("IO error");
                return;
            }
            
            d_bufferFill = ret;
            break;
        }
    }
    
    if (d_bufferFill == 0) {
        d_ch = EOF;
    } else {
        d_ch = d_buffer[d_bufferPos++];
    }
}

void Lexer::error(std::string const &str)
{
    std::cerr << "Error " << d_lineNumber << ": " << str << ".\n";
}
