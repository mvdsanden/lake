//
//  ASTVisitor.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ASTVisitor_hpp
#define ASTVisitor_hpp

#include <string>

namespace lake {
    
    template <class T> class ConstValueAST;
    class IdentifierAST;
    class FunctionPrototypeAST;
    class ConstDefAST;
    
    class ASTVisitor
    {
    public:
        
        virtual void visit(ConstValueAST<double> const *node) = 0;
        
        virtual void visit(ConstValueAST<std::string> const *node) = 0;
        
        virtual void visit(ConstValueAST<int> const *node) = 0;
        
        virtual void visit(FunctionPrototypeAST const *node) = 0;
        
        virtual void visit(IdentifierAST const *node) = 0;
        
        virtual void visit(ConstDefAST const *node) = 0;
        
    };
    
    template <class T>
    class ASTVisitable
    {
    public:
        
        virtual void accept(ASTVisitor *visitor)
        {
            visitor->visit(static_cast<T*>(this));
        }
        
    };
    
}

#endif /* ASTVisitor_hpp */
