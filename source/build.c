#include "huffman/build.h"
#include "huffman/node.h"
#include "huffman/sort.h"
#include <stdio.h>

bool huffman_build(huffman_state_t *state)
{
	if (state->num_leaves == 0)
	{
		fprintf(stderr, "\nFichier vide.\n\n");
		return false;
	}
	else if (state->num_leaves == 1)
	{
		state->tree.root = state->leaves[0];
		return true;
	}
	huffman_heapsort(state, state->leaves, state->num_leaves - 1);
	huffman_node_t *root = &state->tree.nodes[state->tree.root];
	root->freq = state->tree.nodes[state->leaves[0]].freq + state->tree.nodes[state->leaves[1]].freq;
	root->type = kHuffmanNodeNode;
	root->u.node.left_child = state->leaves[0];
	root->u.node.right_child = state->leaves[1];
	state->tree.nodes[state->leaves[0]].parent = state->tree.root;
	state->tree.nodes[state->leaves[1]].parent = state->tree.root;

	uint16_t small_leaf = 2;
	uint16_t small_node = state->tree.root;
	state->tree.root++;

	while (state->tree.root < CHAR_COUNT + state->num_leaves - 1)
	{
		root = &state->tree.nodes[state->tree.root];
		if (small_leaf < state->num_leaves &&
		    state->tree.nodes[state->leaves[small_leaf]].freq <= state->tree.nodes[small_node].freq)
		{
			root->freq = state->tree.nodes[state->leaves[small_leaf]].freq;
			root->type = kHuffmanNodeNode;
			root->u.node.left_child = state->leaves[small_leaf];
			state->tree.nodes[state->leaves[small_leaf]].parent = state->tree.root;
			small_leaf++;
		}
		else
		{
			root->freq = state->tree.nodes[small_node].freq;
			root->type = kHuffmanNodeNode;
			root->u.node.left_child = small_node;
			state->tree.nodes[small_node].parent = state->tree.root;
			small_node++;
		}
		if (small_node >= state->tree.root ||
		    (small_leaf < state->num_leaves &&
		     state->tree.nodes[state->leaves[small_leaf]].freq <= state->tree.nodes[small_node].freq))
		{
			root->freq += state->tree.nodes[state->leaves[small_leaf]].freq;
			root->type = kHuffmanNodeNode;
			root->u.node.right_child = state->leaves[small_leaf];
			state->tree.nodes[state->leaves[small_leaf]].parent = state->tree.root;
			small_leaf++;
		}
		else
		{
			root->freq += state->tree.nodes[small_node].freq;
			root->u.node.right_child = small_node;
			state->tree.nodes[small_node].parent = state->tree.root;
			small_node++;
		}
		root++;
	}
	root--;
	return true;
}
