#ifndef COMMON_H
#define COMMON_H

#include <set>
#include <vector>

using namespace std;

struct element {
	set<int> pipe[2];
};

extern vector<int> dfa_dom(int entry, int exit, int node, const vector<pair<int,int> > &edges);
extern vector<set<int> > dfa_callout(int node, int totGlobVar, const vector<pair<int,int> > &edges, const vector<set<int> > &procGlobModify);
extern vector<element> dfa_livevar(int entry, int exit, int node, int totGlobVar, multimap<int,int> &edges, const vector<pair<set<int>, set<int> > > &procGlobUseDef, set<int> *liveInit);
//extern vector<set<int> > dfa_proc_livevar(int node, int totGlobVar, const vector<pair<int,int> > &edges, const vector<set<int> > &procGlobLiveIn);

#endif
