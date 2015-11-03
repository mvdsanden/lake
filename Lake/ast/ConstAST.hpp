//
//  ConstAST.hpp
//  Lake
//
//  Created by Marten van de Sanden on 11/2/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ConstAST_hpp
#define ConstAST_hpp

#include "BaseAST.hpp"
#include "ASTVisitor.hpp"

#include <unordered_map>

namespace lake {
    
    class ConstAST
    : public BaseAST
    {
    public:
        
    };
    
    template <class T>
    class ConstValueAST
    : public ConstAST
    {
    public:

        LAKE_VISITOR_ACCEPT(ASTVisitor);
        
        
        ConstValueAST(T const &value)
        : d_value(value)
        {
        }
        
        static std::shared_ptr<ConstValueAST<T>> get(T const &value)
        {
            static std::unordered_map<T, std::shared_ptr<ConstValueAST<T>>> s_table;
            auto i = s_table.find(value);
            if (i != s_table.end()) {
                return i->second;
            }
            auto v = std::make_shared<ConstValueAST<T>>(value);
            s_table.insert(std::make_pair(value, v));
            return v;
        }
        
        T const &value() const
        {
            return d_value;
        }
        
    private:
        T d_value;
    };

    
}

#endif /* ConstAST_hpp */
