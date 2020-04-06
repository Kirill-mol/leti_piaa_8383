#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <climits>
#include <map>
#include <utility>
//#include <windows.h>

class GraphTop;

class ResultEdges {  // используется для хранения и вывода ребер с фактической величиной потока
public:
    char start;     //начальная вершина
    char end;       //конечная вершина
    int flow;       //поток через ребро
    ResultEdges(char start, char end, int flow) : start(start), end(end), flow(flow) {};  //конструктор
};

class Edge { //класс ребро
public:
    GraphTop* top;      // врешина
    int weight;     //вес ребра
    int factFlow;   //поток через ребро
    bool isStartEdge;   //если ребро было стартовым
    Edge(GraphTop* top, int weight, bool isStartEdge) : top(top), weight(weight), isStartEdge(isStartEdge),factFlow(0) {}; // конаструктор
};

class GraphTop {
public:
    char name; //имя
    bool wasPassed = false; //было ли пройдено поиском в глубину
    std::vector<Edge*> adjacentEdges; //вектор смежных ребер
    explicit GraphTop(char name) : name(name){}; //конструктор

    void addAdjacentEdge(GraphTop* top, int weight, bool isStart) { //добавления смежного ребра
        bool isInAdjacent = false;  //если ребро уже есть в векторе смежных
        for(auto & adjacentEdge : adjacentEdges){ //проходимся по всем смежным ребрам
            if(adjacentEdge->top->name == top->name){   //если ребро уже есть в векторе
                isInAdjacent = true;
                if(adjacentEdge->weight == 0) adjacentEdge = new Edge(top, weight, isStart);  //если вес не нулевой, то обновляем
            }
        }
        if(!isInAdjacent) adjacentEdges.push_back(new Edge(top, weight, isStart)); //если нет ребра
    };
};

class EdgesList {
public:
    std::vector<GraphTop*> tops;    //все вершины графа
    std::vector<ResultEdges*> resultEdgesVec;  //ребра для ответа
    int maxFlow = 0; //максимальный поток через граф

    void sortTopsForAnswer() {  //сортировка ребер для ответа
        for (auto & top : tops) {
            for (int j = 0; j < top->adjacentEdges.size(); ++j) {
                if(top->adjacentEdges[j]->isStartEdge) resultEdgesVec.push_back(new ResultEdges(top->name, top->adjacentEdges[j]->top->name, top->adjacentEdges[j]->factFlow));
            }
        }
        std::sort(resultEdgesVec.begin(), resultEdgesVec.end(), comp);
    };

    static bool comp(ResultEdges* a, ResultEdges* b) { //компаратор
        if(a->start == b->start) return a->end < b->end;
        return a->start < b->start;
    };


    void printResult() { //вывод ребер для ответа
        for (auto & i : resultEdgesVec) {
            std::cout << i->start << " " << i->end << " " << i->flow << std::endl;
        }
    }

    void addEdge(char topName, char adjacentTopName, int weight) {  //добавление ребра
        GraphTop* top = addOrReturnTop(topName); //ищем вершину по имени
        GraphTop* adjacentTop = addOrReturnTop(adjacentTopName); //ищем смежную вершину по имени
        top->addAdjacentEdge(adjacentTop, weight, true); //вызываем функцию добавление смежного ребра
        adjacentTop->addAdjacentEdge(top, 0, false);  //вызываем добавление обратного смежного ребра

    };

    GraphTop* addOrReturnTop(char topName) { //функция проверки наличия ребра либо ее создания
        for(auto & top : tops){ //ищем переданную вершину в графе
            if(top->name == topName) return top;  //если нашли то возвращаем
        }
        auto* top = new GraphTop(topName); //если не нашли то создаем, добавляем в вектор вершин и возвращаем
        tops.push_back(top);    //добавляем вершину в вектор
        return top;
    };

    void makeAllTopsNotPassed(){  //делаем все вершины не пройденными
        for(auto & top : tops){
            top->wasPassed = false;
        }
    };

    void changeEdgesWeights(std::map<GraphTop*, GraphTop*>& way, GraphTop* startTop, GraphTop* endTop){  //изменение веса вершин пути
        std::vector<std::pair<char, Edge*>*> edges;  //ребра пути
        std::vector<std::pair<char, Edge*>*> reversedEdges; //обратные ребра пути
        int min = INT_MAX;  //мин поток через путь
        std::string foundedWay;
        foundedWay += endTop->name;
        GraphTop* top1 = endTop;  //вершина 1
        GraphTop* top2 = way[endTop]; //вершина 2
        while(top1 != startTop) {
            for (int i = 0; i < top2->adjacentEdges.size(); i++) {  //проходимся по всем смежным для 2 вершины
                if (top2->adjacentEdges[i]->top == top1) {  // если нашли ребро из 1 вершины во 2

                    edges.push_back(new std::pair<char, Edge*>(top2->name, top2->adjacentEdges[i]));    //добавляем ребро в вектор ребер пути
                    for(int j = 0; j < top1->adjacentEdges.size(); j++){  //поиск и добавление обратных ребер в вектор
                        if(top2->name == top1->adjacentEdges[j]->top->name)
                            reversedEdges.push_back(new std::pair<char, Edge*>(top1->name, top1->adjacentEdges[j]));  //д
                    }
                    if (top2->adjacentEdges[i]->weight < min) { //сравниваем вес ребра с минимальным в пути
                        min = top2->adjacentEdges[i]->weight;   //если нашли меньшее
                    }
                }
            }
            top1 = top2; //переходи к след. ребру пути
            top2 = way[top1];
            foundedWay += top1->name;
        }
        std::reverse(foundedWay.begin(), foundedWay.end());
        std::cout <<"Найденный путь: " << foundedWay << std::endl;
        std::cout <<"Поток в пути: " << min << std::endl;
        for (auto & edge : edges) {  //уменьшаем веса ребер пути
            std::cout << "Меняем вес ребра: (" << edge->first << "," << edge->second->top->name << ") " << edge->second->weight << " -> ";
            edge->second->weight -= min;
            std::cout << edge->second->weight << std::endl;
            edge->second->factFlow += min;  // увеличиваем максимальный поток через это ребро графа
            if (edge->second->weight == 0){  //если вес ребра равен нулю, то удаляем
                for (auto & top : tops) {
                    if(top->name == edge->first) {
                        for (auto j = top->adjacentEdges.begin(); j < top->adjacentEdges.end(); j++) {
                            if(edge->second == *j) {
                                resultEdgesVec.push_back(new ResultEdges(edge->first, edge->second->top->name, edge->second->factFlow)); //добавляем удаленное в ответ
                                top->adjacentEdges.erase(j);
                                std::cout << "\tВес ребра равен нулю, удаляем его из графа" << std::endl;
                            }
                        }
                    }
                }
            }
        }
        for (auto & edge : reversedEdges) {  //увеличиваем вес обратного ребра
            std::cout << "Меняем вес обратного ребра: (" << edge->first << "," << edge->second->top->name << ") " << edge->second->weight << " -> ";
            edge->second->weight += min;
            std::cout << edge->second->weight << std::endl;
        }
        maxFlow += min; //увеличиваем поток через граф
        std::cout << "Увеличиваем поток графа на " << min << std::endl;

    }

    bool dfs(char start, char end) {
        std::cout << "Запуск поиск пути" << std::endl;
        std::map<GraphTop*, GraphTop*> way; //путь после поиска в глубину
        makeAllTopsNotPassed(); //делаем все вершины не пройденными
        std::stack<GraphTop*> stackForDfs; //стек для поиска в глубину
        GraphTop* startTop = addOrReturnTop(start);  //кладем стартовую на стек
        stackForDfs.push(startTop);
        GraphTop* endTop = addOrReturnTop(end);

        while(!stackForDfs.empty()){ //пока стек не пустой
            GraphTop* curTop = stackForDfs.top();  //делаем текущую вершиной стека
            std::cout << "Обрабатываемая вершина: " << curTop -> name<< std::endl;
            stackForDfs.pop(); //снимаем вершину со стека
            for (int i = 0; i < curTop->adjacentEdges.size(); ++i) { //проходим по всем смежным для текущей
                std::cout<< "\t" << "Проверяем смежную вершину: " << curTop->adjacentEdges[i]->top->name << std::endl;
                if(curTop->adjacentEdges[i] -> top == endTop && curTop->adjacentEdges[i]->weight != 0) {  //если путь до конечной не нулевой
                    std::cout << "\t" << "Вершина является конечной, заканчиваем поиск" << std::endl;
                    endTop->wasPassed = true; //делаем кон пройденной
                    stackForDfs.push(endTop); //пушим на стек
                    way[endTop] = curTop; //добавляем ее в путь
                    changeEdgesWeights(way, startTop, endTop); //делаем перерасчет весов
                    return true; //возвращаем что путь был найден
                }
                if(curTop->adjacentEdges[i]->weight != 0 && !curTop->adjacentEdges[i]->top->wasPassed) {  //если не пройдена вершина и вес пути не нулевой
                    std::cout << "\tДобавляем на стек" << std::endl;
                    curTop->adjacentEdges[i]->top->wasPassed = true;  //делаем пройденной
                    stackForDfs.push(curTop->adjacentEdges[i]->top); //кладем на стек

                    way[curTop->adjacentEdges[i] -> top] = curTop; //добавляем в путь
                }
                else std::cout << "\tПройдена либо вес равен нулю" << std::endl;
            }
        }
        std::cout << "Путей нет" << std::endl;
        return false; //если не нашил вершину
    };
};

int main() {
    //SetConsoleOutputCP(CP_UTF8);
    EdgesList edgesList;  //создаем граф
    char source, stock, curStart, curEnd;
    int numOrientedEdges, curWeight;
    std::cin >> numOrientedEdges >> source >> stock; //считываем начальную вершину, конечную и кол-во ребер
    for (int i = 0; i < numOrientedEdges; ++i) {
        std::cin >> curStart >> curEnd >> curWeight;        //счиываем ребро
        edgesList.addEdge(curStart, curEnd, curWeight); //добавляем в вектор
    }
    while(edgesList.dfs(source, stock)){} //производим поиск пока есть пути
    std::cout << "__________________________________________________" << std::endl;
    std::cout << "Результат работы алгоритма: " << std::endl;
    std::cout <<edgesList.maxFlow << std::endl; //вывод максимального потока
    edgesList.sortTopsForAnswer(); //сортируем и печатаем результат
    edgesList.printResult();
    return 0;
}