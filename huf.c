#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!
 *	\file huf.c
 *	\brief Compresseur HUFFMAN
 *	\author JOUT Samir
 *	\author DEGEORGE Philip
 *
 *	Programme de compression/decompression de fichier suivant le codage huffman.
 *
 */

/*!
 *	\def FAIL
 *
 *	Lors de l'écriture, si putc renvoie une valeur égale à EOF, ce message d'erreur sera renvoyé et le programme se
 *terminera.
 */
#define FAIL                                                                                                           \
	fprintf(stderr, "\nErreur : Lors de l'écriture (putc).\n\n");                                                  \
	exit(EXIT_FAILURE);
/*!
 *	\struct node
 *	\brief Représentation d'un noeud dans l'arbre.
 *
 *	Un noeud est composé d'une valeur, une frequence, un fils gauche / droit et un père.
 *	S'il est une feuille, les fils seront mis à -1.
 */
struct node
{
	unsigned char c;         /*!<Valeur du noeud.*/
	unsigned int freq;       /*!<Fréquence du noeud.*/
	int left, right, parent; /*!<Fils gauche, droit et parent.*/
};

struct node arrayN[2 * 256 - 1]; /*!< \brief Tableau de noeuds de taille 2 * 256 - 1.*/
int arrayL[256];                 /*!< \brief Tableau de feuilles de taille 256. */
int root = 256;                  /*!< \brief Position de la racine dans arrayN. */
unsigned int nleafs = 0;         /*!< \brief Nombre de feuilles. */
unsigned int sizeFile = 0;       /*!< \brief Nombre de caractères dans le fichier. */

void print(char **code);
void printGain(FILE *file);
/*!
 *	\fn int compare(unsigned int, unsigned int)
 *	\param x Une position dans le tableau de feuilles.
 *	\param y Une position dans le tableau de feuilles.
 *	\return 1 Si la frequence de la feuille à la position arrayL[x] est plus grande que celle à arrayL[y].
 *	\return 0 Sinon.
 */
int compare(unsigned int x, unsigned int y)
{
	if (arrayN[arrayL[x]].freq < arrayN[arrayL[y]].freq)
		return 1;
	return 0;
}
/*!
 *	\fn void sift(int *, int, int)
 *	\param array Un tableau d'entier.
 *	\param pos Position de la racine (en supposant que le tableau soit un tas).
 *	\param size Taille du tableau array.
 *
 *	Opération de tamisage qui consite à échanger la racine avec le plus grand de ses fils.
 */
void sift(int *array, int pos, int size)
{
	int j, tmp;

	j = 2 * pos;
	while (j <= size)
	{
		if (j < size && compare(j, j + 1))
			j++;
		if (compare(pos, j))
		{
			tmp = array[pos], array[pos] = array[j], array[j] = tmp;
			pos = j, j = 2 * pos;
		}
		else
			j = size + 1;
	}
}
/*!
 *	\fn void heapsort(int *,int)
 *	\param array un tableau d'entier.
 *	\param size taille du tableau array.
 *
 *	Cette fonction effectue un tri par tas sur arrayL à l'aide de la fonction compare(unsigned int, unsigned int).
 */
void heapsort(int *array, int size)
{
	int i, tmp;

	for (i = size / 2; i >= 0; i--)
		sift(array, i, size);
	for (i = size; i > 0; i--)
	{
		tmp = array[i], array[i] = array[0], array[0] = tmp;
		sift(array, 0, i - 1);
	}
}
/*!
 *	\fn int huffman()
 *	\return 0 en cas de problème.
 *	\return 1 sinon.
 *
 *	La fonction huffman() va construire l'arbre en mettant à jour l'arrayN.\n
 *	Les opérations : \n
 *		1 - On fait un tri de arrayL à l'aide du tri par tas (on a ainsi les feuilles ordonnées de la plus
 *petite fréquence à la plus grande).\n 2 - Cas particulier : on vérifie si il y'a une seule feuille, si oui la racine
 *est égale à arrayL[0].\n 3 - Sinon on crée un noeud avec arrayL[0] et arrayL[1] en fils gauche/droit. On a donc le
 *plus petit noeud possible à la position 256 dans arrayN et la plus petite feuille à la position 2 dans arrayL.\n 4 -
 *On compare la plus petite feuille et le plus petit noeud pour trouver le fils gauche. Puis on met à jour la plus
 *petite feuille et le plus petit noeud.\n 5 - On fait de même pour le fils droit.\n 6 - On répète l'action tant que
 *l'on n'a pas créé le nombre de noeuds qui est égal à nleafs - 1.\n
 */
int huffman()
{
	int smallLeaf, smallNode;
	if (nleafs == 0)
	{
		fprintf(stderr, "\nFichier vide.\n\n");
		return 0;
	}
	else if (nleafs == 1)
	{
		root = arrayL[0];
		return 1;
	}
	heapsort(arrayL, nleafs - 1);
	arrayN[root].freq = arrayN[arrayL[0]].freq + arrayN[arrayL[1]].freq;
	arrayN[root].left = arrayL[0];
	arrayN[root].right = arrayL[1];
	arrayN[arrayL[0]].parent = root;
	arrayN[arrayL[1]].parent = root;
	smallLeaf = 2, smallNode = root;
	root++;

	while (root < 256 + nleafs - 1)
	{
		if (smallLeaf < nleafs && arrayN[arrayL[smallLeaf]].freq <= arrayN[smallNode].freq)
		{
			arrayN[root].freq = arrayN[arrayL[smallLeaf]].freq;
			arrayN[root].left = arrayL[smallLeaf];
			arrayN[arrayL[smallLeaf]].parent = root;
			smallLeaf++;
		}
		else
		{
			arrayN[root].freq = arrayN[smallNode].freq;
			arrayN[root].left = smallNode;
			arrayN[smallNode].parent = root;
			smallNode++;
		}
		if (smallNode >= root ||
		    (smallLeaf < nleafs && arrayN[arrayL[smallLeaf]].freq <= arrayN[smallNode].freq))
		{
			arrayN[root].freq += arrayN[arrayL[smallLeaf]].freq;
			arrayN[root].right = arrayL[smallLeaf];
			arrayN[arrayL[smallLeaf]].parent = root;
			smallLeaf++;
		}
		else
		{
			arrayN[root].freq += arrayN[smallNode].freq;
			arrayN[root].right = smallNode;
			arrayN[smallNode].parent = root;
			smallNode++;
		}
		root++;
	}
	root--;
	return 1;
}
/*!
 *	\fn void calculCode(char **, char *, int, int)
 *	\param code Un tableau de taille 256 composé de char * pour représenter les codages.
 *	\param buff Un tableau de taille 256 (la taille maximale d'un codage) qui sera composé des 0 et 1 du codage.
 *	\param node Le noeud actuel dans le parcours de l'arbre.
 *	\param pos La position dans buff.
 *
 *	Fonction récursive qui parcourt l'arbre à gauche en ajoutant 0 jusqu'a tomber sur une feuille. Une fois sur une
 *feuille on écrit le codage puis on remplace le dernier 0 par un 1 pour aller à droite et on parcours l'arbre toujours
 *par la gauche.
 *
 */
void calculCode(char **code, char *buff, int node, int pos)
{
	int i;
	if (arrayN[node].left == -1)
	{
		if (!(code[node] = (char *)malloc(sizeof(char) * (pos + 1))))
		{
			fprintf(stderr, "\nErreur : Allocation mémoire dynamique.\n\n");
			return;
		}
		for (i = 0; i < pos; i++)
			code[node][i] = buff[i];
		code[node][pos] = '\0';
	}
	else
	{
		buff[pos++] = '0';
		calculCode(code, buff, arrayN[node].left, pos);
		buff[pos - 1] = '1';
		calculCode(code, buff, arrayN[node].right, pos);
	}
}
/*!
 *	\fn void write(FILE *, unsigned int)
 *	\param wf Le fichier sur lequel x sera écrit.
 *	\param x L'entier non signé à écrire sur le fichier.
 *
 * 	On suppose que x sera stocké sur 32 bits au maximum. \n
 * 	Ensuite on décale de 24, 16, 8, 0 pour avoir chacun des quatres octets. \n
 * 	En faisant un opérateur unaire & avec 0xff pour nettoyer les bits après le premier octet.
 */
void write(FILE *wf, unsigned int x)
{
	if (putc(x >> 24 & 0xff, wf) == EOF || putc(x >> 16 & 0xff, wf) == EOF || putc(x >> 8 & 0xff, wf) == EOF ||
	    putc(x >> 0 & 0xff, wf) == EOF)
	{
		FAIL;
	}
}
/*!
 *	\fn writeTree(FILE *, int, int *, int *)
 *	\param wf Le fichier sur lequel écrire.
 *	\param node La position dans le tableau de noeud arrayN.
 *	\param buff Entier modifié qui represente la valeur à insérer dans le fichier.
 *	\param shift La valeur du décalage pour modifier buff.
 *
 *	On parcours l'arbre récursivement de gauche à droite. On met 0 quand on croise un noeud et 1 dès que l'on arrive
 *à une feuille.\n Si on trouve une feuille, la recursion nous fait remonter d'un cran et l'on va à droite.\n On vérifie
 *aussi à chaque fois si shift < 0 c'est que l'on a fait 8 insertion de bit 0/1 donc il faut écrire le caractère et
 *remettre à 0 buff et à 7 shift.
 */
void writeTree(FILE *wf, int node, int *buff, int *shift)
{
	if (*shift < 0)
	{
		if (putc(*buff, wf) == EOF)
		{
			FAIL;
		}
		*buff = 0;
		*shift = 7;
	}
	if (arrayN[node].left == -1)
	{
		*buff = (*buff) | (1 << *shift);
		(*shift)--;
	}
	else
	{
		(*shift)--;
		writeTree(wf, arrayN[node].left, buff, shift);
		writeTree(wf, arrayN[node].right, buff, shift);
	}
}
/*!
 *	\fn void writeHeader(FILE *,int)
 *	\param wf Fichier sur lequel écrire.
 *	\param node Position du noeud actuel dans l'arbre.
 *
 *	On parcourt l'arbre récursivement de gauche à droite.\n
 *	Quand on arrive sur une feuille, on écrit le caractère. Ainsi on écrit les caractères de l'arbre de gauche à
 *droite.\n Ceci nous permet de respecter l'ordre d'apparitions des feuilles dans la fonction writeTree(FILE *, int, int
 **, int *).
 */
void writeHeader(FILE *wf, int node)
{
	if (arrayN[node].left == -1)
	{
		if (putc(arrayN[node].c, wf) == EOF)
		{
			FAIL;
		}
	}
	else
	{
		writeHeader(wf, arrayN[node].left);
		writeHeader(wf, arrayN[node].right);
	}
}
/*!
 *	\fn compress(FILE *, FILE *)
 *	\param rf Fichier à compresser.
 *	\param wf Fichier compressé.
 *
 *	On parcourt le fichier à compresser pour créer l'arbre des fréquences.\n
 *	On remplit arrayL, arrayN, sizeFile, nleafs.\n
 *	On crée l'arbre à l'aide de huffman().\n
 *	On insére l'entête et on encode le fichier.\n
 *	L'entête est composé de : \n
 *		 1 - 4 octets pour le nombre de caractère dans le fichier. \n
 *		 2 - 2 octets pour le nombre de feuille.\n
 *		 3 - (nombre de feuille) octets pour les feuilles.\n
 *		 4 - Codage de l'arbre.\n
 */
void compress(FILE *rf, FILE *wf)
{
	int c;
	int index;
	int shift;
	int tmp;
	unsigned char encode;
	char *code[256];
	char buff[256];

	for (c = 0; c < 256; c++)
	{
		arrayN[c].c = c;
		arrayN[c].freq = 0;
		arrayN[c].left = -1;
		arrayN[c].right = -1;
		arrayN[c].parent = -1;
	}
	for (c = getc(rf); c != EOF; c = getc(rf))
	{
		if (arrayN[c].freq == UINT_MAX)
		{
			fprintf(stderr, "\nLimite UINT_MAX atteinte.\n\n");
			exit(EXIT_FAILURE);
		}
		arrayN[c].freq++;
		if (sizeFile == UINT_MAX)
		{
			fprintf(stderr, "\nLimite UINT_MAX atteinte.\n\n");
			exit(EXIT_FAILURE);
		}
		sizeFile++;
	}
	for (c = 0; c < 256; c++)
	{
		if (arrayN[c].freq != 0)
			arrayL[nleafs++] = c;
	}
	if (!huffman())
		return;
	if (nleafs == 1)
	{
		code[arrayL[0]] = (char *)malloc(sizeof(char) * 2);
		code[arrayL[0]][0] = '0';
		code[arrayL[0]][1] = '\0';
	}
	else
	{
		calculCode(code, buff, root, 0);
	}
	print(code);

	/* Compression */
	rewind(rf);
	write(wf, sizeFile);
	if (nleafs == 256)
	{
		if (putc(1, wf) == EOF || putc(255, wf) == EOF)
		{
			FAIL;
		}
	}
	else
	{
		if (putc(0, wf) == EOF || putc(nleafs & 0xff, wf) == EOF)
		{
			FAIL;
		}
	}
	writeHeader(wf, root);

	shift = 7, tmp = 0;
	writeTree(wf, root, &tmp, &shift);
	if (tmp != 0)
	{
		if (putc(tmp, wf) == EOF)
		{
			FAIL;
		}
	}
	shift = 0, encode = 0;
	for (c = getc(rf); c != EOF; c = getc(rf))
	{
		index = 0;
		while (code[c][index] != '\0')
		{
			if (shift < 8)
			{
				encode = (encode << 1) + (code[c][index] == '0' ? 0 : 1);
				shift++, index++;
			}
			else
			{
				if (putc(encode, wf) == EOF)
				{
					FAIL;
				}
				shift = 0, encode = 0;
			}
		}
	}
	if (shift > 0)
	{
		encode <<= (8 - shift);
		if (putc(encode, wf) == EOF)
		{
			FAIL;
		}
	}
	for (c = 0; c < nleafs; c++)
		free(code[arrayL[c]]);
}

int main(int argc, char **argv)
{
	FILE *rf, *wf;

	if (argc != 3)
	{
		fprintf(stderr, "\nFormat : %s [input] [output]\n\n", argv[0]);
		return 1;
	}
	if (!(rf = fopen(argv[1], "r")))
	{
		fprintf(stderr, "\nErreur :  Fichier %s introuvable.\n\n", argv[1]);
		return 1;
	}
	if (!(wf = fopen(argv[2], "wb")))
	{
		fprintf(stderr, "\nErreur : Impossible de créer le fichier %s.\n\n", argv[2]);
		return 1;
	}

	compress(rf, wf);
	printGain(wf);

	fclose(rf), fclose(wf);
	return 0;
}

void print(char **code)
{
	int i;
	unsigned int sum;
	sum = 0;
	printf("%8s%8s%15s%14s%14s\n", "Noeud", "Pere", "Fils Gauche", "Fils Droit", "Fréquence");
	printf("%7d%9s%11d%16d%13.4f\n", root, "R", arrayN[root].left, arrayN[root].right,
	       (float)arrayN[root].freq / sizeFile);
	for (i = root - 1; i >= 256; i--)
		printf("%7d%9d%11d%16d%13.4f\n", i, arrayN[i].parent, arrayN[i].left, arrayN[i].right,
		       (float)arrayN[i].freq / sizeFile);
	if (nleafs > 1)
	{
		for (i = 0; i < nleafs; i++)
			printf("%7d%9d%11d%16d%13.4f\n", arrayL[i], arrayN[arrayL[i]].parent, arrayN[arrayL[i]].right,
			       arrayN[arrayL[i]].left, (float)arrayN[arrayL[i]].freq / sizeFile);
	}
	printf("\n");
	for (i = 2; i < 30; i++)
		printf("- ");

	printf("\n\n%13s%30s\n", "Caractère", "Codage");
	for (i = 0; i < nleafs; i++)
		printf("%9d%32s\n", arrayL[i], code[arrayL[i]]);

	for (i = 0; i < nleafs; i++)
		sum += (arrayN[arrayL[i]].freq * strlen(code[arrayL[i]]));
	printf("\n\nLongueur moyenne du codage : %.2f\n", (float)sum / sizeFile);
}
void printGain(FILE *file)
{
	unsigned int size;
	rewind(file);
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	printf("\nTaille originelle : %d\n", sizeFile);
	printf("\nTaille compressée: %d\n", size);
	if (size > sizeFile)
	{
		printf("\nIl y'a une perte de : %.2f%%\n", ((float)size / sizeFile) * 100 - 100);
	}
	else
		printf("\nIl y'a un gain de : %.2f%%\n", 100 - ((float)size / sizeFile) * 100);
}
