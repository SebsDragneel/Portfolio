//! \file QueueList_TODO.h
//! \brief Defines the <code>fullsail_ai::data_structures::QueueList</code> class template
//! implementation.
//! \author Cromwell D. Enage
#ifndef _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_
#define _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_

namespace fullsail_ai { namespace fundamentals {

	//! \TODO
	//!   - Complete the definition of the <code>QueueList::Iterator</code> pre-increment operator.
	//!   - Complete the definition of the <code>QueueList::Iterator</code> dereference operator.
	//!   - Define the <code>QueueList</code> constructor.
	//!   - Define the <code>QueueList</code> destructor.
	//!   - Define the <code>QueueList</code> methods.

	template <typename T>
	typename QueueList<T>::Iterator& QueueList<T>::Iterator::operator++()
	{
		// TODO: Complete the definition of this operator.
		if (currentNode)
		this->currentNode = this->currentNode->next;

		
			return *this;
	}

	template <typename T>
	T QueueList<T>::Iterator::operator*() const
	{
		// TODO: Complete the definition of this operator.
		return this->currentNode->data;
	}

	template <typename T>
	 QueueList<T>::QueueList()
	{
		 this->front = nullptr;
		 this->back = nullptr;
	}

	template <typename T>
	QueueList<T>::~QueueList()
	{
		removeAll();
	}

	template <typename T>
	bool QueueList<T>::isEmpty() const
	{

		if (this->front)
			return false;
		else
		return true;
	}

	template <typename T>
	void QueueList<T>::enqueue(T element)
	{
		Node* Temp = new Node(element);

		if (this->front)
		{
			this->back->next = Temp;
			this->back = Temp;
		}
		else
		{
			this->front = Temp;
			this->back = Temp;
		}
	}

	template <typename T>
	T QueueList<T>::getFront() const
	{
		return this->front->data;
	}

	template <typename T>
	void QueueList<T>::dequeue()
	{

		if (this->front == this->back)
		{
			Node* temp = this->front;
			this->front = nullptr;
			this->back = nullptr;
			delete temp;
		}
		else
		{
			Node* temp = this->front;
			this->front = this->front->next;
			delete temp;
		}
	}

	template <typename T>
	void QueueList<T>::removeAll()
	{
		while (this->front != nullptr && this->back != nullptr)
			dequeue();
	}

	template <typename T>
	bool QueueList<T>::contains(T element) const
	{

		Node* Temp = this->front;

		while (Temp)
		{
			if (Temp->data == element)
				return true;

			Temp = Temp->next;
		}

		return false;
	}

	template <typename T>
	void QueueList<T>::remove(T element)
	{

		if (this->front->data == element)
		{
			dequeue();
			return;
		}

		Node* Iterator = this->front;

		while (Iterator->next)
		{
			if (Iterator->next->data == element)
			{
				if (Iterator->next == this->back)
				{
					this->back = Iterator;
					delete Iterator->next;
					Iterator->next = nullptr;
					return;
				}
				else
				{
					Node* Temp = Iterator->next;
					Iterator->next = Iterator->next->next;
					delete Temp;
					return;
				}
			}
			Iterator = Iterator->next;
		}
	}

	
	

	


	
	
}
}  // namespace fullsail_ai::fundamentals

#endif  // _FULLSAIL_AI_FUNDAMENTALS_QUEUELIST_TODO_H_

