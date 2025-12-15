#define NOMINMAX
#include"KamataEngine.h"
#include <algorithm>
// AABBとAABBの衝突判定
bool IsCollisionAABBtoAABB(const KamataEngine::Vector3& center1, const KamataEngine::Vector3& size1, const KamataEngine::Vector3& center2, const KamataEngine::Vector3& size2);

// AABBとSphereの衝突判定
bool IsCollisionAABBtoSphere(const KamataEngine::Vector3& aabbCenter, const KamataEngine::Vector3& aabbSize, const KamataEngine::Vector3& sphereCenter, float sphereRadius);