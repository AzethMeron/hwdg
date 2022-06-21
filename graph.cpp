
#include <string>
#include "tools.hpp"
#include "graph.hpp"

namespace HWDG
{

	size_t Graph::size_edges(void) const
	{
		return this->_edges.size();
	}

	size_t Graph::size_nodes(void) const
	{
		return this->_nodes.size();
	}

	Map::const_iterator<uint32_t, NodeInGraph> Graph::begin() const
	{
		return this->_nodes.cbegin();
	}

	Map::const_iterator<uint32_t, NodeInGraph> Graph::end() const
	{
		return this->_nodes.cend();
	}

	bool Graph::has(const Edge& edge) const
	{
		Map::const_iterator<uint64_t, Edge> iter = this->_edges.find(edge.id());
		if (iter == this->_edges.cend())
		{
			return false;
		}
		return true;
	}

	bool Graph::has(const Node& node) const
	{
		Map::const_iterator<uint32_t, NodeInGraph> iter = this->_nodes.find(node.id());
		if (iter == this->_nodes.cend())
		{
			return false;
		}
		return true;
	}

	Map::const_iterator<uint32_t, NodeInGraph> Graph::get_node(const Node& node) const
	{
		return this->_nodes.find(node.id());
	}

	Map::const_iterator<uint64_t, Edge> Graph::get_edge(const Edge& edge) const
	{
		return this->_edges.find(edge.id());
	}

	const NodeInGraph& Graph::fetch(const Node& node) const
	{
		auto iter = this->get_node(node);
		if (iter == this->end()) throw std::out_of_range(Tools::string_format("No such node: %s", node.str().c_str()));
		return *iter;
	}

	const Edge& Graph::fetch(const Edge& edge) const
	{
		auto iter = this->get_edge(edge);
		if (iter == this->_edges.cend()) throw std::out_of_range(Tools::string_format("No such edge: %s", edge.str().c_str()));
		return *iter;
	}

	bool Graph::add(const Node& node)
	{
		if (this->has(node)) return false;
		NodeInGraph n = node;
		this->_nodes.insert({ node.id(), n });
		return true;
	}

	bool Graph::add(const Edge& edge)
	{
		if (this->has(edge)) return false;
		if (edge.weight() < 0) { this->_negative_edges++; }
		if (edge.source() == edge.target()) { this->_loops++; }
		this->_edges.insert({ edge.id(), edge });
		this->_weight_sum = this->_weight_sum + edge.weight();
		const Node& src = edge.source();
		if (!this->has(src)) this->add(src);
		auto iter = this->_nodes.find(src.id());
		(*iter).add(edge);
		this->add(edge.target());
		return true;
	}

	void Graph::update(const Edge& edge)
	{
		this->remove(edge);
		this->add(edge);
	}


	bool Graph::remove(const Edge& edge)
	{
		if (!this->has(edge)) return false;
		if (this->fetch(edge).weight() < 0) { this->_negative_edges--; }
		if (edge.source() == edge.target()) { this->_loops--; }
		this->_weight_sum = this->_weight_sum - this->fetch(edge).weight();
		this->_edges.erase(edge.id());
		const Node& src = edge.source();
		auto iter = this->_nodes.find(src.id());
		(*iter).remove(edge);
		return true;
	}

	std::string Graph::str(void) const
	{
		std::string output = "[\n";
		for (const auto& node : *this)
		{
			std::string tmp = Tools::string_format("	%s\n", node.str().c_str());
			output.append(tmp);
		}
		output.append("]");
		return output;
	}

	std::string Graph::str_edges(void) const
	{
		std::string output = "[\n";
		for (const auto& edge : this->_edges)
		{
			output.append(Tools::string_format("	%s\n", edge.str().c_str()));
		}
		output.append("]");
		return output;
	}

	double Graph::density(void) const
	{
		double max_edges = (double)this->size_nodes() * (double)this->size_nodes();
		double edges = this->size_edges();
		return edges / max_edges;
	}

	Graph::Graph()
	{
		this->_weight_sum = 0;
		this->_negative_edges = 0;
		this->_loops = 0;
	}

	double Graph::weight_sum(void) const
	{
		return this->_weight_sum;
	}

	const Map::unordered_map<uint64_t, Edge>& Graph::edges(void) const
	{
		return this->_edges;
	}

	const Map::unordered_map<uint32_t, NodeInGraph>& Graph::nodes(void) const
	{
		return this->_nodes;
	}

	Graph::Graph(const std::initializer_list<Edge>& l) : Graph()
	{
		this->add(l);
	}

	Graph::Graph(const std::initializer_list<Node>& l) : Graph()
	{
		this->add(l);
	}

	Graph::Graph(const std::initializer_list<Node>& nodes, const std::initializer_list<Edge>& edges) : Graph()
	{
		this->add(nodes);
		this->add(edges);
	}

	void Graph::add(const std::initializer_list<Edge>& l)
	{
		for (const Edge& edge : l)
		{
			this->add(edge);
		}
	}

	void Graph::add(const std::initializer_list<Node>& l)
	{
		for (const Node& node : l)
		{
			this->add(node);
		}
	}

	bool Graph::has_negative_weights(void) const
	{
		return this->_negative_edges > 0;
	}

	bool Graph::has_loops(void) const
	{
		return this->_loops > 0;
	}

	Graph Graph::Transpose(void) const
	{
		Graph output;
		output.reserve_nodes(this->size_nodes());
		output.reserve_edges(this->size_edges());
		for (const NodeInGraph& node : *this)
		{
			output.add(node);
			output.reserve_edges_in_node(node, node.size_edges());
		}
		for (const Edge& edge : this->edges())
		{
			output.add(edge.Reverse());
		}
		return output;
	}

	Graph Graph::ScaleWeight(float factor) const
	{
		Graph output;
		output.reserve_nodes(this->size_nodes());
		output.reserve_edges(this->size_edges());
		for (const NodeInGraph& node : *this)
		{
			output.add(node);
			output.reserve_edges_in_node(node, node.size_edges());
		}
		for (const Edge& edge : this->edges())
		{
			output.add(edge.Scale(factor));
		}
		return output;
	}

	void Graph::reserve_nodes(const size_t& count)
	{
		if (count > this->size_nodes())
		{
			this->_nodes.reserve(count);
		}
	}

	void Graph::reserve_edges(const size_t& count)
	{
		if (count > this->size_edges())
		{
			this->_edges.reserve(count);
		}
	}

	void Graph::reserve_edges_in_node(const Node& node, const size_t& count)
	{
		auto iter = this->_nodes.find(node.id());
		if (iter == this->_nodes.end()) return;
		(*iter).reserve_edges(count);
	}

	void Graph::SaveBin(std::ostream& file, const Graph& graph)
	{
		size_t size = graph.size_nodes();
		file.write((const char*)&size, sizeof(size));
		size_t size_edges = graph.size_edges();
		file.write((const char*)&size_edges, sizeof(size_edges));
		for (const NodeInGraph& node : graph)
		{
			Node::SaveBin(file, node);
			size_t edges = node.size_edges();
			file.write((const char*)&edges, sizeof(edges));
			for (const Edge& edge : node)
			{
				Node::SaveBin(file, edge.target());
				float weight = edge.weight();
				file.write((const char*)&weight, sizeof(weight));
			}
		}
	}
	
	Graph Graph::LoadBin(std::istream& file)
	{
		Graph graph;
		size_t size_nodes = 0;
		file.read((char*)&size_nodes, sizeof(size_nodes));
		graph.reserve_nodes(size_nodes);
		size_t size_edges = 0;
		file.read((char*)&size_edges, sizeof(size_edges));
		graph.reserve_edges(size_edges);
		for (size_t i = 0; i < size_nodes; ++i)
		{
			Node src = Node::LoadBin(file);
			graph.add(src);
			size_t size_edges = 0;
			file.read((char*)&size_edges, sizeof(size_edges));
			graph.reserve_edges_in_node(src, size_edges);
			for (size_t j = 0; j < size_edges; ++j)
			{
				Node tgt = Node::LoadBin(file);
				float weight = 0;
				file.read((char*)&weight, sizeof(weight));
				graph.add(Edge(src, tgt, weight));
			}
		}
		return graph;
	}

	void Graph::SaveTxt(std::ostream& file, const Graph& graph)
	{
		file << graph.size_nodes() << std::endl;
		file << graph.size_edges() << std::endl;
		for (const NodeInGraph& node : graph)
		{
			Node::SaveTxt(file, node);
			file << node.size_edges();
			for (const Edge& edge : node)
			{
				file << ' ';
				Node::SaveTxt(file, edge.target());
				file << edge.weight();
			}
			file << std::endl;
		}
	}

	Graph Graph::LoadTxt(std::istream& file)
	{
		Graph graph;
		size_t size_nodes;
		file >> size_nodes;
		graph.reserve_nodes(size_nodes);
		size_t size_edges;
		file >> size_edges;
		graph.reserve_edges(size_edges);
		for (size_t i = 0; i < size_nodes; ++i)
		{
			Node src = Node::LoadTxt(file);
			graph.add(src);
			size_t size_edges;
			file >> size_edges;
			graph.reserve_edges_in_node(src, size_edges);
			for (size_t j = 0; j < size_edges; ++j)
			{
				Node tgt = Node::LoadTxt(file);
				float weight = 0;
				file >> weight;
				Edge edge = Edge(src, tgt, weight);
				graph.add(edge);
			}
		}
		return graph;
	}
}
