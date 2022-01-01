#ifndef HUFFMAN_TREE_H_
#define HUFFMAN_TREE_H_

#include "huffman/limits.h"
#include "huffman/node.h"

typedef struct huffman_tree
{
	huffman_node_t nodes[2 * CHAR_COUNT - 1]; /*!< \brief Tableau de noeuds de taille 2 * 256 - 1.*/
	uint16_t root;                            /*!< \brief Position de la racine dans arrayN. */
} huffman_tree_t;

#endif
