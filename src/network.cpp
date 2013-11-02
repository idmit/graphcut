//
//  network.cpp
//  graphcut
//
//  Created by Ivan Dmitrievsky on 25.10.13.
//  Copyright (c) 2013 Ivan Dmitrievsky. All rights reserved.
//

#include <queue>
#include <stack>
#include <iostream>
#include "network.h"

double Network::edmondskarp(Network &minimumCut)
{
    double maxFlow = 0, capacity = 0, minCapacity = 0;
    int i = 0, j = 0;
    Network residualNetwork = *this, flowNetwork(_nodes, source, sink);
    std::vector<int> path;
    std::vector<int>::iterator it;
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            flowNetwork._arcs[i][j] = 0;
        }
    }
    
    while (!(path = residualNetwork.shortestAugmentingPath()).empty())
    {
        minCapacity = residualNetwork._arcs[*path.begin()][*(path.begin() + 1)];
        for (it = path.begin() + 1; it != path.end() - 1; it++)
        {
            if (residualNetwork._arcs[*it][*(it + 1)] < minCapacity)
            {
                minCapacity = residualNetwork._arcs[*it][*(it + 1)];
            }
        }
        for (it = path.begin(); it != path.end() - 1; it++)
        {
            flowNetwork._arcs[*it][*(it + 1)] +=  minCapacity;
            flowNetwork._arcs[*(it + 1)][*it]  = -1 * flowNetwork._arcs[*it][*(it + 1)];
        }
        for (i = 0; i < _nodes; i++)
        {
            for (j = 0; j < _nodes; j++)
            {
                capacity = _arcs[i][j] - flowNetwork._arcs[i][j];
                residualNetwork._arcs[i][j] = capacity > 0 ? capacity : 0;
            }
        }
    }
    
    for (i = 0; i < _nodes; i++)
    {
        maxFlow += flowNetwork._arcs[source][i];
    }

    residualNetwork.minimumCut(minimumCut);
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            if (_arcs[i][j] == 0)
            {
                minimumCut._arcs[i][j] = 0;
            }
        }
    }
    
    return maxFlow;
}

Network::Network(int __nodes, int _source, int _sink):Graph(__nodes)
{
    source = _source;
    sink = _sink;
}

std::vector<int> Network::shortestAugmentingPath(void)
{
    std::queue<int> toVisit;
    std::vector<int> visitedNodes(_nodes, 0), ancestors(_nodes, 0);
    std::vector<int> reversedPath, path;
    int currentNode = 0,
        neighbour;
    int i = 0;
    
    toVisit.push(source);
    
    visitedNodes[source] = 1;
    ancestors[source] = -1;
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.front();
        toVisit.pop();
        
        for (i = 0; i < _nodes; i++)
        {
            if (_arcs[currentNode][i] <= 0)
            {
                continue;
            }
            neighbour = i;
            if (!visitedNodes[neighbour])
            {
                visitedNodes[neighbour] = 1;
                ancestors[neighbour] = currentNode;
                
                toVisit.push(neighbour);
                if (neighbour == sink)
                {
                    goto found;
                }
            }
        }
    }
    
    if (toVisit.empty())
    {
        return reversedPath;
    }
    
found:
    currentNode = sink;
    while (currentNode != -1)
    {
        reversedPath.push_back(currentNode);
        currentNode = ancestors[currentNode];
    }
    std::for_each(reversedPath.rbegin(), reversedPath.rend(), [&path](int val) { path.push_back(val); });

    return path;
}

void Network::obduct(Graph &graph, int src, int snk)
{
    int i = 0, j = 0;
    
    for (i = 0; i < graph.nodes(); i++)
    {
        for (j = 0; j < graph.nodes(); j++)
        {
            _arcs[i][j] = graph.arcs(i, j);
        }
    }
    
    for (i = 0; i < graph.nodes(); i++)
    {
        _arcs[i][src] = 0;
        _arcs[snk][i] = 0;
    }

    source = src;
    sink = snk;
    
    print();
}

void Network::minimumCut(Network &minCutEdges)
{
    std::vector<int> visitedNodes(_nodes, 0);
    std::stack<int> toVisit;
    int currentNode = 0,
    neighbour;
    int i = 0, j = 0;
    
    visitedNodes[0] = 1;
    
    toVisit.push(0);
    
    while (!toVisit.empty())
    {
        currentNode = toVisit.top();
        toVisit.pop();
        
        for (int i = 0; i < _nodes; i++)
        {
            if (_arcs[currentNode][i] <= 0)
            {
                continue;
            }
            neighbour = i;
            if (!visitedNodes[neighbour])
            {
                visitedNodes[neighbour] = 1;
                toVisit.push(neighbour);
            }
        }
    }
    
    for (i = 0; i < _nodes; i++)
    {
        for (j = 0; j < _nodes; j++)
        {
            if (visitedNodes[i] && !visitedNodes[j])
            {
                minCutEdges._arcs[i][j] = 1;
            }
        }
    }
}