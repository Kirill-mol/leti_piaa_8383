#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <utility>

class GraphTop;

class Pair { //класс ребро
public:
    GraphTop* top;  // врешина
    double weight; //вес ребра
    Pair(GraphTop* top, double weight) : top(top), weight(weight){}; // конаструктор
};

class GraphTop {  //класс вершина
public:
    char name;  //имя вершины
    double gFunc = 0; //значение функции g
    double hFunc = 0; //значение функции h
    double fFunc = 0;//значение функции f
    std::vector<Pair*> adjacentEdges; // смежные ребра
    explicit GraphTop(char name) : name(name){}; // конструктор

    void sortTops(char goal){     //сортировка всмежных вершин
        for(auto & adjacentEdge : adjacentEdges){  // заплняем значений функций g,f,h
            adjacentEdge->top->gFunc = adjacentEdge->weight;
            adjacentEdge->top->setHFunc(goal);
            adjacentEdge->top->setFFunc();
        }
        for(int i = 0; i < adjacentEdges.size() ; i++){
            for (int j = i+1; j < adjacentEdges.size(); ++j) {
                if(adjacentEdges[j]->top->fFunc < adjacentEdges[i]->top->fFunc){ //сортируем ребра пузырьком
                    std::swap(adjacentEdges[j], adjacentEdges[i]);
                }
            }
        }
    };

    void setHFunc(char b){  //эврестическа функция
        hFunc =  (double)abs((int)name - (int)b);
    };

    void setFFunc(){
        fFunc = hFunc + gFunc;
    };

    void addAdjacentEdge(GraphTop* top, double weight) { //добавления смежного ребра
        adjacentEdges.push_back(new Pair(top, weight));
    };

    void print(){ //печать смежных ребер
        std::cout << name << ":";
        for(auto & adjacentEdge : adjacentEdges){
            std::cout << "(" << adjacentEdge->top->name << "," << adjacentEdge->weight << "," << adjacentEdge->top->gFunc << "," << adjacentEdge->top->hFunc << "," << adjacentEdge->top->fFunc<< ")";
        }
    };
    void print1(){ //печать смежных ребер без h, g , f
        std::cout << name << ":";
        for(auto & adjacentEdge : adjacentEdges){
            std::cout << "(" << adjacentEdge->top->name << "," << adjacentEdge->weight << ")";
        }
    };
};

class EdgesList {
private:
    static GraphTop* findBestTop(std::vector<GraphTop*>& openSet){  //функция нахождения вершины с минимальной fFunc в векторе
        GraphTop* bestTop = openSet[0];  //приравниваем возвращаемой вершине первый элемент в векторе
        for(int i = 1; i < openSet.size(); i++){ //проходимся по всем вершинам в векторе
            if(openSet[i]->fFunc < bestTop->fFunc) bestTop = openSet[i];
        }
        return bestTop;
    }

    static std::string reconstructPath(std::map<GraphTop*, GraphTop*>& passedTops, GraphTop* start, GraphTop* goal){ //функция построения пути
        std::string resString; //возвращаемая строка
        GraphTop* curr = goal;  //приравниваем текущую вершину к конечной
        resString += goal->name; //добавляем имя конечной вершины к рез строке
        while(curr != start){  //пока не дойдем до начальной
            curr = passedTops[curr]; //по ключю находим следующую вершину
            resString += curr->name; //добавляем ее имя в рез строку
        }
        std::reverse(resString.begin(), resString.end()); //переворачиваем строку
        return resString;
    }

    static bool isInClosedSet(GraphTop* top, std::vector<GraphTop*>& openSet){  //находим врешину в вектрое
        for (auto & i : openSet) { //проходимся по каждой вершине в векторе
            if(i == top) return true;
        }
        return false;
    }

    static void removeFromOpenSet(GraphTop* top, std::vector<GraphTop*>& openSet){ //удаление вершины из вектора
        for(auto i = openSet.begin(); i <= openSet.end(); i++){ //с помощью итератора проходимся по вершинам в веторе
            if((*i) == top){ //сравниваем значение в итераторе и удаляемую вершину
                openSet.erase(i); //если равны то удаляем
            }
        }
    }


public:
    std::vector<GraphTop*> tops; //вектор всех вершин графа

    void sortTops(char goal){ //сортировка всех смежных вершин у каждой вершины
        for(auto & top : tops){ // проходимся по каждой вершине в вкторе и вызываем для нее сортировку
            top->sortTops(goal);
        }
    }

    void addEdge(char topName, char adjacentTopName, double weight) {  //добавление ребра
        GraphTop* top = isExistTop(topName); //ищем вершину по имени
        GraphTop* adjacentTop = isExistTop(adjacentTopName); //ищем смежную вершину по имени
        top->addAdjacentEdge(adjacentTop, weight); //вызываем функцию добавление смежного ребра
    };

    GraphTop* isExistTop(char topName) { //вынкция проверки наличия ребра либо ее создания
        for(auto & top : tops){ //ищем переданную вершину в графе
            if(top->name == topName) return top;  //если нашли то возвращаем
        }
        auto* top = new GraphTop(topName); //если не нашли то создаем, добавляем в вектор вершин и возвращаем
        tops.push_back(top);
        return top;
    };

    void print() { //печать всех вершин
        for (auto & top : tops) {
            top->print();
            std::cout << std::endl;
        }
    };

    std::string findBestWay(char start, char goal){ //поиск пути по алгоритму А*
        std::vector<GraphTop*> closedSet; //обработанные вершины
        std::vector<GraphTop*> openSet; //вершины, которые предстоит обработать
        std::map<GraphTop*, GraphTop*> passedTops; //словарь для пути
        GraphTop* startTop = isExistTop(start);//ищем стартовую вершину
        openSet.push_back(startTop);//добавляем в вектор для обработкаи
        startTop->gFunc = 0.0; // заплняем значений функций g,f,h
        startTop->setHFunc(goal);
        startTop->setFFunc();
        GraphTop* curTop = startTop; //приравниваем текущей начальную
        while(!openSet.empty()) { //пока в списке есть вершины для обработки
            GraphTop* curTop = findBestTop(openSet); //ищем вершины с наименьшем значением fFunc
            std::cout << "Current top:" << std::endl;
            curTop->print1();
            std::cout << std::endl;
            if(curTop->name == goal) {                                                                                  //если нашли конечную вызываем функцию для определения пути
                std::cout << "End was finded" << std::endl;
                std::cout << "Result :" << std::endl;
                return reconstructPath(passedTops, startTop, isExistTop(goal));
            }
            removeFromOpenSet(curTop, openSet); //удаляем тек вершину из списка обрабатываемых
            closedSet.push_back(curTop);// и добавляем в обработанные
            std::cout << "Check all adjacent for:" << curTop ->name << std::endl;
            for(int i = 0; i < curTop->adjacentEdges.size(); i++){ //проходимся по всем смежным для текущей
                bool tentativeIsBetter; //вляг для проверки является ли лучшей
                if(isInClosedSet(curTop->adjacentEdges[i]->top, closedSet)) continue; // если смежная уже обработана, то пропускаем
                double tentativeGScore = curTop->gFunc + curTop->adjacentEdges[i]->weight; // по данной переменной будем определять, является ли вершина лучшей
                if(!isInClosedSet(curTop->adjacentEdges[i]->top, openSet)) { //если нет в очереди на обработку, то добавим туда
                    openSet.push_back(curTop->adjacentEdges[i]->top);
                    tentativeIsBetter = true;
                }
                else{
                    if (tentativeGScore < curTop->adjacentEdges[i]->top->gFunc) tentativeIsBetter = true; // если gFunc у смежной больше
                    else continue;
                }
                if (tentativeIsBetter){//если вершина лучшае
                    std::cout << "Adjacent top "<< curTop->adjacentEdges[i]->top->name << " is better" << std::endl;
                    passedTops[curTop->adjacentEdges[i]->top] = curTop; //то добавим ее в путь
                    curTop->adjacentEdges[i]->top->gFunc = tentativeGScore; // заполняем значений функций g,f,h
                    curTop->adjacentEdges[i]->top->setHFunc(goal);
                    curTop->adjacentEdges[i]->top->setFFunc();
                }
            }


        }
        return "0";
    }
};

int main() {
    EdgesList edgesList;  //создание графа
    char a, b, start, goal;
    double weight;
    std::cin >> start >> goal;  //считывание начальной и конечной вершин
    while(std::cin >> a >> b >> weight) { //считываем пока не cin не вернет false
        edgesList.addEdge(a, b, weight);  //добваление ребра графа
    }
    edgesList.sortTops(goal);  //сортируем для каждой вершины смежные по приоритету
    std::cout << "Graph after reading and sotring" << std::endl;
    edgesList.print();      //печатаем граф
    std::cout << edgesList.findBestWay(start, goal) << std::endl; //вызываем функцию нахождения пути в графе
    return 0;
}