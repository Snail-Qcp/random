// gen_random_numbers.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdint.h>
#include <time.h>
#include <list>
#include <random>
#include "rand.h"
#include "Rand_tlbb.h"
#include "Random_moshou.h"

static uint32_t nRandomSeed = 42;

//---------------------------------------------------------------------------
// 函数:	RandomnSeed
// 功能:	设置随机数种子
// 参数:	nRandomSeed	:	随机数种子
// 返回:	void
//---------------------------------------------------------------------------
void g_RandomSeed(uint32_t nSeed)
{
	nRandomSeed = nSeed;
}

#define _USENEWRANDOMFUNC

uint32_t g_Random(uint32_t nMax)
{
	if (nMax)
	{
#ifdef _USENEWRANDOMFUNC
		//--> Rocker 2004/08/23
		unsigned int f = nRandomSeed * 0x08088405 + 1;
		nRandomSeed = f;
#ifndef _WIN32
		long long t = (long long)f * (long long)nMax;
#else
		int64_t t = (int64_t)f * (int64_t)nMax;
#endif
		t = t >> 32;
		return (uint32_t)t;
		//<-- End
#else
		// Modify by cooler
		// liuyujun@263.net 2004/3/25 -->
		//nRandomSeed = nRandomSeed * IA + IC;
		//return nRandomSeed % nMax;
		// Standard C rand function, max rand number is 32767
		nRandomSeed = nRandomSeed * 214013L + 2531011L;
		return ((nRandomSeed >> 16) & 0x7fff) % nMax;
		// <-- End cooler modify.
#endif		
	}
	else
	{
		return 0;
	}
}


void dump(std::list<int> &lstNumbers)
{
	lstNumbers.sort();

	int nIndex = 0;
	for (std::list<int32_t>::iterator it = lstNumbers.begin(); it != lstNumbers.end(); ++it)
	{
		std::cout << *it << "\t";
		if ((++nIndex) % 10 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

#define RANDNUM 1000

#define BEGIN_TEST_RAND(rand_algorithm) std::cout<<"=================="<<rand_algorithm<<"======================"<<std::endl
#define END_TEST_RAND(lstNumbers) dump(lstNumbers)

void test_random_crand()
{
	BEGIN_TEST_RAND("test_random_crand");

	std::list<int32_t> lstNumbers;
	lstNumbers.clear();

	srand((unsigned int)time(NULL));

	for (int nIndex = 0; nIndex < 100; ++nIndex)
	{
		lstNumbers.push_back(rand() % RANDNUM);
	}

	END_TEST_RAND(lstNumbers);
}

void test_random_cpp11()
{
	BEGIN_TEST_RAND("test_random_cpp11");

	std::uniform_int_distribution<int> cpp_random;
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());

	std::list<int32_t> lstNumbers;
	lstNumbers.clear();

	for (int nIndex = 0; nIndex < 100; ++nIndex)
	{
		lstNumbers.push_back(cpp_random(generator) % RANDNUM);
	}

	END_TEST_RAND(lstNumbers);
}

void test_random_jgy()
{
	BEGIN_TEST_RAND("test_random_jgy");

	std::list<int32_t> lstNumbers;
	lstNumbers.clear();

	g_RandomSeed((unsigned int)time(NULL));

	for (int nIndex = 0; nIndex < 100; ++nIndex)
	{
		lstNumbers.push_back(g_Random(RANDNUM));
	}

	END_TEST_RAND(lstNumbers);
}

void test_random_redis()
{
	BEGIN_TEST_RAND("test_random_redis");

	std::list<int32_t> lstNumbers;
	lstNumbers.clear();

	redisSrand48((int32_t)time(NULL));

	for (int nIndex = 0; nIndex < 100; ++nIndex)
	{
		lstNumbers.push_back(redisLrand48() % RANDNUM);
	}

	END_TEST_RAND(lstNumbers);
}

void test_random_tlbb()
{
	BEGIN_TEST_RAND("test_random_tlbb");

	RandGen								tlbb_rand;

	std::list<int32_t> lstNumbers;
	lstNumbers.clear();

	redisSrand48((int32_t)time(NULL));

	for (int nIndex = 0; nIndex < 100; ++nIndex)
	{
		lstNumbers.push_back(tlbb_rand.GetRand(0, RANDNUM));
	}

	END_TEST_RAND(lstNumbers);
}

void test_random_moshou()
{
	BEGIN_TEST_RAND("test_random_wow");

	std::list<int32_t> lstNumbers;
	lstNumbers.clear();

	for (int nIndex = 0; nIndex < 100; ++nIndex)
	{
		lstNumbers.push_back(rand32() % RANDNUM);
	}

	END_TEST_RAND(lstNumbers);
}


int main()
{
	test_random_crand();
	test_random_cpp11();
	test_random_redis();
	test_random_tlbb();
	test_random_moshou();
	test_random_jgy();

	//FILE *pFile = nullptr;
	//fopen_s(&pFile, "numbers.txt", "w");
	//if (nullptr != pFile)
	//{
	//	for (int nIndex = 0; nIndex < 10000; ++nIndex)
	//	{
	//		char szNumber[32] = { 0 };
	//		sprintf_s(szNumber, sizeof(szNumber), "%d\n", arNumbers[nIndex]);
	//		fputs(szNumber, pFile);
	//	}

	//	fclose(pFile);
	//	pFile = nullptr;
	//}
	std::cin.get();
	return EXIT_SUCCESS;
}
