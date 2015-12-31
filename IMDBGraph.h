#pragma once

#include <list>
#include <string>
#include <unordered_map>

// This struct is used to define a directed edge
// to another actor
struct Edge
{
    // Name of the movie (label for the edge)
    const std::string& mMovieName;
    
    // Other actor that this edge connects to
    struct ActorNode* mOtherActor;
    
    // To create an edge, you need the name of the movie
    // and a pointer to the other actor
    Edge(const std::string& movieName, struct ActorNode* otherActor)
    : mMovieName(movieName)
    , mOtherActor(otherActor)
    { }
};

// This struct is used in Part 2 of the lab -- it basically stores
// info about one "hop" during the BFS. It allows us to track
// which edges are visited along the way
struct PathPair
{
    // Each path pair must be associated with a specific movie name and
    // an actor name
    PathPair(const std::string& movieName, const std::string& actorName)
    : mMovieName(&movieName)
    , mActorName(&actorName)
    { }
    
    // Function: getMovieName
    // Purpose: Gets the name of the movie stored in this path pair
    // Input: None
    // Returns: Name of movie (by const reference)
    const std::string& getMovieName() const
    {
        return *mMovieName;
    }
    
    // Function: getActorName
    // Purpose: Gets the name of the actor stored in this path pair
    // Input: None
    // Returns: Name of actor (by const reference)
    const std::string& getActorName() const
    {
        return *mActorName;
    }
    
private:
    // Pointer to name of movie
    const std::string* mMovieName;
    
    // Pointer to name of actor
    const std::string* mActorName;
};

struct ActorNode
{
    // The name of the actor
    const std::string& mName;
    
    // Adjacency list of edges that come out from this actor
    std::list<Edge*> mEdges;
    
    // This list is used to track the path during a BFS
    // (For implementing Part 2)
    std::list<PathPair> mPath;
    
    // This bool is used to track whether or not the node was visited
    bool mIsVisited;
    
    // Constructor takes in the name of the actor
    ActorNode(const std::string& name)
    : mName(name)
    , mIsVisited(false)
    { }
    
    // Destructor deletes all the edges
    ~ActorNode();
};

// Actual Graph class
class IMDBGraph
{
public:
    // Function: containsActor
    // Purpose: Checks whether the requested actor is in the graph
    // Input: Name of actor (by const reference)
    // Returns: True if the actor is in the graph
    bool containsActor(const std::string& actorName);
    
    // Function: getActorNode
    // Purpose: If the actor doesn't exist, creates a node for the actor.
    // Otherwise, returns the ActorNode associated w/ the specified actor.
    // Input: Name of actor (by const reference)
    // Returns: Pointer to ActorNode associated with the actor
    ActorNode* getActorNode(const std::string& actorName);
    
    // Function: createActorEdge
    // Purpose: Creates a directed edge from firstActor to secondActor, and
    // from secondActor to firstActor. Both edges have the specified movie name as
    // their label.
    // Input: Pointer to the first ActorNode, pointer to the second ActorNode,
    // and the movie name (by reference)
    // Returns: Nothing
    void createActorEdge(ActorNode* firstActor, ActorNode* secondActor, const std::string& movieName);
    
    // Function: clearVisited
    // Purpose: Clears the mIsVisited flag on every ActorNode in the graph,
    // and also clears out the PathPair lists.
    // Input: None
    // Returns: Nothing
    void clearVisited();
    
    // Function: Destructor
    // Purpose: Deletes all of the actor nodes in the graph
    // Input: None
    // Returns: Nothing
    ~IMDBGraph();
private:
    // Hash Map of actor names to ActorNode pointers
	std::unordered_map<std::string, ActorNode*> mActorNodeMap;
};
