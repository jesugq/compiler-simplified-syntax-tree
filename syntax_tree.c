#include "syntax_tree.h"

/**
 * Tree Node is each instance of a node that can be added in order to interpret
 * the compiler, and each instruction is ran depending on the nodetype showing
 * what to do with the info attribute.
 * @param   nodetype    Type of this node.
 * @param   operation   Operation of this node.
 * @param   evaluation  Evaluation of this node.
 * @param   instruction Instruction of this node.
 * @param   identifier  Identifier of this node.
 * @param   value       Value of this node.
 * @param   nodea       First auxiliar node for usage.
 * @param   nodeb       Second auxiliar node for usage.
 * @param   nodec       Third auxiliar node for usage.
 */
// typedef struct syntax_node {
//     char nodetype;
//     char operation;
//     bool evaluation;
//     char instruction;
//     char * identifier;
//     data_value * value;
//     struct syntax_node * nodea;
//     struct syntax_node * nodeb;
//     struct syntax_node * nodec;
// } syntax_node;

/**
 * Syntax Initialize simply returns an empty tree node.
 * @return  Node created.
 */
syntax_node * syntax_initialize() {
    syntax_node * node;
    node = (syntax_node *)malloc(sizeof(syntax_node));

    node->nodetype      = SYNTAX_NULLT;
    node->evaluation    = SYNTAX_NULLB;
    node->instruction   = SYNTAX_NULLT;
    node->identifier    = SYNTAX_NULLV;
    node->value         = SYNTAX_NULLV;
    node->nodea         = SYNTAX_NULLV;
    node->nodeb         = SYNTAX_NULLV;
    node->nodec         = SYNTAX_NULLV;

    return node;
}

/**
 * Syntax Create Node creates an node of the type specified on nodetype, which
 * will define the content inside of info. Documentation regarding how the
 * nodes will be used are in the README file.
 * @param   nodetype    Type of this node.
 * @param   evaluation  Evaluation of this node.
 * @param   instruction Instruction of this node.
 * @param   identifier  Identifier of this node.
 * @param   value       Value of this node.
 * @param   nodea       First auxiliar node for usage.
 * @param   nodeb       Second auxiliar node for usage.
 * @param   nodec       Third auxiliar node for usage.
 * @return  Node created.
 */
syntax_node * syntax_create_node(
    char nodetype,
    char operation,
    bool evaluation,
    char instruction,
    char * identifier,
    data_value * value,
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    syntax_node * node;
    node = (syntax_node *)malloc(sizeof(syntax_node));

    node->nodetype      = nodetype;
    node->operation     = operation;
    node->evaluation    = evaluation;
    node->instruction   = instruction;
    node->identifier    = identifier;
    node->value         = value;
    node->nodea         = nodea;
    node->nodeb         = nodeb;
    node->nodec         = nodec;

    return node;
}

/**
 * Syntax Create Expression is different because of the usage of relop.
 * The relop can be LESS GREATER EQUALS LTE LTE or ZERO.
 * @param   operation   Type of relop.
 * @param   nodea       First node.
 * @param   nodeb       Second node.
 * @param   nodec       Third node.
 * @return  Node created.
 */
syntax_node * syntax_create_expression(
    char operation,
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        operation,
        SYNTAX_NULLB,
        SYNTAX_EXPRESSION,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}

/**
 * Syntax Create Expr is different because of the usage of the terminals.
 * The terminal can be SUM SUBSTRACT or NEGATIVE.
 * @param   operation   Type of terminal.
 * @param   nodea       First node.
 * @param   nodeb       Second node.
 * @param   nodec       Third node.
 * @return  Node created.
 */
syntax_node * syntax_create_expr(
    char operation,
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        operation,
        SYNTAX_NULLB,
        SYNTAX_EXPR,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}

/**
 * Syntax Create Expr is different because of the usage of the terminals.
 * The terminal can be SUM SUBSTRACT or NEGATIVE.
 * @param   operation   Type of terminal.
 * @param   nodea       First node.
 * @param   nodeb       Second node.
 * @param   nodec       Third node.
 * @return  Node created.
 */
syntax_node * syntax_create_term(
    char operation,
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        operation,
        SYNTAX_NULLB,
        SYNTAX_TERM,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}

/**
 * Syntax Create Value is used for the creation of identifiers, integers or
 * floating point numbers.
 * @param   nodetype    Type, identifier integer or float.
 * @param   identifier  Identifier if available.
 * @param   value       Value included.
 * @return  Node created.
 */
syntax_node * syntax_create_value(
    char nodetype,
    char * identifier,
    data_value * value
) {
    return syntax_create_node(
        nodetype,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_NULLT,
        identifier,
        value,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        SYNTAX_NULLV
    );
}


/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
syntax_node * syntax_create_stmt(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_STMT,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}
syntax_node * syntax_create_assign(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_ASSIGN,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}
syntax_node * syntax_create_if(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_IF,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}
syntax_node * syntax_create_ifelse(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_IFELSE,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}
syntax_node * syntax_create_while(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_WHILE,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}
syntax_node * syntax_create_read(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_READ,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}
syntax_node * syntax_create_print(
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        SYNTAX_NULLT,
        SYNTAX_NULLB,
        SYNTAX_PRINT,
        SYNTAX_NULLV,
        SYNTAX_NULLV,
        nodea,
        nodeb,
        nodec
    );
}