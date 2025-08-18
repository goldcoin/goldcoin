// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_UTIL_MERKLE_H
#define GOLDCOIN_UTIL_MERKLE_H

#include <mdspan>
#include <span>
#include <vector>
#include <expected>
#include <ranges>
#include <algorithm>
#include "uint256.h"
#include "hash.h"
#include "log.h"

namespace goldcoin {

// C++23 std::mdspan for efficient merkle tree operations
class MerkleTree {
public:
    using Hash = uint256;
    using HashSpan = std::span<const Hash>;
    using MutableHashSpan = std::span<Hash>;
    
    // Error types for merkle operations
    enum class MerkleError {
        EMPTY_TREE,
        INVALID_SIZE,
        HASH_MISMATCH,
        PROOF_INVALID
    };

private:
    std::vector<Hash> m_tree;
    size_t m_leaf_count;
    size_t m_tree_height;

    // Calculate tree height from leaf count
    static constexpr size_t CalculateHeight(size_t leaf_count) {
        if (leaf_count == 0) return 0;
        size_t height = 0;
        size_t size = 1;
        while (size < leaf_count) {
            size <<= 1;
            height++;
        }
        return height + 1;
    }

    // Calculate total nodes needed for tree
    static constexpr size_t CalculateTreeSize(size_t leaf_count) {
        if (leaf_count == 0) return 0;
        size_t size = 1;
        while (size < leaf_count) size <<= 1;
        return 2 * size - 1;
    }

    // Hash two nodes together
    static Hash HashNodes(const Hash& left, const Hash& right) {
        CHash256 hasher;
        hasher.Write(left.begin(), 32);
        hasher.Write(right.begin(), 32);
        Hash result;
        hasher.Finalize(result.begin());
        return result;
    }

public:
    // Build merkle tree from transactions using C++23 features
    static auto Build(HashSpan transactions) 
        -> std::expected<MerkleTree, MerkleError> {
        
        if (transactions.empty()) {
            return std::unexpected(MerkleError::EMPTY_TREE);
        }

        MerkleTree tree;
        tree.m_leaf_count = transactions.size();
        tree.m_tree_height = CalculateHeight(tree.m_leaf_count);
        tree.m_tree.resize(CalculateTreeSize(tree.m_leaf_count));

        // Use mdspan for 2D view of tree levels
        size_t level_size = 1;
        while (level_size < tree.m_leaf_count) level_size <<= 1;
        
        // Copy leaves to tree (bottom level)
        size_t leaf_offset = tree.m_tree.size() - level_size;
        std::ranges::copy(transactions, tree.m_tree.begin() + leaf_offset);
        
        // Fill remaining leaves with last transaction (Bitcoin convention)
        if (tree.m_leaf_count < level_size) {
            std::fill(tree.m_tree.begin() + leaf_offset + tree.m_leaf_count,
                     tree.m_tree.begin() + leaf_offset + level_size,
                     transactions.back());
        }

        // Build tree bottom-up using mdspan for level access
        for (size_t level = tree.m_tree_height - 1; level > 0; level--) {
            size_t current_level_size = 1 << (level - 1);
            size_t current_offset = (1 << level) - 1;
            size_t child_offset = (1 << (level + 1)) - 1;
            
            // Create mdspan views for current and child levels
            std::mdspan<Hash, std::dextents<size_t, 1>> current_level(
                tree.m_tree.data() + current_offset, current_level_size);
            std::mdspan<const Hash, std::dextents<size_t, 1>> child_level(
                tree.m_tree.data() + child_offset, current_level_size * 2);
            
            // Hash pairs of children
            for (size_t i = 0; i < current_level_size; i++) {
                current_level[i] = HashNodes(child_level[i * 2], child_level[i * 2 + 1]);
            }
        }

        LOG_DEBUG("Built merkle tree with {} leaves, height {}, root: {}", 
                  tree.m_leaf_count, tree.m_tree_height, tree.GetRoot().ToString());
        
        return tree;
    }

    // Get merkle root
    [[nodiscard]] const Hash& GetRoot() const {
        return m_tree[0];
    }

    // Get merkle proof for a transaction
    auto GetProof(size_t index) const 
        -> std::expected<std::vector<Hash>, MerkleError> {
        
        if (index >= m_leaf_count) {
            return std::unexpected(MerkleError::INVALID_SIZE);
        }

        std::vector<Hash> proof;
        proof.reserve(m_tree_height - 1);

        size_t level_size = 1 << (m_tree_height - 1);
        size_t tree_index = m_tree.size() - level_size + index;

        for (size_t level = m_tree_height - 1; level > 0; level--) {
            size_t sibling_index = tree_index ^ 1; // XOR to get sibling
            proof.push_back(m_tree[sibling_index]);
            tree_index = (tree_index - 1) / 2; // Move to parent
        }

        return proof;
    }

    // Verify merkle proof using C++23 ranges
    static auto VerifyProof(const Hash& leaf, const std::vector<Hash>& proof, 
                           const Hash& root, size_t index) 
        -> std::expected<bool, MerkleError> {
        
        Hash current = leaf;
        size_t idx = index;

        for (const auto& sibling : proof) {
            if (idx & 1) {
                current = HashNodes(sibling, current);
            } else {
                current = HashNodes(current, sibling);
            }
            idx >>= 1;
        }

        if (current != root) {
            LOG_WARNING("Merkle proof verification failed for index {}", index);
            return std::unexpected(MerkleError::PROOF_INVALID);
        }

        return true;
    }

    // Get tree statistics
    void PrintStats() const {
        std::println("Merkle Tree Statistics:");
        std::println("  Leaf count: {}", m_leaf_count);
        std::println("  Tree height: {}", m_tree_height);
        std::println("  Total nodes: {}", m_tree.size());
        std::println("  Root hash: {}", GetRoot().ToString());
        std::println("  Memory usage: {} bytes", m_tree.size() * sizeof(Hash));
    }

    // C++23 ranges view of tree levels
    auto GetLevel(size_t level) const {
        if (level >= m_tree_height) {
            return std::span<const Hash>{};
        }
        
        size_t level_size = 1 << level;
        size_t offset = level_size - 1;
        
        return std::span<const Hash>(m_tree.data() + offset, level_size);
    }

    // Modern iterator support
    auto begin() const { return m_tree.begin(); }
    auto end() const { return m_tree.end(); }
    size_t size() const { return m_tree.size(); }
};

// Benchmark merkle tree operations
inline void BenchmarkMerkleTree(size_t num_transactions) {
    BENCHMARK(std::format("MerkleTree with {} transactions", num_transactions));
    
    // Generate test hashes
    std::vector<uint256> transactions;
    transactions.reserve(num_transactions);
    
    for (size_t i = 0; i < num_transactions; i++) {
        transactions.push_back(Hash256(std::to_string(i)));
    }
    
    // Build tree
    auto result = MerkleTree::Build(transactions);
    if (!result) {
        LOG_ERROR("Failed to build merkle tree");
        return;
    }
    
    auto& tree = result.value();
    tree.PrintStats();
    
    // Test proof generation and verification
    {
        BENCHMARK("Proof generation and verification");
        auto proof = tree.GetProof(num_transactions / 2);
        if (proof) {
            auto verified = MerkleTree::VerifyProof(
                transactions[num_transactions / 2], 
                proof.value(), 
                tree.GetRoot(), 
                num_transactions / 2
            );
            LOG_INFO("Proof verification: {}", verified.value_or(false));
        }
    }
}

} // namespace goldcoin

#endif // GOLDCOIN_UTIL_MERKLE_H