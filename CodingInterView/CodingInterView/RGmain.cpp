
#include "RandomGacha.h"

int main()
{
	RandomGacha RG;
	RG.SetStarForce("../RandomTable/StarForce.csv");

	RG.MakeWeapon(1000);

	RG.ReinforceStarforce();

	return 0;
}

//// compile with: /EHsc /W4
//
//#include <random>
//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <map>
//
//using namespace std;
//
//void test(const int s) {
//
//	// uncomment to use a non-deterministic generator
//	// random_device rd;
//	// mt19937 gen(rd());
//	mt19937 gen(1701);
//
//	discrete_distribution<> distr({ 1, 2, 3, 4, 5 });
//
//	cout << endl;
//	cout << "min() == " << distr.min() << endl;
//	cout << "max() == " << distr.max() << endl;
//	cout << "probabilities (value: probability):" << endl;
//	vector<double> p = distr.probabilities();
//	int counter = 0;
//	for (const auto& n : p) 
//	{
//		cout << fixed << setw(11) << counter << ": " << setw(14) << setprecision(10) << n << endl;
//		++counter;
//	}
//	cout << endl;
//
//	// generate the distribution as a histogram
//	map<int, int> histogram;
//	for (int i = 0; i < s; ++i) 
//	{
//		++histogram[distr(gen)];
//	}
//
//	// print results
//	cout << "Distribution for " << s << " samples:" << endl;
//	for (const auto& elem : histogram) 
//	{
//		cout << setw(5) << elem.first << ' ' << string(elem.second, ':') << endl;
//	}
//	cout << endl;
//}
//
//int main()
//{
//	int samples = 100;
//
//	cout << "Use CTRL-Z to bypass data entry and run using default values." << endl;
//	cout << "Enter an integer value for the sample count: ";
//	cin >> samples;
//
//	test(samples);
//}