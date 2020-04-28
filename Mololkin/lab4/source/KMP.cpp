#include <iostream>
#include <vector>

void prefixFunction(const std::string& str, std::vector<int>& result) {
    int n = str.length();
    for (int i = 1; i < n; ++i){
        int j = result[i - 1];
        while(j > 0 && str[i] != str[j])
            j = result[j-1];
        if(str[i] == str[j]) ++j;
        result[i] = j;
    }
}

void KMP(const std::string& pattern, const std::string& text) {
    std::vector<int> prefixFuncRes(pattern.size());
    prefixFunction(pattern, prefixFuncRes);
    int textIndex = 0;
    int patternIndex = 0;
    bool isFirst = true;
    bool isAnyEntry = false;
    while(textIndex != text.length()){
        if(pattern[patternIndex] == text[textIndex]) {
            textIndex++;
            patternIndex++;
            if(patternIndex == pattern.size()) {
                isAnyEntry = true;
                if(isFirst) {
                    isFirst = false;
                    std::cout << textIndex - pattern.length();
                }
                else std::cout << "," << textIndex - pattern.length();
            }
        }
        else {
            if(patternIndex == 0){
                textIndex++;
            }
            else {
                patternIndex = prefixFuncRes[patternIndex - 1];
            }
        }
    }
    if(!isAnyEntry) std::cout << -1;
}

int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;
    std::vector<int> result3;
    KMP(pattern, text);
    return 0;
}