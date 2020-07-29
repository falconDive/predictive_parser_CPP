#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"


class Variable {
public:
    TokenType type;
    std::string name;
    int line_no;
    bool isReferenced, isInitialized;

    Variable(TokenType type_of_var, std::string name_of_var, int line_declared_at);

    // set Referenced when the variable is seen as reference
    void set_Referenced(bool val) {
        isReferenced = val;
    }

    // check if variable has been referenced, to check Semantic Error
    bool get_Referenced() const {
        return isReferenced;
    }

    // When variable is initialized, set this to true
    void set_Initialized(bool val) {
        isReferenced = (val || isReferenced);
        isInitialized = val;
    }

    // check if Variable was initialized or not, used to check for Uninitialized Error
    bool get_Initialized() const {
        return isInitialized;
    }
};

// Helper class to facilitate storing whenever a variable was referenced without being initialized
class Uninitialized {
public:
    std::string name;
    int line_ref_no;

    Uninitialized(std::string name_reference, int line_no);
};

// Class to store the data of the scope for each scope in the program
class Scope {
public:

    // line and position where current scope begins and ends

    // parent of current scope
    Scope *parent;

    // list of variables declared in current scope (does not consider children or parent scope, only local scope)
    std::vector<Variable> var_decl;
    std::vector<std::vector<Variable>> saved_inits;

    // Constructor with initial line, position and parent
    Scope(Scope *);

    // Add a variable declaration
    bool add_var_decl(const Variable &);

    // Check if name exists in the local scope
    bool contains_name(const std::string &) const;

    // Returns parent scope when the end line and position of current scope is reached
    Scope *end_of_scope() const;

    // Checks if all variables declared have been referenced.
    int allVarsReferenced();

    // lookup in current scope and returns the type of the variable requested. Returns ERROR if not declared
    TokenType local_lookup(const Token &);

    // sets Variable to be referenced
    void OnVariableReferenced(const std::string &, int);

    // lookup in current and parent scope(s) and return the type of the variable requested. Returns ERROR if not declared.
    TokenType lookup(const Token &);

    // if the variable appears as lvar, sets the variable to be initialized
    void set_Variable_Initialized(const Token &, bool);

    // returns if variable is initialized or not
    bool get_Variable_Initialized(const Token &);

    // if the type of a Token is ID, checks if the variable with same name has been declared and returns the type of the variable
    TokenType ID_to_Type(const Token &, bool = true);

    void SaveScopeInit();

    void RestoreScopeInit();

};


class Parser {
private:

public:

    typedef std::vector<Token> TokenList;

    // checks for type mismatches and returns the resultant TokenType. Returns ERROR if type mismatch
    static TokenType type_checker(TokenType, int, TokenType, TokenType);

    // compares lvar and rvar types in assignment and checks for assignment type mismatch
    static void lvar_checker(const Token &lvar, TokenType rvar_type);

    // checks if line is a Declaration
    static int parse_declaration(TokenList &L);

    // parses the ID List for declarations
//    static int parse_id_list(TokenList L, int end);
    static void parse_id_list(TokenList &L, TokenList &Decl);

    // Parses the expression and returns the type of the result. Returns ERROR if any type mismatch occurs.
    static TokenType parse_expr(TokenList &TL);

    // Constructor to handle the main Parser. Automatically reads and parses from stdin.
    Parser();

    static Token expect(TokenList &T, TokenType expected_type);

    static void parse_scope_list(TokenList &T);

    static TokenType parse_primary(TokenList &T);

    static void parse_stmt(TokenList &T);

    static void parse_stmt_list(TokenList &T);

    static void parse_scope(TokenList &T);

    static TokenType parse_arithmetic(TokenList &TL);

    static TokenType parse_boolean(TokenList &TL);

    static TokenType parse_relational_operator(TokenList &TL);

    static TokenType parse_arithmetic_operator(TokenList &TL);

    static TokenType parse_boolean_operator(TokenList &TL);
};


#endif
