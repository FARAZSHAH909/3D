#pragma once
#include "core.h"
#include "Static_Vertex.h"
#include"GamesEngineeringBase.h"
#include"Mesh.h"
#include "GEMLoader.h"
#include"Shader.h"
#include"PipeLineState.h"
    

class StaticMesh
{

public:
    PSOManager psos;
    std::vector<Mesh*> meshes;
    Shader shader;

	// World Matrix for this mesh instance
	Matrix worldMatrix; 
    std::string type = "static"; // "static", "collectible", "player", etc.
    
    struct AABB {
        Vec3 min;
        Vec3 max;
        AABB() { min = Vec3(1e9f, 1e9f, 1e9f); max = Vec3(-1e9f, -1e9f, -1e9f); }
    } localAABB;

    StaticMesh() {
        // Ensure worldMatrix is identity by default using constructor
    }
    
    // Check if a world point is inside this OBB (Approximated)
    // Actually, let's just do simple Radius check or AABB check in World Space?
    // Transforming AABB is better.
    bool checkCollision(const Vec3& worldPoint) {
       // Simple approach: Transform point to local space
       Matrix inv = worldMatrix.invert();
       Vec3 localP = inv.mulPoint(worldPoint);
       
       // Expand bounds slightly for player radius
       float padding = 0.5f; 
       return (localP.x >= localAABB.min.x - padding && localP.x <= localAABB.max.x + padding &&
               localP.y >= localAABB.min.y - padding && localP.y <= localAABB.max.y + padding &&
               localP.z >= localAABB.min.z - padding && localP.z <= localAABB.max.z + padding);
    }


    void loadMeshes(Core* core, const std::string& filename)
    {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        loader.load(filename, gemmeshes);
        for (int i = 0; i < gemmeshes.size(); i++) {
            Mesh* mesh=new Mesh;
            std::vector<STATIC_VERTEX> vertices;
            for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                STATIC_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
                vertices.push_back(v);
                
                // Update AABB
                if (v.pos.x < localAABB.min.x) localAABB.min.x = v.pos.x;
                if (v.pos.y < localAABB.min.y) localAABB.min.y = v.pos.y;
                if (v.pos.z < localAABB.min.z) localAABB.min.z = v.pos.z;
                if (v.pos.x > localAABB.max.x) localAABB.max.x = v.pos.x;
                if (v.pos.y > localAABB.max.y) localAABB.max.y = v.pos.y;
                if (v.pos.z > localAABB.max.z) localAABB.max.z = v.pos.z;
            }
            mesh->init(core, vertices, gemmeshes[i].indices);
            meshes.push_back(mesh);
            shader.LoadShaders("VertexShader.hlsl", "PixelShader.hlsl");

            // Reflect shaders to populate constant buffer offsets
            shader.ReflectShaders(core, shader.pixelShader, false);
            shader.ReflectShaders(core, shader.vertexShader, true);

            psos.createPSO(
                core,
                "Triangle",
                shader.vertexShader,
                shader.pixelShader,
                mesh->inputLayoutDesc
            );
        }

	}

    void draw(Core* core, Matrix& w, Matrix& vp, float time, const Vec3& camPos)
    {
       
        core->beginRenderPass();

        for (int i = 0; i < meshes.size(); i++)
        {
            
            // Check if constant buffer variables exist before updating to avoid crashes if reflection failed or variables optimized out
            if (shader.vsConstantBuffers.size() > 0) {
                 shader.vsConstantBuffers[0]->update("W", &w);
                 shader.vsConstantBuffers[0]->update("VP", &vp);
                 shader.vsConstantBuffers[0]->update("time", (void*)&time); // Explicit cast just in case
                 shader.vsConstantBuffers[0]->update("cameraPos", (void*)&camPos);
            }



            shader.apply(core);

            psos.bind(core, "Triangle");
            meshes[i]->draw(core);
          
        }
	}
};