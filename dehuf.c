#include <stdio.h>
#include <stdlib.h>

/*!
 *	\file dehuf.c
 *	\brief Compresseur HUFFMAN
 *	\author JOUT Samir
 *	\author DEGEORGE Philip
 *	
 *	Programme de compression/decompression de fichier suivant le codage huffman.
 *
 */

/*!
 *	\def fail(x, rf)
 *	Si un caractère extrait à l'aide getc correspond à EOF et que ferror(rf) est vrai, putc a recontré une erreur.\n
 *	Il y'a donc un affichage de message d'erreur suite à la lecture et le programme se termine.
 */
#define fail(x, rf) \
	if (x == EOF && ferror(rf)) \
		{\
			fprintf(stderr, "\nErreur : Lecture du fichier (getc).\n\n");\
			fclose(rf); exit(EXIT_FAILURE);\
		}
/*!
 *	\struct node
 *	\brief Représentation d'un noeud dans l'arbre.
 *
 *	Un noeud est composé d'une valeur, une frequence, un fils gauche / droit et un pere.
 *	Si il est une feuille, les fils gauche et droit seront mis à -1.
 */
struct node 
{
	unsigned char c;		/*!<Valeur du noeud.*/
	unsigned int freq;		/*!<Fréquence du noeud.*/
	int left, right, parent;	/*!<Fils gauche, droit et parent.*/
};

struct node arrayN[2 * 256 - 1];	/*!< \brief Tableau de noeuds de taille 2 * 256 - 1.*/
int arrayL[256];			/*!< \brief Tableau de feuilles de taille 256. */
int root = 256;				/*!< \brief Position de la racine dans arrayN. */
unsigned int nleafs = 0;		/*!< \brief Nombre de feuilles. */
unsigned int sizeFile = 0;		/*!< \brief Nombre de caractères dans le fichier. */

/*!
 *	\fn unsigned int read(FILE *)
 *	\param rf Fichier sur lequel nous allons lire.
 *	\return Un unsigned int.
 *	On récupère 4 octets et que l'on replace dans un unsigned int avec des décalages de 24, 16, 8, 0 et un | unaire.
 */
unsigned int read (FILE *rf)
{
	int x, y, z, t;
	x = getc(rf); fail(x, rf);
	y = getc(rf); fail(y, rf);
	z = getc(rf); fail(z, rf);
	t = getc(rf); fail(t, rf);
	return (x << 24 | y << 16 | z << 8 | t);
}
/*!
 *	\fn int buildTree(int *, int, int*, int)
 *	\param STACK Un tableau d'entier (qui sera un tableau provenant de la lecture de l'arbre en entête du fichier compressé).
 *	\param size Taille du tableau STACK.
 *	\param i Position dans le tableau STACK.
 *	\param node Position dans arrayN lors de la reconstruction.
 *	\return La position d'un noeud à l'aide de STACK si elle est inférieure sinon -1.
 *
 *	Cette fonction reconstruit l'arbre récursivement à partir de l'entête.
 */
int buildTree (int *STACK, int size, int *i, int node)
{
	if (*i < size)
	{
		if (STACK[*i] < 256)
		{
			return STACK[*i];
		}
		else 
		{
			node = *i;
			*i += 1;
			arrayN[STACK[node]].left = buildTree(STACK, size, i, node);
			*i += 1;
			arrayN[STACK[node]].right = buildTree(STACK, size, i, node);
			return STACK[node];
		}
	}
	return -1;
}
/*!
 *	\fn uncompress(FILE *)
 *	\param rf Fichier sur lequel nous allons lire.
 *	
 *	Cette fonction décompresse le fichier et envoie tout sur la sortie standard.\n
 *	On fait une lecture de l'en-tête pour reconstruire l'arbre.\n
 *	Puis on parcours l'arbre en fonction de chaque bit provenant du fichier jusqu'à arriver à une feuille pour l'afficher.
 */
void uncompress (FILE *rf)
{
	int c, elem, shift, node, count;
	int STACK[2 * 256 - 1];

	for (c = 0;c < 256;c++) 
	{
		arrayN[c].freq = 0, arrayN[c].c = c;
		arrayN[c].right = -1, arrayN[c].left = -1, arrayN[c].parent = -1;
	}

	sizeFile = read(rf);
	nleafs = getc(rf) + getc(rf);
	count = 0;
	while (count < nleafs)
	{
		arrayL[count++] = getc(rf);
		fail(arrayL[count - 1], rf);
	}
	count = 0, shift = 7, node = 0, c = getc(rf);
	fail(c, rf);
	while (count < nleafs)
	{
		if (shift < 0) 
		{
			shift = 7;
			c = getc(rf);
			fail(c, rf);
		}
		if (c >> shift & 1)
		{
			STACK[node] = arrayL[count];
			count++;
		}
		else 
		{
			STACK[node] = 256 + nleafs - 2 - node + count;
		}
		node++, shift--;
	}
	root = 256 + nleafs - 2;
	count = 0;
	if (STACK[0] < 256) root = arrayL[0];
	else buildTree(STACK, node, &count, 0);
	shift = -1;
	while (sizeFile > 0)
	{
		node = root;
		while (node >= 256)
		{
			if (shift < 0)
			{
				shift = 7;
				elem = getc(rf);
				fail(elem, rf);
			}
			c = elem >> shift;
			c &= 1;
			if (c == 1) node = arrayN[node].right;
			else node = arrayN[node].left;
			shift--;
		}
		printf("%c",arrayN[node].c);
		sizeFile--;
	}
}

int main (int argc, char **argv)
{
	FILE *rf;

	if (argc != 2)
	{
		fprintf(stderr, "\nFormat : %s [input]\n\n", argv[0]);
		return 1;
	}
	if(!(rf = fopen(argv[1], "r")))
	{
		fprintf(stderr, "\n Fichier %s inexistant.\n\n", argv[1]);
		return 1;	
	}
	uncompress(rf);

	fclose(rf);
	return 0;
}
