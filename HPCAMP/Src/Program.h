//Morgan Ruffell, 2021



#pragma once

#include <iostream>
#include <vector>
#include <amp.h>

//Dynamic array of accelerators, we are getting all of them through the accelerator::get_all() function
std::vector<concurrency::accelerator> Accelerators = concurrency::accelerator::get_all();

concurrency::accelerator DefaultAccelerator(concurrency::accelerator::default_accelerator);
