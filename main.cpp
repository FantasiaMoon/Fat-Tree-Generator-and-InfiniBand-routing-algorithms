/* 
 * Description: Main function to run the program
 *
 * @Usage   ./main [topoFile] [routeFile]
 * @file    calculation.cpp
 * @version 1.0
 * @author  Badr TOUNZI <contact@boostio.fr>
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <set>

#include "calculation.h"

using namespace std;

void usage(char * name)
{
	cout << "Usage : " << name << " [topoFile] [routeFile]" << endl;
	getchar();
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc < 2) usage(argv[0]);

	char *topoFile = argv[1];
	char *routeFile = argv[2];
	
	Calculation routecalc(topoFile, routeFile);

	cout << "Topo file data" << endl;
	getchar();
	routecalc.printTopology();

	cout << "Route file data" << endl;
	getchar();
	routecalc.printRouting();

	cout << "Number of hops calculation" << endl;
	getchar();
	cout<<endl<< "\tMinimum number of hops = " << routecalc.calculate() << endl;

	cout << "Route balancing " << endl;
	getchar();
	cout << "\tRoute balancing = " << routecalc.balance() << endl;
	getchar();

	cout<<endl;	
	return 0;
}
