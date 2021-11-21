#include "pch.h"
#include <thread>

#define Silence

using namespace winrt;
using namespace Microsoft::WRL;


void CheckCompatibility(ComPtr<ID3D12Device8>& GraphicsCard)
{

	D3D12_FEATURE_DATA_D3D12_OPTIONS BasicDX12Support{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 RaytracingCheck{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 MeshAndAmplificationShaders{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS3 BarycentricsSupport{};
	D3D12_FEATURE_DATA_ARCHITECTURE1 AdapterProperties{};
	D3D12_FEATURE_DATA_FEATURE_LEVELS FeatureLevels{};
	D3D12_FEATURE_DATA_SHADER_MODEL ShaderModels{};



	HRESULT BasicFeatureSet = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &BasicDX12Support, sizeof(BasicDX12Support));
	HRESULT TimestampsQueriesAndBarycentrics = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &BarycentricsSupport, sizeof(BarycentricsSupport));
	HRESULT DoesSupportRayTracing = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &RaytracingCheck, sizeof(RaytracingCheck));
	HRESULT DoesSupportMeshAndAmplificationShaders = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &MeshAndAmplificationShaders, sizeof(MeshAndAmplificationShaders));
	HRESULT TypeOfArchitecture = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE1, &AdapterProperties, sizeof(AdapterProperties));


	//To do -  fix me!
	//HRESULT ScopeOfFeatureLevelSupport = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &FeatureLevels, sizeof(FeatureLevels)); 
	//HRESULT MaxShaderLevel = GraphicsCard->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &ShaderModels, sizeof(ShaderModels));

	std::thread Outputthread = std::thread([&]() {

		if (BasicDX12Support.ConservativeRasterizationTier == D3D12_CONSERVATIVE_RASTERIZATION_TIER_3 || BasicDX12Support.ConservativeRasterizationTier == D3D12_CONSERVATIVE_RASTERIZATION_TIER_2 || BasicDX12Support.ConservativeRasterizationTier == D3D12_CONSERVATIVE_RASTERIZATION_TIER_1)
		{
			std::cout << "	" << " Supports Conservative Rasterisation" << std::endl;
		}

		if (RaytracingCheck.RaytracingTier == D3D12_RAYTRACING_TIER_1_0 || RaytracingCheck.RaytracingTier == D3D12_RAYTRACING_TIER_1_1)
		{
			std::cout << "	" << " Supports GPU based DXR" << std::endl;
		}

		if (RaytracingCheck.RenderPassesTier == D3D12_RENDER_PASS_TIER_0)
		{
			std::cout << "	" << " Render Passes are only software emulated" << std::endl;

		}

		if (RaytracingCheck.RenderPassesTier == D3D12_RENDER_PASS_TIER_1)
		{
			std::cout << "	" << " Render Passes are implemented by the user-mode display driver. Render Target/depth buffer may also be accelerated. UAV writes are not supported within render passes" << std::endl;
		}

		if (RaytracingCheck.RenderPassesTier == D3D12_RENDER_PASS_TIER_2)
		{
			std::cout << "	" << " Render Passes are implemented by the user-mode display driver. Render Target/depth buffer may also be accelerated. UAV writes are supported within render passes" << std::endl;
		}

		if (MeshAndAmplificationShaders.MeshShaderTier != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED)
		{
			std::cout << "	" << " Supports GPU based Mesh Shaders" << std::endl;
		}


		});

	Outputthread.join();
}


std::string GetComputeGranularity(DXGI_ADAPTER_DESC3 DeviceAdapter)
{
	const DXGI_COMPUTE_PREEMPTION_GRANULARITY GraphicsGran = DeviceAdapter.ComputePreemptionGranularity;

	switch (GraphicsGran)
	{
	case DXGI_COMPUTE_PREEMPTION_DMA_BUFFER_BOUNDARY:
		return "Compute Packet";
		break;


	case DXGI_COMPUTE_PREEMPTION_DISPATCH_BOUNDARY:
		return "Dispatch, Part of a compute packet";
		break;


	case DXGI_COMPUTE_PREEMPTION_THREAD_GROUP_BOUNDARY:
		return "Thread Group, Group of threads, part of a dispatch";
		break;


	case DXGI_COMPUTE_PREEMPTION_THREAD_BOUNDARY:
		return "Thread - a line of execution, Part of a thread group.";
		break;


	case DXGI_COMPUTE_PREEMPTION_INSTRUCTION_BOUNDARY:
		return "Compute Instruction Boundary, during execution";
		break;

	default:
		break;
	}
}

std::string GetGraphicsPreemptGranularity(DXGI_ADAPTER_DESC3 DeviceAdapter)
{
	const DXGI_GRAPHICS_PREEMPTION_GRANULARITY GraphicsGran = DeviceAdapter.GraphicsPreemptionGranularity;

	switch (GraphicsGran)
	{
	case DXGI_GRAPHICS_PREEMPTION_DMA_BUFFER_BOUNDARY:
		return "DMA Buffer, where primitives are stored";
		break;


	case DXGI_GRAPHICS_PREEMPTION_PRIMITIVE_BOUNDARY:
		return "Primitive, a section of a DMA Buffer";
		break;


	case DXGI_GRAPHICS_PREEMPTION_TRIANGLE_BOUNDARY:
		return "Triangle, a section of a Primitive";
		break;


	case DXGI_GRAPHICS_PREEMPTION_PIXEL_BOUNDARY:
		return "Pixel, after a triangle has been drawn";
		break;


	case DXGI_GRAPHICS_PREEMPTION_INSTRUCTION_BOUNDARY:
		return "Graphics Instruction Boundary, during pixel operations";
		break;

	default:
		break;
	}
}

std::string GetVendorID(DXGI_ADAPTER_DESC3 DeviceAdapter)
{
	const UINT value = DeviceAdapter.VendorId;

	if (value == 4318)
	{
		return "NVidia Graphics Accelerator";
	}

	if (value == 4310)
	{
		return "AMD Graphics Accelerator";
	}

	if (value == 8086)
	{
		return "Intel Graphics Accelerator";
	}

	if (value == 4136)
	{
		return "Dell Graphics Accelerator";
	}

	if (value == 6372)
	{
		return "Acer Graphics Accelerator";
	}

	return "Not listed! Uncommon GPU in 2021!";

}

int main()
{
	ComPtr<IDXGIAdapter> adapter = {};
	ComPtr<IDXGIFactory7> Factory;

	std::thread ThreadA = std::thread([&]() {
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory4), &Factory);
		if (FAILED(hr))
			std::cout << "Creating Factory Failed!" << std::endl;
		});

	UINT index = 1;

	DXGI_FORMAT Format = {};



	std::vector<ComPtr<ID3D12Device8>> Collection;

	std::cout << "  ------- Adapter Information ------" << std::endl << std::endl;

	ThreadA.join();
	while (Factory->EnumAdapters(index, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		ComPtr<ID3D12Device8> DirectX12Device; //do not use device nine it doesn't work -- no idea why, will find out! Probably in GraphicsGran later version of windows

		DXGI_ADAPTER_DESC3 AdapterInfo;

		std::thread ThreadB = std::thread([&]() {
			ComPtr<IDXGIAdapter4> NewAdapter;

			HRESULT Res = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&DirectX12Device));
			if (FAILED(Res))
				std::cout << "Failed to create device instance!" << std::endl;

			HRESULT Res2 = Factory->EnumAdapterByLuid(DirectX12Device->GetAdapterLuid(), __uuidof(IDXGIAdapter4), &NewAdapter);
			if (FAILED(Res2))
				std::cout << "Failed to create adapter!" << std::endl;


			NewAdapter->GetDesc3(&AdapterInfo);
		});

		std::wstring text = L" Adapter: ";


		ThreadB.join();
		text += AdapterInfo.Description;
		std::wcout << text.c_str() << std::endl << std::endl;
		std::cout << "  " << " " << AdapterInfo.DedicatedSystemMemory / 1e7 << "Gb of Dedicated System Memory" << std::endl;
		std::cout << "  " << " " << AdapterInfo.DedicatedVideoMemory / 1e9 << "Gb of Dedicated Video Memory" << std::endl;
		std::cout << "  " << " " << AdapterInfo.SharedSystemMemory / 1e9 << "Gb of Shared System Memory" << std::endl;

		std::cout << "  " << " The Device id is: " << AdapterInfo.DeviceId << std::endl;
		std::cout << "  " << " For Driver programming only, these are the unique Id values of the adapater: " << AdapterInfo.AdapterLuid.HighPart << std::endl;
		std::cout << "  " << " For Driver programming only, these are the unique Id values of the adapater: " << AdapterInfo.AdapterLuid.LowPart << std::endl;
		std::cout << "  " << " The PCI Revision ID number of the adapter is " << AdapterInfo.Revision << std::endl;
		std::cout << "  " << " Device Flags are:   " << AdapterInfo.Flags << std::endl;


		std::cout << "  " << " Accelerator Vender is:  " << GetVendorID(AdapterInfo) << std::endl;
		std::cout << "  " << " Compute Preempt Granularity: " << GetComputeGranularity(AdapterInfo) << std::endl;
		std::cout << "  " << " Graphics Preempt Granularity: " << GetGraphicsPreemptGranularity(AdapterInfo) << std::endl << std::endl;



		std::cout << "    Device Information - DX12 Data" << std::endl << std::endl;

		std::cout << "	" << " Amount of Graphics Accelerators: " << DirectX12Device->GetNodeCount() << std::endl;


		std::thread ThreadE = std::thread([&]() {
			CheckCompatibility(DirectX12Device);
		});

		//std::cout << "	" << " " <<  std::endl;


		std::cout << " " << std::endl;
		ThreadE.join();
		index++;
	}


	std::cin.get();

	return 0;
}
