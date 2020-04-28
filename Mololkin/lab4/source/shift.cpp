#include <iostream>
#include <vector>

void prefixFunction(std::string& str, std::vector<int>& result) {
    int n = str.length();
    for (int i = 1; i < n; ++i){
        int j = result[i - 1];
        while(j > 0 && str[i] != str[j])
            j = result[j-1];
        if(str[i] == str[j]) ++j;
        result[i] = j;
    }
}

void shift(std::string& str1, std::string& str2){
    if(str1.length() != str2.length()) {
        std::cout << -1;
    }
    else if (str1 == str2) {
        std::cout << 0;
    }
    else {
        int size = str1.size();
        str2 += str1;
        str2 += str1;        
        str1.clear();
        std::vector<int> result(str2.size());
        prefixFunction(str2, result);
        str2.clear();
        bool isShift = false;
        for(int i = 0; i < result.size(); i ++){
            if(result[i] == size) {
                isShift = true;
                std::cout << i - size*2 + 1;
                break;
            }
        }
        if(!isShift) std::cout << -1;
    }
}

int main() {
    std::string str1;
    std::string str2;
    std::cin >> str1 >> str2;
    shift(str1, str2);
    return 0;
}