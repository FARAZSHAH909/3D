#pragma once
#include "GEMLoader.h"
#include "StaticMesh.h"
#include "core.h"
#include "maths.h"
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>

// Simple GameObject structure if we want to separate Data from Renderer, 
// but for now we might just enhance StaticMesh or use it as is.
// Actually, StaticMesh in this project seems to hold the VertexBuffer too. 
// So cloning it for every object is wasteful if they share the same model.
// But to keep it simple and safe given the constraints:
// We will load a StaticMesh for each unique model file, and re-use it?
// Or just load everything as separate objects for now (easiest to implement, though memory inefficient).
// Let's stick to "One StaticMesh per object in level" for simplicity unless it crashes.
// BETTER APPROACH: A ResourceManager that holds loaded Meshes, and GameObjects that reference them.
// But let's start with a simple loader.

class LevelLoader
{
public:
    void load(std::string filename, Core* core, std::vector<StaticMesh>& outMeshes)
    {
        // read file
        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonString = buffer.str();

        // parse JSON
        GEMLoader::GEMJsonParser parser;
        GEMLoader::GEMJson json = parser.parse(jsonString);

        // simple cache for prototypes keyed by filename
        std::map<std::string, StaticMesh> prototypeCache;

        if (json.vDict.find("objects") != json.vDict.end()) {
            std::vector<GEMLoader::GEMJson> objects = json.vDict["objects"].vArr;
            for (auto& obj : objects) {
                std::string modelFile = obj.vDict["file"].vStr;

                // get or load prototype
                if (prototypeCache.find(modelFile) == prototypeCache.end()) {
                    StaticMesh proto;
                    proto.loadMeshes(core, modelFile);
                    prototypeCache[modelFile] = proto;
                }

                // clone prototype metadata into instance (shallow copy is fine for meshes vector pointers)
                StaticMesh instance = prototypeCache[modelFile];

                // Transform: position, rotation (degrees), scale
                Vec3 pos(0,0,0);
                if (obj.vDict.find("pos") != obj.vDict.end()) {
                    auto p = obj.vDict["pos"].vArr;
                    if(p.size() >= 3) pos = Vec3(p[0].vFloat, p[1].vFloat, p[2].vFloat);
                }

                Vec3 rot(0,0,0); // Euler degrees
                if (obj.vDict.find("rot") != obj.vDict.end()) {
                    auto r = obj.vDict["rot"].vArr;
                    if (r.size() >= 3) rot = Vec3(r[0].vFloat, r[1].vFloat, r[2].vFloat);
                } else if (obj.vDict.find("rotation") != obj.vDict.end()) {
                    auto r = obj.vDict["rotation"].vArr;
                    if (r.size() >= 3) rot = Vec3(r[0].vFloat, r[1].vFloat, r[2].vFloat);
                }

                Vec3 scale(1,1,1);
                if (obj.vDict.find("scale") != obj.vDict.end()) {
                    auto s = obj.vDict["scale"].vArr;
                    if(s.size() >= 3) scale = Vec3(s[0].vFloat, s[1].vFloat, s[2].vFloat);
                }

                if (obj.vDict.find("type") != obj.vDict.end()) {
                    instance.type = obj.vDict["type"].vStr;
                }

                // Build scale, rotation (Z Y X), translation matrices and compose as World = T * R * S
                Matrix sMat; sMat.setIdentity(); sMat.scaling(scale);

                // rotation degrees -> radians
                float rx = rot.x * PI_F / 180.0f;
                float ry = rot.y * PI_F / 180.0f;
                float rz = rot.z * PI_F / 180.0f;

                Matrix rotX; rotX.setIdentity(); rotX.rotAroundX(rx);
                Matrix rotY; rotY.setIdentity(); rotY.rotAroundY(ry);
                Matrix rotZ; rotZ.setIdentity(); rotZ.rotAroundZ(rz);

                // R = Rz * Ry * Rx
                Matrix rMat = rotZ.multiply(rotY.multiply(rotX));

                Matrix tMat; tMat.setIdentity(); tMat.translation(pos);

                instance.worldMatrix = tMat.multiply(rMat.multiply(sMat));

                outMeshes.push_back(instance);
            }
        }
    }
};
