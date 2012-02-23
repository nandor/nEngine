/**
	@file SplayTree.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2012 Licker Nandor
*/

#ifndef SPLAYTREE_HPP
#define SPLAYTREE_HPP
#pragma once

namespace nEngine {
	template<typename Key, typename Value>
	class SplayTree {
	public:
		/**
			Create a new splay tree
		*/
		SplayTree()
			:mRoot(NULL),
			 mSize(0)
		{

		}

		/**
			Delete the splay tree
		*/
		~SplayTree()
		{
			if (mRoot != NULL) {
				delete mRoot;
			}
		}

		/**
			Get the size of the tree
			@return				Number of nodes
		*/
		unsigned size()
		{
			return mSize;
		}

		/**
			Insert a new value
			@param key			Key
			@param value		Value
		*/
		void insert(Key key, Value value)
		{
			Node* node = insertNode(key);
			node->mValue = value;
			if (mRoot != NULL)
			{
				splay(node);
			}
			mRoot = node;
		}

		/**
			Erase a node from the tree
			@param key			Key of the node
		*/
		void erase(Key key)
		{

		}

		/**
			Loop through nodes
			@param func			Function to be called for each element
		*/
		void foreach(boost::function<void(Key, Value)> func)
		{
			std::stack<Node*> stk;
			Node* cursor = mRoot;

			while (true) {
				if (cursor != NULL) {
					stk.push(cursor);
					cursor = cursor->mLeft;
					continue;
				}

				if (stk.empty()) {
					return;
				}

				cursor = stk.top();
				stk.pop();
				func(cursor->mKey, cursor->mValue);
				cursor = cursor->mRight;
			}
		}

	private:

		class Node;

	private:

		Node* rotateLeft(Node* p)
		{
			Node* x = p->mRight;
			p->mRight = x->mRight;

			if (x->mLeft != NULL) {
				x->mLeft->mParent = p;
			}
			
			x->mLeft = p;

			if (p->mParent != NULL) {
				if (p == p->mParent->mLeft) {
					p->mParent->mLeft = x;
				} else {
					p->mParent->mRight = x;
				}
			}

			x->mParent = p->mParent;
			p->mParent = x;

			return (p == mRoot) ? x : mRoot;
		}

		Node* rotateRight(Node* p)
		{
			Node* x = p->mLeft;
			p->mLeft = x->mRight;

			if (x->mRight != NULL) {
				x->mRight->mParent = p;
			}
			
			x->mRight = p;

			if (p->mParent != NULL) {
				if (p == p->mParent->mRight) {
					p->mParent->mRight = x;
				} else {
					p->mParent->mLeft = x;
				}
			}

			x->mParent = p->mParent;
			p->mParent = x;

			return (p == mRoot) ? x : mRoot;
		}

		void splay(Node* x)
		{
			if (x == mRoot) {
				return;
			}

			if (x->mParent == mRoot) {
				if (x == x->mParent->mLeft) {
					mRoot = rotateRight(mRoot);
				} else {
					mRoot = rotateLeft(mRoot);
				}
			} else {
				Node* p = x->mParent;
				Node* q = x->mParent->mParent;

				if (x == p->mLeft && p == q->mLeft) {
					mRoot = rotateRight(q);
					mRoot = rotateRight(p);
				} else if (x == p->mRight && p == q->mRight) {
					mRoot = rotateLeft(q);
					mRoot = rotateLeft(p);
				} else if (x == p->mRight && p == q->mLeft) {
					mRoot = rotateLeft(p);
					mRoot = rotateRight(q);
				} else {
					mRoot = rotateRight(p);
					mRoot = rotateLeft(q);
				}

				splay(x);
			}
		}

		Node* insertNode(Key key)
		{
			if (mRoot == NULL) {
				Node* nd = allocate(key);
				assert(nd != NULL);
				return nd;
			}

			Node *b = mRoot, *a;
			while (b != NULL)
			{
				a = b;
				if (key < b->mKey) {
					b = b->mLeft;
				} else if (b->mKey < key) {
					b = b->mRight;
				} else {
					return b;
				}
			}

			Node* parent = a;
			if (key < a->mKey) {
				assert((a->mLeft = new Node(key)) != NULL);
				a = a->mLeft;
			} else {
				assert((a->mRight = new Node(key)) != NULL);
				a = a->mRight;
			}

			a->mParent = parent;
			splay(a);

			return a;
		}

		Node* remove(Node* p, Key key)
		{
			return NULL;
		}

	private:

		unsigned mSize;
		Node* mRoot;
	};

	/**
		A node in the splay tree
	*/
	template<typename Key, typename Value>
	class SplayTree<Key, Value>::Node {
	public:
		Node(Key key)
			:mKey(key)
		{
			mLeft = mRight = mParent = NULL;
		}

		~Node() {
			if (mLeft != NULL) {
				delete mLeft;
			}

			if (mRight != NULL) {
				delete mRight;
			}
		}
		
	private:

		/// Key - requires comparison operator
		Key mKey;

		/// Data - requires assignment operator
		Value mValue;

		/// Child nodes
		Node *mLeft, *mRight, *mParent;

		friend class SplayTree<Key, Value>;
	};
};

#endif /*SPLAYTREE_HPP*/
