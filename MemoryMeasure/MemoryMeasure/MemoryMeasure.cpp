// MemoryMeasure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory.h>
#include <atomic>
#include "BaseClass.h"
#include "MemoryMeasure.h"



struct SFloat3
{
	SFloat3()
	{
		m_x = 1.0f;
		m_y = 1.0f;
		m_z = 1.0f;
	}

	SFloat3(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}
	float m_x, m_y, m_z;
};

struct SFloat4
{
	SFloat4() = delete;

	SFloat4(float x, float y, float z, float w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	float m_x, m_y, m_z, m_w;
};

struct S4x4Matrix
{
	S4x4Matrix()
	{
		int size = sizeof(Data) / sizeof(Data[0]);

		for (int i = 0; i < size; i++)
		{
			Data[i] = 10.0f;
		}
	};

	float Data[16];

};

std::atomic<bool> AtomicBool;
std::atomic<int> Atomicint;
std::atomic<float> Atomicfloat;
std::atomic<SFloat3> Atomic3Vector;



int main()
{
	std::cout << " ------- Memory Sizes on Target System -- x64 Architecture ------ " << std::endl;

	//CreateSomeStructures();

	BaseClass Base;
	BaseClass* Base_Pointer = new BaseClass();

	SFloat3 BaseThreepointVector(1.0f, 1.0f, 1.0f);
	SFloat4 BaseFourPointVector(1.0f,1.0f, 1.0f, 1.0f);
	S4x4Matrix BaseMatrix;

	std::cout << "Size of char : " << sizeof(char) << std::endl;
	std::cout << "Size of bool : " << sizeof(bool) << std::endl;
	std::cout << "Size of int : " << sizeof(int) << std::endl;
	std::cout << "Size of short int : " << sizeof(short int) << std::endl;
	std::cout << "Size of long int : " << sizeof(long int) << std::endl;
	std::cout << "Size of float : " << sizeof(float) << std::endl;
	std::cout << "Size of double : " << sizeof(double) << std::endl;
	std::cout << "Size of wchar_t : " << sizeof(wchar_t) << std::endl << std::endl;

	std::cout << "--- Custom Data Types ---" << std::endl << std::endl;

	std::cout << "Size of basic class: " << sizeof(BaseClass) << std::endl;
	std::cout << "Size of Instantiated Basic Class: " << sizeof(Base) << std::endl;
	std::cout << "Size of Instantiated Basic Class Pointer: " << sizeof(Base_Pointer) << std::endl;
	std::cout << "Size of string : " << sizeof(std::string) << std::endl;
	std::cout << "Size of SFloat3 : " << sizeof(BaseThreepointVector) << std::endl;
	std::cout << "Size of SFloat4 : " << sizeof(BaseFourPointVector) << std::endl;
	std::cout << "Size of SFloat4 Matrix: " << sizeof(BaseMatrix) << std::endl << std::endl;


	std::cout << "--- Concurrent Data Types ---" << std::endl << std::endl;
	std::cout << "Size of Atomic Bool : " << sizeof(AtomicBool) << std::endl;
	std::cout << "Size of Atomic int : " << sizeof(Atomicint) << std::endl;
	std::cout << "Size of Atomic float : " << sizeof(Atomicfloat) << std::endl;
	std::cout << "Size of Atomic 3 Point Vector : " << sizeof(Atomic3Vector) << std::endl;




}

