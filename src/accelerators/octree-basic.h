
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

#ifndef PBRT_ACCELERATORS_OCTREEBASICACCEL_H
#define PBRT_ACCELERATORS_OCTREEBASICACCEL_H

// accelerators/octree.h*
#include "pbrt.h"
#include "primitive.h"
#include <array>

namespace pbrt {

const int MAX_DEPTH = 15; //15
const int MAX_PRIMS = 32; //30

const Float PRM_THRESH = 0.9;
const Float VOL_THRESH = 1;

// Change this for different type
typedef uint64_t BITFIELD_TYPE;
const int BFS_CHUNK_DEPTH = 7; // Size of chunk array of type BITFIELD_X below

// DON'T change these!
const int DFS_CHUNK_DEPTH = BFS_CHUNK_DEPTH / 2;
const int BITFIELD_SIZE = 8 * sizeof(BITFIELD_TYPE);
const int NUM_SETS_PER_BITFIELD = sizeof(BITFIELD_TYPE);
const int BFS_NUM_SETS_PER_CHUNK = NUM_SETS_PER_BITFIELD * BFS_CHUNK_DEPTH;
const int DFS_NUM_SETS_PER_CHUNK = NUM_SETS_PER_BITFIELD * DFS_CHUNK_DEPTH;
const BITFIELD_TYPE ZERO = (BITFIELD_TYPE)0;
const BITFIELD_TYPE ONE = (BITFIELD_TYPE)1;

struct ChildHit { int idx; float tMin; };
struct ChildTraversal { std::array<ChildHit, 4> nodes; int size; };

Vector3f BoundsHalf(Bounds3f b);
Bounds3f DivideBounds(Bounds3f b, int idx, Vector3f b_half);
ChildTraversal FindTraversalOrder(const Ray &ray, Bounds3f b, Float tMin);
int Rank(BITFIELD_TYPE bits, int n = BITFIELD_SIZE);
bool BoundsContainPrim(Bounds3f b, std::shared_ptr<Primitive> p);
bool MakeLeafNode(Bounds3f b, std::vector<std::shared_ptr<Primitive>>);

// OcteeAccel Declarations

class OctreeBasicAccel : public Aggregate {
  public:
    // KdTreeAccel Public Methods
    OctreeBasicAccel();
    OctreeBasicAccel(std::vector<std::shared_ptr<Primitive>> p);
    Bounds3f WorldBound() const { return wb; }
    std::vector<uint32_t> &Nodes() { return nodes; }
    std::vector<uint32_t> &Sizes() { return sizes; }
    std::vector<std::shared_ptr<Primitive>> &Leaves() { return leaves; }
    ~OctreeBasicAccel();
    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool IntersectP(const Ray &ray) const;

  private:

    // OctreeAccel Private Data
    std::vector<std::shared_ptr<Primitive>> primitives;
    Bounds3f wb; // World Bounds
    
    void Recurse(int offset, std::vector<std::shared_ptr<Primitive>> primitives, Bounds3f bounds, int depth);
    void RecurseIntersect(const Ray &ray, SurfaceInteraction *isect, uint32_t offset, Bounds3f bounds, Float tMin, bool &hit) const;
    void lh_dump(const char *path);
    void lh_dump_rec(FILE *f, uint32_t *vcnt_, int offset, Bounds3f bounds);

    std::vector<uint32_t> nodes, sizes; 
    std::vector<std::shared_ptr<Primitive>> leaves;
};

std::shared_ptr<OctreeBasicAccel> CreateOctreeBasicAccelerator(
    std::vector<std::shared_ptr<Primitive>> prims, const ParamSet &ps);

} // namespace pbrt

#endif  // PBRT_ACCELERATORS_KDTREEACCEL_H
