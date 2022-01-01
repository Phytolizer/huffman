#include "huffman/node.h"

void huffman_node_init_leaf(huffman_node_t *node, uint8_t c, uint16_t parent)
{
	node->type = kHuffmanNodeLeaf;
	node->u.leaf.c = c;
	node->freq = 0;
	node->parent = parent;
}

void huffman_node_init_node(huffman_node_t *node, uint16_t left_child, uint16_t right_child, uint32_t freq,
                            uint16_t parent)
{
	node->type = kHuffmanNodeNode;
	node->u.node.left_child = left_child;
	node->u.node.right_child = right_child;
	node->freq = freq;
	node->parent = parent;
}
