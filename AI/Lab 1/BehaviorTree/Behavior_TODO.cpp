//! \file Behavior_TODO.cpp
//! \brief Implements the <code>fullsail_ai::fundamentals::Behavior</code> abstract class.
//! \author Jeremiah Blanchard with code from Cromwell D. Enage

// #include <queue>   NOTE: Only use if your QueueList is not working
#include <stack>
#include "Behavior.h"
#include "../QueueList/QueueList.h"

namespace fullsail_ai { namespace fundamentals {

	// DO NOT Edit or Use these two variables for any reason, it is for memory leak check purposes only.
	unsigned Behavior::createdCount = 0;
	unsigned Behavior::destroyedCount = 0;

	unsigned Behavior::getCreatedCount()
	{
		return createdCount;
	}

	unsigned Behavior::getDestroyedCount()
	{
		return destroyedCount;
	}

	Behavior::Behavior(char const* _description)
	{
		description = _description;
	}

	char const* Behavior::toString() const
	{
		return description;
	}

	// DO NOT edit this function, it is supposed to just return false
	bool Behavior::isLeaf() const
	{
		return false;
	}

	//! \TODO
	//!   - Implement the <code>Behavior</code> methods.
	
	size_t Behavior::getChildCount() const
	{
		return children.size();
	}


	Behavior* Behavior::getChild(size_t i)
	{
		return children.at(i);
	}

	Behavior const* Behavior::getChild(size_t i) const
	{
		return children.at(i);
	}

	void Behavior::addChild(Behavior* child)
	{
		child->parent = this;
		children.push_back(child);
	}

	void Behavior::preOrderTraverse(void(*dataFunction)(Behavior const*)) const
	{
		dataFunction(this);

		for (unsigned int i = 0; i < children.size(); i++)
			children.at(i)->preOrderTraverse(dataFunction);
	}

	void Behavior::postOrderTraverse(void(*dataFunction)(Behavior const*)) const
	{
		for (unsigned int i = 0; i < children.size(); i++)
			children.at(i)->postOrderTraverse(dataFunction);

		dataFunction(this);
	}

	void Behavior::breadthFirstTraverse(void(*dataFunction)(Behavior const*)) const
	{
		QueueList<Behavior const*> QList;

		QList.enqueue(this);


		while (!QList.isEmpty())
		{
			Behavior const* node = QList.getFront();

			dataFunction(node);

			QList.dequeue();

			for (unsigned int i = 0; i < node->getChildCount(); i++)
				QList.enqueue(node->getChild(i));
		}
	}
}

}  // namespace fullsail_ai::fundamentals
