// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#ifndef LIB_COMMON_SHARED_GEOMETRYOBJECTS_H_
#define LIB_COMMON_SHARED_GEOMETRYOBJECTS_H_

// NOTE: this is a copy of moonray/lib/common/geometry/GeometryObjects.h
// we can't include the original because this library is used by
// arras clients, and therefore needs to build with the full range
// of pseudo-compiler : moonray is only built for icc150_64.
//
// This should therefore probably be in a separate library...but is
// currently the only file that has this particular issue.

#include <vector>
#include <string>

namespace moonray {

struct ObjectMesh {
    std::string mMeshName;
    std::vector<float> mMeshPositions;
};
typedef std::vector<ObjectMesh> ObjectMeshes;

struct ObjectData {
    std::string mAssetName;
    std::string mSubAssetName;
    std::string mNodeName;
    ObjectMeshes mObjectMeshes;
};

} // end namespace moonray

#endif 
