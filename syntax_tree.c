#include "syntax_tree.h"

/**
 * Tree Node is each instance of a node that can be added in order to interpret
 * the compiler, and each instruction is ran depending on the nodetype showing
 * what to do with the info attribute.
 * @param   nodetype    Type of this node.
 * @param   nodeinfo    Information of the instruction or operation to do.
 * @param   evaluation  Evaluation of this node.
 * @param   identifier  Identifier of this node.
 * @param   value       Value of this node.
 * @param   nodea       First auxiliar node for usage.
 * @param   nodeb       Second auxiliar node for usage.
 * @param   nodec       Third auxiliar node for usage.
 */
// typedef struct syntax_node {
//     char nodetype;
//     char nodeinfo;
//     bool evaluation;
//     char * identifier;
//     data_value * value;
//     syntax_node * node_a;
//     syntax_node * node_b;
//     syntax_node * node_c;
// } syntax_node;

/**
 * Tree Create Node creates an node of the type specified on nodetype, which
 * will define the content inside of info. Documentation regarding how the
 * nodes will be used are in the README file.
 * @param   nodetype    Type of this node.
 * @param   nodeinfo    Information of the instruction or operation to do.
 * @param   evaluation  Evaluation of this node.
 * @param   identifier  Identifier of this node.
 * @param   value       Value of this node.
 * @param   nodea       First auxiliar node for usage.
 * @param   nodeb       Second auxiliar node for usage.
 * @param   nodec       Third auxiliar node for usage.
 */
syntax_node * syntax_create_node(
    char          nodetype,
    char          nodeinfo,
    bool          evaluation,
    char *        identifier,
    data_value *  value,
    syntax_node * nodea,
    syntax_node * nodeb,
    syntax_node * nodec
) {
    syntax_node * node;
    node = (syntax_node *)calloc(0, sizeof(syntax_node));

    node->nodetype      = nodetype;
    node->nodeinfo      = nodeinfo;
    node->eval          = eval
    node->value         = value;
    node->identifier    = identifier;
    node->nodea         = node_a;
    node->nodeb         = node_b;
    node->nodec         = node_c;

    return node;
}