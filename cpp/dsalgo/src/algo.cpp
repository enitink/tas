#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
 
int parser()
{
    unordered_map<string, vector<int>> u;
	string word; 
	int i = 0;
    while(cin >> word) {
		u[word].push_back(i);
		i += (word.size() + 1);
    }

	for( const auto& n : u ) {
		cout << "Key:[" << n.first << "] Value:["; 
		for( const auto& x : n.second)
			cout << x << " ";
		cout << "]" << endl;
	}
    return 0;
}

int main()
{
	parser(); 
    return 0;
}
