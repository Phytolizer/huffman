#ifndef HUFFMAN_NODE_H_
#define HUFFMAN_NODE_H_

#include <stddef.h>
#include <stdint.h>

#define HUFFMAN_NODE_NONE UINT16_MAX

typedef enum
{
	kHuffmanNodeLeaf,
	kHuffmanNodeNode,
} huffman_node_type_t;

typedef struct huffman_node
{
	huffman_node_type_t type;
	union {
		struct
		{
			uint8_t c; /*!<Valeur du noeud.*/
		} leaf;

		struct
		{
			uint16_t left_child;
			uint16_t right_child;
		} node;
	} u;
	uint32_t freq; /*!<Fréquence du noeud.*/
	uint16_t parent;
} huffman_node_t;

void huffman_node_init_leaf(huffman_node_t *node, uint8_t c, uint16_t parent);
void huffman_node_init_node(huffman_node_t *node, uint16_t left_child, uint16_t right_child, uint32_t freq,
                            uint16_t parent);

#endif
