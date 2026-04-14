#pragma once
#include "KamataEngine.h"
#include <vector>

namespace KamataEngine { class Camera; }

class GpuParticleSystem {
public:
	// シングルトン取得
	static GpuParticleSystem* GetInstance();

	// 初期化（最大パーティクル数）
	void Initialize(size_t maxParticles = 1024);

	// 破棄
	void Finalize();

	// WorldTransform でパーティクルを発生させる
	// color.w は未使用（透過などは拡張で使用可）
	// size.x/size.y はローカル Quad のスケール（幅・高さ）
	// billboard==true のときカメラへ常に向く
	void Emit(const KamataEngine::WorldTransform& worldTransform,
	          const KamataEngine::Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f},
	          const KamataEngine::Vector2& size = {1.0f, 1.0f},
	          bool billboard = true);

	// 更新（CPU -> インスタンスバッファ転送）
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& camera);

private:
	GpuParticleSystem() = default;
	~GpuParticleSystem() = default;
	GpuParticleSystem(const GpuParticleSystem&) = delete;
	GpuParticleSystem& operator=(const GpuParticleSystem&) = delete;

	struct InstanceData {
		// world matrix (row-major) を float4 x4 として渡す
		float mWorld[16];
		// color rgba
		float color[4];
		// params: x = billboard(1/0), y = size.x, z = size.y, w = unused
		float params[4];
	};

	// CPU 側リスト（現在有効なパーティクル）
	std::vector<InstanceData> instancesCPU_;

	// D3D オブジェクト（作成は Initialize 内）
	Microsoft::WRL::ComPtr<ID3D12Resource> vbVertex_;
	Microsoft::WRL::ComPtr<ID3D12Resource> ib_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vbInstanceUpload_; // mapped upload heap
	D3D12_VERTEX_BUFFER_VIEW vbvVertex_ = {};
	D3D12_INDEX_BUFFER_VIEW ibv_ = {};
	D3D12_VERTEX_BUFFER_VIEW vbvInstance_ = {};

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;

	size_t maxInstances_ = 0;

	bool initialized_ = false;
};