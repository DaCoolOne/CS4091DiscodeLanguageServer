

#include "flex/parser.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "lang/function.hpp"

std::string getNodeName(AST_Node * node)
{
    switch (node->type)
    {
        case AST_NODE_ADD: return "ADD";
        case AST_NODE_SUB: return "SUB";
        case AST_NODE_MUL: return "MUL";
        case AST_NODE_DIV: return "DIV";
        case AST_NODE_MOD: return "MOD";
        case AST_NODE_AND: return "AND";
        case AST_NODE_OR: return "OR";
        case AST_NODE_EQ: return "EQ";
        case AST_NODE_GTR: return "GTR";
        case AST_NODE_GTEQ: return "GTEQ";
        case AST_NODE_RESOLVE_NAME: return "NAME_RESOLVE";
        case AST_NODE_ARG_LIST_CONT: return "ARG_LIST_CONT";
        case AST_NODE_STATEMENT_LIST: return "STATEMENT_LIST";
        case AST_NODE_ASSIGN: return "ASSIGN";
        case AST_NODE_FCALL: return "FCALL";
        case AST_NODE_IF: return "IF";
        case AST_NODE_ALSO: return "ALSO";
        case AST_NODE_BRANCH: return "BRANCH";
        case AST_NODE_WHILE: return "WHILE";
        case AST_NODE_FOR: return "FOR";
        case AST_NODE_DECLARE: return "DECLARE";
        case AST_NODE_DECLARE_FUNCTION: return "DECLARE_FUNCTION";
        case AST_NODE_DECLARE_METHOD: return "DECLARE_METHOD";
        case AST_NODE_FUNCTION_SIGNATURE: return "FUNCTION_SIG";
        case AST_NODE_NEG: return "NEG";
        case AST_NODE_NOT: return "NOT";
        case AST_NODE_GLOBAL_SCOPE: return "GLOBAL";
        case AST_NODE_LIB_SCOPE: return "LIBRARY";
        case AST_NODE_IDENT_LIST_START: return "NODE_IDENT_LIST_START";
        case AST_NODE_ARG_LIST_START: return "NODE_ARG_LIST_START";
        case AST_NODE_RETURN: return "RETURN";
        case AST_NODE_IDENT_LIST_CONT: return "IDENT_LIST_CONT";
        case AST_NODE_INDEX: return "INDEX";
    }

    return "Unknown";
}

void printNode(AST_Node * node, std::string indent)
{
    if (node == NULL) { return; }

    switch(node->type) {
        // Binary
        case AST_NODE_ADD:
        case AST_NODE_SUB:
        case AST_NODE_MUL:
        case AST_NODE_DIV:
        case AST_NODE_MOD:
        case AST_NODE_AND:
        case AST_NODE_OR:
        case AST_NODE_EQ:
        case AST_NODE_GTR:
        case AST_NODE_GTEQ:

        case AST_NODE_RESOLVE_NAME:
        case AST_NODE_ARG_LIST_CONT:
        case AST_NODE_STATEMENT_LIST:
        case AST_NODE_ASSIGN:
        case AST_NODE_FCALL:
        case AST_NODE_IF:
        case AST_NODE_ALSO:
        case AST_NODE_BRANCH:
        case AST_NODE_WHILE:
        case AST_NODE_FOR:
        case AST_NODE_DECLARE:
        case AST_NODE_DECLARE_FUNCTION:
        case AST_NODE_DECLARE_METHOD:
        case AST_NODE_FUNCTION_SIGNATURE:

        case AST_NODE_IDENT_LIST_CONT:
        case AST_NODE_INDEX:

            std::cout << indent << getNodeName(node) << std::endl;
            printNode(node->left, indent + "  ");
            printNode(node->right, indent + "  ");
        break;

        // Unary ops
        case AST_NODE_NEG:
        case AST_NODE_NOT:

        case AST_NODE_GLOBAL_SCOPE:
        case AST_NODE_LIB_SCOPE:
        case AST_NODE_IDENT_LIST_START:
        case AST_NODE_ARG_LIST_START:
        case AST_NODE_RETURN:
            std::cout << indent << getNodeName(node) << std::endl;
            printNode(node->right, indent + "  ");
        break;

        // Terminals
        case AST_NODE_NULL:
            std::cout << indent << "NULL" << std::endl;
        break;
        case AST_NODE_BOOL:
            std::cout << indent << (getBool(node) ? "Bool: TRUE" : "Bool: FALSE") << std::endl;
        break;
        case AST_NODE_NUM:
            std::cout << indent << "Number: " << getDouble(node) << std::endl;
        break;
        case AST_NODE_STR:
            std::cout << indent << "String: " << getStr(node) << std::endl;
        break;
        case AST_NODE_IDENT:
            std::cout << indent << "Ident: " << getStr(node) << std::endl;
        break;
    }
}

// Pre: AST_Node is of type AST_NODE_RESOLVE_NAME
std::vector<std::string> discode_internal::getArgs(AST_Node * node) {
    if(node == NULL) {
        return std::vector<std::string>();
    }
    std::vector<std::string> other = getArgs(node->right);
    std::vector<std::string> args;
    args.reserve(other.size() + 1);
    args.push_back(getStr(node->left));
    args.insert(args.end(), other.begin(), other.end());
    return args;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::constructNameResolution(AST_Node * name, AST_NODE_TYPE scope) {
    // std::cout << "NAME RESOLUTION" << std::endl;

    if (name->left == nullptr) {
        std::vector<std::shared_ptr<discode::Instruction>> ins;
        
        if(scope == AST_NODE_LIB_SCOPE) {
            ins.push_back(std::make_shared<discode::InstructionGetLib>(name->lineno, getStr(name->right)));
        }
        else if(scope == AST_NODE_GLOBAL_SCOPE) {
            ins.push_back(std::make_shared<discode::InstructionGetGlobal>(name->lineno, getStr(name->right)));
        }
        else {
            ins.push_back(std::make_shared<discode::InstructionGetLocal>(name->lineno, getStr(name->right)));
        }
        return ins;
    }
    else if (name->type == AST_NODE_RESOLVE_NAME) {
        std::vector<std::shared_ptr<discode::Instruction>> resolution = discode_internal::constructNameResolution(name->left, scope);
        resolution.push_back(std::make_shared<discode::InstructionGetStack>(name->lineno, getStr(name->right)));
        return resolution;
    }
    else {
        throw std::logic_error("Invalid type during name resolution " + std::to_string(name->type));
    }
}

// Pre: node is of type AST_NODE_LIB_SCOPE, AST_NODE_GLOBAL_SCOPE, or AST_NODE_RESOLVE_NAME 
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::resolveName(AST_Node * name) {
    // std::cout << "RESOLVE NAME" << std::endl;

    switch (name->type)
    {
        case AST_NODE_GLOBAL_SCOPE:
        case AST_NODE_LIB_SCOPE:
            return discode_internal::constructNameResolution(name->right, name->type);
        case AST_NODE_RESOLVE_NAME:
            return discode_internal::constructNameResolution(name, name->type);
    
        default:
            throw std::logic_error("Unknown NAME_RES node type " + std::to_string(name->type));
    }
}

// Pre: node is of type AST_RESOLVE_NAME
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildIndexResolve(AST_Node * index) {
    // std::cout << "INDEX_RESOLVE" << std::endl;

    auto lhs = discode_internal::buildExpressionEval(index->left);
    auto rhs = discode_internal::buildExpressionEval(index->right);
    lhs.reserve(lhs.size() + rhs.size() + 1);
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    lhs.push_back(std::make_shared<discode::InstructionGetIndexedStack>(index->lineno));
    return lhs;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildConstant(AST_Node * constant) {
    // std::cout << "CONSTANT" << std::endl;

    std::shared_ptr<discode::Data> data;
    std::vector<std::shared_ptr<discode::Instruction>> ins_list;
    std::string _temp;
    switch (constant->type)
    {
        case AST_NODE_NULL:
            data = std::make_shared<discode::Null>();
        break;
        case AST_NODE_BOOL:
            data = std::make_shared<discode::Boolean>(getBool(constant));
        break;
        case AST_NODE_NUM:
            data = std::make_shared<discode::Number>(getDouble(constant));
        break;
        case AST_NODE_STR:
            _temp = getStr(constant) + 1;
            _temp.pop_back();
            data = std::make_shared<discode::String>(util::unescape(_temp));
        break;
    
        default:
            throw std::logic_error("Cannot create object of type " + std::to_string(constant->type));
        break;
    }
    if (ins_list.size() == 0 && data != nullptr) {
        ins_list.push_back(std::make_shared<discode::InstructionPush>(constant->lineno, data));
    }
    return ins_list;
}

std::pair<std::vector<std::shared_ptr<discode::Instruction>>, uint16_t> discode_internal::buildArgList(AST_Node * arglist) {
    // std::cout << "ARG_LIST" << std::endl;

    if (arglist == NULL) {
        return std::pair<std::vector<std::shared_ptr<discode::Instruction>>, uint16_t>(std::vector<std::shared_ptr<discode::Instruction>>(), 0);
    }
    
    auto rhs = discode_internal::buildArgList(arglist->right);

    auto lhs = discode_internal::buildExpressionEval(arglist->left);

    rhs.first.reserve(rhs.first.size() + lhs.size());
    rhs.first.insert(rhs.first.end(), lhs.begin(), lhs.end());

    ++rhs.second;

    return rhs;
}

// Pre: fcall is a AST_NODE_FCALL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildFunctionCall(AST_Node * fcall) {
    // std::cout << "FCALL" << std::endl;

    // Get the function arguments
    auto arglist = discode_internal::buildArgList(fcall->right->right);

    // Get the namespace of the function
    auto name_resolution = discode_internal::buildExpressionEval(fcall->left);

    arglist.first.reserve(name_resolution.size() + arglist.first.size() + 1);
    arglist.first.insert(arglist.first.end(), name_resolution.begin(), name_resolution.end());
    arglist.first.push_back(std::make_shared<discode::InstructionFunctionCall>(fcall->lineno, arglist.second));

    return arglist.first;
}

// Pre: expr is an operator
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildBinaryOperator(AST_Node * node) {
    // std::cout << "BINARY_OPERATOR" << std::endl;

    auto lhs = discode_internal::buildExpressionEval(node->left);
    auto rhs = discode_internal::buildExpressionEval(node->right);

    lhs.reserve(lhs.size() + rhs.size() + 1);
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());

    switch (node->type)
    {
        case AST_NODE_ADD:  lhs.push_back(std::make_shared<discode::InstructionAdd>(node->lineno)); break;
        case AST_NODE_SUB:  lhs.push_back(std::make_shared<discode::InstructionSub>(node->lineno)); break;
        case AST_NODE_MUL:  lhs.push_back(std::make_shared<discode::InstructionMul>(node->lineno)); break;
        case AST_NODE_DIV:  lhs.push_back(std::make_shared<discode::InstructionDivide>(node->lineno)); break;
        case AST_NODE_MOD:  lhs.push_back(std::make_shared<discode::InstructionMod>(node->lineno)); break;
        case AST_NODE_GTR:  lhs.push_back(std::make_shared<discode::InstructionGtr>(node->lineno)); break;
        case AST_NODE_GTEQ: lhs.push_back(std::make_shared<discode::InstructionGteq>(node->lineno)); break;
        case AST_NODE_EQ:   lhs.push_back(std::make_shared<discode::InstructionEq>(node->lineno)); break;
        case AST_NODE_AND:  lhs.push_back(std::make_shared<discode::InstructionAnd>(node->lineno)); break;
        case AST_NODE_OR:   lhs.push_back(std::make_shared<discode::InstructionOr>(node->lineno)); break;

        default:
            throw std::logic_error("Unknown EXPR OP node type " + std::to_string(node->type));
        break;
    }
    return lhs;
}

std::vector<std::shared_ptr<discode::Instruction>> buildUnaryOperator(AST_Node * node) {
    // std::cout << "UNARY" << std::endl;

    auto rhs = discode_internal::buildExpressionEval(node->right);

    switch (node->type)
    {
        case AST_NODE_NEG: rhs.push_back(std::make_shared<discode::InstructionNeg>(node->lineno)); break;
        case AST_NODE_NOT: rhs.push_back(std::make_shared<discode::InstructionNot>(node->lineno)); break;

        default:
            throw std::logic_error("Unknown EXPR OP node type " + std::to_string(node->type));
        break;
    }
    return rhs;
}

// Pre: expr is not NULL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildExpressionEval(AST_Node * expr) {
    // std::cout << "EXPRESSION" << std::endl;

    switch (expr->type)
    {
        case AST_NODE_FCALL:
            return discode_internal::buildFunctionCall(expr);
        case AST_NODE_RESOLVE_NAME:
        case AST_NODE_LIB_SCOPE:
        case AST_NODE_GLOBAL_SCOPE:
            return discode_internal::resolveName(expr);
        case AST_NODE_INDEX:
            return discode_internal::buildIndexResolve(expr);
        
        case AST_NODE_NULL:
        case AST_NODE_BOOL:
        case AST_NODE_NUM:
        case AST_NODE_STR:
            return discode_internal::buildConstant(expr);
        
        case AST_NODE_ADD:
        case AST_NODE_SUB:
        case AST_NODE_MUL:
        case AST_NODE_DIV:
        case AST_NODE_MOD:
        case AST_NODE_GTR:
        case AST_NODE_GTEQ:
        case AST_NODE_EQ:
        case AST_NODE_AND:
        case AST_NODE_OR:
            return discode_internal::buildBinaryOperator(expr);

        default:
            throw std::logic_error("Unknown EXPR node type " + std::to_string(expr->type));
        break;
    }

    std::cout << "WARN: EXPR not implemented! " << expr->type << std::endl;
    return std::vector<std::shared_ptr<discode::Instruction>>();
}

// Pre: node type is NAME_RESOLVE, LOCAL, or GLOBAL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildAssignReturn(AST_Node * ret_node) {
    // std::cout << "RETURN" << std::endl;

    // For now, only assign local
    std::vector<std::shared_ptr<discode::Instruction>> ins;
    if (ret_node->type == AST_NODE_RESOLVE_NAME) {
        if (ret_node->left) {
            
        }
        else {
            ins.push_back(std::make_shared<discode::InstructionWriteLocal>(ret_node->lineno, getStr(ret_node->right)));
        }
    }
    else if (ret_node->type == AST_NODE_GLOBAL_SCOPE) {

    }
    return ins;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildAssign(AST_Node * assign, uint16_t jumpoffset) {
    // std::cout << "ASSIGN" << std::endl;

    AST_Node * returns_to = assign->left;
    AST_Node * expr = assign->right;

    // Ignore return value
    if (returns_to == nullptr) {
        auto eval = discode_internal::buildExpressionEval(expr);
        eval.push_back(std::make_shared<discode::InstructionPop>(assign->lineno));
        return eval;
    }
    // Assign return value
    else {
        printNode(returns_to, "");
        auto eval = discode_internal::buildExpressionEval(expr);
        auto assign = discode_internal::buildAssignReturn(returns_to);
        eval.reserve(eval.size() + assign.size());
        eval.insert(eval.end(), assign.begin(), assign.end());
        return eval;
    }

}

// Pre: node is AST_NODE_RETURN
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildReturn(AST_Node * ret_node) {
    // std::cout << "RETURN" << std::endl;

    std::vector<std::shared_ptr<discode::Instruction>> ins;

    if(ret_node->right) {
        ins = discode_internal::buildExpressionEval(ret_node->right);
        ins.push_back(std::make_shared<discode::InstructionReturn>(ret_node->lineno));
    }
    else {
        ins.push_back(std::make_shared<discode::InstructionPush>(ret_node->lineno, std::make_shared<discode::Null>()));
        ins.push_back(std::make_shared<discode::InstructionReturn>(ret_node->lineno));
    }

    return ins;
}

std::vector<std::vector<std::shared_ptr<discode::Instruction>>> discode_internal::buildAlsoChain(AST_Node * also_root, uint16_t jumpoffset) {
    // std::cout << "ALSO_CHAIN" << std::endl;

    std::vector<std::vector<std::shared_ptr<discode::Instruction>>> ins;

    AST_Node * pos = also_root;
    do {
        auto expr = discode_internal::buildExpressionEval(pos->left);
        ins.push_back(expr);
        pos = pos->right;
    } while(pos);

    return ins;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildIfStatement(AST_Node * if_statement, uint16_t jumpoffset) {
    // std::cout << "IF" << std::endl;

    // Let the conditional branch shenanigans begin!
    auto alsoChain = discode_internal::buildAlsoChain(if_statement->left, jumpoffset);
    uint16_t alsoChainSize = alsoChain.size();
    for(auto const& a : alsoChain) {
        alsoChainSize += a.size();
    }

    std::vector<std::shared_ptr<discode::Instruction>> if_part = discode_internal::buildStatements(if_statement->right->left, jumpoffset + alsoChainSize);
    
    std::vector<std::shared_ptr<discode::Instruction>> else_part;
    if(if_statement->right->right) {
        else_part = discode_internal::buildStatements(if_statement->right->right, jumpoffset + alsoChainSize + if_part.size() + 1);
    }

    std::vector<std::shared_ptr<discode::Instruction>> ins;
    for(auto const& also : alsoChain) {
        ins.insert(ins.end(), also.begin(), also.end());
        ins.push_back(std::make_shared<discode::InstructionCJump>(if_statement->left->lineno, jumpoffset + alsoChainSize + if_part.size() + (else_part.size() ? 1 : 0)));
    }

    ins.insert(ins.end(), if_part.begin(), if_part.end());

    if (else_part.size()) {
        ins.push_back(std::make_shared<discode::InstructionUJump>(if_statement->left->lineno, jumpoffset + ins.size() + else_part.size() + 1));
        ins.insert(ins.end(), else_part.begin(), else_part.end());
    }

    return ins;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildWhileStatement(AST_Node * while_statement, uint16_t jumpoffset) {
    // std::cout << "WHILE" << std::endl;

    auto alsoChain = discode_internal::buildAlsoChain(while_statement->left, jumpoffset);
    uint16_t alsoChainSize = alsoChain.size();
    for(auto const& a : alsoChain) {
        alsoChainSize += a.size();
    }

    std::vector<std::shared_ptr<discode::Instruction>> while_part = discode_internal::buildStatements(while_statement->right, jumpoffset + alsoChainSize);

    std::vector<std::shared_ptr<discode::Instruction>> ins;
    for(auto const& also : alsoChain) {
        ins.insert(ins.end(), also.begin(), also.end());
        ins.push_back(std::make_shared<discode::InstructionCJump>(while_statement->left->lineno, jumpoffset + alsoChainSize + while_part.size() + 1));
    }

    ins.insert(ins.end(), while_part.begin(), while_part.end());
    ins.push_back(std::make_shared<discode::InstructionUJump>(while_statement->left->lineno, jumpoffset));

    return ins;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildForStatement(AST_Node * for_statement, uint16_t jumpoffset) {
    // std::cout << "FOR" << std::endl;
    std::vector<std::shared_ptr<discode::Instruction>> ins;

    auto iterval = getStr(for_statement->left->left);

    auto init = discode_internal::buildExpressionEval(for_statement->left->right->left);
    init.push_back(std::make_shared<discode::InstructionWriteLocal>(for_statement->left->left->lineno, iterval));
    auto comp_expr = discode_internal::buildExpressionEval(for_statement->left->right->right);

    auto body = discode_internal::buildStatements(for_statement->right, jumpoffset + init.size() + comp_expr.size() + 1);
    body.push_back(std::make_shared<discode::InstructionIterInc>(for_statement->left->lineno, iterval));

    ins.insert(ins.end(), init.begin(), init.end());
    ins.insert(ins.end(), comp_expr.begin(), comp_expr.end());
    ins.push_back(std::make_shared<discode::InstructionIterCheck>(for_statement->left->lineno, iterval, jumpoffset + body.size() + ins.size() + 2));
    ins.insert(ins.end(), body.begin(), body.end());
    ins.push_back(std::make_shared<discode::InstructionUJump>(for_statement->left->lineno, jumpoffset + init.size()));

    return ins;
}

// Pre: statement is not NULL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildStatement(AST_Node * statement, uint16_t jumpoffset) {
    // std::cout << "STATEMENT" << std::endl;

    switch (statement->type)
    {
        case AST_NODE_ASSIGN:
            return discode_internal::buildAssign(statement, jumpoffset);
        
        case AST_NODE_IF:
            return discode_internal::buildIfStatement(statement, jumpoffset); 
        
        case AST_NODE_WHILE:
            return discode_internal::buildWhileStatement(statement, jumpoffset); 
        
        case AST_NODE_FOR:
            return discode_internal::buildForStatement(statement, jumpoffset); 

        case AST_NODE_RETURN:
            return discode_internal::buildReturn(statement);

        default:
            throw std::logic_error("Unknown STATEMENT node type " + std::to_string(statement->type));
        break;
    }

    std::cout << "WARN: STATEMENT not implemented! " << statement->type << std::endl;
    return std::vector<std::shared_ptr<discode::Instruction>>();
}

// Pre: AST_Node is of type STATEMENT_LIST
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildStatements(AST_Node * node, uint16_t jumpoffset) {
    // std::cout << "STATEMENTS" << std::endl;

    if (node == NULL) {
        return std::vector<std::shared_ptr<discode::Instruction>>();
    }
    auto ins1 = discode_internal::buildStatement(node->left, jumpoffset);

    if (node->right != NULL) {
        if (node->right->type != AST_NODE_STATEMENT_LIST) { throw std::logic_error("Expected statement list! 2"); }
        auto ins2 = discode_internal::buildStatements(node->right, jumpoffset + ins1.size());
        if (ins2.size()) {
            ins1.reserve(ins1.size() + ins2.size());
            ins1.insert(ins1.end(), ins2.begin(), ins2.end());
        }
    }

    return ins1;
}

// Pre: AST_Node is of type function
std::pair<std::shared_ptr<discode::Function>, std::string> discode_internal::buildFunction(AST_Node * node) {
    // std::cout << "FUNCTION" << std::endl;

    AST_Node * f_sig = node->left;
    if (f_sig->type != AST_NODE_FUNCTION_SIGNATURE) { throw std::logic_error("Expected function signature!"); }

    AST_Node * f_name = f_sig->left;
    if (f_name->type != AST_NODE_IDENT) { throw std::logic_error("Expected function name!"); }
    std::string function_name = getStr(f_name);

    std::vector<std::string> arglist = discode_internal::getArgs(f_sig->right->right);

    AST_Node * f_body = node->right;
    if (f_body->type != AST_NODE_STATEMENT_LIST) { throw std::logic_error("Expected statement list!"); }

    std::vector<std::shared_ptr<discode::Instruction>> instructions = discode_internal::buildStatements(f_body);

    return std::pair<std::shared_ptr<discode::Function>, std::string>(std::make_shared<discode::Function>(arglist, instructions), function_name);
}

// Loads an AST into the VM
void discode::load(discode::VM * vm, AST_Node * node, std::string channel, std::string msg_id)
{
    std::pair<std::shared_ptr<discode::Function>, std::string> func;
    json::JsonArray arglist;
    json::JsonObject jsonData;
    switch (node->type)
    {
        case AST_NODE_DECLARE:
            // Todo
        break;
        case AST_NODE_DECLARE_FUNCTION:
            // std::cout << "START DECL F" << std::endl;
            printNode(node, "");
            // std::cout << "WHAT THE FRICK IS GOING ON?" << std::endl;
            func = discode_internal::buildFunction(node);
            func.first->messageId = msg_id;
            std::cout << func.first->deepRepr() << std::endl;
            vm->writeGlobal(channel, func.second, func.first);
            if(channel == "commands") {
                jsonData.add("Name", std::make_shared<json::JsonString>("Add Func"));
                jsonData.add("Server_ID", std::make_shared<json::JsonString>(vm->getServerId()));
                jsonData.add("Function_name", std::make_shared<json::JsonString>(func.second));
                for (auto const &arg : func.first->args()) {
                    arglist.add(std::make_shared<json::JsonString>(arg));
                }
                jsonData.add("Arguments", std::make_shared<json::JsonArray>(arglist));
                vm->sendObject(&jsonData);
            }
        break;
        case AST_NODE_DECLARE_METHOD:
            // Todo
        break;
        default:
            throw std::logic_error("Unknown entry node type");
        break;
    }
}

void discode::analyze_file(std::string path)
{
    Parse_Error err;
    AST_Node * ast = parse(path.c_str(), &err);

    if (ast == NULL) {
        // Error occured: Todo error handling
        return;
    }

    // Print the tree
    printNode(ast, "");

    // Print the generated code
    if (ast->type == AST_NODE_DECLARE_FUNCTION) {
        auto func = discode_internal::buildFunction(ast);
        std::cout << func.second << " = " << func.first->deepRepr() << std::endl;
    }
    else {
        std::cout << "Unknown node" << std::endl;
    }

    freeAST(ast);
}

void discode::analyze_string(std::string str)
{
    std::cout << str << std::endl;

    std::ofstream out;
    out.open("temp.txt");
    out << str;
    out.close();

    discode::analyze_file("temp.txt");
}

void discode::loadVM(discode::VM * vm, std::string channel, std::string path, std::string msg_id)
{
    Parse_Error err;
    AST_Node * ast = parse(path.c_str(), &err);

    if(ast) {
        load(vm, ast, channel, msg_id);
        freeAST(ast);
    }
    else {
        json::JsonObject obj = json::JsonObject();
        obj.add("Name", std::make_shared<json::JsonString>("Error"));
        obj.add("Error", std::make_shared<json::JsonString>(err.txt));
        obj.add("Message_id", std::make_shared<json::JsonString>(msg_id));
        vm->sendObject(&obj);
    }
}

void discode::loadVM_string(discode::VM * vm, std::string channel, std::string str, std::string msg_id)
{
    std::ofstream out;
    out.open("temp.txt");
    out << str;
    out.close();

    discode::loadVM(vm, channel, "temp.txt", msg_id);
}
