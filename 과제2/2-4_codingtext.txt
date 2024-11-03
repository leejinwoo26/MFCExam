#include <string>
#include <vector>
#include <cstring>
using namespace std;

int solution(int a, int b) 
{
    string aa = to_string(a) + to_string(b);
    string bb = to_string(b) + to_string(a);

    if (strcmp(aa.c_str(), bb.c_str())>0){
        return stoi(aa);
    }
    else if (strcmp(aa.c_str(), bb.c_str()) < 0){
        return stoi(bb);
    }
}