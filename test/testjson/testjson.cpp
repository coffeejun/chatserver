#include "json.h"
using json = nlohmann::json;
#include <iostream>
#include <string>
using namespace std;
string func1()
{
    json js;
    js["from"] = 2;
    js["to"] = "kkk";
    // cout << js << endl; // 提供了输出运算符的重载函数
    string sendBuf = js.dump(); // dump函数就是用于输出的
    // cout << sendBuf.c_str() << endl; // c_str()获取字符串的首地址
    return sendBuf;
}

string func2()
{
    json js;
    js["id"] = {1, 2, 3, 4, 5};             // 添加数组
    js["name"] = "zhang san";               // 添加kv
    js["msg"]["zhang san"] = "hello world"; // 添加对象
    js["msg"]["liu shuo"] = "hello china";
    // 上面的等同下面的
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
    string sendBuf = js.dump();
    return sendBuf;
}

string func3()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    string sendBuf = js.dump();
    return sendBuf;
}

int main()
{
    string recvBuf = func1();
    json jsbuf = json::parse(recvBuf);
    cout << jsbuf["from"] << endl;
    string recvBuf2 = func2();
    json jsbuf2 = json::parse(recvBuf2);
    auto id = jsbuf2["id"];
    cout << id[0] << endl;
    return 0;
}