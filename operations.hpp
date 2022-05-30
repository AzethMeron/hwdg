#ifndef HWDG_OPERATIONS_HPP
#define HWDG_OPERATIONS_HPP

/**
* @file operations.hpp
* @author Jakub Grzana
* @date March 2022
* @brief Operations that can be performed on Graph. 
*/

#include <functional>
#include <unordered_set>
#include <vector>
#include "graph.hpp"

namespace HWDG
{
	/**
	* Graph union with edge-weight addition.
	* Algorithm for weight of edge: \f$ w_{e_{out}} = \left\{ \begin{array}{ll}
		w_{e_A} & \textrm{if $e \in A$ and $e \notin B$}\\
		w_{e_B} & \textrm{if $e \notin A$ and $e \in B$}\\
		w_{e_A} + w_{e_B} & \textrm{if $ e \in A $ and $ e \in B$}
		\end{array} \right. \f$
	*
	* where \f$ w_{e_x} \f$ stands for 'weight' of edge 'e' in graph 'x'.
	* \param a Graph A, one of components for union (sum)
	* \param b Graph B, one of components for union (sum)
	* \return Graph consisting of all nodes and edges existing in either a or b, with weight computed by algorithm.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	Graph Union(const Graph& a, const Graph& b); // linear

	/**
	* Graph union with edge-weight balancer.
	* Algorithm for weight of edge: \f$ w_{e_{out}} = \left\{ \begin{array}{ll}
		w_{e_A} & \textrm{if $e \in A$ and $e \notin B$}\\
		w_{e_B} & \textrm{if $e \notin A$ and $e \in B$}\\
		L*w_{e_A} + (1-L) * w_{e_B} & \textrm{if $ e \in A $ and $ e \in B$}
		\end{array} \right. \f$
	*
	* where \f$ w_{e_x} \f$ stands for 'weight' of edge 'e' in graph 'x'.
	* \param a Graph A, one of components for union (sum)
	* \param b Graph B, one of components for union (sum)
	* \param balancer Balancing factor used in weight computation. MUST belong to [0,1]
	* \return Graph consisting of all nodes and edges existing in either a or b, with weight computed by algorithm.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	Graph Union(const Graph& a, const Graph& b, float balancer); // linear

	/**
	* Graph union in place, with edge-weight addition.
	* Algorithm for weight of edge: \f$ w_{e_{out}} = \left\{ \begin{array}{ll}
		w_{e_A} & \textrm{if $e \in A$ and $e \notin B$}\\
		w_{e_B} & \textrm{if $e \notin A$ and $e \in B$}\\
		w_{e_A} + w_{e_B} & \textrm{if $ e \in A $ and $ e \in B$}
		\end{array} \right. \f$
	*
	* where \f$ w_{e_x} \f$ stands for 'weight' of edge 'e' in graph 'x'.
	* \param a Graph A, to which Graph B will be added.
	* \param b Graph B, that will be added to A.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	void MergeUnion(Graph& a, const Graph& b); // linear

	/**
	* Graph union in place, with edge-weight balancer.
	* Algorithm for weight of edge: \f$ w_{e_{out}} = \left\{ \begin{array}{ll}
		w_{e_A} & \textrm{if $e \in A$ and $e \notin B$}\\
		w_{e_B} & \textrm{if $e \notin A$ and $e \in B$}\\
		L*w_{e_A} + (1-L) * w_{e_B} & \textrm{if $ e \in A $ and $ e \in B$}
		\end{array} \right. \f$
	*
	* where \f$ w_{e_x} \f$ stands for 'weight' of edge 'e' in graph 'x'.
	* \param a Graph A, to which Graph B will be added.
	* \param b Graph B, that will be added to A.
	* \param balancer Balancing factor used in weight computation. MUST belong to [0,1]
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	void MergeUnion(Graph& a, const Graph& b, float balancer); // linear

	/**
	* Graph intersection, with edge-weight balancer.
	* Algorithm for weight of edge: \f$ w_{e_{out}} = \left\{ \begin{array}{ll}
		L*w_{e_A} + (1-L) * w_{e_B} & \textrm{if $ e \in A $ and $ e \in B$}
		\end{array} \right. \f$
	*
	* where \f$ w_{e_x} \f$ stands for 'weight' of edge 'e' in graph 'x'.
	* \param a Graph A, one of component for intersection
	* \param b Graph B, one of component for intersection
	* \param balancer Balancing factor used in weight computation. MUST belong to [0,1]
	* \return Graph that consists only of nodes and edges that belong to both A and B
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	Graph Intersection(const Graph& a, const Graph& b, float balancer); // linear

	/**
	* Inverse of intersection. Returns Graph consisting of nodes and edges that are part of either A or B, but not both at the same time.
	*
	* Effectively, it is equal to Difference(Union(A,B), Intersection(A,B,dummy)).
	* \param a Graph A, one of component for inverse intersection
	* \param b Graph B, one of component for inverse intersection
	* \return Graph that consists only of nodes and edges that belong to either A and B, but not both at the same time.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	Graph InverseIntersection(const Graph& a, const Graph& b);

	/**
	* Graph difference.
	* \param a Graph A, left-side graph (minuend) that B will be "substracted" from.
	* \param b Graph B, right-side graph (subtrahend)
	* \return Graph consisting of those edges and nodes that belong to A, but NOT to B.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	Graph Difference(const Graph& a, const Graph& b); // linear

	/**
	* Containment Similarity (CS) comparison of graphs.
	* Algorithm: \f$ CS(A,B) = \frac{\sum\nolimits_{e \in A \cap B} 1}{min(|A|,|B|)} \f$
	*
	* where \f$ |X| \f$ stands for number of edges in graph X
	* \param a Graph A
	* \param b Graph B
	* \return Result of CS algorithm, belongs to [0,1]
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	double ContainmentSimilarity(const Graph& a, const Graph& b); // linear

	/**
	* Size Similarity (SS) comparison of graphs.
	* Algorithm: \f$ SS(A,B) = \frac{min(|A|,|B|)}{max(|A|,|B|)} \f$
	*
	* where \f$ |X| \f$ stands for number of edges in graph X
	* \param a Graph A
	* \param b Graph B
	* \return Result of SS algorithm, belongs to [0,1]
	* \par Time complexity:
	* \f$O(1)\f$
	*/
	double SizeSimilarity(const Graph& a, const Graph& b); // constant

	/**
	* Value Similarity (VS) comparison of graphs.
	* Algorithm: \f$ VS(A,B) = \frac{\sum\nolimits_{e \in A \cap B}
	\frac
		{ min(w_{e_A}, w_{e_B}) }
		{ max(w_{e_A}, w_{e_B}) }}
	{max(|A|,|B|)} \f$
	*
	* where \f$ |X| \f$ stands for number of edges in graph X, and \f$ w_{e_X} \f$ is 'weight' of edge 'e' in graph 'x'
	* \param a Graph A
	* \param b Graph B
	* \return Result of VS algorithm, belongs to [0,1]
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	double ValueSimilarity(const Graph& a, const Graph& b); // linear

	/**
	* Normalized Value Similarity (NVS) comparison of graphs.
	* Ignores the relative sizes of the compared graphs and focuses on the weights of common edges.
	*
	* Algorithm: \f$ NVS(A,B) = \frac{VS(A,B)}{SS(A,B)} \f$
	*
	* Check ValueSimilarity() and SizeSimilarity()
	* \param a Graph A
	* \param b Graph B
	* \return Result of NVS algorithm
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	double NormalizedValueSimilarity(const Graph& a, const Graph& b); // linear

	/**
	* Breadth First Search (BFS) of graph.
	* BFS traverses graph's nodes in such a way that each Node is visited only once and with minimal number of traversals (weight isn't considered here, only number of traversals)
	*
	* It starts at starting node and adds all of its neightbours before into to-be-visited queue. Then the same is done to each element in the queue.
	* However if neighbour was visited already or is currently in queue, then it's ignored.
	* Note that all of neighbours of Node are added into queue, but they're not processed immediately. It's FIFO queue.
	*
	* \param graph Graph to be traversed,
	* \param starting_node Starting Node from which traversal will happen.
	* \param func Function (typically lambda expression) that will be called for each Edge that is traversed. Unordered set contains all already visited nodes.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	void BreadthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func);

	/**
	* Depth First Search (DFS) of graph.
	* DFS traverses graph's nodes in such a way that each Node is visited only once.
	*
	* It starts at starting node and follows one neighbour as deep in graph as possible before moving on to next neighbour. Next neighbours to follow are chosen randomly.
	*
	* \param graph Graph to be traversed,
	* \param starting_node Starting Node from which traversal will happen.
	* \param func Function (typically lambda expression) that will be called for each Edge that is traversed. Unordered set contains all already visited nodes.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	void DepthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func);
	
	/**
	* Depth First Search (DFS) of graph.
	* DFS traverses graph's nodes in such a way that each Node is visited only once.
	*
	* It starts at starting node and follows one neighbour as deep in graph as possible before moving on to next neighbour. Neighbour sequence must be provided by function passed as parameter.
	*
	* \param graph Graph to be traversed,
	* \param starting_node Starting Node from which traversal will happen.
	* \param func Function (typically lambda expression) that will be called for each Edge that is traversed. Unordered set contains all already visited nodes.
	* \param priority Function (typically lambda expression) that decides on sequence in which nodes are visited. This function MUST add all neighbours into parameter 'res', then res.front() will be visited first and res.back() will be visited last.
	* \par Time complexity:
	* \f$O(edges)\f$
	*/
	void DepthFirstSearch(const Graph& graph, const Node& starting_node, std::function<void(const Edge& edge, const std::unordered_set<Node, Node::HashFunction>& visited)> func, std::function<void(const NodeInGraph& node, std::vector<Edge>& to_be_traversed_first)> priority);
}
#endif