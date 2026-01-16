// #include "Wordlist.h"
//
// int main() {
// 	Wordlist *test = new Wordlist("tiny_shakespeare.txt");
//
// 	AVLTreeNode *root = test->getRoot();
//
// 	std::cout << test->differentWords() << std::endl
// 	          << test->totalWords() << std::endl;
//
// 	std::cout << root->word << std::endl << root->left->word << std::endl;
//
// 	test->printStatistics();
//
// 	// test->printWords();
// }
//
#include "Wordlist.h"
#include <cassert>
#include <iostream>

static void showTop3(const Wordlist &wl) {
	AVLTreeNode *r = wl.getRoot();
	if (!r) {
		std::cout << "(empty)\n";
		return;
	}
	std::cout << "root: " << r->word << "\n";
	if (r->left)
		std::cout << "root->left: " << r->left->word << "\n";
	if (r->right)
		std::cout << "root->right: " << r->right->word << "\n";
}

int main() {
	try {
		// Build from file
		Wordlist wl("tiny_shakespeare.txt");
		std::cout << "== printWords(small.txt) ==\n";
		wl.printWords();

		std::cout << "\n== statistics ==\n";
		wl.printStatistics(); // throws if empty

		// Basic queries
		std::cout << "\ngetCount(\"This\") = " << wl.getCount("This") << "\n";
		std::cout << "getCount(\"this\") = " << wl.getCount("this") << "\n";

		// Remove tests: leaf / one-child / two-child cases will vary by
		// contents
		std::cout << "\n== remove(\"test?\") ==\n";
		wl.remove("test?");
		wl.printWords();

		// Copy constructor
		std::cout << "\n== copy constructor test ==\n";
		Wordlist w2(wl);
		w2.printWords();
		assert(w2.differentWords() == wl.differentWords());
		assert(w2.totalWords() == wl.totalWords());

		// Assignment operator
		std::cout << "\n== assignment operator test ==\n";
		Wordlist w3("small.txt");
		w3 = wl;
		w3.printWords();
		assert(w3.differentWords() == wl.differentWords());
		assert(w3.totalWords() == wl.totalWords());

		// Peek a few nodes safely
		std::cout << "\n== root snapshot ==\n";
		showTop3(wl);
	} catch (const std::exception &e) {
		std::cerr << "error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
