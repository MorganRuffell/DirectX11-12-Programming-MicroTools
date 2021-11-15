// Morgan Ruffell, 2021

#include "Program.h"

bool DetermineWhetherAcceleratorIsEmulated(concurrency::accelerator& Accelerator)
{
	return Accelerator.get_is_emulated();
}

void DetermineAutoSelection(concurrency::accelerator& Accelerator)
{
	bool IsAutomaticSelection = Accelerator.get_default_view().get_is_auto_selection();

	if (IsAutomaticSelection)
	{
		std::cout << "	Automatic Selection if passed into a Parallel for loop!" << std::endl;
	}
	else
	{
		std::cout << "	Non-Automatic Selection if passed into a Parallel for loop!" << std::endl;
	}
}

void DetermineNatureOfDebug(concurrency::accelerator& x)
{
	bool IsDebug = x.get_default_view().get_is_debug();

	if (IsDebug)
	{
		std::cout << "	Supports a Debug layer for error reporting!" << std::endl;
	}
	else
	{
		std::cout << "	Does not support a Debug Layer for error reporting!" << std::endl;
	}
}


void DetermineQueuingMode(concurrency::accelerator& x)
{
	concurrency::queuing_mode QueuingMode = x.get_default_view().get_queuing_mode();

	switch (QueuingMode)
	{
	case Concurrency::queuing_mode_immediate:
		std::cout << "	Queuing mode is Immediate" << std::endl;
		break;
	case Concurrency::queuing_mode_automatic:
		std::cout << "	Queuing mode is Automatic" << std::endl;
		break;
	default:
		break;
	}
}

void DetermineCPUAccessType(concurrency::accelerator& x)
{
	Concurrency::access_type CPUAccessType = x.default_cpu_access_type;
	std::cout << " The default CPU access type is ";

	switch (CPUAccessType)
	{
	case Concurrency::access_type_none:
		std::cout << "Access Type None" << std::endl;
		break;
	case Concurrency::access_type_read:
		std::cout << "Access Type Read" << std::endl;
		break;
	case Concurrency::access_type_write:
		std::cout << "Access Type Write" << std::endl;
		break;
	case Concurrency::access_type_read_write:
		std::cout << "Access Type Read Write" << std::endl;
		break;
	case Concurrency::access_type_auto:
		std::cout << "Access Type Automatic" << std::endl;
		break;
	default:
		break;
	}
}

void FetchGenericParams(concurrency::accelerator& Accelerator)
{
	if (Accelerator.has_display)
	{
		std::cout << " This Accelerator can output to a display!" << std::endl;
	}

	if (Accelerator.supports_cpu_shared_memory)
	{
		std::cout << " Supports CPU shared memory" << std::endl;
	}

	if (Accelerator.supports_double_precision)
	{
		std::cout << " Supports Double Precision" << std::endl;
	}

	if (Accelerator.supports_limited_double_precision)
	{
		std::cout << " Supports Limited Double Precision" << std::endl;
	}
}


void Program(concurrency::accelerator& Accelerator)
{
	//for (concurrency::accelerator& Accelerator : Accelerators)

	std::wcout << Accelerator.description << std::endl << std::endl;

	std::cout << " Has " << Accelerator.dedicated_memory / 1e6 << " Gb of dedicated memory" << std::endl;

	std::thread SubThread = std::thread ( [&] {

		if (DetermineWhetherAcceleratorIsEmulated(Accelerator) == true)
		{
			std::cout << " This is an emulated Accelerator." << std::endl;
		}
		if (DetermineWhetherAcceleratorIsEmulated(Accelerator) == false)
		{
			std::cout << " This is not an emulated Accelerator." << std::endl;
		}
	});

	std::thread SubThreadTwo = std::thread([&] {
		FetchGenericParams(Accelerator);
		DetermineCPUAccessType(Accelerator);
	});

	SubThread.join();
	SubThreadTwo.join();
	
	std::wcout << " This device is stored at " << Accelerator.get_device_path() << std::endl << std::endl;

	std::cout << " Getting Values for Accelerator Viewer " << std::endl;

	std::thread subthreadThree = std::thread([&] {
		DetermineQueuingMode(Accelerator);
		DetermineNatureOfDebug(Accelerator);
		DetermineAutoSelection(Accelerator);
	});

	subthreadThree.join();

	if (Accelerator.version)
	{
		std::cout << std::endl;
		std::wcout << " Version Number of the accelerator is " << Accelerator.version << std::endl;
	}

	std::cout << std::endl << std::endl << std::endl << std::endl;

}

int main()
{

	std::cout << " -------------------  Device Accelerators ---------------------------" << std::endl << std::endl;

	std::cout << " Morgan Ruffell, 2021" << std::endl << std::endl << std::endl;

	std::cout << " Default Accelerator at " << Accelerators[0].default_accelerator << std::endl;
	std::cout << " CPU Accelerator at " << Accelerators[0].cpu_accelerator << std::endl;
	std::cout << " Direct3D Reference Accelerator at " << Accelerators[0].direct3d_ref << std::endl;
	std::cout << " Direct3D WARP Accelerator at " << Accelerators[0].direct3d_warp << std::endl << std::endl << std::endl;

	std::wcout << " Default accelerator is " << DefaultAccelerator.description << std::endl;
	std::wcout << " You can change code execution through parallel_for_each lambdas, by getting the view handle of the chosen accelerator, it defaults to " << DefaultAccelerator.description << std::endl << std::endl;

	std::cout << " --------------------------------------------------------------------" << std::endl << std::endl;

	int AmountOfElements = Accelerators.size();

	std::thread Thread = std::thread([=]{
		for (int i = 0; i < AmountOfElements; i++)
		{
			Program(Accelerators.at(i));
		}
	});

	Thread.join();

	std::cin.get();
}

