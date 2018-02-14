

#ifndef strtokenizer_h
#define strtokenizer_h
#include <string>
#include <vector>

using namespace std;

class strtokenizer {
protected:
    vector<string> tokens;
    int idx;
    
public:
    strtokenizer(string str, string seperators = " ");
    
    void parse(string str, string seperators);
    
    int count_tokens();
    string next_token();
    void start_scan();
    
    string token(int i);
};


#endif /* strtokenizer_h */
