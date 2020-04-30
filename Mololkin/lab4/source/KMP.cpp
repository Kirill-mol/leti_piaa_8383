#include <iostream>
#include <vector>

void prefixFunction(const std::string& str, std::vector<int>& result) { //префикс функция
    std::cout << "Start prefix function for string: "<< str << std::endl;
    std::cout << "result[0] = 0" << std::endl;
    int n = str.length();                                           //получаем длину входной строки
    std::cout << "Input string length: " << n << std::endl;
    for (int i = 1; i < n; ++i){
        std::cout << "Check symbol: " << str[i] << " index in input string = " << i << std::endl;   
        int j = result[i - 1];                                      //приравниваем текущий j к значению результата префикс ффункции для предыдущего символа  
        std::cout << "Current j = " << j << std::endl;
        while(j > 0 && str[i] != str[j]) {                          //если символы не совпадают и j больше 0 переходим к предыдущему результирующем векторе
            j = result[j - 1];   
            std::cout << "Symbols at position j in input string and current checking symbol not equal, change j value to result[j - 1]: j =  " << j << std::endl;
        }
        if(str[i] == str[j]) { //если нашли совпадени увеличиваем j
            ++j;
            std::cout << "Symbols at position j in input string and current checking symbol are equal, increase j, j = " << j << std::endl;
        }                                  
        result[i] = j;                                              // записываем результат для тек. символа
        std::cout << "Prefix function for symbol " << str[i] << " index in input string = " << i << " is " << result[i] << std::endl;
    }
    std::cout << "End prefix function \n" << std::endl;
}

void KMP(const std::string& pattern, const std::string& text) {
    std::vector<int> prefixFuncRes(pattern.size());   
    std::vector<int> result;
    prefixFunction(pattern, prefixFuncRes);                         //вычисляем префикс функцию для шаблона
    int textIndex = 0;  
    int patternIndex = 0;
    bool isAnyEntry = false;
    std::cout << "Start KMP" << std::endl;
    while(textIndex != text.length()){                              //запускаем цикл пока не пройдем весь текст
        if(pattern[patternIndex] == text[textIndex]) {              //если нашли совпадение символов шаблона и текста
            std::cout << "Symbol on index " << patternIndex << " in pattern is equal to symbol on index " << textIndex << " in text" << std::endl;
            std::cout << "Increase indexes" << std::endl;
            textIndex++;
            patternIndex++;
            if(patternIndex == pattern.size()) {                    //если прошли весь шаблон значит нашли вхождение
                isAnyEntry = true;
                result.push_back(textIndex - pattern.length());     //индекс вхождения записываем в рез. вектор
                std::cout << "Entry was found, the occurrence index " << textIndex - pattern.length() << std::endl;
            }
        }
        else {
            std::cout << "Symbols not equals" << std::endl;
            if(patternIndex == 0){                                 //символы не совпали и индекс шаблона 0
                std::cout << "Pattern index is 0, increase text index" << std::endl;
                textIndex++;
            }
            else {                                                 //индекс шаблона не 0
                patternIndex = prefixFuncRes[patternIndex - 1];    //переходим к предидущему символу в строке значений префикс функции
                std::cout << "Change pattern index to " << patternIndex << std::endl;
            }
        }
    }
    std::cout << "End KMP print result: " << std::endl;
    if(!isAnyEntry) std::cout << -1;                               //если не нашил вхождений
    else {                                                         // вывод результата
        std::cout << result[0];  
        for(int i = 1; i < result.size(); i++){
            std::cout << "," << result[i];
        }
    }
}

int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;
    KMP(pattern, text);
    std::cout << std::endl;
    return 0;
}
