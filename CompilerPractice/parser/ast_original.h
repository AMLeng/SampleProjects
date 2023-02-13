#ifndef _AST_
#define _AST_

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <cassert>
#include "token.h"
#include "decaf_grammar.h"

namespace ast{
class Node;
class Program;
class Decl;
class DeclP;
class FunctionDecl;
class Type;
class Ident;
class StmtBlock;
class Stmt;
class PrintStmt;
class Expr;
class Constant;
class Int;
class String;
class Bool;

class Node{
    protected:
        std::string node_type;
};
class Program{

};
class DeclP{
    protected:
        DeclP(Ident n) : name(n) {}
        Ident name;
};
class Decl{
    protected:
        Decl(Ident n) : name(n) {}
        Ident name;
};
class FunctionDecl : public Decl final{
    public:
        //FunctionDecl(Ident name, Type ret_type, std::vector<std::unique_ptr<) :
    private:
        Type return_type;
        
};
class Type;
class Ident final{
    public:
        Ident(const token::Token* tok){
            assert(tok.type == token::TokenType::Identifier);
            name = tok.value;
        }
    private:
        std::string name;
}
class Stmt;
class PrintStmt : public Stmt final{
    public:
        PrintStmt(std::vector<Expr> a) : args(a) {}
    private:
        std::vector<Expr> args;
}
class Expr{
}
class Constant : public Expr final{
    public:
        Constant(const token::Token& const_token) : tok(const_token){
        }
    private:
        token::Token tok;
}

enum class NodeType{
    Program, Decl, FunctionDecl, Type, Ident, StmtBlock, Stmt, PrintStmt, Expr, Constant, Int, String, Bool
    }
const std::map<std::string, NodeType> string_to_type = {{
    {"Program",Program},
    {"Decl+",DeclP},
    {"Decl",Decl},
    {"FunctionDecl",FunctionDecl},
    {"Type",Type},
    {"Ident",Ident},
    {"StmtBlock",StmtBlock},
    {"Stmt",Stmt},
    {"PrintStmt",PrintStmt},
    {"Expr",Expr},
    {"Constant",Constant},
    {"Int",Int},
    {"String",String},
    {"Bool",Bool},
}};

template <typename ...Args>
std::unique_ptr<Node> make_node(std::string class_name, Args&&... args){
    auto type = string_to_type.at(class_name);
    switch(type){
        case NodeType::Program:
            return std::make_unique<Program>(std::forward<Args>(args)...);
        case NodeType::Decl:
            return std::make_unique<Decl>(std::forward<Args>(args)...);
        case NodeType::FunctionDecl:
            return std::make_unique<FunctionDecl>(std::forward<Args>(args)...);
        case NodeType::Type:
            return std::make_unique<Type>(std::forward<Args>(args)...);
        case NodeType::Ident:
            return std::make_unique<Ident>(std::forward<Args>(args)...);
        case NodeType::StmtBlock:
            return std::make_unique<StmtBlock>(std::forward<Args>(args)...);
        case NodeType::Stmt:
            return std::make_unique<Stmt>(std::forward<Args>(args)...);
        case NodeType::PrintStmt:
            return std::make_unique<PrintStmt>(std::forward<Args>(args)...);
        case NodeType::Expr:
            return std::make_unique<Expr>(std::forward<Args>(args)...);
        case NodeType::Constant:
            return std::make_unique<Constant>(std::forward<Args>(args)...);
        case NodeType::Int:
            return std::make_unique<Int>(std::forward<Args>(args)...);
        case NodeType::String:
            return std::make_unique<String>(std::forward<Args>(args)...);
        case NodeType::Bool:
            return std::make_unique<Bool>(std::forward<Args>(args)...);
    }
}
template <typename Iterator>
std::unique_ptr<Node> construct_ast(Iterator start, Iterator end, 
    const State start_state = find(ItemSet<g>(std::set<Item<g>>{Item<g>("Start",g->der("Start").at(0))})))
    {
    const grammar::grammar const* g = &decaf_grammar::decaf_grammar;
    auto state_stack = std::vector<State>{start_state};
    auto type_stack = std::vector<std::string>{};
    while(start != end){
        std::string next_type = *start;
        assert(state_stack.back() != nullptr);
        State shift_state = shift(state_stack.back(),next_type);
        auto reduction = reduce(state_stack.back(),next_type);
        if(!shift_state && !reduction){
            return nullptr;
        }
        if(shift_state){
            assert(!reduction);
            //std::cout<<"Shift "<<next_type<<std::endl;
            state_stack.push_back(shift_state);
            type_stack.push_back(next_type);
            start++;
        }
        if(reduction){
            //std::cout<<"Reduce ";
            for(int i = reduction->right.size() - 1; i>=0; i--){
                assert(reduction->right.at(i) == type_stack.back());
                type_stack.pop_back();
                state_stack.pop_back();
            }
            type_stack.push_back(reduction->left);
            assert(shift(state_stack.back(), reduction->left) != nullptr);
            state_stack.push_back(shift(state_stack.back(), reduction->left));
        }
    }
    //Have now read entire input, so remains to reduce as much as possible
    for(auto reduction = reduce(state_stack.back(),"$");reduction != nullptr; reduction = reduce(state_stack.back(),"$")){
        for(int i = reduction->right.size() - 1; i>=0; i--){
            assert(reduction->right.at(i) == type_stack.back());
            type_stack.pop_back();
            state_stack.pop_back();
        }
        type_stack.push_back(reduction->left);
        if(reduction->left == "Start"){
            return true;
        }
        assert(shift(state_stack.back(), reduction->left) != nullptr);
        state_stack.push_back(shift(state_stack.back(), reduction->left));
    }
    //Even after reducing as much as possible, did not get start symbol
    return nullptr;
}
} //namespace AST
#endif // _AST_
