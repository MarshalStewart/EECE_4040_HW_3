// Algorithms_Assignment_3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>

using namespace std;

constexpr auto ERROR = -1;
constexpr auto EQUAL = 0;
constexpr auto LEFT = 1;
constexpr auto RIGHT = 2;

class Person {
public:
	string firstName;
	string lastName;
	string phoneNumber;

	Person* leftChild;
	Person* rightChild;

	Person() {
		lastName = "Doe";
		firstName = "John";
		phoneNumber = "012-345-6789";
	}

	Person(string _lastName, string _firstName, string _phoneNumber) { 
		lastName = _lastName;
		firstName = _firstName;
		phoneNumber = _phoneNumber;
	}

private:
};


class Book {
public:

	Person *bst;
	string outputFile;

	Book() {
		bst = nullptr;
		outputFile = "";
	}

	Book(string fname) : outputFile(fname) {
		bst = nullptr;
	}
	
	// Adds a person’s name (first and last) and phone number to the phone book.
	void Add(string lastName, string firstName, string phoneNumber) {

		Person* node = new Person(lastName, firstName, phoneNumber);
		_InsertBST(bst, node);

	}

	// Deletes a given person’s phone number, given only the name.
	void Delete(string lastName, string firstName) {
		// Post Order Traversal
	}

	// Locates a person’s phone number, given only the person’s name. 
	// Inorder Traveral
	string Find(string lastName, string firstName) {
		// _Search is a private function that implements BinSrchTreeSearch. This
		// is done so that the search algorithm didn't need implemented in multiple places.
		Person* cur = new Person(lastName, firstName, "");
		Person* person = _SearchBST(bst, cur);
		delete cur;

		return (person == nullptr) ? "" : person->phoneNumber;
	}

	// Changes a person’s phone number given the person’s name and new phone number.
	bool Change(string lastName, string firstName, string phoneNumber) {
		Person* cur = new Person(lastName, firstName, "");
		Person* person = _SearchBST(bst, cur);
		delete cur;
		
		if (person == nullptr) return false;
		else {
			person->phoneNumber = phoneNumber;
			return true;
		}
	}

	// Displays (dumps) entire phone book in alphabetical order. Use inorder traversal
	void Display() {
		printf("\n");
		printf("%-15s %-15s %-15s\n", "Last Name", "First Name", "Phone Number");
		_InOrder(bst);
	}
	
	// Restore Tree from file
	void Load_Tree(const char* fname) {
		FILE* f = nullptr;
		char line[256];
		fopen_s(&f, fname, "r+");

		if (f == nullptr)
			return;
		
		int cnt = 0;
		char* token;
		while (fgets(line, sizeof(line), f)) {
			if (cnt == 0) {
				cnt++;
				continue; // skip header
			}
			
			string str = string(line);
			stringstream ss(str);
			string lastName, firstName, phoneNumber;
			ss >> lastName >> firstName >> phoneNumber;

			Add(lastName, firstName, phoneNumber);

		}

		fclose(f);

	}

	// Write contents of tree to file
	void Dump_Tree() {
		if (outputFile == "") {
			return;
		}

		FILE* f = nullptr;
		fopen_s(&f, outputFile.c_str(), "w+");

		if (f == nullptr)
			return;

		fprintf(f, "%-15s %-15s %-15s\n", "Last Name", "First Name", "Phone Number");
		_InOrder_File(bst, f);
		fclose(f);
	}


private:
	/*
	* _Search
	* 
	* Private function used to search for a ptr to node. Implements BinSrchTreeSearch, 
	* inorder traversal. Could also be performed recursively
	*/
	Person* _SearchBST(Person* root, Person* person) {
		if (root == nullptr) return nullptr;

		int ret = _Compare(root, person);

		if (ret == EQUAL)
			return root;
		else if (ret == LEFT) 
			return _SearchBST(root->leftChild, person);
		else 
			return _SearchBST(root->rightChild, person);

	}

	/*
	 * _InsertBST
	 * 
	 * Implements InsertBST from PSN. 
	 * 
	 */
	void _InsertBST(Person* root, Person* node) {
		if (node == nullptr) return;

		node->leftChild = nullptr;
		node->rightChild = nullptr;

		if (root == nullptr)
			bst = node;
		else {
			if (_Compare(node, root) == LEFT) {
				if (root->leftChild == nullptr) root->leftChild = node;
				else _InsertBST(root->leftChild, node);
			}
			else { // LEFT or EQUAL
				if (root->rightChild == nullptr) root->rightChild = node;
				else _InsertBST(root->rightChild, node);
			}
		}

	}

	// returns bigger person
	int _Compare(Person* a, Person* b) {
		if (a == nullptr || b == nullptr) return ERROR;

		int ret = a->lastName.compare(b->lastName);
		if (ret == 0) {
			ret = a->firstName.compare(b->firstName);
			if (ret == 0) return EQUAL;
			else if (ret > 0) return RIGHT;
			else return LEFT;
		}
		else if (ret > 0) 
			return RIGHT;
		else 
			return LEFT;

	}

	void _InOrder(Person* root) {
		if (root == nullptr) return;

		_InOrder(root->leftChild);
		printf("%-15s %-15s %-15s\n", root->lastName.c_str(), 
			root->firstName.c_str(), root->phoneNumber.c_str());
		_InOrder(root->rightChild);

	}
	
	void _InOrder_File(Person* root, FILE* f) {
		if (root == nullptr) return;

		_InOrder_File(root->leftChild, f);
		fprintf(f, "%-15s %-15s %-15s\n", root->lastName.c_str(), 
			root->firstName.c_str(), root->phoneNumber.c_str());
		_InOrder_File(root->rightChild, f);

	}

};

class UserInterface {
public:

	Book* phoneBook;
	bool quit_flag;

	UserInterface() {
		phoneBook = new Book();
		quit_flag = false;
	}

	void Start() {
		string userInput;
		system("CLS"); // clear screen
		while (!quit_flag) {
			const char* intro = "Welcome to PhoneBook Main Menu\n"
				"Press one of the following characters\n"
				"a: Print PhoneBook\n"
				"b: Load PhoneBook from file\n"
				"c: Dump Current PhoneBook to file(save)\n"
				"d: Quit\n"
				"e: Add Contact\n";
			cout << intro;
			cin >> userInput;
			if (userInput == "a") {
				phoneBook->Display();
			}
			else if (userInput == "b") {
				string userInput2;
				cout << "Enter relative path of file\n";
				cin >> userInput2;
				if (userInput2.empty()) {
					cout << "Invalid input\n";
				}
				else {
					phoneBook->Load_Tree(userInput2.c_str());
				}
			}
			else if (userInput == "c") {
				string outputFile;
				cout << "Enter relative path of output file\n";
				cin >> outputFile;
				phoneBook->outputFile = outputFile;
				phoneBook->Dump_Tree();

			}
			else if (userInput == "d") {
				Quit();
			}
			else if (userInput == "e") {
				string firstName, lastName, phoneNumber;
				cout << "Enter first name\n";
				cin >> firstName;
				cout << "Enter last name\n";
				cin >> lastName;
				cout << "Enter phone number\n";
				cin >> phoneNumber;

				phoneBook->Add(firstName, lastName, phoneNumber);
			}
			
		}
	}

	// Quits the application, after first saving the phone book in a text file. 
	void Quit() {
		phoneBook->Dump_Tree();
		quit_flag = true;
	}

private:

};

int main()
{
	UserInterface* ui = new UserInterface();
	ui->Start();

}

