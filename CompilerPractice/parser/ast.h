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

class Program;
class DeclP;
class Decl;
class VariableDecl;
class Variable;
class Type;
class FunctionDecl;
class Formals;
class VariableList;
class ClassDecl;
class IdentList;
class FieldM;
class Field;
class InterfaceDecl;
class PrototypeM;
class Prototype;
class StmtBlock;
class VariableDeclM;
class StmtM;
class Stmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class ReturnStmt;
class BreakStmt;
class PrintStmt;
class ExprList;
class Expr;
class LValue;
class Call;
class Actuals;
class Constant;


Program,{DeclP}

DeclP,{DeclP,Decl}
DeclP,{Decl}

Decl,{VariableDecl}
Decl,{FunctionDecl}
Decl,{ClassDecl}
Decl,{InterfaceDecl}

VariableDecl,{Variable,;}

Variable,{Type,Ident}

Type,{int}
Type,{bool}
Type,{double}
Type,{string}
Type,{ident}
Type,{Type,[,]}

FunctionDecl,{Type,Ident,(,Formals,),StmtBlock}
FunctionDecl,{void,Ident,(,Formals,),StmtBlock}

Formals,{}
Formals,{VariableList}

VariableList,{Variable}
VariableList,{Variable,,,VariableList}

ClassDecl,{class,Ident,{,FieldM,}}
ClassDecl,{class,Ident,extends,Ident,{,FieldM,}}
ClassDecl,{class,Ident,extends,Ident,implements,IdentList,{,FieldM,}}
ClassDecl,{class,Ident,implements,IdentList,{,FieldM,}}

IdentList,{Ident}
IdentList,{Ident,,,IdentList}

FieldM,{}
FieldM,{Field,FieldM}

Field,{VariableDecl}
Field,{FunctionDecl}

InterfaceDecl,{interface,Ident,{,PrototypeM,}}

PrototypeM,{}
PrototypeM,{Prototype,PrototypeM}

Prototype,{Type,Ident,(,Formals,),;}
Prototype,{void,Ident,(,Formals,),;}

StmtBlock,{VariableDeclM, StmtM}

VariableDeclM,{}
VariableDeclM,{VariableDecl,VariableDeclM}

StmtM,{}
StmtM,{Stmt,StmtM}

Stmt,{;}
Stmt,{Expr,;}
Stmt,{IfStmt}
Stmt,{WhileStmt}
Stmt,{ForStmt}
Stmt,{BreakStmt}
Stmt,{ReturnStmt}
Stmt,{PrintStmt}
Stmt,{StmtBlock}

IfStmt,{if,(,Expr,),Stmt,else,Stmt}
IfStmt,{if,(,Expr,),Stmt}

WhileStmt,{while,(,Expr,),Stmt}

ForStmt,{for,(,Expr,;,Expr,;,Expr,),Stmt}
ForStmt,{for,(,Expr,;,Expr,;,),Stmt}
ForStmt,{for,(,;,Expr,;,Expr,),Stmt}

ReturnStmt,{return,;}
ReturnStmt,{return,Expr,;}

BreakStmt,{break,;}

PrintStmt,{Print,(,ExprList,),;}

ExprList,{Expr}
ExprList,{Expr,,,ExprList}

Expr,{LValue,=,Expr}
Expr,{Constant}
Expr,{LVlue}
Expr,{this}
Expr,{Call}
Expr,{(,Expr,)}
Expr,{Expr,+,Expr}
Expr,{Expr,=,Expr}
Expr,{Expr,*,Expr}
Expr,{Expr,/,Expr}
Expr,{Expr,%,Expr}
Expr,{-,Expr}
Expr,{Expr,<,Expr}
Expr,{Expr,<,=,Expr}
Expr,{Expr,>,Expr}
Expr,{Expr,>,=,Expr}
Expr,{Expr,=,=,Expr}
Expr,{Expr,!,=,Expr}
Expr,{Expr,&,&,Expr}
Expr,{Expr,|,|,Expr}
Expr,{!,Expr}
Expr,{ReadInteger,(,)}
Expr,{ReadLine,(,)}
Expr,{new,Ident}
Expr,{NewArray,(,Expr,,,Type,)}

LValue,{Ident}
LValue,{Expr,.,Ident}
LValue,{Expr,[,Expr,]}

Call,{Ident,(,Actuals,)}
Call,{Expr,.,Ident,(,Actuals,)}

Actuals,{}
Actuals,{ExprList}

Constant,{IntConstant}
Constant,{DoubleConstant}
Constant,{StringConstant}
Constant,{BoolConstant}
Constant,{null}


enum class NodeType{
    //Nonterminals
    Program,DeclP,Decl,VariableDecl,Variable,Type,FunctionDecl,Formals,VariableList,ClassDecl,IdentList,FieldM,Field,InterfaceDecl,PrototypeM,Prototype,StmtBlock,VariableDeclM,StmtM,Stmt,IfStmt,WhileStmt,ForStmt,ReturnStmt,BreakStmt,PrintStmt,ExprList,Expr,LValue,Call,Actuals,Constant,
    }
//Int, String, Bool
const std::map<std::string, NodeType> string_to_type = {{
    //Nonterminals
    {"Program",Program},
    {"DeclP",DeclP},
    {"Decl",Decl},
    {"VariableDecl",VariableDecl},
    {"Variable",Variable},
    {"Type",Type},
    {"FunctionDecl",FunctionDecl},
    {"Formals",Formals},
    {"VariableList",VariableList},
    {"ClassDecl",ClassDecl},
    {"IdentList",IdentList},
    {"FieldM",FieldM},
    {"Field",Field},
    {"InterfaceDecl",InterfaceDecl},
    {"PrototypeM",PrototypeM},
    {"Prototype",Prototype},
    {"StmtBlock",StmtBlock},
    {"VariableDeclM",VariableDeclM},
    {"StmtM",StmtM},
    {"Stmt",Stmt},
    {"IfStmt",IfStmt},
    {"WhileStmt",WhileStmt},
    {"ForStmt",ForStmt},
    {"ReturnStmt",ReturnStmt},
    {"BreakStmt",BreakStmt},
    {"PrintStmt",PrintStmt},
    {"ExprList",ExprList},
    {"Expr",Expr},
    {"LValue",LValue},
    {"Call",Call},
    {"Actuals",Actuals},
    {"Constant",Constant},

    {"IntConstant",IntConstant},
    {"DoubleConstant",DoubleConstant},
    {"StringConstant",StringConstant},
    {"BoolConstant",BoolConstant},
    {"BoolConstant",Ident},
}};

const std::string token_to_typename(token::Token tok){
    switch(tok.type){
        case TokenType::Keyword:
            return tok.value;
        case TokenType::Identifier:
            return "Ident";
        case TokenType::Operator:
            return tok.value;
        case TokenType::Bool:
            return "BoolConstant";
        case TokenType::Int:
            return "IntConstant";
        case TokenType::Double:
            return "DoubleConstant";
        case TokenType::String:
            return "StringConstant";
        case TokenType::Semicolon:
            return ";";
        case TokenType::Period:
            return ".";
        case TokenType::Comma:
            return ",";
        case TokenType::LParen:
            return "(";
        case TokenType::RParen:
            return ")";
        case TokenType::LBrace:
            return "{";
        case TokenType::LBrack:
            return "[";
        case TokenType::RBrack:
            return "]";
        case TokenType::RBrace:
            return "}";
        case TokenType::END:
            assert(false);
            output = "End of input reached\n";
            break;
        case TokenType::COMMENT:
            assert(false);
            output = "Comment here\n";
            break;
    }
}

template <typename ...Args>
std::unique_ptr<Node> make_node(std::string class_name, Args&&... args){
    auto type = string_to_type.at(class_name);
    switch(type){
        //Nonterminals
        case NodeType::Program:
            return std::make_unique<Program>(std::forward<Args>(args)...);
        case NodeType::DeclP:
            return std::make_unique<DeclP>(std::forward<Args>(args)...);
        case NodeType::Decl:
            return std::make_unique<Decl>(std::forward<Args>(args)...);
        case NodeType::VariableDecl:
            return std::make_unique<VariableDecl>(std::forward<Args>(args)...);
        case NodeType::Variable:
            return std::make_unique<Variable>(std::forward<Args>(args)...);
        case NodeType::Type:
            return std::make_unique<Type>(std::forward<Args>(args)...);
        case NodeType::FunctionDecl:
            return std::make_unique<FunctionDecl>(std::forward<Args>(args)...);
        case NodeType::Formals:
            return std::make_unique<Formals>(std::forward<Args>(args)...);
        case NodeType::VariableList:
            return std::make_unique<VariableList>(std::forward<Args>(args)...);
        case NodeType::ClassDecl:
            return std::make_unique<ClassDecl>(std::forward<Args>(args)...);
        case NodeType::IdentList:
            return std::make_unique<IdentList>(std::forward<Args>(args)...);
        case NodeType::FieldM:
            return std::make_unique<FieldM>(std::forward<Args>(args)...);
        case NodeType::Field:
            return std::make_unique<Field>(std::forward<Args>(args)...);
        case NodeType::InterfaceDecl:
            return std::make_unique<InterfaceDecl>(std::forward<Args>(args)...);
        case NodeType::PrototypeM:
            return std::make_unique<PrototypeM>(std::forward<Args>(args)...);
        case NodeType::Prototype:
            return std::make_unique<Prototype>(std::forward<Args>(args)...);
        case NodeType::StmtBlock:
            return std::make_unique<StmtBlock>(std::forward<Args>(args)...);
        case NodeType::VariableDeclM:
            return std::make_unique<VariableDeclM>(std::forward<Args>(args)...);
        case NodeType::StmtM:
            return std::make_unique<StmtM>(std::forward<Args>(args)...);
        case NodeType::Stmt:
            return std::make_unique<Stmt>(std::forward<Args>(args)...);
        case NodeType::IfStmt:
            return std::make_unique<IfStmt>(std::forward<Args>(args)...);
        case NodeType::WhileStmt:
            return std::make_unique<WhileStmt>(std::forward<Args>(args)...);
        case NodeType::ForStmt:
            return std::make_unique<ForStmt>(std::forward<Args>(args)...);
        case NodeType::ReturnStmt:
            return std::make_unique<ReturnStmt>(std::forward<Args>(args)...);
        case NodeType::BreakStmt:
            return std::make_unique<BreakStmt>(std::forward<Args>(args)...);
        case NodeType::PrintStmt:
            return std::make_unique<PrintStmt>(std::forward<Args>(args)...);
        case NodeType::ExprList:
            return std::make_unique<ExprList>(std::forward<Args>(args)...);
        case NodeType::Expr:
            return std::make_unique<Expr>(std::forward<Args>(args)...);
        case NodeType::LValue:
            return std::make_unique<LValue>(std::forward<Args>(args)...);
        case NodeType::Call:
            return std::make_unique<Call>(std::forward<Args>(args)...);
        case NodeType::Actuals:
            return std::make_unique<Actuals>(std::forward<Args>(args)...);
        case NodeType::Constant:
            return std::make_unique<Constant>(std::forward<Args>(args)...);
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
