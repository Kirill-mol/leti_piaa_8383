#include <iostream>
#include <map>
#include <vector>

class Vertex {
public:
    std::map<char, int> nextVertexes; // вектор вершин потомков
    std::pair<char, int> parentNode; //предок
    std::map<char, int> nextNode; //следующее ребро из вершины по какому-либо символу
    int suffixLink; //суффиксальная ссылка
    bool isTerminal; //флаг терминальной вершины
    int level; //глубина вершины
    std::vector<int> posInJokerPattern; //позиция в паттерне

    Vertex() : isTerminal(false), suffixLink(-1), level(0), parentNode(std::pair<char, int>(' ', -1)) {};
    Vertex(int prevInd, char prevChar) : isTerminal(false), suffixLink(-1), level(0), parentNode(std::pair<char, int>(prevChar, prevInd)) {};

    void printVertex() {
        if(parentNode.second == -1)
            std::cout << "Vertex is root" << std::endl;
        else if(isTerminal){
            std::cout << "Vertex is terminal" << std::endl;
            std::cout << "Symbol to vertex: \'" << parentNode.first << "\' parent index: " << parentNode.second << std::endl;
            std::cout << "Vertex level: " << level << std::endl;
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

class BohrWithJoker {
public:
    std::vector<Vertex> vertexes;  //все вершины бора
    char joker;  //символ джокера
    int jokerPatternSize = 0;  //размер шаблона джокера

    explicit BohrWithJoker(char joker) : joker(joker) {  //в конструкторе создаем корень и добавляем в вектор вершин
        Vertex root;
        vertexes.push_back(root);
    }

    void printBohr() {
        for (int i = 0; i < vertexes.size(); ++i) {
            std::cout << "Vertex index: " << i << std::endl;
            vertexes[i].printVertex();
        }
    }

    void addJokerPattern(std::string& jokerPattern){  //добавление шаблона в бор
        std::cout << "Add pattern with joker: " << jokerPattern << " to bohr" << std::endl;
        int cur = 0;
        int counter = 0;
        bool isPrevJoker = false;
        jokerPatternSize = jokerPattern.size();
        for (int j = 0; j < jokerPattern.length(); j++) {  //проходимся по каждому символу шаблона
            std::cout << "Cur symbol: \'" << jokerPattern[j] << "\' ";
            if (jokerPattern[j] == joker) {  //если встертили джокер
                std::cout << "is joker ";
                if (j == 0) {  //для первого символа
                    std::cout << ", it's first symbol in pattern" << std::endl;
                    counter = 0;
                    isPrevJoker = true;
                }
                else if (isPrevJoker) { //если перед тек джокером был джокер
                    std::cout << ", previous is joker " << std::endl;
                    cur = 0;
                    counter = 0;
                }
                else { //делаем последний символ не джокер терминальной вершиной и добавляем в бор
                    std::cout << ", make current vertex terminal" << std::endl;
                    isPrevJoker = true;
                    vertexes[cur].isTerminal = true;
                    vertexes[cur].posInJokerPattern.push_back(j - counter);
                    if (vertexes[cur].level == 0) {
                        vertexes[cur].level = counter;
                    }
                    counter = 0;
                    cur = 0;
                    std::cout << "Now current = 0" << std::endl;
                }
            }
            else {
                isPrevJoker = false;
                counter++;  //увеличиваем длину тек подстроки
                std::cout << " is not joker" << std::endl;
                if (vertexes[cur].nextVertexes.find(jokerPattern[j]) == vertexes[cur].nextVertexes.end()) {   //если нет потомка по тек символу
                    std::cout <<", no vertexes from current by symbol: " << jokerPattern[j] <<", so add new vertex with index: " << vertexes.size() << std::endl;
                    Vertex vert(cur, jokerPattern[j]);
                    vertexes.push_back(vert);
                    vertexes[cur].nextVertexes[jokerPattern[j]] = vertexes.size() - 1;
                }
                cur = vertexes[cur].nextVertexes[jokerPattern[j]];
                std::cout << "Go to new vertex new current vertex index: " << cur << std::endl;
            }
        }
        if (!isPrevJoker) {   //если перед последним был джокер либо последний джокер
            if (vertexes[cur].level == 0) {
                vertexes[cur].level = counter;
            }
            vertexes[cur].isTerminal = true;
            vertexes[cur].posInJokerPattern.push_back(jokerPattern.length() - counter);
            std::cout << "Make pattern end terminal" << std::endl;
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

    void findAllJokerPatternsOnText(std::string& text) {   //поиск
        std::cout << "Strat to find pattern in text: " << text << std:: endl;
        std::vector<int> foundedForSymbols(text.size() );
        int cur = 0;
        int numOfFoundedStr = 0;  //количество найденных подстрок
        for (auto & vertex : vertexes) {
            if (vertex.isTerminal) {
                for (int j = 0; j < vertex.posInJokerPattern.size(); j++) {
                    numOfFoundedStr++;
                }
            }
        }
        std::cout << "Number of founded  substrings: " << numOfFoundedStr << std::endl;
        for (int i = 0; i < text.length(); i++){  //проходимся по всем симвоалм такста
            cur = getNextVertex(cur, text[i]);  //идем по тек символу по бору
            std::cout << "Current vertex index: " << cur << std::endl;
            for (int j = cur; j != 0; j = getSuffixLink(j)) {  // возвращаемся по суффиксальным в корень
                std::cout << "Start searching terminal by suffix link from current vertex" << std::endl;
                if (vertexes[j].isTerminal) {  //если нашли терминальную
                    std::cout << "Terminal was founded" << std::endl;
                    int indInText = i + 1 - vertexes[j].level;
                    for (int k = 0; k < vertexes[j].posInJokerPattern.size(); k++){   //по вычесленному месту тек подстроки и добавляем в вектор числа совпадений
                        if (indInText - vertexes[j].posInJokerPattern[k] >= 0) {
                            foundedForSymbols[indInText - vertexes[j].posInJokerPattern[k]] ++;
                            std::cout << "Increase element at index: " << indInText - vertexes[j].posInJokerPattern[k]
                            << " in array of number of matches, it's value: " << foundedForSymbols[indInText - vertexes[j].posInJokerPattern[k]] << std::endl;
                        }
                    }
                }
            }
        }
        std::cout << "RESULT:" << std::endl;
        for (int i = 0; i < foundedForSymbols.size() - jokerPatternSize + 1; i++) {  //вывод результата
            if (foundedForSymbols[i] == numOfFoundedStr) {
                std::cout << i + 1 << std::endl;
            }
        }
    }
};

int main() {
    std::string text;
    std::string jokerPattern;
    char joker;
    std::cin >> text;
    std::cin >> jokerPattern;
    std::cin >> joker;
    BohrWithJoker bohrWithJoker(joker);
    bohrWithJoker.addJokerPattern(jokerPattern);
    bohrWithJoker.printBohr();
    bohrWithJoker.findAllJokerPatternsOnText(text);
}