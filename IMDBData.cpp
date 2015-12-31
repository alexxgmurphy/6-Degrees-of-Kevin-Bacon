#include "IMDBData.h"
#include <fstream>
#include <iostream>
#include <queue>

// Do for Part 1
// Function: Default Constructor
// Purpose: Loads in the actor/movie file specified,
// and populates the data into the appropriate hash maps.
// Also constructs the graph with actor relationships.
// Input: Name of file to load (by reference)
// Returns: Nothing
IMDBData::IMDBData(const std::string& fileName)
{
	std::ifstream top250(fileName, std::ifstream::in);
	std::vector<std::string> fileText;
	std::string line;
	std::string actor;
	while (!top250.eof())
	{
		getline(top250, line);
		if (line[0] == '|')
		{
			actor.append(line);
		}
		else
		{
			fileText.push_back(actor);
			actor = line;
		}
	}
	std::vector<std::string> movieVec;

	for (int i = 0; i < fileText.size(); i++)
	{
		movieVec.clear();
		size_t index = fileText[i].find_first_of('|');
		while (index != std::string::npos)
		{
			movieVec.push_back(fileText[i].substr(0, index));
			fileText[i] = fileText[i].substr(index + 1);
			index = fileText[i].find_first_of('|');
		}
		movieVec.push_back(fileText[i]);
		actor = movieVec[0];
		movieVec.erase(movieVec.begin(), movieVec.begin() + 1);

		mActorsToMoviesMap.insert(std::make_pair(actor, movieVec));
		reverseMap(actor, movieVec);
	}
}

// Do for Part 1
// Function: reverseMap
// Purpose: This is a helper function that is used to generate
// the reverse map (movies to actors)
// Input: Name of actor (by reference), vector of their movies (by reference)
// Returns: Nothing
void IMDBData::reverseMap(const std::string& actorName, const std::vector<std::string>& movies)
{
	for (int i = 0; i < movies.size(); i++)
	{
		if (mMoviesToActorsMap.find(movies[i]) == mMoviesToActorsMap.end())
		{
			std::vector<std::string> actor;
			actor.push_back(actorName);
			mMoviesToActorsMap.insert(std::make_pair(movies[i], actor));
		}
		else
		{
			bool actorHere = false;
			for (int v = 0; v < mMoviesToActorsMap[movies[i]].size(); v++)
			{
				if (mMoviesToActorsMap[movies[i]][v] == actorName)
				{
					actorHere = true;
				}
			}
			if (!actorHere)
			{
				mMoviesToActorsMap[movies[i]].push_back(actorName);
			}
		}
	}
}

// Do for Part 1
// Function: getMoviesFromActor
// Purpose: Given an actor's name, returns a vector containing their movies
// Input: Name of actor (by reference)
// Returns: Vector of movies (by reference)
std::vector<std::string>& IMDBData::getMoviesFromActor(const std::string& actorName)
{
	if (mActorsToMoviesMap.find(actorName) != mActorsToMoviesMap.end())
	{
		return mActorsToMoviesMap[actorName];
	}
	else
	{
		return sEmptyVector;
	}
}

// Do for Part 1
// Function: getActorsFromMovie
// Purpose: Given a movie's name, returns the name of its actors
// Input: Name of movie (by reference)
// Returns: Vector of actors (by reference)
std::vector<std::string>& IMDBData::getActorsFromMovie(const std::string& movieName)
{
	if (mMoviesToActorsMap.find(movieName) != mMoviesToActorsMap.end())
	{
		return mMoviesToActorsMap[movieName];
	}
	else
	{
		return sEmptyVector;
	}
}

// Do for Part 2
// Function: findRelationship
// Purpose: Tries to find a relationship between two actors, using a BFS
// and outputs (to cout) the result of the search.
// Input: Name of two actors to check for a relationship
// Returns: Nothing
void IMDBData::findRelationship(const std::string& fromActor, const std::string& toActor)
{
	//If fromActor and toActor are in the graph
	if (mGraph.containsActor(fromActor) && mGraph.containsActor(toActor))
	{
		//Set fromActor as source node
		ActorNode* source = mGraph.getActorNode(fromActor);
		//Populate queue with source
		std::queue<ActorNode*> nodeQ;
		nodeQ.push(source);
		//While queue.size() > 0
		while (nodeQ.size() > 0)
		{
			//frontNode.isVisited = true
			nodeQ.front()->mIsVisited == true;
			//If front node is goal node (toActor)	
			if (nodeQ.front() == mGraph.getActorNode(toActor))
			{
				std::cout << "Found a path!\n" << fromActor << " was in..." << std::endl;
				for (std::list<PathPair>::const_iterator iterator = nodeQ.front()->mPath.begin(), end = nodeQ.front()->mPath.end(); iterator != end; ++iterator)
				{
					std::cout << (*iterator).getMovieName() << " with " << (*iterator).getActorName();
					if ((*iterator).getActorName() != toActor)
					{
						std::cout << " who was in...";
					}
					std::cout << std::endl;
				}
				break;
			}
			//else
			else
			{
				//for each edge of front node
				for (std::list<Edge*>::const_iterator iterator = nodeQ.front()->mEdges.begin(), end = nodeQ.front()->mEdges.end(); iterator != end; ++iterator)
				{
					int count = 0;
					//if otherActor node has not been visited
					if (!(*iterator)->mOtherActor->mIsVisited)
					{
						//enqueue otherActor
						nodeQ.push((*iterator)->mOtherActor);
						if ((*iterator)->mOtherActor->mPath.empty())
						{
							(*iterator)->mOtherActor->mPath = nodeQ.front()->mPath;
							(*iterator)->mOtherActor->mPath.push_back(PathPair((*iterator)->mMovieName, (*iterator)->mOtherActor->mName));
						}
					}
				}
				//pop front node
				nodeQ.pop();
			}
		}
	}
	else if (!mGraph.containsActor(fromActor))
	{
		std::cout << fromActor << " not found!" << std::endl;
	}
	else if (!mGraph.containsActor(toActor))
	{
		std::cout << toActor << " not found!" << std::endl;
	}
	else
	{
		std::cout << toActor << " and " << fromActor << " not found!" << std::endl;
	}

	mGraph.clearVisited();
}

// For Part 2
// Function: createGraph
// Purpose: This helper function constructs the IMDBGraph from the movie to actors map
// Input: None
// Returns: Nothing
void IMDBData::createGraph()
{
	// DO NOT EDIT THIS FUNCTION
	// For every movie in the actors map...
	for (auto& p : mMoviesToActorsMap)
	{
		const std::string& movieName = p.first;
		// Get the actors for this movie
		const std::vector<std::string>& actors = mMoviesToActorsMap[movieName];

		// Make n^2 edges between these actors
		for (size_t i = 0; i < actors.size(); i++)
		{
			ActorNode* firstActor = mGraph.getActorNode(actors[i]);
			for (size_t j = i + 1; j < actors.size(); j++)
			{
				ActorNode* secondActor = mGraph.getActorNode(actors[j]);
				mGraph.createActorEdge(firstActor, secondActor, movieName);
			}
		}
	}
}

// Leave here! Do not edit!!! Use in part 1
// DO NOT REMOVE THIS LINE
std::vector<std::string> IMDBData::sEmptyVector;
