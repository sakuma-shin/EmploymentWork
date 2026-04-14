#include "GpuParticle.h"
#include "KamataEngine.h"
#include <base/DirectXCommon.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <wrl.h>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <sstream>
#include <Windows.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace KamataEngine;
using namespace MathUtility;
using namespace Microsoft::WRL;

GpuParticleSystem* GpuParticleSystem::GetInstance() {
	static GpuParticleSystem instance;
	return &instance;
}

void GpuParticleSystem::Initialize(size_t maxParticles) {
	if (initialized_) { return; }
	assert(maxParticles > 0);
	maxInstances_ = maxParticles;

	// クアッド頂点（XY平面、中心原点）
	struct Vertex {
		float pos[3];
		float uv[2];
	};
	Vertex vertices[4] = {
	    {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
	    {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
	    {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
	    {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
	};
	uint16_t indices[6] = {0, 1, 2, 0, 2, 3};

	auto device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点バッファ（default -> upload にコピーは省略して簡潔化。小数回数なら upload のまま問題ない。）
	{
		const UINT byteSize = sizeof(vertices);
		// upload heap（簡潔に upload を使う）
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		                                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vbVertex_));

		// マップしてコピー
		void* mapped = nullptr;
		vbVertex_->Map(0, nullptr, &mapped);
		std::memcpy(mapped, vertices, byteSize);
		vbVertex_->Unmap(0, nullptr);

		vbvVertex_.BufferLocation = vbVertex_->GetGPUVirtualAddress();
		vbvVertex_.SizeInBytes = byteSize;
		vbvVertex_.StrideInBytes = sizeof(Vertex);
	}

	// インデックスバッファ
	{
		const UINT byteSize = sizeof(indices);
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		                                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&ib_));

		void* mapped = nullptr;
		ib_->Map(0, nullptr, &mapped);
		std::memcpy(mapped, indices, byteSize);
		ib_->Unmap(0, nullptr);

		ibv_.BufferLocation = ib_->GetGPUVirtualAddress();
		ibv_.SizeInBytes = sizeof(indices);
		ibv_.Format = DXGI_FORMAT_R16_UINT;
	}

	// インスタンスバッファ（upload、フレーム毎に Map して上書き）
	{
		const UINT byteSize = static_cast<UINT>(sizeof(InstanceData) * maxInstances_);
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		                                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vbInstanceUpload_));

		vbvInstance_.BufferLocation = vbInstanceUpload_->GetGPUVirtualAddress();
		vbvInstance_.SizeInBytes = byteSize;
		vbvInstance_.StrideInBytes = static_cast<UINT>(sizeof(InstanceData));
	}

	// シェーダコンパイル（実行ファイル基準の絶対パスを生成して使用、存在チェックと詳細ログを追加）
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;
	HRESULT hr;

	// 実行ファイルのディレクトリを基準にシェーダパスを作成
	wchar_t exePath[MAX_PATH] = {};
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);
	std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
	std::wstring vsPath = (exeDir / L"Resources" / L"shaders" / L"GpuParticleVS.hlsl").wstring();
	std::wstring psPath = (exeDir / L"Resources" / L"shaders" / L"GpuParticlePS.hlsl").wstring();

	// ログ出力ヘルパ
	auto LogW = [](const std::wstring& s) { OutputDebugStringW(s.c_str()); };
	auto LogA = [](const std::string& s) { OutputDebugStringA(s.c_str()); };

	// パスと存在チェックログ
	{
	    std::wstringstream ss;
	    ss << L"GpuParticle: VS path=" << vsPath << L"\n";
	    ss << L"GpuParticle: PS path=" << psPath << L"\n";
	    LogW(ss.str());
	}
	if (GetFileAttributesW(vsPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
	    std::wstringstream ss;
	    ss << L"GpuParticle: VS file not found: " << vsPath << L"\n";
	    LogW(ss.str());
	    assert(false);
	}
	if (GetFileAttributesW(psPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
	    std::wstringstream ss;
	    ss << L"GpuParticle: PS file not found: " << psPath << L"\n";
	    LogW(ss.str());
	    assert(false);
	}

	// 実コンパイル（VS）
	hr = D3DCompileFromFile(vsPath.c_str(), nullptr,
	                        D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
	                        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
				LogA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		std::wstringstream ss;
		ss << L"D3DCompileFromFile(VS) failed hr=0x" << std::hex << hr << L" path=" << vsPath << L"\n";
		LogW(ss.str());
		assert(false);
	}

	// 実コンパイル（PS）
	errorBlob.Reset();
	hr = D3DCompileFromFile(psPath.c_str(), nullptr,
	                        D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
	                        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			LogA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		std::wstringstream ss;
		ss << L"D3DCompileFromFile(PS) failed hr=0x" << std::hex << hr << L" path=" << psPath << L"\n";
		LogW(ss.str());
		assert(false);
	}

	// 入力レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	// vertex slot 0: per-vertex
	inputLayout.push_back({"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
	inputLayout.push_back({"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});

	// instance slot 1: world matrix (4 x float4)
	for (UINT i = 0; i < 4; ++i) {
		D3D12_INPUT_ELEMENT_DESC desc = {};
		desc.SemanticName = "INSTANCEWORLD";
		desc.SemanticIndex = i;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.InputSlot = 1;
		desc.AlignedByteOffset = sizeof(float) * 4 * i;
		desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
		desc.InstanceDataStepRate = 1;
		inputLayout.push_back(desc);
	}
	// instance color
	D3D12_INPUT_ELEMENT_DESC colorDesc = {};
	colorDesc.SemanticName = "INSTANCECOLOR";
	colorDesc.SemanticIndex = 0;
	colorDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	colorDesc.InputSlot = 1;
	colorDesc.AlignedByteOffset = sizeof(float) * 16;
	colorDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
	colorDesc.InstanceDataStepRate = 1;
	inputLayout.push_back(colorDesc	);
	// instance params
	inputLayout.push_back({"INSTANCEPARAM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(float) * 20, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1});

	// ルートシグネチャ: b0 = ViewProjection（camera の CB を流用）
	CD3DX12_ROOT_PARAMETER rootparams[1];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	CD3DX12_ROOT_SIGNATURE_DESC rootDesc;
	rootDesc.Init(_countof(rootparams), rootparams, 0, nullptr,
	              D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	hr = D3DX12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		assert(false);
	}
	hr = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));

	// PSO 作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	// ラスタライザ/デプス/ブレンドは簡易設定（既存の設定に合わせる）
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// alpha blending enable
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.pRootSignature = rootSignature_.Get();

	// input layout set
	psoDesc.InputLayout.pInputElementDescs = inputLayout.data();
	psoDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout.size());

	hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(hr));

	instancesCPU_.reserve(maxInstances_);

	initialized_ = true;
}

void GpuParticleSystem::Finalize() {
	// Release resources
	vbVertex_.Reset();
	ib_.Reset();
	vbInstanceUpload_.Reset();
	rootSignature_.Reset();
	pipelineState_.Reset();
	instancesCPU_.clear();
	initialized_ = false;
}

static void SetMatrixFromWorldTransform(const WorldTransform& wt, float outMat[16]) {
	// WorldTransform::AffineMatrix() を既に用意している場合は world matrix を使うが
	// here we compute row-major matrix from translation/rotation/scale similarly to other code.
	// Use helper functions from MathUtility if available; otherwise construct manually.

	// We will create row-major matrix as scale * rotZ * rotX * rotY * trans as used elsewhere.
	Matrix4x4 matScale = MakeScaleMatrix(wt.scale_);
	Matrix4x4 matRotX = MakeRotateXMatrix(wt.rotation_.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(wt.rotation_.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(wt.rotation_.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;
	Matrix4x4 matTrans = MakeTranslateMatrix(wt.translation_);
	Matrix4x4 matWorld = matScale * matRot * matTrans;

	// Row-major copy
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			outMat[r * 4 + c] = matWorld.m[r][c];
		}
	}
}

void GpuParticleSystem::Emit(const WorldTransform& worldTransform, const Vector4& color, const Vector2& size, bool billboard) {
	if (!initialized_) return;
	if (instancesCPU_.size() >= maxInstances_) return;

	InstanceData d{};
	SetMatrixFromWorldTransform(worldTransform, d.mWorld);
	d.color[0] = color.x;
	d.color[1] = color.y;
	d.color[2] = color.z;
	d.color[3] = color.w;
	d.params[0] = billboard ? 1.0f : 0.0f;
	d.params[1] = size.x;
	d.params[2] = size.y;
	d.params[3] = 0.0f;

	instancesCPU_.push_back(d);
}

void GpuParticleSystem::Update() {
	// ここでは CPU 側の寿命管理などを行っても良いが、今回は簡潔化のため発生のみで寿命は扱わない。
	// インスタンスバッファへコピー
	if (!initialized_) return;
	if (instancesCPU_.empty()) return;

	// Map してコピー
	void* mapped = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	vbInstanceUpload_->Map(0, &readRange, &mapped);
	std::memcpy(mapped, instancesCPU_.data(), sizeof(InstanceData) * instancesCPU_.size());
	vbInstanceUpload_->Unmap(0, nullptr);
}

void GpuParticleSystem::Draw(Camera& camera) {
	if (!initialized_) return;
	if (instancesCPU_.empty()) return;

	auto dx = DirectXCommon::GetInstance();
	auto cmdList = dx->GetCommandList();

	// PSO とルートシグネチャ
	cmdList->SetPipelineState(pipelineState_.Get());
	cmdList->SetGraphicsRootSignature(rootSignature_.Get());

	// camera の CBV を b0 にセット
	cmdList->SetGraphicsRootConstantBufferView(0, camera.GetConstBuffer()->GetGPUVirtualAddress());

	// IA 設定
	// vertex: slot0, instance: slot1
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = {vbvVertex_, vbvInstance_};
	cmdList->IASetVertexBuffers(0, 2, vbViews);
	cmdList->IASetIndexBuffer(&ibv_);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// DrawIndexedInstanced
	const UINT indexCount = 6;
	const UINT instanceCount = static_cast<UINT>(instancesCPU_.size());
	cmdList->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);

	// 発生済みインスタンスは今回描画後も残す（必要なら寿命管理で消す）
	instancesCPU_.clear();
}