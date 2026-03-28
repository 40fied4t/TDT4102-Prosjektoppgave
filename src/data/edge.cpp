#include "include/edge.h"

Edge::Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int weight):
    nodeVec{from, to},
    weight{weight}
{}

Edge::Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to):
    nodeVec{from, to},
    weight{1}
{}

std::vector<std::shared_ptr<Node>> Edge::getFrom() const {
    return nodeVec;
}

std::vector<std::shared_ptr<Node>> Edge::getTo() const {
    return nodeVec;
}

int Edge::getWeight() const {
    return weight;
}

DirectionalEdge::DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int weight):
    Edge{from, to, weight}
{}

DirectionalEdge::DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to):
    Edge{from, to}
{}

std::vector<std::shared_ptr<Node>> DirectionalEdge::getFrom() const {
    std::vector<std::shared_ptr<Node>> fromVec{nodeVec[0]};
    return fromVec;
}

std::vector<std::shared_ptr<Node>> DirectionalEdge::getTo() const {
    std::vector<std::shared_ptr<Node>> toVec{nodeVec[1]};
    return toVec;
}
