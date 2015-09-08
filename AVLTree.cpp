#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <map> 
#define mp make_pair
#define f(i,n) for(int i=0;i<n;i++)
#define NO_ELEMENT -10000
using namespace std;

struct Node {
	int val;
	int howMuch;
	int height;
	Node *left, *right;
	Node(){}
};

void rotateLeft(Node *&);
void rotateRight(Node *&);
void balance(Node *&);
int findMin(Node *);
void deleteInternNode(Node *&, bool, int);
int countNodes(Node *);

int height(Node *p) {
	if (p == NULL) return 0;
	else return p->height;
	//else return max(height(p->left),height(p->right))+1;	
}

void insert(Node *&node, int key) {
	if(node == NULL){
		node = new Node();
		node->val = key;
		node->left = NULL;
		node->right = NULL;
		node->howMuch=0;
		node->height = 1;
	}else {
		if(key < node->val){
			insert(node->left, key);
			node->howMuch++;
		}else {
			insert(node->right, key);
		}
		node->height = max(height(node->left),height(node->right)) + 1;
	}
	balance(node);
}

void balance(Node *&node){
	if(height(node->right) - height(node->left) == 2) {
		if(height(node->right->left) - height(node->right->right) >= 1) {
			//rotateRight(node->right);
			rotateRight(node->right);
			rotateLeft(node);
		}
		else {
			rotateLeft(node);
		}
	}
	else if(height(node->left) - height(node->right) == 2) {
		if(height(node->left->right) - height(node->left->left) >= 1) {
			//rotateLeft(node->left);
			//cout << "evo me" << endl;
			rotateLeft(node->left);
			rotateRight(node);
		}
		else {
			//cout << "evo me1" << endl;
			rotateRight(node);
		} 
	}
}

void deleteRoot(Node *&node, int key) {
	if(node->val == key) {
		//moram izbrisati roota
		if(node->right != NULL) {
			int p = findMin(node->right);
			node->val = p;
			deleteInternNode(node, false, p);
		}
		else {
			node = node->left;
		}
	}
	else {
		if(key < node->val) {
			deleteInternNode(node, true, key);
		}else {
			deleteInternNode(node, false, key);
		}
	}
	node->height = max(height(node->left),height(node->right)) + 1;
	balance(node);
}


/*pret[pstavka je da sigurno node postoji
iskoristi funckiju find da provjeris*/

void deleteInternNode(Node *&parent, bool left, int key) {
	if(left == true) {
		parent->howMuch--;
		if(parent->left->val == key) {
			//find min
			if(parent->left->right != NULL) {
				//find min
				int p = findMin(parent->left->right);
				parent->left->val = p;
				deleteInternNode(parent->left, false, p);
			}
			else {
				parent->left = parent->left->left;
			}
		}
		else if(key < parent->left->val) {
			deleteInternNode(parent->left, true, key);
		}
		else {
			deleteInternNode(parent->left, false, key);
		}
	}else {
		//cout << "Vrijednost parenta " << parent->val << endl;
		//cout << "Vrijednost desnog " << parent->right->val << endl;
		//cout << key << endl;
		if(parent->right->val == key) {
			//cout << "what what" << endl;
			if(parent->right->right != NULL) {
				//find min
				//cout << "tu sam gdje ne treba" << endl;
				int p = findMin(parent->right->right);
				parent->right->val = p;
				deleteInternNode(parent->right, false, p);
			}
			else {
				// ?????
				//cout << "tu sam gdje treba" << endl;
				parent->right = parent->right->left;
			}
		}
		else if(key < parent->right->val) {
			deleteInternNode(parent->right, true, key);
		}
		else {
			deleteInternNode(parent->right, false, key);
		}
	}
	parent->height = max(height(parent->left),height(parent->right)) + 1;
	balance(parent);
}

int findMin(Node *node) {
	if(node->left == NULL) return node->val;
	else return findMin(node->left);
}

void rotateLeft(Node *&node){
	Node *nextRight = node->right;
	Node *tmp = node;
	Node *tmpLeft = nextRight->left;
	node = nextRight;
	node->left = tmp;
	node->howMuch = tmp->howMuch+1;
	if(tmpLeft!= NULL) {
		node->howMuch += countNodes(tmpLeft);
	}
	tmp->right = tmpLeft;
	tmp->height = max(height(tmp->left),height(tmp->right)) + 1;
	node->height = max(height(node->left), height(node->right)) + 1;
}

void rotateRight(Node *&node) {
	Node *nextLeft = node->left;
	Node *tmp = node;
	Node *tmpRight = nextLeft->right;
	node = nextLeft;
	node->right = tmp;
	//howMuch
	tmp->left = tmpRight;
	if(tmp->left != NULL){
		tmp->howMuch = countNodes(tmp->left);
		
	}else {
		tmp->howMuch = 0;
	}
	tmp->height = max(height(tmp->left), height(tmp->right)) + 1;
	node->height = max(height(node->left), height(node->right)) + 1;
}

void binaryTreeSort(Node *node, vector<int> *elem){
	if(node == NULL) return;
	binaryTreeSort(node->left, elem);
	(*elem).push_back(node->val);
	printf("%d\n", node->val);
	binaryTreeSort(node->right, elem);
}

int countNodes(Node *node) {
	if(node == NULL) return 0;
	else return node->howMuch + 1 + countNodes(node->right);	
}

int rank(Node *node, int key) {
	if(node == NULL) return 0;
	//cout << "searching for " << key << endl;
	if(node->val == key) return node->howMuch + 1;
	else if(node->val <= key) return node->howMuch + rank(node->right, key) + 1;
	else return rank(node->left,key);
}

int select(Node *node, int r) {
	if(r > 0 && node == NULL) return NO_ELEMENT; //nemoguce jer cu koristiti provjeru
	if (r == node->howMuch + 1) return node->val;
	else if(r > node->howMuch + 1) return select(node->right, r-node->howMuch-1);
	else return select(node->left, r);
}

void printNode(Node *node, int key) {
	if(node->val == key) {
		cout << "Statistika za node " << key << endl;
		cout << "  Height: " << node->height << endl;
		cout << "  HowMuch: " << node->howMuch << endl;
	}
	else if(key < node->val) {
		printNode(node->left, key);
	}
	else {
		printNode(node->right, key);
	}
}


Node *root = NULL;

int main(){
	/*insert(root, 5);
	insert(root, 10);
	insert(root, 15);*/
	insert(root, 5);
	insert(root, 2);
	insert(root, 8);
	insert(root, 1);
	insert(root, 3);
	insert(root, 7);
	insert(root, 10);
	insert(root, 4);
	insert(root, 6);
	insert(root, 9);
	insert(root, 11);
	insert(root, 12);
	deleteRoot(root, 1);
	vector<int> elem;
	binaryTreeSort(root, &elem);
	for(int i=0;i<elem.size(); i++) {
		printNode(root, elem[i]);
	}
	return 0;
}

