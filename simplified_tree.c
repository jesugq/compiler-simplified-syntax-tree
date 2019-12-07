#include "simplified_tree.h"

/**
 * Tree Info pertains to the information that is inside of the current node,
 * which can be accessed by knowing the nodetype attribute in the Tree Node.
 * Tree Info can be either an instruction to run, an identifier to extract the
 * value from, or the value itself.
 * @param   instruction     Instruction of this node.
 * @param   identifier      Identifier of this node.
 * @param   value           Numerical value of this node.
 */
// typedef union tree_info {
//     int instruction;
//     char * identifier;
//     data_value value;
// } tree_info;

/**
 * Tree Node is each instance of a node that can be added in order to interpret
 * the compiler, and each instruction is ran depending on the nodetype showing
 * what to do with the info attribute.
 * @param   nodetype    Type of this node.
 * @param   info        Information union that shows what is inside.
 * @param   node_a      First node auxiliar for usage.
 * @param   node_b      Second node auxiliar for usage.
 * @param   node_c      Third node auxiliar for usage.
 */
// typedef struct tree_node {
//     char nodetype;
//     tree_info info;
//     tree_node * node_a;
//     tree_node * node_b;
//     tree_node * node_c;
// } tree_node;

