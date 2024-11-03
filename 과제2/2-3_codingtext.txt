#include <string>
#include <vector>

using namespace std;

int solution(vector<string> order) {
    int answer = 0;
for (int i = 0; i < order.size(); i++)
{
	auto _americano = order[i].find("americano");
	auto _cafelatte = order[i].find("cafelatte");
	if (_americano != string::npos) {
		answer += 4500;
	}
	else if (_cafelatte != string::npos) {
		answer += 5000;
	}
	else
		answer += 4500;
}
return answer;
}