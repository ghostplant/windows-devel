#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>

#include "common.h"

using namespace std;

class BasicBlock {
public:
	int startLine, endLine;
	vector<vector<string> > subInstruction;
	map<string, vector<pair<int,int> > > phiFunction;
};

class Procedure {
public:
	vector<string> localVar;
	map<string,int> localArrLen;
	vector<string> formalArgs;
	string name;

	vector<element> live;
	vector<BasicBlock> block;
	multimap<int,int> flowgraph, rev_flowgraph;
	int entry, exit;
	
	vector<string> finalEntryArgs;

	set<string> globModify, globUse;
};

string module;

vector<string> globVar;
map<string,int> globArrLen;

vector<Procedure> procedure;
vector<vector<int> > callgraph;
map<string,int> funcToIndex;

template <class T>
void debugVect(const vector<T> &vect, string iso=" ") {
	for (int i=0;i<vect.size();i++)
		cerr<<vect[i]<<iso;
	cerr<<endl;
}

vector<string> readInput() {
	vector<string> words;
	string line, word;
	if (getline(cin, line)) {
		stringstream strs;
		strs<<line;
		while (strs>>word)
			words.push_back(word);
	}
	return words;
}

pair<string, int> getRealName(const string &word) {
	char c=word[word.size()-1];
	pair<string, int> ret;
	if (c<'0' || c>'9')
		ret.first=word, ret.second=-1;
	else {
		int at=word.find('['), size;
		stringstream ss;
		ss<<word.substr(at+2, word.length()-(at+2));
		ss>>size;
		ret.first=word.substr(0,at);
		ret.second=size;
	}
	return ret;
}

/******************************************************************/

void fetchGlobVar() {
	vector<string> head=readInput();
	if (!head.size())
		exit(0);
	for (int i=1;i<head.size();i++) {
		pair<string, int> real = getRealName(head[i]);
		globVar.push_back(real.first);
		if (~real.second)
			globArrLen[real.first]=real.second;
	}

	cerr<<"# "<<"-------- Global Variable --------"<<endl<<"# ";
	for (vector<string>::iterator i=globVar.begin();i!=globVar.end();i++) {
		cerr<<*i;
		map<string,int>::iterator it;
		if ((it=globArrLen.find(*i)) != globArrLen.end())
			cerr<<"(arr-len:"<<it->second<<")";
		cerr<<", ";
	}
	cerr<<endl;
}

bool fetchLocalVar(Procedure &oneproc) {
	static int stmp = 0;
	vector<string> words = readInput();
	if (!words.size())
		return false;
	
	int it = 1;
	while (words[it]!="|")
		oneproc.formalArgs.push_back(words[it++]);
	oneproc.name = words[++it];
	funcToIndex[oneproc.name]=stmp++;

	++it;
	while (++it < words.size()) {
		pair<string, int> real = getRealName(words[it]);
		oneproc.localVar.push_back(real.first);
		if (~real.second)
			oneproc.localArrLen[real.first]=real.second;
	}
	
	stringstream strs;
	strs<<it;
	strs>>words[0];

	cerr<<"# "<<"-------- Procedure Heading --------"<<endl;
	cerr<<"# ProcName: "<<oneproc.name<<endl;
	cerr<<"# FormalArgs: "; debugVect(oneproc.formalArgs, ", ");
	cerr<<"# LocalVar: ";
	for (vector<string>::iterator i=oneproc.localVar.begin();i!=oneproc.localVar.end();i++) {
		cerr<<*i;
		map<string,int>::iterator it;
		if ((it=oneproc.localArrLen.find(*i)) != oneproc.localArrLen.end())
			cerr<<"(arr-len:"<<it->second<<")";
		cerr<<", ";
	}
	cerr<<endl;
	return true;
}

void classifyInstruction(vector<vector<string> > &instruction) {
	vector<string> words;
	do {
		words=readInput();
		words[0]="F";
		 // 确定指令类型
		if (words.size()>3 && words[3]=="=")
			words[1]="RD";
		else if (words.size()>2 && (words[2]=="if" || words[2]=="ifnot" || words[2]=="goto"))
			words[1]="JP";
		else if (words.size()>2 && words[2]=="call")
			words[1]="CL";
		else
			words[1]="EL";
		instruction.push_back(words);
	} while (words[2]!="ret");

	// 确定首指令
	instruction[0][0]="T";
	for (int i=0;i<instruction.size();i++) {
		if (instruction[i][1]=="JP") {
			instruction[i+1][0]="T";
			stringstream strs;
			int j;
			if (instruction[i].size()>4)
				strs<<instruction[i][7];
			else
				strs<<instruction[i][3];
			strs>>j;
			instruction[j][0]="T";
		}
	}

	cerr<<"# "<<"-------- Classify Instruction --------"<<endl;
	for (int i=0;i<instruction.size();i++) {
		cerr<<"# "<<i<<": ";
		debugVect(instruction[i]);
	}
}

void divideBasicBlock(Procedure &oneproc, const vector<vector<string> > &instruction, map<int,int> &lineToBlock) {
	// 划分基本块
	cerr<<"# "<<"-------- Divide Basic Block --------"<<endl;
	oneproc.block.push_back(BasicBlock());
	for (int i=0;i<instruction.size();) {
		BasicBlock block;
		cerr<<"# "<<"block "<<oneproc.block.size()<<" {"<<endl;
		lineToBlock[i]=oneproc.block.size();
		block.startLine=i;
		do {
			block.subInstruction.push_back(instruction[i++]);
			cerr<<"#    * ";
			debugVect(*(--block.subInstruction.end()));
		} while (i<instruction.size() && instruction[i][0]!="T");
		block.endLine=i;
		const vector<string> &last=instruction[block.endLine-1];
		if (last[1]!="JP" && (last[1]!="EL" || last[2]!="ret")) {
			vector<string> build;
			build.push_back("T");
			build.push_back("JP");
			build.push_back("goto");
			stringstream ss;
			string word;
			ss<<block.endLine;
			ss>>word;
			build.push_back(word);
			block.subInstruction.push_back(build);
		}
		cerr<<"# "<<"} line: ("<<block.startLine<<","<<block.endLine<<");"<<endl;
		oneproc.block.push_back(block);
	}
	oneproc.block.push_back(BasicBlock());
	oneproc.entry=0, oneproc.exit=oneproc.block.size()-1;

}

void addEdgesToFg(Procedure &oneproc, int u,int v) {
	oneproc.flowgraph.insert(pair<int,int>(u,v));
	oneproc.rev_flowgraph.insert(pair<int,int>(v,u));
}

void fixAndBuildFlowGraph(Procedure &oneproc, const vector<vector<string> > &instruction, map<int,int> &lineToBlock) {
	// 更正跳转标号、构造流图、反向流图
	cerr<<"# "<<"-------- Fix Transfer Id --------"<<endl;
	addEdgesToFg(oneproc, 0, 1);

	for (int i=1;i+1<oneproc.block.size();i++) {
		vector<string> &words=oneproc.block[i].subInstruction[oneproc.block[i].subInstruction.size()-1];
		if (words[1]!="JP") {
			if (i+1<oneproc.block.size())
				addEdgesToFg(oneproc, i, i+1);
		} else if (words.size()<=4) {
			stringstream strs;
			int j;
			string word;
			strs<<words[3];
			strs>>j;
			j=lineToBlock[j];
			strs.clear();
			strs<<j;
			strs>>word;
			words[3]=word;
			addEdgesToFg(oneproc, i, j);

			cerr<<"# In B"<<i<<": ";
			debugVect(oneproc.block[i].subInstruction[oneproc.block[i].subInstruction.size()-1]);
		} else {
			if (i+1<oneproc.block.size())
				addEdgesToFg(oneproc, i, i+1);
			stringstream strs;
			int j;
			string word;
			strs<<words[7];
			strs>>j;
			j=lineToBlock[j];
			strs.clear();
			strs<<j;
			strs>>word;
			words[7]=word;
			addEdgesToFg(oneproc, i, j);

			cerr<<"# In B"<<i<<": ";
			debugVect(oneproc.block[i].subInstruction[oneproc.block[i].subInstruction.size()-1]);
		}
	}

	cerr<<"# "<<"-------- Flow Graph Edges --------"<<endl<<"# ";
	for (multimap<int,int>::iterator it=oneproc.flowgraph.begin();it!=oneproc.flowgraph.end();++it)
		cerr<<"B"<<it->first<<" -> "<<"B"<<it->second<<", ";
	cerr<<endl;
}

void updateCallGraph(vector<vector<string> > &instruction) {
	vector<int> destIndex;
	for (int i=0;i<instruction.size();i++)
		if (instruction[i][1]=="CL") {
			int hisIndex=funcToIndex[instruction[i][3]];
			destIndex.push_back(hisIndex);
		}
	callgraph.push_back(destIndex);
}

bool workOnNextProc() {
	Procedure oneproc;
	if (!fetchLocalVar(oneproc))
		return false;

	vector<vector<string> > instruction;
	classifyInstruction(instruction);
	
	map<int,int> lineToBlock;
	divideBasicBlock(oneproc, instruction, lineToBlock);
	
	fixAndBuildFlowGraph(oneproc, instruction, lineToBlock);
	
	updateCallGraph(instruction);

	procedure.push_back(oneproc);
	return true;
}

bool existVar(const vector<string> &vect, const string &str) {
	for (int i=0;i<vect.size();i++)
		if (vect[i]==str)
			return true;
	return false;
}

void calcInitProcGlobModify() {
	for (int i=0;i<procedure.size();i++) {
		for (int j=0;j<procedure[i].block.size();j++) {
			vector<vector<string> > &instr = procedure[i].block[j].subInstruction;
			for (int k=0;k<instr.size();k++) {
				if (instr[k][1]=="RD") {
					string &def=instr[k][2];
					if (existVar(globVar, def) && !existVar(procedure[i].formalArgs,def) && !existVar(procedure[i].localVar,def)) {
						procedure[i].globModify.insert(def);
					}
				}
			}
		}
	}
}

void callGraphLiveOut(const vector<pair<int,int> > &edges) {
	calcInitProcGlobModify();

	vector<set<int> > procGlobModifyInt;
	vector<string> intToGlobVar;
	map<string,int> grobVarToInt;
	for (int i=0;i<globVar.size();i++) {
		grobVarToInt[globVar[i]]=i;
		intToGlobVar.push_back(globVar[i]);
	}
	for (int i=0;i<procedure.size();i++) {
		set<int> modInt;
		set<string> &globModify = procedure[i].globModify;
		for (set<string>::iterator it=globModify.begin();it!=globModify.end();++it)
			modInt.insert(grobVarToInt[*it]);
		procGlobModifyInt.push_back(modInt);
	}
	vector<set<int> > ret = dfa_callout(callgraph.size(), globVar.size(), edges, procGlobModifyInt);
	for (int i=0;i<ret.size();i++) {
		for (set<int>::iterator it=ret[i].begin();it!=ret[i].end();++it)
			procedure[i].globModify.insert(intToGlobVar[*it]);
	}

	cerr<<"# "<<"-------- DFA Proc Glob Live Out --------"<<endl;
	for (int i=0;i<procedure.size();i++) {
		set<string> &ref=procedure[i].globModify;
		cerr<<"# "<<procedure[i].name<<".glob-def :";
		for (set<string>::iterator it=ref.begin();it!=ref.end();++it)
			cerr<<" "<<*it;
		cerr<<endl;
	}
}

pair<set<string>, set<string> > calcProcBlockGlobUseDef(const vector<vector<string> > &instr, Procedure &proc, map<string, int> &procToInt) {
	pair<set<string>, set<string> > ret;
	for (int i=instr.size()-1;i>=0;i--) {
		int start = 2;
		if (instr[i][1]=="RD") {
			const string &ref=instr[i][2];
			if (ref[0]=='_' && existVar(globVar,ref) && !existVar(proc.localVar,ref) && !existVar(proc.formalArgs,ref)) {
				ret.second.insert(ref);
				ret.first.erase(ref);
			}
			start=4;
		} else if (instr[i][1]=="CL") {
			int id=procToInt[instr[i][3]];
			set<string> &defs = procedure[id].globModify;
			for (set<string>::iterator it=defs.begin();it!=defs.end();++it) {
				ret.second.insert(*it);
				ret.first.erase(*it);
			}
			set<string> &uses = procedure[id].globUse;
			for (set<string>::iterator it=uses.begin();it!=uses.end();++it) {
				ret.first.insert(*it);
			}
			start=5;
		}
		for (int j=start;j<instr[i].size();j++) {
			const string &ref=instr[i][j];
			if (ref[0]=='_' && existVar(globVar,ref) && !existVar(proc.localVar,ref) && !existVar(proc.formalArgs,ref)) {
				ret.first.insert(ref);
			}
		}
	}
	return ret;
}

void callGraphLiveIn(const vector<pair<int,int> > &edges, map<string, int> &procToInt) {
	vector<vector<pair<int,int> > > procEdges;

	for (int i=0;i<procedure.size();i++) {
		vector<pair<int,int> > oneProcEdges;
		for (multimap<int,int>::iterator it=procedure[i].flowgraph.begin();it!=procedure[i].flowgraph.end();++it)
			oneProcEdges.push_back(pair<int,int>(it->first, it->second));
		procEdges.push_back(oneProcEdges);
	}
	map<string, int> globToInt;
	for (int i=0;i<globVar.size();i++) {
		globToInt[globVar[i]]=i;
	}

	bool flag = true;
	while (flag) {
		flag = false;
		vector<set<int> > procGlobLiveIn;
		for (int i=0;i<procedure.size();i++) {
			vector<pair<set<int>, set<int> > > globUseDef;
			cerr<<"# --- Proc "<<procedure[i].name<<" Current Use-Def Set ---"<<endl;
			for (int j=0;j<procedure[i].block.size();j++) {
				pair<set<string>, set<string> > ret = calcProcBlockGlobUseDef(procedure[i].block[j].subInstruction, procedure[i],procToInt);
				pair<set<int>, set<int> > toInt;
				cerr<<"# B["<<j<<"].semi-use = {";
				for (set<string>::iterator it=ret.first.begin();it!=ret.first.end();++it) {
					toInt.first.insert(globToInt[*it]);
					cerr<<*it<<" ";
				}
				cerr<<"}; B["<<j<<"].semi-def = {";
				for (set<string>::iterator it=ret.second.begin();it!=ret.second.end();++it) {
					toInt.second.insert(globToInt[*it]);
					cerr<<*it<<" ";
				}
				cerr<<"}."<<endl;
				globUseDef.push_back(toInt);
			}
			set<int> liveInit;
			for (set<string>::iterator it=procedure[i].globModify.begin();it!=procedure[i].globModify.end();++it)
				liveInit.insert(globToInt[*it]);
			procedure[i].live = dfa_livevar(procedure[i].entry,procedure[i].exit,procedure[i].block.size(),globVar.size(),procedure[i].flowgraph, globUseDef, &liveInit);
			set<int> &ref=procedure[i].live[procedure[i].entry].pipe[1];

			cerr<<"# --- DFA Proc "<<procedure[i].name<<" Current Glob Live In ---"<<endl<<"# ";
			set<string> &uses = procedure[i].globUse;
			for (set<int>::iterator it=ref.begin();it!=ref.end();++it) {
				string var=globVar[*it];
				if (uses.find(var)==uses.end()) {
					uses.insert(var);
					flag=true;
				}
				cerr<<globVar[*it]<<", ";
			}
			cerr<<endl;

			procGlobLiveIn.push_back(ref);
		}
	}


	cerr<<"# -------- DFA Proc Glob Live In (Final) --------"<<endl;
	for (int i=0;i<procedure.size();i++) {
		set<string> &ref=procedure[i].globUse;
		cerr<<"# "<<procedure[i].name<<" :";
		for (set<string>::iterator it=ref.begin();it!=ref.end();++it)
			cerr<<" "<<*it;
		cerr<<endl;
	}
}

void fixProcCallSite(map<string, int> &procToInt) {
	for (int i=0;i<procedure.size();i++) {
		for (int j=0;j<procedure[i].block.size();j++) {
			BasicBlock &block=procedure[i].block[j];
			for (int k=0;k<block.subInstruction.size();k++) {
				vector<string> &instr=block.subInstruction[k];
				if (instr[1]=="CL") {
					vector<string> newInstr;
					newInstr.push_back(instr[0]);
					newInstr.push_back("RD");
					
					int procInt = procToInt[instr[3]];
					set<string> &ref=procedure[procInt].globModify;
					for (set<string>::iterator it=ref.begin();it!=ref.end();++it)
						newInstr.push_back(*it);
					newInstr.push_back("=");
					for (int l=2;l<instr.size();l++)
						newInstr.push_back(instr[l]);
					newInstr.push_back("|");

					set<string> &use=procedure[procInt].globUse;
					for (set<string>::iterator it=use.begin();it!=use.end();++it)
						newInstr.push_back(*it);
					instr=newInstr;
				}
			}
		}
	}

	for (int i=1;i+1<procedure.size();i++) {
		cerr<<"# --- Proc "<<procedure[i].name<<" Blocks after Fixing Call Site ---"<<endl;
		for (int j=0;j<procedure[i].block.size();j++) {
			BasicBlock &block=procedure[i].block[j];
			cerr<<"# block "<<j<<" {"<<endl;
			for (int k=0;k<block.subInstruction.size();k++) {
				cerr<<"#    * ";
				debugVect(block.subInstruction[k]);
			}
			cerr<<"# };"<<endl;
		}
	}
}

void workOnCallGraph() {
	vector<pair<int,int> > edges;

	cerr<<"# "<<"-------- Call Graph --------"<<endl<<"# ";
	for(int i=0;i<callgraph.size();i++)
		for (int j=0;j<callgraph[i].size();j++) {
			edges.push_back(pair<int,int>(i, callgraph[i][j]));
			cerr<<procedure[i].name<<"->"<<procedure[callgraph[i][j]].name<<", ";
		}
	cerr<<endl;

	callGraphLiveOut(edges);
	callGraphLiveIn(edges, funcToIndex);
	
	fixProcCallSite(funcToIndex);
}

void buildDomTree(Procedure &proc, vector<int> &father) {
	vector<pair<int,int> > edges;
	for (multimap<int,int>::iterator it=proc.flowgraph.begin();it!=proc.flowgraph.end();++it)
		edges.push_back(*it);
	father = dfa_dom(proc.entry, proc.exit, proc.block.size(), edges);

	cerr<<"# "<<"-------- Dominate Relation --------"<<endl<<"# ";
	for (int i=1;i<father.size();i++)
		cerr<<father[i]<<" idom "<<i<<"; ";
	cerr<<endl;
}

void calcDFSet(Procedure &oneproc, const vector<int> &father, vector<vector<int> > &dfset) {
	int n=oneproc.block.size();
	dfset.clear();
	for (int i=0;i<n;i++)
		dfset.push_back(vector<int>());
	for (int i=0;i<n;i++) {
		multimap<int,int>::iterator ia, it, ib, end=oneproc.rev_flowgraph.end();
		it = ia = oneproc.rev_flowgraph.lower_bound(i);
		ib = oneproc.rev_flowgraph.upper_bound(i);
		if (it!=end && ++it!=end && ++it==ib) {
			while (ia!=ib) {
				int route=ia->second;
				while (route != father[i]) {
					dfset[route].push_back(i);
					route = father[route];
				}
				++ia;
			}
		}
	}

	cerr<<"# "<<"-------- Dominator Frontier --------"<<endl<<"# ";
	for (int i=0;i<n;i++) {
		cerr<<"# DF("<<i<<") =  ";
		debugVect(dfset[i], ", ");
	}
}

map<string, vector<pair<int,int> > > getDefinition(const BasicBlock &block) {
	map<string, vector<pair<int,int> > > ret = block.phiFunction;
	for (int i=0;i<block.subInstruction.size();i++) {
		const vector<string> &instr = block.subInstruction[i];
		if (instr[1]=="RD") {
			for (int j=2;instr[j]!="=";j++)
				if (instr[j][0]=='_')
					ret[instr[j]]=vector<pair<int,int> >();
		}
	}
	return ret;
}

void addPhiFunction(Procedure &proc, const vector<vector<int> > &dfset) {
	bool flag = true;
	while (flag) {
		flag = false;
		for (int i=0;i<dfset.size();i++)
			for (int j=0;j<dfset[i].size();j++) {
				int addDest = dfset[i][j];
				map<string, vector<pair<int,int> > > def = getDefinition(proc.block[i]);
				map<string, vector<pair<int,int> > > &phi = proc.block[addDest].phiFunction;
				int prevSize = phi.size();
				for (map<string, vector<pair<int,int> > >::iterator it=def.begin();it!=def.end();++it)
					phi[it->first]=it->second;
				if (phi.size()>prevSize)
					flag = true;
			}
	}

	cerr<<"# "<<"-------- Insert Phi Function --------"<<endl;
	for (int i=0;i<proc.block.size();i++) {
		cerr<<"# B"<<i<<" += ";
		map<string, vector<pair<int,int> > > &phi = proc.block[i].phiFunction;
		for (map<string, vector<pair<int,int> > >::iterator it=phi.begin();it!=phi.end();++it)
			cerr<<it->first<<", ";
		cerr<<endl;
	}
}

#define TOP(st) ((st).empty()?0:(st).top())

void dfsSSA(int u, Procedure &proc, map<string, pair<stack<int>, int> > &varStack, set<int> &visited, int from) {
	BasicBlock &block=proc.block[u];
	vector<string> selfGenerate;
	for (map<string,vector<pair<int,int> > >::iterator it=block.phiFunction.begin();it!=block.phiFunction.end();++it) {
		pair<stack<int>, int> &prop = varStack[it->first];
		int orgId = TOP(prop.first);
		if (!it->second.size()) {
			int newId = ++prop.second;
			it->second.push_back(pair<int,int>(-1,newId));
			prop.first.push(newId);
			selfGenerate.push_back(it->first);
		}
		it->second.push_back(pair<int,int>(from,orgId));
	}

	if (visited.find(u)==visited.end()) {
		visited.insert(u);

		stringstream ss;
		string word;
	
		for (int i=0;i<block.subInstruction.size();i++) {
			vector<string> &words = block.subInstruction[i];
			bool isDefinition = (words[1]=="RD");
			int start = 1;
			if (isDefinition) {
				while (words[start]!="=")
					start++;
			}
			for (int j=start+1;j<words.size();j++) {
				if (words[j][0]=='_') {
					pair<stack<int>, int> &prop = varStack[words[j]];
					int orgId = TOP(prop.first);
					ss.clear();
					ss<<orgId;
					ss>>word;
					words[j] = "Id"+ word + words[j];
				}
			}
			if (isDefinition) {
				for (int j=2;j<start;j++)
					if (words[j][0]=='_') {
						pair<stack<int>, int> &prop = varStack[words[j]];
						int newId = ++prop.second;
						prop.first.push(newId);
						selfGenerate.push_back(words[j]);

						ss.clear();
						ss<<newId;
						ss>>word;
						words[j] = "Id"+ word + words[j];
					}
			}
		}

		multimap<int,int> &fg = proc.flowgraph;
		multimap<int,int>::iterator ia=fg.lower_bound(u), ib=fg.upper_bound(u);
		while (ia!=ib)
			dfsSSA((ia++)->second, proc, varStack, visited, u);
	}
	
	for (int i=0;i<selfGenerate.size();i++)
		varStack[selfGenerate[i]].first.pop();
}

void addIndexToVars(Procedure &proc) {
	map<string, pair<stack<int>, int> > varStack;
	set<int> visited;
	dfsSSA(0, proc, varStack, visited, -1);
	
	cerr<<"# "<<"-------- Instruction after Modify Subscript --------"<<endl;
	for (int i=1;i+1<proc.block.size();i++) {
		cerr<<"# block "<<i<<" {"<<endl;
		map<string, vector<pair<int,int> > > &phi = proc.block[i].phiFunction;
		for (map<string, vector<pair<int,int> > >::iterator it = phi.begin(); it!=phi.end(); ++it) {
			vector<pair<int,int> > &index=it->second;
			cerr<<"#    ! Id";
			cerr<<index[0].second<<it->first<<" = phi(";
			cerr<<index[1].first<<":Id"<<index[1].second<<it->first;
			for (int j=2;j<index.size();j++)
				cerr<<", "<<index[j].first<<":Id"<<index[j].second<<it->first;
			cerr<<")"<<endl;
		}
		vector<vector<string> > &instr = proc.block[i].subInstruction;
		for (int j=0;j<instr.size();j++) {
			cerr<<"#    * ";
			debugVect(instr[j]);
		}
		cerr<<"# }"<<endl;
	}
}

void convertToSSA() {
	for (int i=0;i<procedure.size();i++) {
		Procedure &oneproc=procedure[i];
		vector<int> father;
		buildDomTree(oneproc, father);
	
		vector<vector<int> > dfset;
		calcDFSet(oneproc, father, dfset);
	
		addPhiFunction(oneproc, dfset);
	
		addIndexToVars(oneproc);
	}
}

string spitRealPart(string str) {
	int at=str.find('_');
	return str.substr(at, str.size()-at);
}

pair<set<int>, set<int> > getBlockAllLiveIn(vector<vector<string> > &instr, map<string,int> &anyVarToInt) {
	pair<set<int>, set<int> > ret;
	for (int i=instr.size()-1;i>=0;i--) {
		int start = 2;
		if (instr[i][1]=="RD") {
			while (instr[i][start]!="=") {
				if (instr[i][start][0]=='I') {
					int realId=anyVarToInt[spitRealPart(instr[i][start])];
					ret.first.erase(realId);
					ret.second.insert(realId);
				}
				start++;
			}
			++start;
		}
		for (int j=start;j<instr[i].size();j++) {
			if (instr[i][j][0]=='I') {
				int realId=anyVarToInt[spitRealPart(instr[i][j])];
				ret.first.insert(realId);
			}
		}
	}
	return ret;
}

string getLastUseNameOf(string var, BasicBlock &block) {
	vector<vector<string> > &instr=block.subInstruction;
	map<string, vector<pair<int,int> > > &phi=block.phiFunction;
	for (int i=instr.size()-1;i>=0;i--) {
		for (int j=2;j<instr[i].size();j++) {
			if (instr[i][j][0]=='I') {
				if (spitRealPart(instr[i][j])==var)
					return instr[i][j];
			}
		}
	}
	for (map<string, vector<pair<int,int> > >::iterator it=phi.begin();it!=phi.end();++it)
		if (it->first==var) {
			stringstream ss;
			int index=it->second[0].second;
			string word;
			ss<<index;
			ss>>word;
			return "Id"+word+var;
		}
	return var;
}

vector<string> getHeadParamWithIndex(int destId, vector<string> &anyIntToVar, vector<element> &allLiveVar, Procedure &proc) {
	BasicBlock &block=proc.block[destId];
	vector<string> ret;
	set<int> args = allLiveVar[destId].pipe[0];	

	if (args.size()) {
		for (set<int>::iterator it=args.begin();it!=args.end();++it) {
			map<string, vector<pair<int,int> > > &phi=block.phiFunction;
			map<string, vector<pair<int,int> > >::iterator is;
			string withOutIndex=anyIntToVar[*it];
			if ((is=phi.find(withOutIndex))!=phi.end()) {
				vector<pair<int,int> > &ref = is->second;
				int useIndex=useIndex=ref[0].second;
				stringstream ss;
				string word;
				ss<<useIndex;
				ss>>word;
				ret.push_back("Id"+word+withOutIndex);
			} else {
				bool flag=true;
				vector<vector<string> > &instr=proc.block[destId].subInstruction;
				for (int i=0;flag && i<instr.size();i++) {
					for (int j=instr[i].size()-1;j>=2;j--)
						if (instr[i][j][0]=='I' && spitRealPart(instr[i][j])==withOutIndex) {
							ret.push_back(instr[i][j]);
							flag=false;
							break;
						}
				}
				if (flag)
					ret.push_back(withOutIndex);
			}
		}
	}
	return ret;
}

vector<string> getActualParamWithIndex(int destId, int fromId, vector<string> &anyIntToVar, vector<element> &allLiveVar, Procedure &proc) {
	BasicBlock &block=proc.block[destId];
	vector<string> ret;
	set<int> args = allLiveVar[destId].pipe[0];	

	if (args.size()) {
		for (set<int>::iterator it=args.begin();it!=args.end();++it) {
			map<string, vector<pair<int,int> > > &phi=block.phiFunction;
			map<string, vector<pair<int,int> > >::iterator is;
			string withOutIndex=anyIntToVar[*it];
			ret.push_back(getLastUseNameOf(withOutIndex,proc.block[fromId]));
		}
	}
	return ret;
}

string ccValue(string var, Procedure &proc) {
	if (proc.localArrLen.find(var) != proc.localArrLen.end() || existVar(proc.finalEntryArgs, var))
		return var;
	return "0";
}

void dumpErlangSource(Procedure &proc, vector<string> &anyIntToVar, vector<element> &allLiveVar) {
	queue<pair<int,int> > toGenerate;
	set<int> visited;
	toGenerate.push(pair<int,int>(0,-1));
	while (!toGenerate.empty()) {
		pair<int,int> prop=toGenerate.front();
		int blockId = prop.first, fromId = prop.second;
		toGenerate.pop();
		
		if (blockId==proc.entry) {
			cout<<proc.name<<"(";
			vector<string> buffer;
			for (vector<string>::iterator it=proc.formalArgs.begin();it!=proc.formalArgs.end();++it)
				buffer.push_back(*it);
			for (set<string>::iterator it=proc.globUse.begin();it!=proc.globUse.end();++it)
				buffer.push_back(*it);
			proc.finalEntryArgs = buffer;
			if (buffer.size()) {
				cout<<buffer[0];
				for (int i=1;i<buffer.size();i++)
					cout<<", "<<buffer[i];
			}
			cout<<") ->"<<endl;
			for (map<string,int>::iterator it=proc.localArrLen.begin();it!=proc.localArrLen.end();++it)
				cout<<"  "<<it->first<<" = erlang:make_tuple("<<it->second<<", 0),"<<endl;
			cout<<"  "<<proc.name<<"_b1(";
			vector<string> vect=getActualParamWithIndex(1,0,anyIntToVar,allLiveVar, proc);
			if (vect.size()) {
				cout<<ccValue(vect[0],proc);
				for (int i=1;i<vect.size();i++)
					cout<<", "<<ccValue(vect[i],proc);
			}
			cout<<")."<<endl;
			cout<<endl;
		} else if (blockId!=proc.exit) {
			cout<<proc.name<<"_b"<<blockId<<"(";
			vector<string> vect=getHeadParamWithIndex(blockId,anyIntToVar,allLiveVar, proc);
			if (vect.size()) {
				cout<<vect[0];
				for (int i=1;i<vect.size();i++)
					cout<<", "<<vect[i];
			}
			cout<<") ->"<<endl;
			vector<vector<string> > &instr=proc.block[blockId].subInstruction;
			for (int i=0;i<instr.size();i++) {
				if (instr[i][1]=="RD") {
					for (int j=2;j<instr[i].size();j++)
						if (instr[i][j]=="=") {
							if (instr[i][j+1]=="call") {
								cout<<"  ";
								if (j>2) {
									cout<<"{"<<instr[i][2];
									for (int k=3;k<j;k++)
										cout<<", "<<instr[i][k];
									cout<<"} = ";
								}
								cout<<instr[i][j+2]<<"(";
								int k;
								vector<string> allActArgs;
								for (k=j+4;instr[i][k]!="|";k++)
									allActArgs.push_back(instr[i][k]);
								for (++k;k<instr[i].size();k++)
									allActArgs.push_back(instr[i][k]);
								if (allActArgs.size()) {
									cout<<allActArgs[0];
									for (int p=1;p<allActArgs.size();p++)
										cout<<", "<<allActArgs[p];
								}
								cout<<")";
							} else if (instr[i][j+1]=="read") {
								cout<<"  "<<instr[i][2]<<" = list_to_integer(lists:delete(10,io:get_line([])))";
							} else if (instr[i][j+1]=="=[]") {
								cout<<"  "<<instr[i][2]<<" = element(1 + "<<instr[i][6]<<", "<<instr[i][5]<<")";
							} else if (instr[i][j+1]=="[]=") {
								cout<<"  "<<instr[i][2]<<" = setelement(1 + "<<instr[i][6]<<", "<<instr[i][5]<<", "<<instr[i][7]<<")";
							} else {
								cout<<"  "<<instr[i][2]<<" =";
								int sz=instr[i].size();
								for (int p=4;p<sz;p++)
									cout<<" "<<instr[i][p];
							}
							break;
						}
				} else if (instr[i][1]=="JP") {
					string True="    true -> ", False="    false -> ";
					if (instr[i][2]=="if" || instr[i][2]=="ifnot") {
						if (instr[i][2]=="ifnot")
							swap(True, False);
						stringstream ss;
						int targetId;
						ss<<instr[i][7];
						ss>>targetId;
						vector<string> param;
						param = getActualParamWithIndex(targetId, blockId, anyIntToVar, allLiveVar, proc);
						cout<<"  case ("<<instr[i][3]<<" "<<instr[i][4]<<" "<<instr[i][5]<<") of"<<endl;
						cout<<True;
						cout<<proc.name<<"_b"<<targetId<<"(";
						if (param.size()) {
							cout<<param[0];
							for (int p=1;p<param.size();p++)
								cout<<", "<<param[p];
						}
						cout<<");"<<endl;

						cout<<False;
						targetId=blockId+1;
						param = getActualParamWithIndex(targetId, blockId, anyIntToVar, allLiveVar, proc);
						cout<<proc.name<<"_b"<<targetId<<"(";
						if (param.size()) {
							cout<<param[0];
							for (int p=1;p<param.size();p++)
								cout<<", "<<param[p];
						}
						cout<<")"<<endl;
						cout<<"  end";
					} else {
						assert(instr[i][2]=="goto");
						stringstream ss;
						int targetId;
						ss<<instr[i][3];
						ss>>targetId;
						vector<string> param = getActualParamWithIndex(targetId, blockId, anyIntToVar, allLiveVar, proc);
						cout<<"  "<<proc.name<<"_b"<<targetId<<"(";
						if (param.size()) {
							cout<<param[0];
							for (int p=1;p<param.size();p++)
								cout<<", "<<param[p];
						}
						cout<<")";
					}
				} else {
					assert(instr[i][1]=="EL");
					if (instr[i][2]=="write") {
						cout<<"  "<<"io:put_chars(integer_to_list("<<instr[i][3]<<"))";
					} else if (instr[i][2]=="writeln") {
						cout<<"  "<<"io:put_chars([10])";
					} else if (instr[i][2]=="ret") {
						if (proc.globModify.size()) {
							cout<<"  "<<"{";
							vector<string> ret;
							for (set<string>::iterator it=proc.globModify.begin();it!=proc.globModify.end();++it)
								ret.push_back(getLastUseNameOf(*it, proc.block[blockId]));
							cout<<ret[0];
							for (int p=1;p<ret.size();p++)
								cout<<", "<<ret[p];
							cout<<"}";
						} else
							cout<<"  ok";
					} else
						assert(false);
				}
				cout<<(i+1==instr.size()?".":",")<<endl;
			}
			cout<<endl;
		}

		multimap<int,int>::iterator it, ia=proc.flowgraph.lower_bound(blockId), ib=proc.flowgraph.upper_bound(blockId);
		for (it=ia;it!=ib;++it)
			if (visited.find(it->second)==visited.end()) {
				visited.insert(it->second);
				toGenerate.push(pair<int,int>(it->second, blockId));
			}
	}
}

void processOneFunction(Procedure &proc) {
	vector<string> anyIntToVar;
	map<string,int> anyVarToInt;
	int stmp=0;
	for (int i=0;i<proc.block.size();i++) {
		vector<vector<string> > &instr=proc.block[i].subInstruction;
		for (int j=0;j<instr.size();j++) {
			for (int k=2;k<instr[j].size();k++) {
				string &ref=instr[j][k];
				if (ref[0]=='I') {
					string real = spitRealPart(ref);
					if (anyVarToInt.find(real)==anyVarToInt.end()) {
						anyIntToVar.push_back(real);
						anyVarToInt[real]=stmp++;
					}
				}
			}
		}
	}
	vector<pair<set<int>, set<int> > > allUseDef;
	for (int i=0;i<proc.block.size();i++) {
		pair<set<int>, set<int> > ret = getBlockAllLiveIn(proc.block[i].subInstruction, anyVarToInt);
		allUseDef.push_back(ret);
	}
	set<int> globLiveOut;
	for (set<string>::iterator it=proc.globModify.begin();it!=proc.globModify.end();++it)
		globLiveOut.insert(anyVarToInt[*it]);
	vector<element> allLiveVar = dfa_livevar(proc.entry, proc	.exit, proc.block.size(), anyVarToInt.size(), proc.flowgraph, allUseDef, &globLiveOut);
	
	dumpErlangSource(proc, anyIntToVar, allLiveVar);
}

void convertToErlang() {
	string redirectFile = module + ".erl";
	freopen(redirectFile.c_str(), "w", stdout);

	cout<<"-module("<<module<<")."<<endl;

	vector<string> functionForm;
	int index = -1;
	map<string, int>::iterator it=funcToIndex.find("main");
	if (it!=funcToIndex.end())
		index = it->second;
	if (~index)
		functionForm.push_back("entry_main/0");

	for (int i=0;i<procedure.size();i++) {
		int totArgs=procedure[i].formalArgs.size()+procedure[i].globUse.size();
		stringstream ss;
		string word;
		ss<<totArgs;
		ss>>word;
		functionForm.push_back(procedure[i].name+"/"+word);
	}

	cout<<"-export([";
	cout<<functionForm[0];
	for (int i=1;i<functionForm.size();i++)
		cout<<", "<<functionForm[i];
	cout<<"])."<<endl;

	for (int i=0;i<procedure.size();i++) {
		cout<<endl;

		processOneFunction(procedure[i]);
	}
	if (~index) {
		vector<string> &args=procedure[index].finalEntryArgs;

		cout<<"entry_main() ->"<<endl;
		for (int i=0;i<args.size();i++) {
			if (globArrLen.find(args[i])!=globArrLen.end())
				cout<<"  "<<args[i]<<" = erlang:make_tuple("<<globArrLen.find(args[i])->second<<", 0),"<<endl;
		}
		cout<<"  main(";
		for (int i=0;i<args.size();i++) {
			if (i)
				cout<<", ";
			if (globArrLen.find(args[i])==globArrLen.end())
				cout<<"0";
			else
				cout<<args[i];
		}
		cout<<"),"<<endl;
		cout<<"  ok."<<endl<<endl;
	}
}

int main(int argc, char *argv[]) {
	if (argc!=2) {
		cerr<<"Please set the specific module name."<<endl;
		return 0;
	}
	module=argv[1];

	fetchGlobVar();

	while (workOnNextProc());
	
	workOnCallGraph();
	
	convertToSSA();
	
	convertToErlang();
}

/*

global _u[]100 _ans
entry _x | f | _i
(0) : ifnot _x > 1 goto 4
(1) : t1 =  - 1
(2) : _ans = t1
(3) : goto 16
(4) : _ans = 1
(5) : _i = 2
(6) : t6 =  _i * _i
(7) : if t6 > _x goto 16
(8) : t8 =  _x % _i
(9) : ifnot t8 == 0 goto 13
(10) : t10 =  - 1
(11) : _ans = t10
(12) : _i = _x
(13) : t13 =  _i + 1
(14) : _i = t13
(15) : goto 6
(16) : ret
entry | main | _n
(0) : t0 = read
(1) : _n = t0
(2) : call f | _n
(3) : write _ans
(4) : ret

*/
