#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <set>
#include <cstdlib>

#include "topology.h"
#include "routing.h"

using namespace std;

#define max(x, y) ((x > y) ? x:y)
#define min(x, y) ((x > y) ? y:x)

class Calculation 
{
	public:
		Calculation();
		~Calculation();
		Calculation(char *topoFile, char* routeFile);

		void printTopology() { topologyTable.printData(); };
		void printRouting() { routingTable.printData(); };
		int calculate();
		int getHopCount(int from, int to);
		int balance();
		int getRoute(int from, int to);
		
	private:
		Topology topologyTable;
		Routing routingTable;
};

