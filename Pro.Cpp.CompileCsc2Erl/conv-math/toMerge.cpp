#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

string funcName;
vector<string> funcArgs;
map<string,vector<string> > links;
vector<string> condition;

string generate_new_variable_name() {
  static int stmp=0;
  stringstream ss;
  ss<<stmp++;
  string ret;
  ss>>ret;
  return "Vars_"+ret;
}

bool is_operator(const string &data) {
  return data=="rem" || data=="div" || data=="and" || data=="or" || data=="+" || data=="-" || data=="*" || data=="/" || data=="==" || data=="<" || data==">" || data=="!=";
}

vector<string> simplify(vector<string> data) {
  for (int i=0;i<data.size();i++) {
    if (data[i]=="")
      continue;
    if (data[i]=="(" && !(i && islower(data[i-1][0]) && !is_operator(data[i-1]))) {
      int level=1, exist=0, j;
      for (j=i+1;j<data.size();j++) {
        if (data[j]=="(")
          level++;
        else if (data[j]==")")
          level--;
        if (!level)
          break;
        if (level==1 && is_operator(data[j])) {
          exist=1;
          break;
        }
      }
      if (!exist)
        data[i]=data[j]="";
    }
  }
  vector<string> final;
  for (int i=0;i<data.size();i++)
    if (data[i]!="")
      final.push_back(data[i]);
  return final;
}

void branch_begin() {
  static string lastFunc;
  if (lastFunc!=funcName) {
    cout<<"<<function>> "<<funcName<<"(";
    lastFunc=funcName;
    for (int i=0;i<funcArgs.size();i++) {
	    if (i)
	      cout<<", ";
    cout<<funcArgs[i];
    }
  cout<<")"<<endl;
  }
  for (int i=0;i<funcArgs.size();i++) {
    vector<string> target;
    target.push_back(funcArgs[i]);
    links[funcArgs[i]]=target;
  }
}

void branch_next(bool exec, const vector<string> &data) {
  if (exec) {
    vector<string> right;
    int minic=-1, count=0;
    for (int i=0;i<data.size();i++)
      if (data[i]=="=") {
        minic=i;
        right.push_back("(");
        for (int j=i+1;j<data.size();j++) {
          if (isupper(data[j][0])) {
            const vector<string> &target=links[data[j]];
            for (int k=0;k<target.size();k++)
              right.push_back(target[k]);
          } else
            right.push_back(data[j]);
        }
        right.push_back(")");
        break;
      } else if (isupper(data[i][0]))
        count++;
    if (~minic) {
      int current=0;
      for (int i=0;i<minic;i++)
        if (isupper(data[i][0])) {
          current++;
          if (count==1)
            links[data[i]]=right;
          else {
            links[data[i]].clear();
            links[data[i]].push_back("element");
            links[data[i]].push_back("(");
            stringstream ss;
            string word;
            ss<<current;
            ss>>word;
            links[data[i]].push_back(word);
            links[data[i]].push_back(",");
            for (int j=0;j<right.size();j++)
              links[data[i]].push_back(right[j]);
            links[data[i]].push_back(")");
          }
        }
    } else {
      vector<string> result;
      for (int j=0;j<data.size();j++) {
        if (isupper(data[j][0])) {
          const vector<string> &target=links[data[j]];
          for (int k=0;k<target.size();k++)
            result.push_back(target[k]);
        } else
          result.push_back(data[j]);
      }
      result=simplify(result);
      cout<<"  --> ";
      for (int i=0;i<result.size();i++)
        cout<<result[i]<<" ";
    }
  } else {
    if (condition.size())
      condition.push_back("and");
    condition.push_back("(");
    for (int i=0;i<data.size();i++) {
      if (isupper(data[i][0])) {
        const vector<string> &target=links[data[i]];
        for (int j=0;j<target.size();j++)
          condition.push_back(target[j]);
      } else
        condition.push_back(data[i]);
    }
    condition.push_back(")");
  }
}

void branch_end() {
  cout<<";   -------- When ";
  condition=simplify(condition);
  for (int i=0;i<condition.size();i++)
    cout<<condition[i]<<" ";
  cout<<endl;

  links.clear();
  condition.clear();
}

int main() {
  string line,word;
  int length;
  bool exist=getline(cin,line);
  while (exist) {
    stringstream ss;
    ss<<line;
    ss>>word>>funcName>>length;
    funcArgs.clear();
    for (int i=0;i<length;i++) {
      ss>>word;
      funcArgs.push_back(word);
    }
    getline(cin,line);
    do {
      branch_begin();
      while (exist && line!=",") {
        vector<string> data;
        stringstream buf;
        buf<<line;
        buf>>word;
        bool exec=(word=="[!]");
        while (buf>>word, buf)
          data.push_back(word);
        branch_next(exec,data);
        exist=getline(cin,line);
      }
      branch_end();
      exist=getline(cin,line);
    } while (exist && line.size() && line[0]!='>');
  }
}
