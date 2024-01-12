// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "GeometryData.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(GeometryData);

void GeometryData::serialize(arras4::api::DataOutStream& out) const
{
    out << mFrame;

    int nObjects = (int)mObjectData.size();
    out << nObjects;

    for (int i=0; i<nObjects; ++i) {

        // the object name
        out << mObjectData[i].mAssetName;
        out << mObjectData[i].mSubAssetName;
        out << mObjectData[i].mNodeName;

        // and then the meshes in the object
        const moonray::ObjectMeshes& objMeshes = mObjectData[i].mObjectMeshes;

        int nMeshes = (int)objMeshes.size();
        out << nMeshes;

        for (int m=0; m<nMeshes; ++m) {
            const moonray::ObjectMesh& objMesh = objMeshes[m];
            out << objMesh.mMeshName;

            int nVerts = (int)objMesh.mMeshPositions.size();
            out << nVerts;
            out.write(&objMesh.mMeshPositions[0], nVerts * sizeof(objMesh.mMeshPositions[0]));
        }
    }
}

void GeometryData::deserialize(arras4::api::DataInStream& in, unsigned)
{
    in >> mFrame;

    int nObjects;
    in >> nObjects;

    mObjectData.resize(nObjects);

    for (int i=0; i<nObjects; ++i) {
        // object name
        in >> mObjectData[i].mAssetName;
        in >> mObjectData[i].mSubAssetName;
        in >> mObjectData[i].mNodeName;

        // then the object meshes
        int nMeshes;
        in >> nMeshes;

        moonray::ObjectMeshes& objMeshes = mObjectData[i].mObjectMeshes;
        objMeshes.resize(nMeshes);

        for (int m=0; m<nMeshes; ++m) {
            moonray::ObjectMesh& objMesh = objMeshes[m];
            in >> objMesh.mMeshName;

            int nVerts;
            in >> nVerts;

            objMesh.mMeshPositions.resize(nVerts);
            in.read(&objMesh.mMeshPositions[0], nVerts * sizeof(objMesh.mMeshPositions[0]));
        }
    }
}

}
