//
//  CodeGenVisitor.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef CodeGenVisitor_hpp
#define CodeGenVisitor_hpp

#include "ASTVisitor.hpp"

#include <memory>

#include <llvm/IR/Module.H>

namespace lake {
    
    class CodeGenVisitor
    : public ASTVisitor
    {
    public:
     
        virtual ~CodeGenVisitor() {}
        
        static std::shared_ptr<CodeGenVisitor> create(std::shared_ptr<llvm::Module> const &module);
        
    };
    
}

#endif /* CodeGenVisitor_hpp */
