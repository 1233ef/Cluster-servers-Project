#include"json.hpp"
using json = nlohmann::json;
#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;
//json反序列化实例1
string fun(){
    json js;
    js["msg_type"]=2;
    js["from"]="txs";
    js["to"]="syc";
    js["msg"]="thank you";
    //cout<<js;
    string sendbuf=js.dump();
    return sendbuf;
}
//json反序列化实例2
string fun2(){
    json js;
    js["id"]={1,2,3,4,5};//添加数组类型
    js["name"]="txs";//字符串类型
    js["msg"]["txs"]="hello,world";//mag键值里的txs键值说了hello,world
    js["msg"]["syc"]="I am syc";
    //上面两句等于同下面一句
    js["msg"]={{"txs","I am txs"},{"syc","I am syc"}};
    return js.dump();
}
//json反序列化实例3
string fun3(){
    json js;
    //序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    js["list"]=vec;

    //序列化一个map容器
    map<int,string> m;
    m.insert({1,"黄山"});
    m.insert({2,"嵩山"});
    m.insert({3,"华山"});
    js["path"]=m;

    string sendbuf=js.dump();//将json的对象序列化成json字符串
    return sendbuf;
}
int main(){
    // string recvbuf=fun();//接收序列化的字符串
    // json js=json::parse(recvbuf);//使用json中的pares方法去传参字符串进行反序列化
    // cout<<js["msg_type"]<<endl;
    // cout<<js["from"]<<endl;
    // cout<<js["to"]<<endl;
    // cout<<js["msg"]<<endl;
    //2
    //"txs"
    //"syc"
    //"thank you"

    // string recvbuf=fun2();//接收序列化的字符串
    // json js=json::parse(recvbuf);//使用json中的pares方法去传参字符串进行反序列化
    
    // cout<<js["id"]<<endl;
    // auto arr=js["id"];
    // cout<<arr[2]<<endl;

    // cout<<js["name"]<<endl;
    
    // auto msgjs=js["msg"];
    // cout<<msgjs["txs"]<<endl;
    // cout<<msgjs["syc"]<<endl;

   string recvbuf=fun3();
   json js=json::parse(recvbuf);
   vector<int> vec=js["list"];
   for(int &it :vec){
        cout<<it<<" ";
   }cout<<endl;

   map<int,string> m=js["path"];
   for(auto &it : m){
    cout<<it.first<<" "<<it.second<<endl;
   }
    return 0;
}