#include"ExMathUtility.h"

// AABBとAABBの衝突判定
bool IsCollisionAABBtoAABB(const KamataEngine::Vector3& center1, const KamataEngine::Vector3& size1, const KamataEngine::Vector3& center2, const KamataEngine::Vector3& size2) {

	float min1X = center1.x - size1.x / 2.0f;
	float max1X = center1.x + size1.x / 2.0f;
	float min1Y = center1.y - size1.y / 2.0f;
	float max1Y = center1.y + size1.y / 2.0f;
	float min1Z = center1.z - size1.z / 2.0f;
	float max1Z = center1.z + size1.z / 2.0f;

	float min2X = center2.x - size2.x / 2.0f;
	float max2X = center2.x + size2.x / 2.0f;
	float min2Y = center2.y - size2.y / 2.0f;
	float max2Y = center2.y + size2.y / 2.0f;
	float min2Z = center2.z - size2.z / 2.0f;
	float max2Z = center2.z + size2.z / 2.0f;

	// 各軸で重なっているかチェック
	bool isCollisionX = (min1X <= max2X && max1X >= min2X);
	bool isCollisionY = (min1Y <= max2Y && max1Y >= min2Y);
	bool isCollisionZ = (min1Z <= max2Z && max1Z >= min2Z);

	return isCollisionX && isCollisionY && isCollisionZ;
}

// AABBとSphereの衝突判定
bool IsCollisionAABBtoSphere(const KamataEngine::Vector3& aabbCenter, const KamataEngine::Vector3& aabbSize, const KamataEngine::Vector3& sphereCenter, float sphereRadius) {

	// AABBの最小点と最大点を計算
	KamataEngine::Vector3 aabbMin = {aabbCenter.x - aabbSize.x / 2.0f, aabbCenter.y - aabbSize.y / 2.0f, aabbCenter.z - aabbSize.z / 2.0f};
	KamataEngine::Vector3 aabbMax = {aabbCenter.x + aabbSize.x / 2.0f, aabbCenter.y + aabbSize.y / 2.0f, aabbCenter.z + aabbSize.z / 2.0f};

	// AABB上の球の中心に最も近い点をクランプで求める
	float closestX = std::max(aabbMin.x, std::min(sphereCenter.x, aabbMax.x));
	float closestY = std::max(aabbMin.y, std::min(sphereCenter.y, aabbMax.y));
	float closestZ = std::max(aabbMin.z, std::min(sphereCenter.z, aabbMax.z));

	// 最も近い点と球の中心の距離の二乗を求める
	float distanceSq =
	    (sphereCenter.x - closestX) * (sphereCenter.x - closestX) + (sphereCenter.y - closestY) * (sphereCenter.y - closestY) + (sphereCenter.z - closestZ) * (sphereCenter.z - closestZ);

	// 距離が半径の二乗以下であれば衝突
	return distanceSq <= sphereRadius * sphereRadius;
}