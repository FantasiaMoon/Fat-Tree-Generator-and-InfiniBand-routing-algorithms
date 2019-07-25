/* 
 * Description: Cette classe permet de calculer les métriques pour une topologie donnée.
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
#include <cstdlib>

#include "calculation.h"
#include "topology.h"
#include "routing.h"

using namespace std;

Calculation::Calculation() {}

Calculation::~Calculation() {}

Calculation::Calculation(char* topoFile, char* routeFile) 
{
	topologyTable.loadData(topoFile);
	routingTable.loadData(routeFile);
}

int Calculation::getHopCount(int fromId, int toId)
{
	int count = 0;
	
	struct hostNode * fromHost = topologyTable.getHostById(fromId);

	string fromNode = fromHost->name;
	string toNode = topologyTable.getHostById(toId)->name;

	string switchName = fromHost->dstName;
	
	do {

		struct routeItem * item = routingTable.getTableByName(switchName);
		int port = 0;
		
		if (!item) break; 

		
		for (int i = 0; i < item->subitems; i++)
		{
			
			if (string(item->dstInfo[i]).compare(toNode) == 0)
			{
				port = item->outport[i];
				break;
			}
		}
		
		if (port > 0)
		{
			count++; 
			
			struct switchNode * swNode = topologyTable.getSwitchByName(switchName);
			
			for (int j = 0; j < swNode->portCount; j++)
			{
				
				if (swNode->srcPort[j] == port)
				{
					switchName = swNode->dstName[j];
					break;
				}
			}
			
			if (string(switchName).compare(toNode) == 0) break;
		}
		else break; 
	} while (1);
	
	cout << "From " << fromNode << " to " << toNode << ": Hop Count = " << count << endl;
	
	return count;
}

int Calculation::calculate() 
{
	int minHop = 0xffff;
	int f, t;

	for (f = 0; f < topologyTable.getHostCount(); f++)
	{
		int x = 0;
		for (t = 0; t < topologyTable.getHostCount(); t++)
		{
			int hopCount = getHopCount(f, t);	
			x = max(x, hopCount);		
		}
		minHop = min(minHop, x);
	}

	return minHop;
}

int Calculation::getRoute(int fromId, int toId)
{
	struct hostNode * fromHost = topologyTable.getHostById(fromId);

	string fromNode = fromHost->name;

	string toNode = topologyTable.getSwitchById(toId)->name;
	
	cout << "From " << fromNode << " to " << toNode;

	string switchName = fromHost->dstName;

	do {
		struct routeItem * item = routingTable.getTableByName(switchName);
		int port = 0;

		if (!item) break; 

		
		for (int i = 0; i < item->subitems; i++)
		{	
			
			if (string(item->dstInfo[i]).compare(toNode) == 0)
			{
				
				port = item->outport[i];
				break;
			}
		}
		
		if (port > 0)
		{	
			struct switchNode * swNode = topologyTable.getSwitchByName(switchName);
			
			for (int j = 0; j < swNode->portCount; j++)
			{	
				if (swNode->srcPort[j] == port)
				{
					switchName = swNode->dstName[j];
					break;
				}
			}

			if (string(switchName).compare(toNode) == 0)
			{
				cout << " reachable" << endl;
				return 1;
			}
		}
		else break; 
	} while (1);

	cout << " unreachable" << endl;
	return 0;
}

int Calculation::balance() 
{
	int balance = 0xffff; 
	int f, t;
	
	for (f = 0; f < topologyTable.getHostCount(); f++)
	{
		int x = 0;
		int m = 0;
		for (t = 0; t < topologyTable.getSwitchCount(); t++)
		{
			x += getRoute(f, t);
			m = max(m, x);	
		}  
		balance = min(balance, m);
	}

	return balance+1;
}
