#pragma once

// Parallelization is easily achieved if each thread instantiates an Optimizer.

#include "SearchState.h"
#include "distance_functions.h"
#include "primitives.h"
#include "quadtree/QuadtreeNode.h"
#include "quadtree/depth_map/DepthMap.h"
#include "quadtree/depth_map/transform.h"
#include "quadtree/morton_keys.h"

#include <algorithm>
#include <ostream>
#include <vector>

class Optimizer
{
public:
    Optimizer(const quadtree::depth_map::DepthMap& depth_map
        , const std::vector<primitives::space_t>& x
        , const std::vector<primitives::space_t>& y)
        : m_depth_map(depth_map), m_x(x), m_y(y) {}

    void find_best();
    void iterate();
    const SearchState& best() const { return m_best; }

private:
    const quadtree::depth_map::DepthMap& m_depth_map;
    const std::vector<primitives::space_t>& m_x;
    const std::vector<primitives::space_t>& m_y;

    size_t m_k{2}; // as in k-opt.
    SearchState m_best;
    SearchState m_current;

    void find_best(int depth, quadtree::depth_map::transform::hash_t node_hash, const quadtree::QuadtreeNode* node);
    void find_best(const quadtree::QuadtreeNode* node);
    void find_best(const quadtree::QuadtreeNode* node, quadtree::QuadtreeNode::SegmentContainer::const_iterator it);

    std::vector<quadtree::QuadtreeNode*> gather_searchable_nodes(int depth, quadtree::depth_map::transform::hash_t center_node_hash) const;
    void check_best();
};

inline std::ostream& operator<<(std::ostream& out, const Optimizer& optimizer)
{
    out << "Best swap found:\n";
    out << "\tPoint ID of each segment:\n";
    for (const auto& s : optimizer.best().segments)
    {
        out << "\t" << s.a << "\t" << s.b << "\n";
    }
    out << "\tOld length: " << optimizer.best().length << "\n";
    out << "\tImprovement: " << optimizer.best().improvement << "\n";
    return out;
}

