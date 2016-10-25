#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#define FUNC_PREFIX "funs_"

using namespace std;

void show_arr(const vector<string> &args) {
  int len=args.size()-1;
  for (int i=0;i<len;i++)
    if (args[i]!="\n" && args[i].size() && args[i][0]!=' ')
      cout<<args[i]<<" ";
    else
      cout<<args[i];
  cout<<args[len]<<endl;
}

vector<string> get_one_command(vector<string> &modVar);
vector<string> go_into_object(vector<string> &modVar);

static string backup;

string get_next_word() {
  if (backup!="") {
    string ans=backup;
    backup="";
    return ans;
  }
  static char word[1024];
  static int buf=getchar(), len;
  while (buf=='\n' || buf==' ' || buf=='\r')
    buf=getchar();
  if (buf==-1)
    return "";
  if (isalpha(buf) || buf=='_') {
    *word=buf, len=1;
    while (buf=getchar(), isalpha(buf) || buf=='_' || isdigit(buf))
        word[len++]=buf;
    word[len]=0;
  } else if (isdigit(buf)) {
    *word=buf, len=1;
    while (buf=getchar(), isdigit(buf))
        word[len++]=buf;
    word[len]=0;
  } else if (buf=='=') {
     buf=getchar();
     if (buf=='=')
      word[0]=word[1]='=',word[len=2]=0, buf=getchar();
     else
      word[0]='=',word[len=1]=0;
  } else if (buf=='&') {
     buf=getchar();
     if (buf=='&')
      word[0]=word[1]='&',word[len=2]=0, buf=getchar();
     else
      word[0]='&',word[len=1]=0;
  } else if (buf=='<') {
     buf=getchar();
     if (buf=='=')
      word[0]='<',word[1]='=',word[len=2]=0, buf=getchar();
     else
      word[0]='<',word[len=1]=0;
  } else if (buf=='>') {
     buf=getchar();
     if (buf=='=')
      word[0]='>',word[1]='=',word[len=2]=0, buf=getchar();
     else
      word[0]='>',word[len=1]=0;
  } else
    *word=buf, word[len=1]=0, buf=getchar();
  return word;  
}

void restore_word(string word) {
  assert(backup=="");
  backup=word;
}

vector<string> get_words_until(string end) {
  string word;
  vector<string> args;
  if (end!=")" && end!=",") {
    while (word=get_next_word(), word.size() && word!=end)
      args.push_back(word);
  } else {
    int rev=1;
    while (rev) {
      word=get_next_word();
      assert(word.size());
      if (word=="(")
        rev++;
      else if (word==")")
        rev--;
      if (rev) {
        args.push_back(word);
      }
    }
  }
  assert(word.size());
  return args;
}

void append_words_after(vector<string> &old, const vector<string> &data) {
  for (int i=0;i<data.size();i++)
    old.push_back(data[i]);
}

vector<string> sort_and_make_args(const vector<string> &data) {
  set<string> ret;
  vector<string> final;
  for (int i=0;i<data.size();i++)
    ret.insert(data[i]);
  for (set<string>::iterator it=ret.begin();it!=ret.end();++it) {
    final.push_back(*it);
    final.push_back(",");
  }
  if (final.size())
    final.erase(--final.end());
  return final;
}

string generate_new_function_name() {
  static int stmp=0;
  stringstream ss;
  ss<<stmp++;
  string ret;
  ss>>ret;
  return FUNC_PREFIX+ret;
}

vector<vector<string> > innerFunctions;

void append_possible_var_to(vector<string>& modVar, const vector<string> data) {
  for (int i=0;i<data.size();i++)
    if (isupper(data[i][0]))
      modVar.push_back(data[i]);
}

vector<string> get_one_command(vector<string> &modVar) {
  modVar.clear();
  vector<string> ret, childs;
  string word=get_next_word();
  if (word=="if") {
    assert(get_next_word()=="(");
    vector<string> conds=get_words_until(")");
    append_possible_var_to(modVar, conds);
    ret.push_back("if"); //!!!
    ret.push_back("("); //!!!
    append_words_after(ret, conds); //!!!
    ret.push_back(")"); //!!!
    ret.push_back("->"); //!!!
    word=get_next_word();
    if (word=="{") {
      vector<string> innerModVar;
      childs=go_into_object(innerModVar);
      append_words_after(modVar,innerModVar);
    } else {
      restore_word(word);
      vector<string> innerModVar;
      childs=get_one_command(innerModVar);
      append_words_after(modVar,innerModVar);
    }
    append_words_after(ret,childs); //!!!
    ret.push_back("ok"); //!!!
    ret.push_back(";"); //!!!
    
    word=get_next_word();
    while (word=="elseif") {
      assert(get_next_word()=="(");
      vector<string> conds=get_words_until(")");
      append_possible_var_to(modVar, conds);
      ret.push_back("("); //!!!
      append_words_after(ret, conds); //!!!
      ret.push_back(")"); //!!!
      ret.push_back("->"); //!!!
      
      word=get_next_word();
      if (word=="{") {
        vector<string> innerModVar;
        childs=go_into_object(innerModVar);
        append_words_after(modVar,innerModVar);
      } else {
        restore_word(word);
        vector<string> innerModVar;
        childs=get_one_command(innerModVar);
        append_words_after(modVar,innerModVar);
      }
      append_words_after(ret,childs); //!!!
      
      ret.push_back("ok"); //!!!
      ret.push_back(";"); //!!!
      word=get_next_word();
    }

    ret.push_back("("); //!!!
    ret.push_back("true"); //!!!
    ret.push_back(")"); //!!!
    ret.push_back("->"); //!!!
    if (word=="else") {
      word=get_next_word();
      if (word=="{") {
        vector<string> innerModVar;
        childs=go_into_object(innerModVar);
        append_words_after(modVar,innerModVar);
      } else {
        restore_word(word);
        vector<string> innerModVar;
        childs=get_one_command(innerModVar);
        append_words_after(modVar,innerModVar);
      }
      append_words_after(ret,childs); //!!!
    } else
      restore_word(word);
    ret.push_back("ok"); //!!!
    ret.push_back("end"); //!!!
    ret.push_back(","); //!!!
  } else if (word=="while") {
    assert(get_next_word()=="(");
    vector<string> conds=get_words_until(")");
    append_possible_var_to(modVar, conds);
    //show_arr(conds,"[tst] ");
    word=get_next_word();
    vector<string> innerModVar;
    if (word=="{") {
      childs=go_into_object(innerModVar);
      append_words_after(modVar,innerModVar);
    } else {
      restore_word(word);
      childs=get_one_command(innerModVar);
      append_words_after(modVar,innerModVar);
    }
    vector<string> liteArgs=sort_and_make_args(innerModVar);
    ret.push_back("{"); //!!!
    append_words_after(ret,liteArgs);
    ret.push_back("}"); //!!!
    ret.push_back("="); //!!!
    string func=generate_new_function_name();
    vector<string> function;
    function.push_back(func);
    function.push_back("(");
    append_words_after(function,liteArgs);
    function.push_back(")");
    function.push_back("->");
    
    function.push_back("if"); //!!!
    function.push_back("("); //!!!
    append_words_after(function, conds); //!!!
    function.push_back(")"); //!!!
    function.push_back("->"); //!!!
    append_words_after(function, childs);
    function.push_back(func); //!!!
    function.push_back("("); //!!!
    append_words_after(function,liteArgs); //!!!
    function.push_back(")"); //!!!
    function.push_back(";"); //!!!
    function.push_back("("); //!!!
    function.push_back("true"); //!!!
    function.push_back(")"); //!!!
    function.push_back("->"); //!!!
    function.push_back("{"); //!!!
    append_words_after(function,liteArgs); //!!!
    function.push_back("}"); //!!!
    function.push_back("end"); //!!!
    function.push_back("."); //!!!
    innerFunctions.push_back(function);

    ret.push_back(func); //!!!
    ret.push_back("("); //!!!
    append_words_after(ret,liteArgs);
    ret.push_back(")"); //!!!
    ret.push_back(","); //!!!
  } else {
    restore_word(word);
    ret=get_words_until(";"); //!!!
    append_possible_var_to(modVar,ret);
    ret.push_back(","); //!!!
  }
  return ret;
}

vector<string> go_into_object(vector<string> &modVar) {
  modVar.clear();
  vector<string> ret;
  string word=get_next_word();
  assert(word.size());
  while (word!="}") {
    restore_word(word);
    vector<string> innerModVar;
    append_words_after(ret,get_one_command(innerModVar)); //!!!
    append_words_after(modVar,innerModVar);
    word=get_next_word();
    assert(word.size());
  }
  return ret;
}

string make_blank(int levelTab) {
  string ret="";
  while (levelTab--)
    ret+=" ";
  return ret;
}

vector<string> correct(const vector<string> &data) {
  vector<string> ret, final;
  bool endsym=false;
  for (int i=0;i<data.size();i++) {
    if (!endsym)
      endsym = (data[i]=="," || data[i]==";");
    else {
      if (data[i]==";" || data[i]=="end" || data[i]==".")
        ret.erase(--ret.end());
      endsym = false;
    }
    ret.push_back(data[i]);
  }
  return ret;
}

vector<string> format(const vector<string> &ret) {
  vector<string> final;
  int levelTab=0;
  for (int i=0;i<ret.size();i++) {
    if (ret[i]=="end") {
      levelTab-=4;
      final.push_back("\n");
      final.push_back(make_blank(levelTab));
    }
    final.push_back(ret[i]);
    if (ret[i]=="if") {
      levelTab+=4;
      final.push_back("\n");
      final.push_back(make_blank(levelTab));
    }
    if (ret[i]==";" || ret[i]==".") {
      final.push_back("\n");
      final.push_back(make_blank(levelTab));
    }
  }
  return final;
}

int main() {
  string word;
  while (word=get_next_word(), word.size()) {
    assert(word=="auto");
    string name=get_next_word();
    assert(get_next_word()=="(");
    vector<string> args=get_words_until(")"), modVar;
    assert(get_next_word()=="{");
    // func_info: name, args
    cout<<"-export(["<<name<<"/"<<args.size()<<"]).\n"<<endl;
    vector<string> final, middle=go_into_object(modVar);
    final.push_back(name); //!!!
    final.push_back("("); //!!!
    for (int i=0;i<args.size()-1;i++) {
      final.push_back(args[i]); //!!!
      final.push_back(","); //!!!
    }
    if (args.size())
      final.push_back(args[args.size()-1]); //!!!
    final.push_back(")"); //!!!
    final.push_back("->"); //!!!
    append_words_after(final,middle); //!!!
    final.push_back("Result"); //!!!
    final.push_back("."); //!!!

    innerFunctions.push_back(final);
    for (int i=0;i<innerFunctions.size();i++) {
      innerFunctions[i]=correct(innerFunctions[i]);
      for (int j=0;j<innerFunctions[i].size();j++)
        cout<<innerFunctions[i][j]<<" ";
      cout<<endl;
    }
  }
}
