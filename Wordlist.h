#pragma once
#include <complex>
#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

class AVLTreeNode {
  public:
	AVLTreeNode *parent;
	AVLTreeNode *left;
	AVLTreeNode *right;
	string word;
	int count;
	int height;

	// Constructors ...
	AVLTreeNode(string w)
	    : word(w), count(1), parent(nullptr), left(nullptr), right(nullptr),
	      height(0) {}
};

// Wordlist class
class Wordlist {
  private:
	// Class attributes go here
	// Your class MUST have a root node named root (below)
	AVLTreeNode *root; // DO NOT REMOVE
	int size{0};       // distinct words
	int total{0};      // sum of counts

	// helper functions
	int height(AVLTreeNode *x);

	void update(AVLTreeNode *x);

	int getBalanceFactor(AVLTreeNode *x);

	void attachParent(AVLTreeNode *child, AVLTreeNode *parent,
	                  AVLTreeNode *from);

	void rotateLeft(AVLTreeNode *x);

	void rotateRight(AVLTreeNode *x);

	void rebalance(AVLTreeNode *x);

	AVLTreeNode *maxNode(AVLTreeNode *x);

	int singletonsSearch(AVLTreeNode *x) const;

	void mostFrequentSearch(AVLTreeNode *x, int &bestCount,
	                        string &bestWord) const;

	void printWordsHelper(AVLTreeNode *x, int &i) const;

	AVLTreeNode *copyHelper(AVLTreeNode *source, AVLTreeNode *parent);

	void deleteHelper(AVLTreeNode *x);

  public:
	// public methods go here
	// default constructor
	Wordlist();

	// file constructor
	Wordlist(const string &file);

	// copy constructor
	Wordlist(const Wordlist &list);

	// overloaded assignment operator
	Wordlist &operator=(Wordlist list);

	// destructor
	~Wordlist();

	// insert word
	bool insert(const string &word);

	// remove word
	bool remove(const string &word);

	// returns an integer equal to the number of times its string parameter is
	// in the word list
	int getCount(const string &word);

	// returns true if the string parameter is in the word list and false
	// otherwise; hint: use getCount to implement this
	bool contains(const string &word);

	// returns an integer equal the number of distinct words in the list
	int differentWords() const;

	// returns an integer equal to the sum of all word counts in the list
	int totalWords() const;

	// returns a string consisting of the word with the largest count and its
	// count in the format word count (e.g. the 5437); throws an
	// invalid_argument exception if the word list is empty; if there is more
	// than one
	string mostFrequent() const;

	// returns an integer equal to the the number of words  with a count of 1 in
	// the word list.
	int singletons() const;

	// prints the words in word list in ascending sorted order - by word - with
	// their counts as shown below.
	void printWords() const;

	// Prints useful statistics about the word list
	void printStatistics() const;

	// Returns the root of the AVL tree
	AVLTreeNode *getRoot() const { return root; }; // DO NOT REMOVE
};
