
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_ACCELERATORS_OCCHUNKDFSACCEL_H
#define PBRT_ACCELERATORS_OCCHUNKDFSACCEL_H

// accelerators/octree.h*
#include "pbrt.h"
#include "primitive.h"
#include "octree-basic.h"
#include <array>

namespace pbrt {

// OcteeAccel Declarations

class OcChunkDFSAccel : public Aggregate {
  public:
    // KdTreeAccel Public Methods
    OcChunkDFSAccel(std::vector<std::shared_ptr<Primitive>> p);
    Bounds3f WorldBound() const { return wb; }
    ~OcChunkDFSAccel();
    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool IntersectP(const Ray &ray) const;

  private:

    // OctreeAccel Private Data
    std::vector<std::shared_ptr<Primitive>> primitives;
    Bounds3f wb; // World Bounds
    OctreeBasicAccel oba;

    struct Node { int bitcnt_nodes; int bitcnt_types; Bounds3f bounds; Float tMin; };
    
    struct ChunkCreator { uint32_t root_node_offset; Bounds3f b_root;
        std::vector<std::shared_ptr<Primitive>> p; int chunk_idx; };

    struct alignas(64) Chunk {
      uint32_t child_chunk_offset;
      uint32_t sizes_offset;
      std::array<bftype, DFS_CHUNK_DEPTH> nodes; // 1 inner node, 0 leaf node
      std::array<bftype, DFS_CHUNK_DEPTH> types; // 1 chunk ptr, 0 real inner
    };
    std::vector<std::shared_ptr<Primitive>> leaves;
    std::vector<int> sizes;

    // TODO make sure array is aligned to 64-bit addresses
    std::vector<Chunk> octree;
    
    std::vector<ChunkCreator> CreateChunk(ChunkCreator cc); 
    void RecurseIntersect(const Ray &ray, SurfaceInteraction *isect, uint32_t chunk_offset, Bounds3f parent_bounds, Float tMin, bool &hit) const;
    void lh_dump(const char *path);
    void lh_dump_rec(FILE *f, uint32_t *vcnt_, uint32_t chunk_offset, Bounds3f bounds);
    void lh_dump_dfs(const char *path);
    void lh_dump_rec_dfs(FILE *f, uint32_t *vcnt_, uint32_t chunk_offset, Bounds3f bounds);

};

std::shared_ptr<OcChunkDFSAccel> CreateOcChunkDFSAccelerator(
    std::vector<std::shared_ptr<Primitive>> prims, const ParamSet &ps);

} // namespace pbrt

#endif  // PBRT_ACCELERATORS_KDTREEACCEL_H
