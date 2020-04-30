#include <iostream>
#include <vector>

void prefixFunction(const std::string& str, std::vector<int>& result) { //префикс функция
    std::cout << "Start prefix function for string: "<< str << std::endl;
    std::cout << "result[0] = 0" << std::endl;
    int n = str.length();                                           //получаем длину входной строки
    std::cout << "Input string length: " << n << std::endl;
    for (int i = 1; i < n; ++i){
        std::cout << "Check symbol: " << str[i] << ", index in input string = " << i << std::endl;   
        int j = result[i - 1];                                      //приравниваем текущий j к значению результата префикс ффункции для предыдущего символа  
        std::cout << "Current j = " << j << std::endl;
        while(j > 0 && str[i] != str[j]) {                          //если символы не совпадают и j больше 0 переходим к предыдущему результирующем векторе
            j = result[j - 1];   
            std::cout << "Symbols at position j in input string and current checking symbol not equal and j > 0, change j value to result[j - 1]: j =  " << j << std::endl;
        }
        if(str[i] == str[j]) { //если нашли совпадени увеличиваем j
            ++j;
            std::cout << "Symbols at position j in input string and current checking symbol are equal, increase j, j = " << j << std::endl;
        }                                  
        result[i] = j;                                              // записываем результат для тек. символа
        std::cout << "Prefix function for symbol " << str[i] << ", index in input string = " << i << " is " << result[i] << std::endl;
    }
    std::cout << "End prefix function \n" << std::endl;
}

void shift(std::string& str1, std::string& str2){
    std::cout << "Start searching shift" << std::endl;
    if(str1.length() != str2.length()) {                            //если длина строк разная
        std::cout << "Strings lengths not equals" << std::endl;
        std::cout << -1;
    }
    else if (str1 == str2) {                                        //если строки полностью совпадают
        std::cout << "Strings are equals" << std::endl;
        std::cout << "Result: ";
        std::cout << 0;
    }
    else {                                                          //если длина одинаковая но строки не совпадают
        std::cout << "Strings not equals" << std::endl;
        int size = str1.size(); 
        str2 += str1;                                               //создаем новую строку состаящуюю из второй и двух первых
        str2 += str1;
        str1.clear();                                               //очищаем первую строку
        std::cout << "Create new string: " << str2 << std::endl;
        std::vector<int> result(str2.size());                       
        std::cout << "Call prefix function for new string: "<< str2 << "\n" << std::endl;
        prefixFunction(str2, result);                               //вызываем префикс функцию для новой строки
        std::cout << "Prefix function result: " << std::endl;
        for(int i = 0; i < result.size(); i ++){
            std::cout << result[i] << " ";
        }
        std::cout << std::endl;
        str2.clear();                                               //очищаем ее так как работать будем только с результатом
        bool isShift = false;                                       //флаг для выхода из цикла
        std::cout << "Search in result vector: " << size << " with positive result index"<< std::endl;
        for(int i = 0; i < result.size(); i ++){
            std::cout << "Check value at index: " << i << " in prefix function result" << std::endl;
            if(result[i] == size) {                                 //нашли сдвиг
                if(i - size*2 + 1 >= 0){
                    std::cout << "Shift was found, result index: ";
                    isShift = true;
                    std::cout << i - size*2 + 1;                        //печааем индекс
                    break;
                }
                else std::cout << size << " was found in vector, but result not positive" << std::endl;
            }
        }
        if(!isShift) {                                              //если строка 2 не является циклическим сдвигом
            std::cout << "Shift wasn't found, result -1" << std::endl;
        }
    }
}

int main() {
    std::string str1;
    std::string str2;
    std::cin >> str1 >> str2;
    shift(str1, str2);
    std::cout << std::endl;
    return 0;
}