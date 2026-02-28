#pragma once
// Minimal stub for ArtifactItemData to satisfy Ogre::FastArray<ArtifactItemData>
// instantiation in Town.h. The full definition is in the game binary.
class ArtifactItemData
{
    ArtifactItemData() {}
    ArtifactItemData(const ArtifactItemData&) {}
    ArtifactItemData& operator=(const ArtifactItemData&) { return *this; }
};
