#include "Wordlist.h"
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>

#define LEFT -1
#define RIGHT 1

using std::fixed;
using std::setprecision;
using std::string;
using std::swap;

Wordlist::Wordlist() { root = nullptr; }

// Wordlist::Wordlist()

Wordlist::Wordlist(const string &file) : root(nullptr), size(0), total(0) {
	std::ifstream in(file);
	if (!in)
		throw std::runtime_error("unable to open file");
	string word;
	while (in >> word)
		insert(word);
}

Wordlist::Wordlist(const Wordlist &list) {
	root = copyHelper(list.root, nullptr);
	size = list.size;
	total = list.total;
}

Wordlist &Wordlist::operator=(Wordlist list) {
	if (this == &list)
		return *this;

	AVLTreeNode *newRoot = copyHelper(list.root, nullptr);

	deleteHelper(root);
	root = newRoot;
	size = list.size;
	total = list.total;
	return *this;
}

Wordlist::~Wordlist() {
	deleteHelper(root);
	root = nullptr;
	size = 0;
	total = 0;
}

bool Wordlist::insert(const string &word) {
	if (!root) {
		root = new AVLTreeNode(word);
		size = 1;
		total = 1;
		return true;
	}

	AVLTreeNode *current = root;
	AVLTreeNode *parent = nullptr;
	int cmp; // keep track of which node we went to last (-1 if left )

	while (current) {
		parent = current;
		if (word < current->word) {
			current = current->left;
			cmp = LEFT;
		} else if (word > current->word) {
			current = current->right;
			cmp = RIGHT;
		} else {
			// if the word is in the list
			current->count++;
			total++;
			return false;
		}
	}

	// word is not found in the list
	AVLTreeNode *node = new AVLTreeNode(word);
	node->parent = parent;
	if (cmp == LEFT)
		parent->left = node;
	else
		parent->right = node;

	size++;
	total++;
	rebalance(parent);
	return true;
}

bool Wordlist::remove(const string &word) {
	// traverse list
	AVLTreeNode *current = root;
	while (current) {
		if (word < current->word) {
			current = current->left;
		} else if (word > current->word) {
			current = current->right;
		} else {
			break;
		}
	}

	if (!current)
		return false;

	total -= current->count;
	size--;

	AVLTreeNode *rebalanceStart = nullptr;

	// node with two children
	if (current->left && current->right) {
		// predecessor
		AVLTreeNode *predecessor = maxNode(current->left);
		// swap values
		swap(current->word, predecessor->word);
		swap(current->count, predecessor->count);

		AVLTreeNode *child = predecessor->left;

		// check if predecessor has left child
		if (child)
			child->parent = predecessor->parent;

		// if predecessor is the root
		if (!predecessor->parent) {
			root = child;
			rebalanceStart = nullptr;
			// if the predecessor is the node left of the parent
		} else if (predecessor->parent->left == predecessor) {
			predecessor->parent->left = child;
			rebalanceStart = predecessor->parent;
			// else the predecessors parent's right is to be replaced with the
			// child
		} else {
			predecessor->parent->right = child;
			rebalanceStart = predecessor->parent;
		}

		delete predecessor;
		// node with 0 or 1 child
	} else {
		AVLTreeNode *child = current->left ? current->left : current->right;
		if (child)
			child->parent = current->parent;

		// if current is root
		if (!current->parent) {
			root = child;
			rebalanceStart = nullptr;
			// if current is the left node
		} else if (current->parent->left == current) {
			current->parent->left = child;
			rebalanceStart = current->parent;
			// if current is the right node
		} else if (current->parent->right == current) {
			current->parent->right = child;
			rebalanceStart = current->parent;
		}

		delete (current);
	}

	rebalance(rebalanceStart);

	return true;
}

int Wordlist::getCount(const string &word) {
	AVLTreeNode *current = root;

	while (current) {
		if (current->word == word)
			break;
		else if (current->word < word)
			current = current->right;
		else if (current->word > word)
			current = current->left;
	}

	if (!current)
		return 0;

	return current->count;
}

bool Wordlist::contains(const string &word) { return getCount(word); }

int Wordlist::differentWords() const { return size; }

int Wordlist::totalWords() const { return total; }

string Wordlist::mostFrequent() const {
	if (!root)
		throw std::invalid_argument("empty");
	int bestCount = -1;
	string bestWord;
	mostFrequentSearch(root, bestCount, bestWord);
	return bestWord + " " + std::to_string(bestCount);
}

int Wordlist::singletons() const { return singletonsSearch(root); }

void Wordlist::printWords() const {
	int i = 1;
	printWordsHelper(root, i);
}

// helper methods

int Wordlist::height(AVLTreeNode *x) { return x ? x->height : -1; }

void Wordlist::update(AVLTreeNode *x) {
	if (!x)
		return;
	int leftHeight = height(x->left);
	int rightHeight = height(x->right);

	x->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int Wordlist::getBalanceFactor(AVLTreeNode *x) {
	return height(x->left) - height(x->right);
}

void Wordlist::attachParent(AVLTreeNode *child, AVLTreeNode *parent,
                            AVLTreeNode *from) {
	if (parent) {
		if (parent->left == from)
			parent->left = child;
		else if (parent->right == from)
			parent->right = child;
	}

	if (child)
		child->parent = parent;
}

void Wordlist::rotateLeft(AVLTreeNode *x) {

	AVLTreeNode *y = x->right;
	attachParent(y, x->parent, x);
	x->right = y->left;
	if (x->right)
		x->right->parent = x;
	y->left = x;
	x->parent = y;

	update(x);
	update(y);

	if (!y->parent)
		root = y;
}

void Wordlist::rotateRight(AVLTreeNode *x) {

	AVLTreeNode *y = x->left;
	attachParent(y, x->parent, x);
	x->left = y->right;
	if (x->left)
		x->left->parent = x;
	y->right = x;
	x->parent = y;

	update(x);
	update(y);

	if (!y->parent)
		root = y;
}

void Wordlist::rebalance(AVLTreeNode *x) {
	while (x) {
		update(x);
		if (getBalanceFactor(x) == 2) {
			if (getBalanceFactor(x->left) < 0)
				rotateLeft(x->left);
			rotateRight(x);
		} else if (getBalanceFactor(x) == -2) {
			if (getBalanceFactor(x->right) > 0)
				rotateRight(x->right);
			rotateLeft(x);
		}

		x = x->parent;
	}
}

AVLTreeNode *Wordlist::maxNode(AVLTreeNode *x) {
	while (x && x->right)
		x = x->right;
	return x;
}

void Wordlist::mostFrequentSearch(AVLTreeNode *x, int &bestCount,
                                  string &bestWord) const {
	if (!x)
		return;
	mostFrequentSearch(x->left, bestCount, bestWord);
	if (x->count > bestCount || (x->count == bestCount && x->word < bestWord)) {
		bestCount = x->count;
		bestWord = x->word;
	}
	mostFrequentSearch(x->right, bestCount, bestWord);
}

int Wordlist::singletonsSearch(AVLTreeNode *x) const {
	if (!x)
		return 0;
	return (x->count == 1) + singletonsSearch(x->left) +
	       singletonsSearch(x->right);
}

void Wordlist::printWordsHelper(AVLTreeNode *x, int &i) const {
	if (!x)
		return;
	printWordsHelper(x->left, i);
	cout << i++ << ". " << x->word << " " << x->count << std::endl;
	printWordsHelper(x->right, i);
}

AVLTreeNode *Wordlist::copyHelper(AVLTreeNode *source, AVLTreeNode *parent) {
	if (!source)
		return nullptr;
	AVLTreeNode *n = new AVLTreeNode(source->word);
	n->count = source->count;
	n->parent = parent;
	n->left = copyHelper(source->left, n);
	n->right = copyHelper(source->right, n);
	int leftTree = n->left ? n->left->height : -1;
	int rightTree = n->right ? n->right->height : -1;
	n->height = 1 + (leftTree > rightTree ? leftTree : rightTree);

	return n;
}

void Wordlist::deleteHelper(AVLTreeNode *x) {
	if (!x)
		return;
	deleteHelper(x->left);
	deleteHelper(x->right);
	delete (x);
}

// Prints useful statistics about the word list
void Wordlist::printStatistics() const {
	cout << "Number of different words: " << differentWords() << endl;
	cout << "    Total number of words: " << totalWords() << endl;
	cout << "       Most frequent word: " << mostFrequent() << endl;
	cout << "     Number of singletons: " << singletons() << setprecision(0)
	     << fixed << " (" << 100.0 * singletons() / differentWords() << "%)"
	     << endl;
}
