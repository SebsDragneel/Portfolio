#pragma once
#include "DynArray.h"
#include "SLList.h"
#include <queue>
template<typename Type>
class Graph
{
public:
	struct Edge
	{
		unsigned int toVertex;
	};
	struct Vertex
	{
		// the data that this vertex is storing
		Type element;

		// the list of edges that connect this vertex to another vertex
		SLList<Edge> edges;

		///////////////////////////////////////////////////////////////////////////
		// Function : addEdge
		// Parameters : toVertex - the index of the vertex we are adjacent to
		///////////////////////////////////////////////////////////////////////////
		void addEdge(const unsigned int& toVertex)
		{
			Edge mEdge;
			mEdge.toVertex = toVertex;
			edges.addHead(mEdge);
		}

	};
private:
	DynArray<Vertex> mGraph;
public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : addVertex
	// Parameters : value - the data to store in this vertex
	// Return : unsigned int - the index this vertex was added at
	/////////////////////////////////////////////////////////////////////////////
	unsigned int addVertex(const Type& value)
	{
		Vertex Bob;
		Bob.element = value;
		mGraph.append(Bob);
		return mGraph.size() - 1;
	}



	/////////////////////////////////////////////////////////////////////////////
	// Function : operator[]
	// Parameters : index - the index in the graph to access
	// Return : Vertex& - the vertex stored at the specified index
	/////////////////////////////////////////////////////////////////////////////
	Vertex& operator[](const unsigned int& index)
	{
		return mGraph[index];
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : size
	// Return : unsiged int - the number of vertices in the graph
	/////////////////////////////////////////////////////////////////////////////
	unsigned int size() const
	{
		return mGraph.size();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : clear
	// Notes : clears the graph and readies it for re-use
	/////////////////////////////////////////////////////////////////////////////
	void clear()
	{
		mGraph.clear();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Function : printBreadthFirst
	// Parameters : startVertex - the vertex to start the traversal at
	// Notes : prints contents of the graph in depth order 
	//			(from closest to farthest)
	/////////////////////////////////////////////////////////////////////////////
	void printBreadthFirst(const unsigned int& startVertex)
	{
		queue<unsigned int> mQueue;

		unsigned int* mTracker = new unsigned int[mGraph.size()];

		for (unsigned int i = 0; i < mGraph.size(); ++i)
		{
			mTracker[i] = -1;
		}

		mTracker[startVertex] = 0;

		mQueue.push(startVertex);


		while (mQueue.empty() == false)
		{
			unsigned int mFront = mQueue.front();
			mQueue.pop();

			SLLIter<Edge> mIty(mGraph[mFront].edges);
			for (mIty.begin(); !mIty.end(); ++mIty)
			{

				if (mTracker[mIty.current().toVertex] == -1)
				{
					mQueue.push(mIty.current().toVertex);

					// incrementing the depth
					mTracker[mIty.current().toVertex] = mTracker[mFront] + 1;
				}

			}
			cout << this->mGraph[mFront].element << " : " << mTracker[mFront] << "\n";

		}

		delete[] mTracker;
	}

};

