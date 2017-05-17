// Name: Brandon Alvino
// Project: Assignment 2
// Date: February 7, 2016
#define RED 126
#define BLACK 120

#include <iostream>
#include <windows.h>
using namespace std;

struct Node
{
	int data;
	__int8 color = RED;
	Node * right = NULL;
	Node * left = NULL;
};

class RBTree
{
private:
	Node * root;
	int size;
	bool violation;
	bool child_violation;

	void goTo(int x, int y, int c)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos;

		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(hConsole, pos);

		if (c == BLACK)
			SetConsoleTextAttribute(hConsole, BLACK * 2 | 0);
		else
			SetConsoleTextAttribute(hConsole, RED * 2 | FOREGROUND_INTENSITY);
	}

	void recPrintTree(int low, int high, int lev, Node * &trav)
	{
		int mid = (high + low) / 2;

		goTo(mid, lev, trav->color);
		cout << trav->data;

		if (trav->left)
			recPrintTree(low, mid, lev + 4, trav->left);

		if (trav->right)
			recPrintTree(mid, high, lev + 4, trav->right);
	}

	void leftRotate(Node * &temp)
	{
		Node * temp2 = temp;
		temp = temp->right;
		temp2->right = temp->left;
		temp->left = temp2;
	}

	void rightRotate(Node * &temp)
	{
		Node * temp2 = temp;
		temp = temp->left;
		temp2->left = temp->right;
		temp->right = temp2;
	}

	void rightError(Node *& trav)
	{
		if (violation)
		{
			if ((trav->left && trav->left->color == BLACK) && (!trav->left->left || trav->left->left->color == BLACK)
				&& (trav->left->right && trav->left->right->color == RED))
			{
				leftRotate(trav->left);
				trav->left->color = BLACK;

				if (trav->left->left)
					trav->left->left->color = RED;

				rightError(trav);
			}

			else if ((trav->left && trav->left->color == BLACK) && (trav->left->left && trav->left->left->color == RED))
			{
				rightRotate(trav);
				trav->color = RED;

				if (trav->right)
					trav->right->color = BLACK;

				if (trav->left)
					trav->left->color = BLACK;

				violation = false;
			}

			else if (trav->color == BLACK && (trav->left && trav->left->color == RED) && (!trav->left->left || trav->left->left->color == BLACK)
				&& (!trav->left->right || trav->left->right->color == BLACK))
			{
				rightRotate(trav);
				trav->color = BLACK;
				trav->right->color = RED;
				rightError(trav->right);
			}

			else if (trav->color == RED && (trav->left && trav->left->color == BLACK) && (!trav->left->left || trav->left->left->color == BLACK)
				&& (!trav->left->right || trav->left->right->color == BLACK))
			{
				trav->color = BLACK;
				trav->left->color = RED;
				violation = false;
			}

			else if (trav->color == BLACK && (trav->left && trav->left->color == BLACK) && (!trav->left->right || trav->left->right->color == BLACK)
				&& (!trav->left->left || trav->left->left->color == BLACK))
			{
				trav->left->color = RED;
				return; //pushes up problem
			}

			else
				cerr << "error generated from rightError()" << endl;
		}
	}

	void leftError(Node *& trav)
	{
		if (violation)
		{
			if ((trav->right && trav->right->color == BLACK) && (!trav->right->right || trav->right->right->color == BLACK)
				&& (trav->right->left && trav->right->left->color == RED))
			{
				rightRotate(trav->right);
				trav->right->color = BLACK;

				if (trav->right->right)
					trav->right->right->color = RED;

				leftError(trav);
			}

			else if ((trav->right && trav->right->color == BLACK) && (trav->right->right && trav->right->right->color == RED))
			{
				leftRotate(trav);
				trav->color = RED;

				if (trav->left)
					trav->left->color = BLACK;

				if (trav->right)
					trav->right->color = BLACK;

				violation = false;
			}

			else if (trav->color == BLACK && (trav->right && trav->right->color == RED) && (!trav->right->right || trav->right->right->color == BLACK)
				&& (!trav->right->left || trav->right->left->color == BLACK))
			{
				leftRotate(trav);
				trav->color = BLACK;
				trav->left->color = RED;
				leftError(trav->left);
			}

			else if (trav->color == RED && (trav->right && trav->right->color == BLACK) && (!trav->right->right || trav->right->right->color == BLACK)
				&& (!trav->right->left || trav->right->left->color == BLACK))
			{
				trav->color = BLACK;
				trav->right->color = RED;
				violation = false;
			}

			else if (trav->color == BLACK && (trav->right && trav->right->color == BLACK) && (!trav->right->left || trav->right->left->color == BLACK)
				&& (!trav->right->right || trav->right->right->color == BLACK))
			{
				trav->right->color = RED;
				return; //Push up problem
			}

			else
				cerr << "error generated from leftError()" << endl;

		}
	}

	void DeleteRec(int data, Node *& trav)
	{
		if (!trav) //Not in the tree
		{
			cout << data << " is not in the tree." << endl;
			return;
		}

		else if (data == trav->data) //Found
		{
			if (trav->color == BLACK)
				violation = true;

			if (!trav->right && !trav->left) //No children
			{
				trav = NULL;
				delete trav;
			}

			else if (trav->right && trav->right)  //Two children
			{
				Node * trav2 = trav->right;
				while (trav2->left)
					trav2 = trav2->left;
				trav->data = trav2->data;
				if (trav2->color == RED)
					violation = false;

				DeleteRec(trav->data, trav->right);
			}

			else
			{
				Node * temp = trav;
				if (trav->left)
					trav = trav->left;

				else
					trav = trav->right;

				trav->color = BLACK;
				violation = false;

				delete temp;
			}

			size--;
		}

		else if (data < trav->data) //right
		{
			DeleteRec(data, trav->left);
			leftError(trav);
		}

		else
		{
			DeleteRec(data, trav->right);
			rightError(trav);
		}
	}

	void Addrec(int data, Node * &trav)
	{
		if (!trav) //found insert point
		{
			trav = new Node;
			trav->data = data;
			child_violation = true;
			size++;
			return;
		}

		else if (trav->data == data)
		{
			cout << data << " already exists in the tree." << endl;
			return;
		}

		else if (data > trav->data) //go right
		{
			Addrec(data, trav->right);

			if (child_violation && trav->color == RED)
			{
				violation = true;
				return;
			}

			if (violation)
			{
				if (trav->left && trav->left->color == RED)
				{
					trav->color = RED;
					trav->left->color = trav->right->color = BLACK;
				}

				else
				{
					if (trav->right->left && trav->right->left->color == RED)
						rightRotate(trav->right);

					leftRotate(trav);
					trav->color = BLACK;
					trav->left->color = trav->right->color = RED;
				}

				violation = false;
				//took out check on trav->color
			}

			if (trav->color == RED)
				child_violation = true;
			else
				child_violation = false;
		}

		else //go left
		{
			Addrec(data, trav->left);
			if (child_violation && trav->color == RED)
			{
				violation = true;
				return;
			}

			if (violation)
			{
				if (trav->right && trav->right->color == RED)
				{
					trav->color = RED;
					trav->left->color = trav->right->color = BLACK;
				}

				else
				{
					if (trav->left->right && trav->left->right->color == RED)
						leftRotate(trav->left);

					rightRotate(trav);
					trav->color = BLACK;
					trav->left->color = trav->right->color = RED;
				}

				violation = false;
				child_violation = false;
			}

			if (trav->color == RED)
				child_violation = true;
			else
				child_violation = false;

		}
	}

	bool recSearchTree(Node * &trav, int data)
	{
		if (trav)
		{
			if (trav->data == data)
				return true;

			else if (data > trav->data)
				return recSearchTree(trav->right, data);

			else
				return recSearchTree(trav->left, data);
		}

		else
			return false;
	}

	void Clear(Node * &x)
	{
		if (x->left)
			Clear(x->left);

		if (x->right)
			Clear(x->right);

		delete x;
		x = NULL;
	}

public:
	RBTree()
	{
		root = NULL;
		size = 0;
	}

	void deleteItem(int data)
	{
		violation = false;
		DeleteRec(data, root);
		if (root)
			root->color = BLACK;
	}

	void AddItem(int data)
	{
		child_violation = violation = false;
		Addrec(data, root);
		root->color = BLACK;
	}

	bool showtree()
	{
		if (root)
		{
			system("cls");
			recPrintTree(0, 78, 0, root);
			goTo(0, 24, BLACK);
			return true;
		}

		return false;
	}

	int Size()
	{
		return size;
	}

	bool Search(int data)
	{
		if (root)
			return recSearchTree(root, data);

		return false;
	}

	bool Clear()
	{
		if (root)
		{
			Clear(root);
			size = 0;
			return true;
		}

		return false;
	}
};

int main()
{
	RBTree Tree;
	int option = 0;
	int data;

	system("color f0");
	while (option != 7)
	{
		cout << "Red Black Tree Menu Options: " << endl;
		cout << "1. Add Item        4. Search" << endl;
		cout << "2. Display Tree    5. Clear Tree" << endl;
		cout << "3. Size            6. Delete" << endl;
		cout << "                   7. Quit" << endl;
		cout << "\nSelect: ";

		cin >> option;
		system("cls");

		switch (option)
		{
		case 1:
			cout << "Data to insert: ";
			cin >> data;
			Tree.AddItem(data);
			break;

		case 2:
			if (!Tree.showtree())
				cout << "Tree is empty." << endl;

			break;

		case 3:
			cout << "Size of tree: " << Tree.Size() << endl;
			break;

		case 4:
			cout << "Search for: ";
			cin >> data;

			if (Tree.Search(data))
				cout << "Found " << data << " in the tree." << endl;

			else
				cout << "Didn't find " << data << " in the tree." << endl;

			break;

		case 5:
			if (Tree.Clear())
				cout << "Cleard the tree" << endl;

			else
				cout << "Tree is empty" << endl;

			break;

		case 6:
			cout << "Delete: ";
			cin >> data;
			Tree.deleteItem(data);
			break;

		case 7:
			break;

		default: cout << "Wrong menu option selected." << endl;

		}

		if (option != 7)
		{
			system("pause");
			system("cls");
		}
	}

	return 0;
}
