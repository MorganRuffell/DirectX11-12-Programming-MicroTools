#pragma once

#include <map>
#include <vector>
#include <string>

class BaseClass
{
	std::vector<std::string> ExampleVector;
	std::map<int, std::string> ExampleMap;

public:

	BaseClass()
	{
		ExampleVector.push_back("Default");
		ExampleVector.push_back("Default");
		ExampleVector.push_back("Default");
		ExampleVector.push_back("Default");
		ExampleVector.push_back("Default");

		ExampleMap.emplace(1, "Default");
		ExampleMap.emplace(2, "Default");
		ExampleMap.emplace(3, "Default");
		ExampleMap.emplace(4, "Default");
		ExampleMap.emplace(5, "Default");
		ExampleMap.emplace(6, "Default");
	}

	~BaseClass()
	{

	}

};

