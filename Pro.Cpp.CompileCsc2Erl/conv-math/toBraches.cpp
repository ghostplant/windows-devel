#include <iostream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

void show_arr(const vector<string> &data) {
  for (int i=0;i<data.size();i++)
    cout<<data[i]<<" ";
  cout<<endl;
}

vector<string> oneFunction;
vector<vector<string> > commandTree;

void parse_next_command(int index) {
  string word=oneFunction[index++];
  vector<string> oneCommand;
  if (word!="if") {
    oneCommand.push_back("[!]");
    oneCommand.push_back(word);
    int level=(word=="("), levelBig=(word=="{");
    while (index<oneFunction.size()) {
      word=oneFunction[index++];
      if (word=="(")
        level++;
      else if (word==")")
        level--;
      else if (word=="{")
        levelBig++;
      else if (word=="}")
        levelBig--;
      else if (!level && !levelBig && (word=="," || word==";" || word=="end" || word=="."))
        break;
      oneCommand.push_back(word);
    }
    if (oneCommand.size()!=2 || oneCommand[1]!="ok")
      commandTree.push_back(oneCommand);
    //cout<<"+ ", show_arr(oneCommand);
    while (true) {
      if (word=="end")
        word=oneFunction[index++];
      if (word==",") {
        parse_next_command(index);
        break;
      } else if (word==".") {
        for (int i=0;i<commandTree.size();i++)
          show_arr(commandTree[i]);
        cout<<","<<endl;
        break;
      } else if (word==";") {
        level=1;
        while (level) {
          word=oneFunction[index++];
          if (word=="if")
            level++;
          else if (word=="end")
            level--;
        }
        word=oneFunction[index++];
      }
    }
    if (oneCommand.size()!=2 || oneCommand[1]!="ok")
      commandTree.erase(--commandTree.end());
    //cout<<"- ", show_arr(oneCommand);
  } else {
    vector<string> oldCondition, nextCondition;
    do {
      assert(oneFunction[index++]=="(");

      nextCondition.clear();
      if (oldCondition.size()) {
        nextCondition.push_back("(");
        nextCondition.push_back("(");
        for (int i=0;i<oldCondition.size();i++)
          nextCondition.push_back(oldCondition[i]);
        nextCondition.push_back(")");
        nextCondition.push_back("==");
        nextCondition.push_back("false");
        nextCondition.push_back(")");
        nextCondition.push_back("and");
      }
      vector<string> thisCondition;
      int level=1;
      while (level) {
        string next=oneFunction[index++];
        if (next=="(")
          level++;
        else if (next==")") {
          --level;
          if (!level)
            break;
        }
        thisCondition.push_back(next);
      }
      if (thisCondition.size()==1 && thisCondition[0]=="true")
        nextCondition.erase(--nextCondition.end());
      else {
        nextCondition.push_back("(");
        for (int i=0;i<thisCondition.size();i++)
          nextCondition.push_back(thisCondition[i]);
        nextCondition.push_back(")");
      }
      oldCondition=nextCondition;
      oneCommand.clear();
      oneCommand.push_back("[?]");
      for (int i=0;i<nextCondition.size();i++)
        oneCommand.push_back(oldCondition[i]);
      commandTree.push_back(oneCommand);
      //cout<<"+ ", show_arr(oneCommand);
      assert(oneFunction[index++]=="->");
      parse_next_command(index);
      commandTree.erase(--commandTree.end());
      //cout<<"- ", show_arr(oneCommand);

      level=1;
      while (level) {
        word=oneFunction[index++];
        if (word=="if")
          level++;
        else if (word=="end")
          level--;
        if (!level || level==1 && word==";")
          break;
      }
    } while (word!="end");
  }
}

int main() {
  string word;
  while (cin>>word) {
    if (word[0]=='-')
      continue;
    string name=word;
    vector<string> args;
    cin>>word;
    assert(word=="(");
    cin>>word;
    while (word!=")") {
      args.push_back(word);
      cin>>word;
      if (word==",")
        cin>>word;
    }
    cin>>word;
    assert(word=="->");
    oneFunction.clear();
    do {
      cin>>word;
      oneFunction.push_back(word);
    } while (word!=".");
    cout<<"> "<<name<<" "<<args.size();
    for (int i=0;i<args.size();i++)
      cout<<" "<<args[i];
    cout<<endl;
    parse_next_command(0);
  }
}

