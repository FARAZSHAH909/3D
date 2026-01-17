#pragma once
#include "maths.h"
#include <vector>
#include"core.h"
#include "Mesh.h"
#include "Static_Vertex.h"

struct Bone
{
    std::string name; // 骨骼的名称 [cite: 399]
    Matrix offset; // 绑定姿势矩阵的逆矩阵（BP^-1），用于蒙皮计算 [cite: 400]
    int parentIndex; // 父骨骼在骨骼列表中的索引，-1 表示它是根骨骼 [cite: 401]
};
struct Skeleton
{
    std::vector<Bone> bones; // 骨骼的列表 [cite: 404]
    Matrix globalInverse; // 全局逆矩阵 (G^-1)，用于抵消模型创建时的全局变换，通常为单位矩阵 [cite: 405, 158, 161]
};

struct ANIMATED_VERTEX
{
    Vec3 pos;
    Vec3 normal;
    Vec3 tangent;
    float tu;
    float tv;
    unsigned int bonesIDs[4];
    float boneWeights[4];
};


class Animation
{
public:
    std::map<std::string, AnimationSequence> animations; // 存储多个命名动画序列的映射 [cite: 467]
    Skeleton skeleton; // 存储骨架结构 [cite: 467]

	void init(ANIMATED_VERTEX* vertices, size_t vertexCount, unsigned int* indices, size_t indexCount)
    {
		Mesh::init(vertices, sizeof(ANIMATED_VERTEX), vertexCount, indices, indexCount);
		inputLayoutDesc = VertexLayoutCache::getAnimatedLayout();

        static const D3D12_INPUT_LAYOUT_DESC& getAnimatedLayout() {
            static const D3D12_INPUT_ELEMENT_DESC inputLayoutAnimated[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            };
            static const D3D12_INPUT_LAYOUT_DESC desc = { inputLayoutAnimated, 6 };
            return desc;
	}

       


    void init(std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
    {
        init(&vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
        inputLayoutDesc = VertexLayoutCache::getAnimatedLayout();
    }


    // 封装 AnimationSequence 中的帧计算 [cite: 468]
    void calcFrame(std::string name, float t, int& frame, float& interpolationFact);

    // 封装 AnimationSequence 中的骨骼全局变换计算 [cite: 470]
    Matrix interpolateBoneToGlobal(...);

    // 计算最终的动画偏移矩阵 A_i = B_model,i * BP^-1 * G^-1 [cite: 477, 478, 482]
    void calcFinalTransforms(Matrix* matrices);
};


struct AnimationFrame
{
    std::vector<Vec3> positions; // 每根骨骼的位置向量 [cite: 413]
    std::vector<Quaternion> rotations; // 每根骨骼的四元数旋转 [cite: 414]
    std::vector<Vec3> scales; // 每根骨骼的缩放向量 [cite: 415]
};
class AnimationSequence
{
public:
    std::vector<AnimationFrame> frames; // 动画帧的集合 [cite: 422]
    float ticksPerSecond; // 每秒的节拍数/帧率 [cite: 423]

    // 辅助函数:
    Vec3 interpolate(Vec3 p1, Vec3 p2, float t); // 向量的线性插值 [cite: 424, 425]
    Quaternion interpolate(Quaternion q1, Quaternion q2, float t); // 四元数的球面线性插值 (slerp) [cite: 426, 427]
    float duration(); // 计算动画总时长 [cite: 430, 431]

    // 根据时间 t 计算当前帧和插值因子 [cite: 436, 437]
    void calcFrame(float t, int& frame, float& interpolationFact);

    // 计算骨骼在模型空间中的变换矩阵 (B_model,i)，涉及 FK [cite: 449, 456, 461]
    Matrix interpolateBoneToGlobal(...);
};

class AnimationInstance
{
public:
    Animation* animation; // 指向共享的 Animation 数据的指针 [cite: 493]
    std::string currentAnimation; // 当前播放的动画名称 [cite: 494]
    float t; // 当前动画的播放时间 [cite: 495]
    Matrix matrices[256]; // 存储当前时刻所有骨骼的最终动画偏移矩阵 (A_i) [cite: 496]

    // 辅助函数:
    void resetAnimationTime(); // 重置动画时间 [cite: 500, 503]
    bool animationFinished(); // 检查动画是否播放完毕 [cite: 504, 509]
    void update(std::string name, float dt); // 更新动画时间并计算所有骨骼的变换矩阵 [cite: 514, 522-527]
};