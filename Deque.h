#pragma once
#include <stdexcept>

template <typename type>
class Deque		// double-ended queue
{
private:
	class Node
	{
	public:
		type data;
		Node* prev;
		Node* next;

		Node() : next(nullptr), prev(nullptr) {}
		Node(const type& _dta, Node* _prv = nullptr, Node* _nxt = nullptr)
			: data(_dta), prev(_prv), next(_nxt) {}
	};
	Node* head;
	Node* tail;
	int len;

public:
	class Handle
	{
	private:
		Node* ptr;
		Handle(Node* p = nullptr) : ptr(p) {}

	public:
		friend class Deque;

		type operator*() const {
			return ptr->data;
		}
		type* operator->() const {
			return &ptr->data;
		}
		bool operator==(const Handle& p) const {
			return ptr == p.ptr;
		}
		bool operator!=(const Handle& p) const {
			return !(*this == p);
		}
		void operator++() {
			ptr = ptr->next;
		}
		void operator++(int) {
			ptr = ptr->next;
		}
		void operator--() {
			ptr = ptr->prev;
		}
	};

	Deque();
	~Deque();
	bool empty() const;
	int size() const;
	type front() const;
	type back() const;
	Handle begin() const;
	Handle end() const;
	void push_front(const type&);
	void push_back(const type&);
	void pop_front();
	void pop_back();
};

// constructor and destructor
template <typename type>
Deque<type>::Deque() : len(0)
{
	head = tail = new Node;
	if (!head) throw std::runtime_error("Constructor");
}

template <typename type>
Deque<type>::~Deque()
{
	Node* p = head;
	while (p != nullptr)
	{
		Node* nxt = p->next;
		delete p;
		p = nxt;
	}
}

// inline and const methods
template <typename type>
bool Deque<type>::empty() const
{
	return head == tail;
}

template <typename type>
int Deque<type>::size() const
{
	return len;
}

template <typename type>
type Deque<type>::front() const
{
	if (empty()) throw std::logic_error("front");
	return head->next->data;
}

template <typename type>
type Deque<type>::back() const
{
	if (empty()) throw std::logic_error("back");
	return tail->data;
}

// traversing methods
template <typename type>
typename::Deque<type>::Handle Deque<type>::begin() const
{
	return Handle(head->next);
}

template <typename type>
typename::Deque<type>::Handle Deque<type>::end() const
{
	return Handle();
}

// modify methods
template <typename type>
void Deque<type>::push_front(const type& x)
{
	Node* first = head->next;
	Node* p = new Node(x, head, first);
	if (!p) throw std::runtime_error("push_front");

	if (empty())
		tail = p;
	else
		first->prev = p;
	head->next = p;
	len++;
}

template <typename type>
void Deque<type>::push_back(const type& x)
{
	Node* p = new Node(x, tail);
	if (!p) throw std::runtime_error("push_back");

	tail->next = p;
	tail = p;
	len++;
}

template <typename type>
void Deque<type>::pop_front()
{
	if (empty()) throw std::logic_error("pop_front");
	Node* nxt = head->next->next;

	delete head->next;
	head->next = nxt;
	if (nxt)
		nxt->prev = head;
	else
		tail = head;
	len--;
}

template <typename type>
void Deque<type>::pop_back()
{
	if (empty()) throw std::logic_error("pop_back");
	Node* prv = tail->prev;

	delete tail;
	prv->next = nullptr;
	tail = prv;
	len--;
}