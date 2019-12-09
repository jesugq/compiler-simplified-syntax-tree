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
    bool evaluation;
    evaluation = data_evaluation(nodea->value, nodeb->value, operation);

    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        operation,
        evaluation,
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
    data_value * value;
    value = data_operation(nodea->value, nodeb->value, operation);

    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        operation,
        SYNTAX_NULLB,
        SYNTAX_EXPR,
        SYNTAX_NULLV,
        value,
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
    data_value * value;
    value = data_operation(nodea->value, nodeb->value, operation);

    return syntax_create_node(
        SYNTAX_INSTRUCTION,
        operation,
        SYNTAX_NULLB,
        SYNTAX_TERM,
        SYNTAX_NULLV,
        value,
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

/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
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

/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
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

/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
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

/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
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

/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
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

/**
 * Basic Node Creation for the Instructions named below.
 * @param   nodea   First node.
 * @param   nodeb   Second node.
 * @param   nodec   Third node.
 * @return  Node created.
 */
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

/**
 * Interpretation of the Node. It decides what to do depending on the nodetype.
 * @param   node    Node to run.
 */
void syntax_execute_nodetype(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Decide what to do depending on the type of node.
    switch (node->nodetype) {
        case SYNTAX_INSTRUCTION:
            syntax_execute_instruction(node);
            break;
        case SYNTAX_IDENTIFIER:
            break;
        case SYNTAX_VALUE:
            break;
        default:
            printf("This nodetype is corrupted.\n");
            break;
    }
}

/**
 * Interpretation of the Instruction. It decides what to do depending on the
 * instruction value.
 * @param   node    Node to run.
 */
void syntax_execute_instruction(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Decide what to do depending on the instruction of the node.
    switch (node->instruction) {
        case SYNTAX_STMT:
            syntax_execute_stmt(node);
            break;
        case SYNTAX_ASSIGN:
            syntax_execute_assign(node);
            break;
        case SYNTAX_IF:
            syntax_execute_if(node);
            break;
        case SYNTAX_IFELSE:
            syntax_execute_ifelse(node);
            break;
        case SYNTAX_WHILE:
            syntax_execute_while(node);
            break;
        case SYNTAX_READ:
            syntax_execute_while(node);
            break;
        case SYNTAX_PRINT:
            syntax_execute_print(node);
            break;
        case SYNTAX_EXPRESSION:
            syntax_evaluate_expression(node);
            break;
        case SYNTAX_EXPR:
            syntax_operate_expr(node);
            break;
        case SYNTAX_TERM:
            syntax_operate_term(node);
        default:
            printf("This instruction is corrupted.\n");
            break;
    }
}

/**
 * Interpretation of the stmt. The stmt will run the conents in nodea, and then
 * run the contents in nodeb.
 * @param   node    Node to run.
 */
void syntax_execute_stmt(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Run contents in nodea and nodeb.
    syntax_execute_nodetype(node->nodea);
    syntax_execute_nodetype(node->nodeb);
}

/**
 * Interpretation of the assign. The assign will change the value of the value 
 * in nodea, using the value in the nodeb.
 * @param   node    Node to run.
 */
void syntax_execute_assign(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Check if the nodes are null.
    if (node->nodea == NULL) {
        printf("Assign: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodeb == NULL) {
        printf("Assign: The nodeb is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Update nodes.
    syntax_execute_nodetype(node->nodea);
    syntax_execute_nodetype(node->nodeb);

    // Get the value of the nodes.
    data_value * nodea_value = node->nodea->value;
    data_value * nodeb_value = node->nodeb->value;

    // Type check the values.
    if (nodea_value->numtype != nodeb_value->numtype) {
        printf("Assign: The nodea and nodeb differ in types.\n");
        exit(EXIT_FAILURE);
    }

    // Assign the value in nodeb to nodea.
    nodea_value->number = nodeb_value->number;
}

/**
 * Interpretation of the if. The if will execute the nodeb only if nodea's 
 * evaluation is true.
 * @param   node    Node to run.
 */
void syntax_execute_if(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Check if the children nodes are null.
    if (node->nodea == NULL) {
        printf("If: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodeb == NULL) {
        printf("If: The nodeb is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Update nodes.
    syntax_execute_nodetype(node->nodea);

    // Get the evaluation of the node.
    bool evaluation = node->nodea->evaluation;

    // Execute nodeb is the evaluation was true.
    if (evaluation)
        syntax_execute_nodetype(node->nodeb);
}

/**
 * Interpretation of the ifelse. The ifelse will execute the nodeb only if 
 * nodea's evaluation is true. It will execute nodec otherwise.
 * @param   node    Node to run.
 */
void syntax_execute_ifelse(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Check if the children nodes are null.
    if (node->nodea == NULL) {
        printf("IfElse: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodeb == NULL) {
        printf("IfElse: The nodeb is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodec == NULL) {
        printf("IfElse: The nodec is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Update nodes.
    syntax_execute_nodetype(node->nodea);

    // Get the evaluation of the node.
    bool evaluation = node->nodea->evaluation;

    // Execute nodeb is the evaluation was true. nodec otherwise
    if (evaluation)
        syntax_execute_nodetype(node->nodeb);
    else
        syntax_execute_nodetype(node->nodec);
}

/**
 * Interpretation of while. The while will execute the nodeb while the 
 * evaluation in nodea is still true.
 * @param   node    Node to run.
 */
void syntax_execute_while(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Check if the children nodes are null.
    if (node->nodea == NULL) {
        printf("While: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodeb == NULL) {
        printf("While: The nodeb is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Update nodes.
    syntax_execute_nodetype(node->nodea);

    // Get the evaluation of the node.
    bool evaluation = node->nodea->evaluation;

    while (evaluation) {
        syntax_execute_nodetype(node->nodeb);
        syntax_execute_nodetype(node->nodea);
        evaluation = node->nodea->evaluation;
    }
}

/**
 * Interpretation of read. The read will scanf depending on the type of nodea
 * and then assign a new value to nodea's value from the console.
 * @param   node    Node to run.
 */
void syntax_execute_read(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Check if the children nodes are null.
    if (node->nodea == NULL) {
        printf("Read: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Get the data of the node.
    char * nodea_identifier = node->nodea->identifier;
    data_value * nodea_value = node->nodea->value;
    char numtype = nodea_value->numtype;

    // Decide how to scan.
    if (numtype == DATA_INTEGER) {
        int input;
        printf("Enter an integer for the identifier %s : ", nodea_identifier);
        scanf("%d", &input);
        nodea_value->number.int_value = input;
    } else if (numtype == DATA_FLOAT) {
        float input;
        printf("Enter a float for the identifier %s :", nodea_identifier);
        scanf("%f", &input);
        nodea_value->number.float_value = input;
    } else {
        printf("Read: The nodea is of type unknown.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Interpretation of print. The print will printf depending on the type of the
 * value inside of nodea.
 * @param   node    Node to run.
 */
void syntax_execute_print(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Check if the children nodes are null.
    if (node->nodea == NULL) {
        printf("Read: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Get the data of the node.
    data_value * nodea_value = node->nodea->value;
    char numtype = nodea_value->numtype;

    // Decide how to scan.
    if (numtype == DATA_INTEGER)
        printf("The value is %d : ", nodea_value->number.int_value);
    else if (numtype == DATA_FLOAT)
        printf("The value is %f :", nodea_value->number.float_value);
    else {
        printf("Print: The nodea is of type unknown.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Interpretation of expression. The expression will update its evaluation
 * attribute by evaluating the values of nodea and nodeb.
 * @param   node    Node to run.
 */
void syntax_evaluate_expression(syntax_node * node) {
    // Check if the node is null.
    if (node == NULL) return;

    // Decide what to do depending on the operation.
    if (node->operation == DATA_ZERO) {
        // Check if the children nodes are null.
        if (node->nodea == NULL) {
            printf("Expression: The nodea is empty.\n");
            exit(EXIT_FAILURE);
        }

        // Evaluate to zero.
        node->evaluation = data_evaluation(node->nodea->value, node->nodeb->value, DATA_ZERO);
    } else {
        // Check if the children nodes are null.
        if (node->nodea == NULL) {
            printf("Expression: The nodea is empty.\n");
            exit(EXIT_FAILURE);
        } else if (node->nodeb = NULL) {
            printf("Expression: The nodeb is empty.\n");
            exit(EXIT_FAILURE);
        }

        // Evaluate to whichever found.
        node->evaluation = data_evaluation(node->nodea->value, node->nodeb->value, node->evaluation);
    }
}

/**
 * Interpretation of expr. The expr will update its value attribute by doing
 * the operation between nodea and nodeb.
 * @param   node    Node to run
 */
void syntax_operate_expr(syntax_node * node) {
    // Check if the node is full.
    if (node == NULL) return;

    // Check if the children are null.
    if (node->nodea == NULL) {
        printf("Expr: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodeb == NULL) {
        printf("Expr: The nodeb is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Operate the contents.
    node->value = data_operation(node->nodea->value, node->nodeb->value, node->operation);
}

/**
 * Interpretation of term. The term will update its value attribute by doing
 * the operation between nodea and nodeb.
 * @param   node    Node to run
 */
void syntax_operate_term(syntax_node * node) {
    // Check if the node is full.
    if (node == NULL) return;

    // Check if the children are null.
    if (node->nodea == NULL) {
        printf("Term: The nodea is empty.\n");
        exit(EXIT_FAILURE);
    } else if (node->nodeb == NULL) {
        printf("Term: The nodeb is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Operate the contents.
    node->value = data_operation(node->nodea->value, node->nodeb->value, node->operation);
}
