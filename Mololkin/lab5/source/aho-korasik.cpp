#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

class Vertex {
public:
    std::map<char, int> nextVertexes; // вектор вершин потомков
    std::pair<char, int> parentNode; //предок
    std::map<char, int> nextNode; //следующее ребро из вершины по какому-либо символу
    int suffixLink; //суффиксальная ссылка
    bool isTerminal; //флаг терминальной вершины
    int level; //уровень вершины
    std::vector<int> numbers; //номера паттернов

    Vertex() : isTerminal(false), suffixLink(-1), level(0), parentNode(std::pair<char, int>(' ', -1)) {};
    Vertex(int prevInd, char prevChar) : isTerminal(false), suffixLink(-1), level(0), parentNode(std::pair<char, int>(prevChar, prevInd)) {};

    void printVertex() {
        if(parentNode.second == -1)
            std::cout << "Vertex is root" << std::endl;
        else if(isTerminal){
            std::cout << "Vertex is terminal" << std::endl;
            std::cout << "Symbol to vertex: \'" << parentNode.first << "\' parent index: " << parentNode.second << std::endl;
            std::cout << "Vertex level: " << level << std::endl;
            std::cout << "Pattern numbers: ";
            for(int number : numbers) {
                std::cout << number + 1 << " ";
            }
            std::cout << std::endl;
            if(!nextVertexes.empty()) {
                std::cout << "Next vertexes: ";
                for (auto &nextVertex : nextVertexes) {
                    std::cout << "(" << nextVertex.first << ", " << nextVertex.second << ") ";
                }
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "Symbol to vertex: \'" << parentNode.first << "\' parent index: " << parentNode.second << std::endl;
            std::cout << "Next vertexes: ";
            for(auto & nextVertex : nextVertexes) {
                std::cout << "(\'" << nextVertex.first << "\', " << nextVertex.second << ") ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

class Bohr {
public:
    std::vector<std::pair<int, int>> result;  //результирующи вектор
    std::vector<Vertex> vertexes; //веришины дерева
    int terminalsNumb; //количество терминальных
    Bohr(){
        Vertex root; //создаем корень и добавляем ввектор, а также остальные инициализируем поля
        vertexes.push_back(root);
        terminalsNumb = 0;
        mostDeepLevel = 0;
    }

    void addStringToBohr(const std::string& str){  //добавление строки в бор
        std::cout << "Add string: \"" << str << "\" to bohr" << std::endl;
        int cur = 0;   //тек. на начало
        for (char i : str) {  //проходимся по всем сиволам переданной строки
            std::cout << "Index of current vertex: " << cur << std::endl;
            if(vertexes[cur].nextVertexes.find(i) == vertexes[cur].nextVertexes.end()) {  //если в боре нет такой вершины
                std::cout << "No next vertex with node: \'" << i << "\' create new vertex with node: \'" << i << "\' it's index in vertexes array: " << vertexes.size() << std::endl;
                Vertex curVertex(cur, i);  //создаем ее и добавляем
                vertexes.push_back(curVertex);
                vertexes[cur].nextVertexes[i] = vertexes.size() - 1; //связываем потомка с родителем
            }
            cur = vertexes[cur].nextVertexes[i]; //если в боре уже есть такая вершина, просто переходим дальше
        }
        std::cout << "String: \"" << str << "\" was added to the bohr" << std::endl;
        std::cout << "Made last vertex terminal" << std::endl;
        std::cout << "It's level: " << str.length();
        std::cout << ", pattern number: " << terminalsNumb << " was added to patterns numbers array of ist vertex" << std::endl;
        vertexes[cur].isTerminal = true;  //последняя вершина терминальная
        vertexes[cur].numbers.push_back(terminalsNumb++); //добавляем номер шаблона данной вершины
        vertexes[cur].level = str.length();
        if (str.length() > mostDeepLevel) {
            mostDeepLevel = str.length(); //изменяем длину самой длинной цепочки прямых ссылок
            std::cout << "This is the most deep vertex of bohr" << std::endl;
        }
    }

    void printBohr() {
        for (int i = 0; i < vertexes.size(); ++i) {
            std::cout << "Vertex index: " << i << std::endl;
            vertexes[i].printVertex();
        }
    }

    int getSuffixLink(int i) {   //получение суффиксальной ссылки для вершины
        std::cout << "Get suffix link of vertex with index: " << i << std::endl;
        if (vertexes[i].suffixLink == -1) { //если еще нет суффиксальной
            std::cout << "Vertex doesn't have suffix link" << std::endl;
            if (i == 0 || vertexes[i].parentNode.second == 0) {  //если корень или его потомок, то ссылка на корень
                std::cout << "Vertex is root or root child it's suffix link to root" << std::endl;
                vertexes[i].suffixLink = 0;
            }
            else { //если нет то ищем путь из суффиксальной вершины родителя, по символу от родителя к тек вершине
                std::cout << "To find suffix link go to parent vertex and check it's suffix link" << std::endl;
                vertexes[i].suffixLink = getNextVertex(getSuffixLink(vertexes[i].parentNode.second), vertexes[i].parentNode.first);
            }
        }
        std::cout << "Suffix link to vertex with index: " << i << " is " << vertexes[i].suffixLink << std::endl;
        return vertexes[i].suffixLink;
    }

    int getNextVertex(int i, char c) { //следующий шаг автомата
        std::cout << "Find next move for vertex with index: " << i << " by symbol: \'" << c << "\'"<< std::endl;
        if (vertexes[i].nextNode.find(c) == vertexes[i].nextNode.end()) {  //если нет пути в словаре путей автомата по переданному сиволу
            std::cout << "No next move by symbol: \'" << c << "\'" << std::endl;
            if (vertexes[i].nextVertexes.find(c) != vertexes[i].nextVertexes.end()) { //если есть прямая ссылка
                std::cout << "There is forward move to next vertex by symbol: \'" << c << "\' to vertex with index: " << vertexes[i].nextVertexes[c] << std::endl;
                vertexes[i].nextNode[c] = vertexes[i].nextVertexes[c]; //то добавляем ее в путь
            }
            else {
                if (i == 0) {  //если корень и нет потомков с путем по переданному символу, то ссылка на корень
                    std::cout << "It is root vertex without child by symbol: \'" << c << "\' so next move is root"<< std::endl;
                    vertexes[i].nextNode[c] = 0;
                }
                else { //в противном случае добавляем в словарь след вершину из суффикасальной ссылки
                    std::cout << "Next move by suffix link" << std::endl;
                    vertexes[i].nextNode[c] = getNextVertex(getSuffixLink(i), c);
                }
            }
        }
        std::cout << "Next move vertex with index: " << vertexes[i].nextNode[c] << " by symbol \'" << c <<"\'" << std::endl;
        return vertexes[i].nextNode[c];
    }

    void findAllPatternsOnText(std::string& text) { //поиск
        std::cout << "Start searching entry of patterns to text: " << text << std::endl;
        int cur = 0; // тек равна корню
        std::cout << "Start from root" << std::endl;
        for (int i = 0; i < text.length(); i++) {
            cur = getNextVertex(cur, text[i]);  //получаем путь по i - ому символу текста
            std::cout << "Next current is vertex with index: " << cur << std::endl;
            for (int j = cur; j != 0; j = getSuffixLink(j)) {  // затем провходимся от тек символа до корня по суффиксальным
                std::cout << "Start searching terminal by suffix link from current vertex" << std::endl;
                if (vertexes[j].isTerminal) { //если нашли терминальную
                    std::cout << "Terminal was founded" << std::endl;
                    for(int k = 0; k < vertexes[j].numbers.size(); k++) {  //то доавляем в результат найденнй паттерн, а если есть одинаковые паттерны, то добаляем в результат все
                        std::pair<int, int> res(vertexes[j].numbers[k], i + 2 - vertexes[j].level);
                        result.push_back(res);
                        std::cout <<"Pattern index and number: " << vertexes[j].numbers[k] + 1<<" " <<  i + 2 - vertexes[j].level << std::endl;
                    }
                }
            }
        }
        std::cout << "All symbols of text was checked, sort result array" << std::endl;
        sort(result.begin(), result.end(), compare);   //сортировка результат
        if(result.empty()) std::cout << "Patterns not founded in text" << std::endl;
        else {
            std::cout << "Result: " << std::endl;
            std::cout << "Index in text | pattern number" << std::endl;
            for (auto &i : result) {    //выводим каждую пару результата
                std::cout << i.second << "\t\t" << i.first + 1 << std::endl;
            }
        }
    }

    static int compare(std::pair<int, int> a, std::pair<int, int> b) {  //компаратор пар для ответа
        if (a.second == b.second) {
            return a.first < b.first;
        }
        else {
            return a.second < b.second;
        }
    }
};

int main() {
    std::string text;
    std::string curPattern;
    int size = 0;
    std::cin >> text;
    std::cin >> size;
    Bohr bohr;
    for (int i = 0; i < size; ++i) {
        std::cin >> curPattern;
        bohr.addStringToBohr(curPattern);
    }
    std::cout << "Created bohr:" << std::endl;
    bohr.printBohr();
    bohr.findAllPatternsOnText(text);
}