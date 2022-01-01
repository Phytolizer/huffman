#ifndef HUFFMAN_STATE_H_
#define HUFFMAN_STATE_H_

#include "huffman/limits.h"
#include "huffman/tree.h"
#include <stdint.h>

typedef struct huffman_state
{
	huffman_tree_t tree;
	uint16_t leaves[CHAR_COUNT]; /*!< \brief Tableau de feuilles de taille 256. */
	uint16_t num_leaves;          /*!< \brief Nombre de feuilles. */
	size_t file_size;             /*!< \brief Nombre de caractères dans le fichier. */
} huffman_state_t;

huffman_state_t *huffman_state_new(void);
void huffman_state_init(huffman_state_t *state);

#endif
