#include "PathSearch.h"

namespace fullsail_ai { namespace algorithms {

	

	PathSearch::PathSearch() : ordered(isGreater)
	{

	}


	PathSearch::~PathSearch()
	{
	}



	void PathSearch::buildGraph(TileMap* tileMap)
	{
			for (int i = 0; i < tileMap->getColumnCount(); i++)
			{
				for (int j = 0; j < tileMap->getRowCount(); j++)
				{
					SearchNode* current = new SearchNode(tileMap->getTile(j, i));
					nodes[current->data] = current;
					
					if (current->data->getRow() % 2 == 0)
					{
						Tile* AdEvenTopLeft = tileMap->getTile(j - 1, i - 1);
						Tile* AdEvenTopmiddle = tileMap->getTile(j-1, i);
						Tile* AdEvenMiddeLeft = tileMap->getTile(j, i-1);
						Tile* AdEvenMiddeRight = tileMap->getTile(j, i + 1);
						Tile* AdEvenBottomLeft = tileMap->getTile(j + 1, i - 1);
						Tile* AdEvenBottomMiddle = tileMap->getTile(j + 1, i);

						if (AdEvenTopLeft != NULL && AdEvenTopLeft->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdEvenTopLeft->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdEvenTopLeft->getYCoordinate(), 2));
							current->edges.push_back(AdEvenTopLeft);
							//current->data->addLineTo(AdEvenTopLeft, 0xFFFF0000);
							current->costs.push_back(Distance * AdEvenTopLeft->getWeight());
						}
						if (AdEvenTopmiddle != NULL && AdEvenTopmiddle->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdEvenTopmiddle->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdEvenTopmiddle->getYCoordinate(), 2));
							current->edges.push_back(AdEvenTopmiddle);
							//current->data->addLineTo(AdEvenTopmiddle, 0xFFFF0000);
							current->costs.push_back(Distance * AdEvenTopmiddle->getWeight());

						}
						if (AdEvenMiddeLeft != NULL && AdEvenMiddeLeft->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdEvenMiddeLeft->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdEvenMiddeLeft->getYCoordinate(), 2));
							current->edges.push_back(AdEvenMiddeLeft);
							//current->data->addLineTo(AdEvenMiddeLeft, 0xFFFF0000);
							current->costs.push_back(Distance * AdEvenMiddeLeft->getWeight());
						}
						if (AdEvenMiddeRight != NULL && AdEvenMiddeRight->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdEvenMiddeRight->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdEvenMiddeRight->getYCoordinate(), 2));
							current->edges.push_back(AdEvenMiddeRight);
							//current->data->addLineTo(AdEvenMiddeRight, 0xFFFF0000);
							current->costs.push_back(Distance * AdEvenMiddeRight->getWeight());
						}
						if (AdEvenBottomLeft != NULL && AdEvenBottomLeft->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdEvenBottomLeft->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdEvenBottomLeft->getYCoordinate(), 2));
							current->edges.push_back(AdEvenBottomLeft);
							//current->data->addLineTo(AdEvenBottomLeft, 0xFFFF0000);
							current->costs.push_back(Distance * AdEvenBottomLeft->getWeight());
						}
						if (AdEvenBottomMiddle != NULL && AdEvenBottomMiddle->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdEvenBottomMiddle->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdEvenBottomMiddle->getYCoordinate(), 2));
							current->edges.push_back(AdEvenBottomMiddle);
							//current->data->addLineTo(AdEvenBottomMiddle, 0xFFFF0000);
							current->costs.push_back(Distance * AdEvenBottomMiddle->getWeight());
						}

					}
					else if (current->data->getRow() % 2 == 1)
					{
						
						Tile* AdOddTopmiddle = tileMap->getTile(j -1, i );
						Tile* AdOddTopRight = tileMap->getTile(j - 1, i + 1);
						Tile* AdOddMiddeLeft = tileMap->getTile(j , i - 1);
						Tile* AdOddMiddeRight = tileMap->getTile(j , i + 1);
						Tile* AdOddBottomMiddle = tileMap->getTile(j + 1, i);
						Tile* AdOddBottomRight = tileMap->getTile(j + 1, i + 1);

						
						if (AdOddTopmiddle != NULL && AdOddTopmiddle->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdOddTopmiddle->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdOddTopmiddle->getYCoordinate(), 2));
							current->edges.push_back(AdOddTopmiddle);
							//current->data->addLineTo(AdOddTopmiddle, 0xFFFF0000);
							current->costs.push_back(Distance * AdOddTopmiddle->getWeight());
						}
						if (AdOddTopRight != NULL && AdOddTopRight->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdOddTopRight->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdOddTopRight->getYCoordinate(), 2));
							current->edges.push_back(AdOddTopRight);
							//current->data->addLineTo(AdOddTopRight, 0xFFFF0000);
							current->costs.push_back(Distance * AdOddTopRight->getWeight());
						}
						if (AdOddMiddeLeft != NULL && AdOddMiddeLeft->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdOddMiddeLeft->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdOddMiddeLeft->getYCoordinate(), 2));
							current->edges.push_back(AdOddMiddeLeft);
							//current->data->addLineTo(AdOddMiddeLeft, 0xFFFF0000);
							current->costs.push_back(Distance * AdOddMiddeLeft->getWeight());

						}
						if (AdOddMiddeRight != NULL && AdOddMiddeRight->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdOddMiddeRight->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdOddMiddeRight->getYCoordinate(), 2));
							current->edges.push_back(AdOddMiddeRight);
							/*current->data->addLineTo(AdOddMiddeRight, 0xFFFF0000);*/
							current->costs.push_back(Distance * AdOddMiddeRight->getWeight());
						
						}
						
						if (AdOddBottomMiddle != NULL && AdOddBottomMiddle->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdOddBottomMiddle->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdOddBottomMiddle->getYCoordinate(), 2));
							current->edges.push_back(AdOddBottomMiddle);
							/*current->data->addLineTo(AdOddBottomMiddle, 0xFFFF0000);*/
							current->costs.push_back(Distance * AdOddBottomMiddle->getWeight());
						}
						if (AdOddBottomRight != NULL && AdOddBottomRight->getWeight() > 0 && current->data->getWeight() > 0)
						{
							float Distance = sqrtf(powf(current->data->getXCoordinate() - AdOddBottomRight->getXCoordinate(), 2) + powf(current->data->getYCoordinate() - AdOddBottomRight->getYCoordinate(), 2));
							current->edges.push_back(AdOddBottomRight);
							/*current->data->addLineTo(AdOddBottomRight, 0xFFFF0000);*/
							current->costs.push_back(Distance * AdOddBottomRight->getWeight());
						}
					}
				}
			}
	}

	void PathSearch::initialize(TileMap* _tileMap)
	{
		done = false;
		map = _tileMap;

		buildGraph(map);

	}

	void PathSearch::enter(int startRow, int startColumn, int goalRow, int goalColumn)
	{
		start = nodes[map->getTile(startRow, startColumn)];
		end = nodes[map->getTile(goalRow, goalColumn)];
		float Distance = sqrtf(powf(start->data->getXCoordinate() - end->data->getXCoordinate(), 2) + powf(start->data->getYCoordinate() - end->data->getYCoordinate(), 2));
		ordered.push(start);
		visited[start->data] = start;
		visited[start->data]->heuristicCost = Distance;
		ordered.front()->givenCost = 0;
		ordered.front()->finalCost = start->givenCost +(start->heuristicCost * hWeight);

	}

	void PathSearch::update(long timeslice)
	{

		do
		{
				SearchNode* current = ordered.front();
				ordered.pop();
				current->data->setFill(0XFF0000FF);

				if (current == end)
				{
					while (current != nullptr)
					{
						temp.push_back(current->data);
						current = current->parent;
					}
					done = true;
					return;
				}

				for (unsigned int i = 0; i < current->edges.size(); i++)
				{
					SearchNode* successor = nodes[current->edges[i]];///////////////////////////////////////////////////
					float tempGivenCost = current->givenCost + current->costs[i];
					if (visited[successor->data] == NULL)
					{			
							successor->givenCost = tempGivenCost;
							successor->heuristicCost = sqrtf(powf(successor->data->getXCoordinate() - end->data->getXCoordinate(), 2) + powf(successor->data->getYCoordinate() - end->data->getYCoordinate(), 2));
							successor->finalCost = successor->givenCost + (successor->heuristicCost * hWeight);
							successor->parent = current;
							visited[successor->data] = successor;
							successor->data->setFill(0XFFFF00FF);
							ordered.push(successor);			
					}
					else
					{
						if (tempGivenCost < visited[successor->data]->givenCost)
						{
							successor = visited[successor->data];
							ordered.remove(successor);
							successor->givenCost = tempGivenCost;
							successor->finalCost = successor->givenCost + (successor->heuristicCost * hWeight);
							successor->parent = current;
							successor->data->setFill(0XFFFF00FF);
							ordered.push(successor);
						}
					}

				}
		} while (timeslice != 0 || !ordered.empty());
		
	}

	void PathSearch::exit()
	{
		temp.clear();
		visited.clear();
		ordered.clear();
		
	}

	void PathSearch::shutdown()
	{
		for (std::pair<Tile*, SearchNode*> Clear : visited)
			delete Clear.second;
	}

	bool PathSearch::isDone() const
	{
		return done;
	}

	std::vector<Tile const*> const PathSearch::getSolution() const
	{
		
		return temp;
	}
}}  // namespace fullsail_ai::algorithms

