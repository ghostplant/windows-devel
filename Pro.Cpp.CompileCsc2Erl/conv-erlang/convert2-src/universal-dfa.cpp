#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>

#include "common.h"

using namespace std;

typedef set<int> (*f_block)(int);

vector<set<int> > graph, rev_graph;

vector<set<int> > e_use, e_kill;
vector<element> anticipated, available,postponable, used;
vector<set<int> > earliest, latest;

vector<element> dominator;

vector<set<int> > defProcGlobModify;
vector<element> callout;

vector<pair<set<int>, set<int> > > curProcGlobUseDef;
vector<element> livevar;
//vector<set<int> > curProcLiveIn;
//vector<element> procLivevar;

int set_size;

void init_graph(int n) {
	graph.clear();
	rev_graph.clear();
	for (int i=0;i<n;i++) {
		graph.push_back(set<int>());
		rev_graph.push_back(set<int>());
	}
}

void add_edge(int u,int v) {
	graph[u].insert(v);
	rev_graph[v].insert(u);
}

/* ============================================================================ */

set<int> init_set(bool full) {
	set<int> ans;
	if (full) {
		for (int i=0;i<set_size;i++)
			ans.insert(i);
	}
	return ans;
}

set<int> intersect(const set<int> &a, const set<int> &b) {
	set<int> ans;
	for (set<int>::iterator it=a.begin();it!=a.end();++it) {
		if (b.find(*it)!=b.end())
			ans.insert(*it);
	}
	return ans;
}

set<int> unionset(const set<int> &a, const set<int> &b) {
	set<int> ans;
	for (set<int>::iterator it=a.begin();it!=a.end();++it)
		ans.insert(*it);
	for (set<int>::iterator it=b.begin();it!=b.end();++it)
		ans.insert(*it);
	return ans;
}

set<int> subtract(const set<int> &a, const set<int> &b) {
	set<int> ans=a;
	for (set<int>::iterator it=b.begin();it!=b.end();++it)
		ans.erase(*it);
	return ans;
}

/* ============================================================================ */

void calc_graph_common(int realEntry, int realExit, int n, bool isReverse, bool isIntersect, vector<element> &answer, f_block f_trans, set<int> *entryInit = NULL) {
	answer.clear();
	for (int i=0;i<n;i++)
		answer.push_back(element());

	vector<set<int> > &revGraph=isReverse?graph:rev_graph;
	int entry=realEntry, exit=realExit, in=0, out=1;
	if (isReverse)
		swap(entry, exit), swap(in, out);
	for (int i=0;i<n;i++)
		answer[i].pipe[out]=init_set(isIntersect);
	if (entry>=0) {
		if (!entryInit)
			answer[entry].pipe[out].clear();
		else
			answer[entry].pipe[out]=*entryInit;
	}

	bool flag=true;
	while (flag) {
		flag=false;
		for (int i=0;i<n;i++) {
			if (i==entry)
				continue;
			answer[i].pipe[in]=init_set(isIntersect);
			for (set<int>::iterator it=revGraph[i].begin();it!=revGraph[i].end();++it)
				if (isIntersect)
					answer[i].pipe[in]=intersect(answer[i].pipe[in], answer[*it].pipe[out]);
				else
					answer[i].pipe[in]=unionset(answer[i].pipe[in], answer[*it].pipe[out]);
			set<int> newValue=f_trans(i);
			if (newValue != answer[i].pipe[out]) {
				flag=true;
				answer[i].pipe[out]=newValue;
			}
		}
	}
	/*
	if (isReverse)
		swap(entry, exit), swap(in, out);
	for (int i=0;i<n;i++) {
		if (i==entry || i==exit)
			continue;
		cout<<prefix<<"[B"<<i<<"].in={ ";
		for (set<int>::iterator it=answer[i].pipe[in].begin();it!=answer[i].pipe[in].end();++it)
			cout<<expr[*it]<<", ";
		cout<<"}."<<endl;
		cout<<prefix<<"[B"<<i<<"].out={ ";
		for (set<int>::iterator it=answer[i].pipe[out].begin();it!=answer[i].pipe[out].end();it++)
			cout<<expr[*it]<<", ";
		cout<<"}."<<endl;
	}*/
}


void calc_line_common(int entry, int exit, int n, vector<set<int> > &answer, f_block f_trans, const char *prefix, const vector<string> &expr) {
	answer.clear();
	for (int i=0;i<n;i++)
		answer.push_back(set<int>());

	for (int i=0;i<n;i++) {
		if (i==entry || i==exit)
			continue;
		answer[i]=f_trans(i);
		cout<<prefix<<"[B"<<i<<"]={ ";
		for (set<int>::iterator it=answer[i].begin();it!=answer[i].end();it++)
			cout<<expr[*it]<<", ";
		cout<<"}."<<endl;
	}
}

/* ============================================================================ */

set<int> f_dominator(int i) {
	set<int> ans=dominator[i].pipe[0];
	ans.insert(i);
	return ans;
}

set<int> f_callout(int i) {
	return unionset(callout[i].pipe[1], defProcGlobModify[i]);
}

set<int> f_livevar(int i) {
	return unionset(subtract(livevar[i].pipe[1], curProcGlobUseDef[i].second), curProcGlobUseDef[i].first);
}

/*set<int> f_procLivevar(int i) {
	return unionset(procLivevar[i].pipe[1], curProcLiveIn[i]);
}*/

set<int> f_anticipated(int i) {
	set<int> ans=subtract(anticipated[i].pipe[1], e_kill[i]);
	return unionset(ans, e_use[i]);
}

set<int> f_available(int i) {
	set<int> ans=unionset(available[i].pipe[0], anticipated[i].pipe[0]);
	return subtract(ans, e_kill[i]);
}

set<int> f_earliest(int i) {
	return subtract(anticipated[i].pipe[0], available[i].pipe[0]);
}

set<int> f_postponable(int i) {
	set<int> ans=unionset(postponable[i].pipe[0], earliest[i]);
	return subtract(ans, e_use[i]);
}

set<int> f_latest(int i) {
	set<int> a=unionset(earliest[i],postponable[i].pipe[0]), b=init_set(true);
	for (set<int>::iterator it=graph[i].begin();it!=graph[i].end();++it) {
		b=intersect(b, unionset(earliest[*it],postponable[*it].pipe[0]));
	}
	b=unionset(e_use[i], subtract(init_set(true), b));
	return intersect(a,b);
}

set<int> f_used(int i) {
	set<int> ans=unionset(e_use[i], used[i].pipe[1]);
	return subtract(ans, latest[i]);
}

void check_optimize(int entry, int exit, int n, const vector<string> &expr) {
	for (int i=0;i<n;i++) {
		if (i==entry || i==exit)
			continue;
		set<int> compute=intersect(latest[i], used[i].pipe[1]);
		if (compute.size()) {
			cout<<"B"<<i<<" : compute -";
			for (set<int>::iterator it=compute.begin();it!=compute.end();++it)
				cout<<" "<<expr[*it]<<",";
			cout<<endl;
		}
		
		set<int> use=intersect(e_use[i],unionset(subtract(init_set(true), latest[i]), used[i].pipe[1]));
		if (use.size()) {
			cout<<"B"<<i<<" : use -";
			for (set<int>::iterator it=use.begin();it!=use.end();++it)
				cout<<" "<<expr[*it]<<",";
			cout<<endl;
		}
	}
}

/* ============================================================================ */

void dfa_pre(int entry, int exit, int node, const vector<string> &expr, const vector<pair<int,int> > &edges, const vector<pair<int,int> > &usekill) {
	init_graph(node);
	set_size=expr.size();
	for (int i=0;i<edges.size();i++)
		add_edge(edges[i].first,edges[i].second);

	e_use.clear();
	e_kill.clear();
	for (int i=0;i<node;i++) {
		e_use.push_back(set<int>());
		e_kill.push_back(set<int>());
	}
	for (int i=0;i<usekill.size();i++) {
		if (usekill[i].first >= 0)
			e_use[usekill[i].first].insert(usekill[i].second);
		else
			e_kill[~usekill[i].first].insert(usekill[i].second);
	}

	calc_graph_common(entry, exit, node, true, true, anticipated, f_anticipated);
	calc_graph_common(entry, exit, node, false, true, available, f_available);
	calc_line_common(entry, exit, node, earliest, f_earliest, "earliest", expr);
	calc_graph_common(entry, exit, node, false, true, postponable, f_postponable);
	calc_line_common(entry, exit, node, latest, f_latest, "latest", expr);
	calc_graph_common(entry, exit, node, true, false, used, f_used);
	check_optimize(entry, exit, node, expr);
}

vector<int> dfa_dom(int entry, int exit, int node, const vector<pair<int,int> > &edges) {
	init_graph(node);
	set_size=node;
	for (int i=0;i<edges.size();i++)
		add_edge(edges[i].first,edges[i].second);
	calc_graph_common(entry, exit, node, false, true, dominator, f_dominator);

	map<set<int>, int> domToNode;
	for (int i=1;i+1<node;i++)
		domToNode[dominator[i].pipe[1]]=i;

	vector<int> ret;
	ret.push_back(-1);
	ret.push_back(0);
	for (int i=2;i+1<node;i++)
		ret.push_back(domToNode[dominator[i].pipe[0]]);
	ret.push_back(node-2);
	return ret;
}

vector<set<int> > dfa_callout(int node, int totGlobVar, const vector<pair<int,int> > &edges, const vector<set<int> > &procGlobModify) {
	vector<set<int> > ret;

	defProcGlobModify = procGlobModify;
	init_graph(node);
	set_size=totGlobVar;
	for (int i=0;i<edges.size();i++)
		add_edge(edges[i].first,edges[i].second);
	calc_graph_common(-1, -1, node, true, false, callout, f_callout);
	for (int i=0;i<node;i++)
		ret.push_back(callout[i].pipe[0]);
	return ret;
}

vector<element> dfa_livevar(int entry, int exit, int node, int totGlobVar, multimap<int,int> &edges, const vector<pair<set<int>, set<int> > > &procGlobUseDef, set<int> *liveInit = NULL) {
	curProcGlobUseDef = procGlobUseDef;
	init_graph(node);
	set_size=totGlobVar;
	for (multimap<int,int>::iterator it=edges.begin();it!=edges.end();it++)
		add_edge(it->first, it->second);
	calc_graph_common(entry, exit, node, true, false, livevar, f_livevar, liveInit);
	return livevar;
}

/*vector<set<int> > dfa_proc_livevar(int node, int totGlobVar, const vector<pair<int,int> > &edges, const vector<set<int> > &procGlobLiveIn) {
	vector<set<int> > ret;

	curProcLiveIn=procGlobLiveIn;
	init_graph(node);
	set_size=totGlobVar;
	for (int i=0;i<edges.size();i++)
		add_edge(edges[i].first,edges[i].second);
	//system("echo 'Wait for a second ..'");
	//system("sleep 1");
	calc_graph_common(-1, -1, node, true, false, procLivevar, f_procLivevar);
	for (int i=0;i<node;i++)
		ret.push_back(procLivevar[i].pipe[0]);
	return ret;
}*/

int test() {
	int nodes=8;
	vector<pair<int,int> > edges;
	edges.push_back(pair<int,int>(0,1));
	edges.push_back(pair<int,int>(1,2));
	edges.push_back(pair<int,int>(2,3));
	edges.push_back(pair<int,int>(3,4));
	edges.push_back(pair<int,int>(3,5));
	edges.push_back(pair<int,int>(4,3));
	edges.push_back(pair<int,int>(5,2));
	edges.push_back(pair<int,int>(5,6));
	edges.push_back(pair<int,int>(5,7));

	dfa_dom(0,nodes-1,nodes,edges);

	vector<string> expr;
	expr.push_back("a+b");
	expr.push_back("c-a");
	expr.push_back("b*d");
	
	vector<pair<int,int> > usekill;
	usekill.push_back(pair<int,int>(~1,0));
	usekill.push_back(pair<int,int>(~1,1));
	usekill.push_back(pair<int,int>(~1,2));
	usekill.push_back(pair<int,int>(2,0));
	usekill.push_back(pair<int,int>(~2,1));
	usekill.push_back(pair<int,int>(~2,2));
	usekill.push_back(pair<int,int>(~3,2));
	usekill.push_back(pair<int,int>(4,0));
	usekill.push_back(pair<int,int>(~4,2));
	usekill.push_back(pair<int,int>(5,0));
	usekill.push_back(pair<int,int>(5,1));
	usekill.push_back(pair<int,int>(~5,2));
	usekill.push_back(pair<int,int>(~6,0));
	usekill.push_back(pair<int,int>(~6,1));
	usekill.push_back(pair<int,int>(~6,2));

	dfa_pre(0,nodes-1,nodes,expr, edges, usekill);
}
