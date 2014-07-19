#pragma once

// Implement this class in this header file

#include <iostream>
#include <queue>
#include <stack>

using namespace std;

// Notes about template object:
// Whatever object is used as the template type for this class must implement the following:
// - Copy constructor
// - int GetKey() function
// - string ToString() function

template <typename T>
class hw6_234tree
{
private:
	struct Node
	{
	public:
		// A, B and C are the actual data values in the node		
		T* A;
		T* B;
		T* C;

		// Child pointers
		Node *Parent, *L, *M1, *M2, *R;

		Node()
		{
			A = B = C = NULL;
			Parent = L = M1 = M2 = R = NULL;
		}

		// Constructor to initialize the node with one value. The other two value 
		// pointers will be set to null and all child pointers will be set to null. 
		// The pointer given as the parameter is directly copied to A.
		Node(T* valueForA)
		{
			A = valueForA;
			B = C = NULL;
			Parent = L = M1 = M2 = R = NULL;
		}

		// Constructor to initialize the node with one value and two child pointers.
		Node(T* valueForA, Node* leftChildOfA, Node* rightChildOfA)
		{
			A = valueForA;
			L = leftChildOfA;
			M1 = rightChildOfA;

			// Everything else is NULL
			B = C = NULL;
			Parent = M2 = R = NULL;
		}

		Node(T* aVal, T* bVal, T* cVal, Node* left, Node* m1, Node* m2, Node* right)
		{
			A = aVal;
			B = bVal;
			C = cVal;
			L = left;
			M1 = m1;
			M2 = m2;
			R = right;
			Parent = NULL;
		}

		bool ContainsKey(int key)
		{
			if (C && C->GetKey() == key) { return true; }
			if (B && B->GetKey() == key) { return true; }
			if (A->GetKey() == key) { return true; }
			return false;
		}

		void DeleteData()
		{
			if (A) { delete A; A = NULL; }
			if (B) { delete B; B = NULL; }
			if (C) { delete C; C = NULL; }

			if (L) { delete A; A = NULL; }
			if (M1) { delete B; B = NULL; }
			if (M2) { delete C; C = NULL; }
			if (R) { delete C; C = NULL; }
		}

		// Returns true if all three data-value pointers, A, B, and C, are non-null, 
		// false otherwise.
		bool IsFull()
		{
			return (A != NULL && B != NULL && C != NULL);
		}

		// Returns true if all 4 child pointers are null, implying that the node is a leaf
		bool IsLeaf()
		{
			return (NULL == L && NULL == M1 && NULL == M2 && NULL == R);
		}

		Node * GetParent()
		{
			return Parent;
		}

		int ValueCount()
		{
			if (C) { return 3; }
			else if (B) { return 2; }
			else if (A) { return 1; }

			// Should never come here
			return 0;
		}

		int ChildrenCount()
		{
			if (L == NULL || R == NULL) { return 0; }
			else if (L != NULL && R == NULL)
			{
				return 0;
			}
			else if (L == NULL && R != NULL)
			{
				return -1;
			}
			else if (L && R)
			{
				if (M1 && M2 == NULL)
				{
					return 3;
				}
				else if (M2 && M1 == NULL)
				{
					return 3;
				}
				else if (M1 && M2)
				{
					return 4;
				}
				else
				{
					return 2;
				}
			}
			else if (L == NULL && R != NULL)
			{
				return 0;
			}
			// Should never come here
			return 0;
		}
	};

	void DeleteTree(Node* root)
	{
		if (root)
		{
			root->DeleteData();
			DeleteTree(root->L);
			DeleteTree(root->M1);
			DeleteTree(root->M2);
			DeleteTree(root->R);
			delete root;
		}
	}

	void Display(Node* n)
	{
		if (!n) { return; }

		// There will always be an A value with a subtree to left and right of it
		Display(n->L);
		string nodeAsString = n->A->ToString();
		cout << nodeAsString << endl;
		Display(n->M1);

		// There may or may not be a B and C value
		if (n->B)
		{
			nodeAsString = n->B->ToString();
			cout << nodeAsString << endl;
			Display(n->M2);

			if (n->C)
			{
				nodeAsString = n->C->ToString();
				cout << nodeAsString << endl;
				Display(n->R);
			}
		}
	}

	// Finds and returns the node containing the minimum value in the subtree.
	Node* FindMin(Node* root)
	{
		if (root->IsLeaf())
		{
			return root;
		}

		Node* n = root->L;
		while (n->L) { n = n->L; }
		return n;
	}

	Node* m_root;

#if defined(DEBUG) || defined(_DEBUG)
	bool IsTreeValid(Node* n)
	{
		if (!n) { return true; }

		// First make sure that A is non-null. This should always be true for any 
		// node in the tree.
		if (!n->A)
		{
			cout << "DEBUG: Tree error: Value A in node is null!" << endl;
			return false;
		}

		// Now make sure A < B < C within the node. If C is null then we just 
		// need to verify that A < B and if B and C are NULL then it's ok.
		if (n->C)
		{
			// B must not be NULL in this case
			if (NULL == n->B)
			{
				cout << "DEBUG: Tree error: Value C in node is non-null but B is null!" << endl;
				return false;
			}

			if (n->C->GetKey() <= n->B->GetKey())
			{
				cout << "DEBUG: Tree error: Value C in node is <= B!" << endl;
				return false;
			}
		}
		else if (n->B)
		{
			if (n->B->GetKey() <= n->A->GetKey())
			{
				cout << "DEBUG: Tree error: Value B in node is <= A!" << endl;
				return false;
			}
		}

		// Next we make sure we have non-null child pointers where appropriate.
		if (!n->IsLeaf())
		{
			if (n->C)
			{
				if (NULL == n->R)
				{
					cout << "DEBUG: Tree error: R pointer in node is NULL but value C is not!" << endl;
					return false;
				}
			}
			if (n->B)
			{
				if (NULL == n->M2)
				{
					cout << "DEBUG: Tree error: M2 pointer in node is NULL but value B is not!" << endl;
					return false;
				}
			}
			if (NULL == n->M1)
			{
				cout << "DEBUG: Tree error: M1 pointer in node (key=";
				cout << n->A->GetKey() << ") is NULL!" << endl;
				return false;
			}
			if (NULL == n->L)
			{
				cout << "DEBUG: Tree error: L pointer in node is NULL!" << endl;
				return false;
			}
		}

		// For each value in the node, the left subtree should be all less and the right 
		// subtree all greater.
		T* vals[] = { n->A, n->B, n->C };
		Node* children[] = { n->L, n->M1, n->M1, n->M2, n->M2, n->R };
		for (int i = 0; i < 3; i++)
		{
			if (NULL == vals[i]) { break; }
			int key = vals[i]->GetKey();

			// Make sure all keys in left subtree are less
			stack<Node*> nodes;
			nodes.push(children[i / 2]);
			while (!nodes.empty())
			{
				Node* temp = nodes.top();
				nodes.pop();
				if (!temp) { continue; }

				if (temp->A->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}
				if (temp->B && temp->B->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}
				if (temp->C && temp->C->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}

				// Push children
				nodes.push(temp->L);
				nodes.push(temp->M1);
				nodes.push(temp->M2);
				nodes.push(temp->R);
			}

			// TODO: Right subtree - implement it on your own if you need the 
			// extra error checking
		}

		// Recurse for all children
		return (IsTreeValid(n->L) && IsTreeValid(n->M1) &&
			IsTreeValid(n->M2) && IsTreeValid(n->R));
	}
#endif

public:
	hw6_234tree(void)
	{
		m_root = new Node();
	}

	virtual ~hw6_234tree(void)
	{
		DeleteTree(m_root);
		m_root = NULL;
	}

	bool Add(T& toCopyAndAdd)
	{
		T* obj = new T(toCopyAndAdd), *buffer, *tempT;

		if (m_root->A == NULL)
		{
			Node *new_node = new Node(obj);
			m_root = new_node;
			return true;
		}

		if (Find(obj->GetKey()))
		{
			return false;
		}

		Node *temp, *new_node, *buff, *newLeft, *newRight;
		queue<Node *> nodes;
		nodes.push(m_root);

		while (!nodes.empty())
		{
			temp = nodes.front();
			nodes.pop();

			if (!temp->IsLeaf()) // If current node has children
			{
/////////////////////////////////////////////////////////////////////////////4 Node Non-Leaf///////////////////////////////////////////////////////////////////////////////////
				if (temp->IsFull()) // if current node is a 4-node
				{
					buffer = temp->B;
					newLeft = new Node(temp->A);
					newRight = new Node(temp->C);
/***********************************************************************************ROOT**************************************************************************************/
					if (temp->Parent == NULL) // If current node is the root
					{
						buff = new Node(buffer); // split node into 3 nodes with B-node as new root and A-node and C-node as its left and right children respectively
						newLeft->L = temp->L;
						newRight->L = temp->M2;
						newLeft->R = temp->M1;
						newRight->R = temp->R;

						newLeft->L->Parent = newLeft;
						newRight->L->Parent = newRight;
						newLeft->R->Parent = newLeft;
						newRight->R->Parent = newRight;

						newLeft->Parent = buff;
						newRight->Parent = buff;

						buff->L = newLeft;
						buff->R = newRight;

						m_root = buff; // Set B-node as new root
						temp = m_root;

						///////////////////////////////////
						if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
						{
							if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
							{
								return false;
							}
							nodes.push(temp->L);
							continue;
						}
						else // if param key is more than root's A-value
						{
							nodes.push(temp->R);
							continue;
						}
						///////////////////////////////////
					}
/***********************************************************************************ROOT**************************************************************************************/

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+**+*+*+INTERNAL NODE+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+**+*+*+*+*+*+*/
					else // Current node is normal internal node
					{
/****************************************************************************PARENT IS A 3-NODE*******************************************************************************/
						if (temp->Parent->B) // if parent node is a 3-node
						{
							if (temp->Parent->A->GetKey() > buffer->GetKey()) // if buffer key is less than Parent's A-value
							{
								tempT = temp->Parent->A; // Make buffer object A-value and shift contents of Parent to right
								temp->Parent->A = buffer;
								temp->Parent->C = temp->Parent->B;
								temp->Parent->B = tempT;

								temp->Parent->M2 = temp->Parent->M1; // Move M1 child to M2

								newLeft->L = temp->L; // Split current node into two new nodes
								newRight->L = temp->M2;
								newLeft->R = temp->M1;
								newRight->R = temp->R;

								newLeft->L->Parent = newLeft;
								newRight->L->Parent = newRight;
								newLeft->R->Parent = newLeft;
								newRight->R->Parent = newRight;

								temp->Parent->L = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->M1 = newRight;

								temp->Parent->L->Parent = temp->Parent;
								temp->Parent->M1->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else if (obj->GetKey() > temp->B->GetKey() && obj->GetKey() <= temp->C->GetKey()) // if param key is more
									// than B-value less than or equal to root's C-value
								{
									if (obj->GetKey() == temp->C->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M2);
									continue;
								}
								else // if param key is more than root's C-value
								{
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////
							}
							else if (temp->Parent->A->GetKey() < buffer->GetKey() && temp->Parent->B->GetKey() > buffer->GetKey())
								// if param key is more than Parent's A-value but less than Parent's B-value
							{
								tempT = temp->Parent->B; // Make param object B-value and shift Parent's current B-value to right
								temp->Parent->B = buffer;
								temp->Parent->C = tempT;

								newLeft->L = temp->L; // Split current node into two new nodes
								newRight->L = temp->M2;
								newLeft->R = temp->M1;
								newRight->R = temp->R;

								newLeft->L->Parent = newLeft;
								newRight->L->Parent = newRight;
								newLeft->R->Parent = newLeft;
								newRight->R->Parent = newRight;

								temp->Parent->M1 = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->M2 = newRight;

								temp->Parent->M1->Parent = temp->Parent;
								temp->Parent->M2->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else if (obj->GetKey() > temp->B->GetKey() && obj->GetKey() <= temp->C->GetKey()) // if param key is more
									// than B-value less than or equal to root's C-value
								{
									if (obj->GetKey() == temp->C->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M2);
									continue;
								}
								else // if param key is more than root's C-value
								{
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
							else // if param key is more than Parent's B-value
							{
								// Make param object C-value
								temp->Parent->C = buffer;

								newLeft->L = temp->L; // Split current node into two new nodes
								newRight->L = temp->M2;
								newLeft->R = temp->M1;
								newRight->R = temp->R;

								newLeft->L->Parent = newLeft;
								newRight->L->Parent = newRight;
								newLeft->R->Parent = newLeft;
								newRight->R->Parent = newRight;

								temp->Parent->M2 = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->R = newRight;

								temp->Parent->M2->Parent = temp->Parent;
								temp->Parent->R->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else if (obj->GetKey() > temp->B->GetKey() && obj->GetKey() <= temp->C->GetKey()) // if param key is more
									// than B-value less than or equal to root's C-value
								{
									if (obj->GetKey() == temp->C->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M2);
									continue;
								}
								else // if param key is more than root's C-value
								{
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
						}
/****************************************************************************PARENT IS A 3-NODE*******************************************************************************/


/****************************************************************************PARENT IS A 2-NODE*******************************************************************************/
						else // if parent node is a 2-node
						{
							if (temp->Parent->A->GetKey() > buffer->GetKey()) // if param key is less than Parent's A-value
							{
								tempT = temp->Parent->A; // Make param object A-value and shift contents of Parent to right
								temp->Parent->A = buffer;
								temp->Parent->B = tempT;

								newLeft->L = temp->L; // Split current node into two new nodes
								newRight->L = temp->M2;
								newLeft->R = temp->M1;
								newRight->R = temp->R;

								newLeft->L->Parent = newLeft;
								newRight->L->Parent = newRight;
								newLeft->R->Parent = newLeft;
								newRight->R->Parent = newRight;

								temp->Parent->L = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->M1 = newRight;

								temp->Parent->L->Parent = temp->Parent;
								temp->Parent->M1->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else // if param key is more than root's C-value
								{
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
							else// if param key is more than Parent's B-value
							{
								// Make param object B-value
								temp->Parent->B = buffer;

								newLeft->L = temp->L; // Split current node into two new nodes
								newRight->L = temp->M2;
								newLeft->R = temp->M1;
								newRight->R = temp->R;

								newLeft->L->Parent = newLeft;
								newRight->L->Parent = newRight;
								newLeft->R->Parent = newLeft;
								newRight->R->Parent = newRight;

								temp->Parent->M1 = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->R = newRight;
								
								temp->Parent->M1->Parent = temp->Parent;
								temp->Parent->R->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else // if param key is more than root's C-value
								{
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
						}
					}
				}
/////////////////////////////////////////////////////////////////////////////4 Node Non-Leaf///////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////3 Node Non-Leaf///////////////////////////////////////////////////////////////////////////////////
				else if (temp->B) // if current node is a 3-node
				{
					if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
					{
						if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
						{
							return false;
						}
						nodes.push(temp->L);
						continue;
					}
					else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
						// than A-value less than or equal to root's B-value
					{
						if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
						{
							return false;
						}
						nodes.push(temp->M1);
						continue;
					}
					else // if param key is more than root's C-value
					{
						nodes.push(temp->R);
						continue;
					}
				}
/////////////////////////////////////////////////////////////////////////////3 Node Non-Leaf///////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////2 Node Non-Leaf///////////////////////////////////////////////////////////////////////////////////
				else // if current node is a 2-node
				{
					if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
					{
						if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
						{
							return false;
						}
						nodes.push(temp->L);
						continue;
					}
					else // if param key is more than root's C-value
					{
						nodes.push(temp->R);
						continue;
					}
				}
			}

			else // Current node is a leaf
			{
/////////////////////////////////////////////////////////////////////////////4 Node Leaf///////////////////////////////////////////////////////////////////////////////////////
				if (temp->IsFull()) // if current node is a 4-node
				{
					buffer = temp->B;
					newLeft = new Node(temp->A);
					newRight = new Node(temp->C);
/***********************************************************************************ROOT**************************************************************************************/
					if (temp->Parent == NULL) // If current node is the root
					{
						buff = new Node(buffer); // split node into 3 nodes with B-node as new root and A-node and C-node as its left and right children respectively

						newLeft->Parent = buff;
						newRight->Parent = buff;

						buff->L = newLeft;
						buff->R = newRight;

						m_root = buff; // Set B-node as new root

						temp = m_root;

						///////////////////////////////////
						if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
						{
							if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
							{
								return false;
							}
							nodes.push(temp->L);
							continue;
						}
						else // if param key is more than root's A-value
						{
							nodes.push(temp->R);
							continue;
						}
						///////////////////////////////////
					}
/***********************************************************************************ROOT**************************************************************************************/


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+**+*+*+INTERNAL NODE+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+**+*+*+*+*+*+*/
					else // Current node is normal internal node
					{
/**************************************************************************PARENT IS A 3-NODE*********************************************************************************/
						if (temp->Parent->B) // if parent node is a 3-node
						{
							if (temp->Parent->A->GetKey() > buffer->GetKey()) // if param key is less than Parent's A-value
							{
								tempT = temp->Parent->A; // Make param object A-value and shift contents of Parent to right
								temp->Parent->A = buffer;
								temp->Parent->C = temp->Parent->B;
								temp->Parent->B = tempT;

								temp->Parent->M2 = temp->Parent->M1; // Move M1 child to M2

								temp->Parent->L = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->M1 = newRight;

								temp->Parent->L->Parent = temp->Parent;
								temp->Parent->M1->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else if (obj->GetKey() > temp->B->GetKey() && obj->GetKey() <= temp->C->GetKey()) // if param key is more
									// than B-value less than or equal to root's C-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M2);
									continue;
								}
								else // if param key is more than root's C-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
							else if (temp->Parent->A->GetKey() < buffer->GetKey() && temp->Parent->B->GetKey() > buffer->GetKey()) // if param key is more than Parent's A-value but less than Parent's B-value
							{
								tempT = temp->Parent->B; // Make param object B-value and shift Parent's current B-value to right
								temp->Parent->B = buffer;
								temp->Parent->C = tempT;

								temp->Parent->M1 = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->M2 = newRight;

								temp->Parent->M1->Parent = temp->Parent;
								temp->Parent->M2->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else if (obj->GetKey() > temp->B->GetKey() && obj->GetKey() <= temp->C->GetKey()) // if param key is more
									// than B-value less than or equal to root's C-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M2);
									continue;
								}
								else // if param key is more than root's C-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
							else // if param key is more than Parent's B-value
							{
								// Make param object C-value
								temp->Parent->C = buffer;

								temp->Parent->M2 = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->R = newRight;

								temp->Parent->M2->Parent = temp->Parent;
								temp->Parent->R->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else if (obj->GetKey() > temp->B->GetKey() && obj->GetKey() <= temp->C->GetKey()) // if param key is more
									// than B-value less than or equal to root's C-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M2);
									continue;
								}
								else // if param key is more than root's C-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
						}
/**************************************************************************PARENT IS A 3-NODE*********************************************************************************/


/**************************************************************************PARENT IS A 2-NODE*********************************************************************************/
						else // if parent node is a 2-node
						{
							if (temp->Parent->A->GetKey() > buffer->GetKey()) // if param key is less than Parent's A-value
							{
								tempT = temp->Parent->A; // Make param object A-value and shift contents of Parent to right
								temp->Parent->A = buffer;
								temp->Parent->B = tempT;

								temp->Parent->L = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->M1 = newRight;

								temp->Parent->L->Parent = temp->Parent; 
								temp->Parent->M1->Parent = temp->Parent;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else // if param key is more than root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
							else// if param key is more than Parent's B-value
							{
								// Make param object B-value
								temp->Parent->B = buffer;

								newLeft->Parent = temp->Parent;
								newRight->Parent = temp->Parent;

								temp->Parent->M1 = newLeft; // Place new nodes into Parent's L-child and M1-child
								temp->Parent->R = newRight;

								temp = temp->Parent;

								///////////////////////////////////
								if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->L);
									continue;
								}
								else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
									// than A-value less than or equal to root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->M1);
									continue;
								}
								else // if param key is more than root's B-value
								{
									if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
									{
										return false;
									}
									nodes.push(temp->R);
									continue;
								}
								///////////////////////////////////


								return true;
							}
						}
					}
				}
/////////////////////////////////////////////////////////////////////////////4 Node Leaf///////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////3 Node Leaf///////////////////////////////////////////////////////////////////////////////////////
				else if (temp->B) // if current node is a 3-node
				{
					if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
					{
						if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
						{
							return false;
						}
						temp->C = temp->B;
						temp->B = temp->A;
						temp->A = obj;
						break;
					}
					else if (obj->GetKey() > temp->A->GetKey() && obj->GetKey() <= temp->B->GetKey()) // if param key is more
						// than A-value less than or equal to root's B-value
					{
						if (obj->GetKey() == temp->B->GetKey()) // No Duplicates
						{
							return false;
						}
						temp->C = temp->B;
						temp->B = obj;
						break;
					}
					else // if param key is more than root's B-value
					{
						temp->C = obj;
						break;
					}
			}
/////////////////////////////////////////////////////////////////////////////3 Node Leaf///////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////2 Node Leaf///////////////////////////////////////////////////////////////////////////////////////
				else // if current node is a 2-node
				{
					if (obj->GetKey() <= temp->A->GetKey()) // if param key is less than or equal to root's A-value
					{
						if (obj->GetKey() == temp->A->GetKey()) // No Duplicates
						{
							return false;
						}
						temp->B = temp->A;
						temp->A = obj;
						break;
				}
				else // if param key is more than root's A-value
				{
					temp->B = obj;
					break;
				}
			}
		}
	}

		return true;
	}

	void Display(ostream& outstream)
	{
		if (!m_root)
		{
			outstream << "(tree is empty)";
		}
		Display(m_root);
	}

	void DisplayLevelKeys(ostream& outstream)
	{
		if (!m_root)
		{
			outstream << "(tree is empty, no levels to display)";
			return;
		}

		struct NodeLevel
		{
			Node* node;
			int level;
			NodeLevel(Node* n, int l) { node = n; level = l; }
		};

		queue<NodeLevel> nodes;
		nodes.push(NodeLevel(m_root, 0));
		int maxLevel = 0;
		while (!nodes.empty())
		{
			NodeLevel nl = nodes.front();
			nodes.pop();
			Node* n = nl.node;
			if (nl.level > maxLevel)
			{
				outstream << endl;
				maxLevel = nl.level;
			}

			outstream << "< " << n->A->GetKey() << " ";
			if (n->B) { outstream << n->B->GetKey() << " "; }
			if (n->C) { outstream << n->C->GetKey() << " "; }
			outstream << ">";

			// Enqueue children
			if (n->L) { nodes.push(NodeLevel(n->L, nl.level + 1)); }
			if (n->M1) { nodes.push(NodeLevel(n->M1, nl.level + 1)); }
			if (n->M2) { nodes.push(NodeLevel(n->M2, nl.level + 1)); }
			if (n->R) { nodes.push(NodeLevel(n->R, nl.level + 1)); }
		}
		outstream << endl;
	}

	T* Find(int key)
	{
		// You must implement this function to return the value for the object 
		// with the associated key. Return NULL if no such object exists in 
		// the tree.
		Node *temp, *temp2;
		T *obj = nullptr;
		int obj_key;
		queue<Node *> nodes;

		if (m_root->A == NULL)
		{
			return obj;
		}

		nodes.push(m_root);

		while (!nodes.empty())
		{
			temp = nodes.front();
			nodes.pop();
			
			if (!temp->IsLeaf())
			{
				if (temp->ValueCount() == 1)
				{
					nodes.push(temp->L);
					nodes.push(temp->R);
				}
				else if (temp->ValueCount() == 2)
				{
					nodes.push(temp->L);
					nodes.push(temp->M1);
					nodes.push(temp->R);
				}
				else
				{
					nodes.push(temp->L);
					nodes.push(temp->M1);
					nodes.push(temp->M2);
					nodes.push(temp->R);
				}
			}

			if (temp->ContainsKey(key))
			{
				obj = temp->A;
				if (obj->GetKey() == key)
				{
					break;
				}
				obj = temp->B;
				if (obj)
				{
					if (obj->GetKey() == key)
					{
						break;
					}
				}
				obj = temp->C;
				if (obj)
				{
					if (obj->GetKey() == key)
					{
						break;
					}
				}
			}
		}

		return obj;
	}

	T* GetMin()
	{
		if (!m_root) { return NULL; }
		Node* n = FindMin(m_root);
		return n->A;
	}

#if defined(DEBUG) || defined(_DEBUG)
	bool IsTreeValid()
	{
		return IsTreeValid(m_root);
	}
#endif

	bool Remove(int key)
	{
		if (!m_root) { return false; }

		// Implement the rest of this function for HW6.
		// Don't worry about it and leave it as is for HW5.
		// Return false if the value is not present in the tree
		// Don't forget the rules for fusing nodes
		return false;
	}
};

