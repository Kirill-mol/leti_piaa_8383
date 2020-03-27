#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <string>

using namespace std;

using Edge = tuple<string, string, double, bool>; //ребро

class EdgesList { //граф
private:
    vector<Edge*> edges; //вектор ребер
    stack<string> stackk; //стек вершин
public:
    EdgesList() = default;
    void addEdge(Edge* edge) { //добавление вершин в граф
        edges.push_back(edge);
    }
    void printList() { //печать ребер
        for (auto & edge : edges) {
            std::cout << get<0>(*edge) << " ";
            std::cout << get<1>(*edge) << " ";
            std::cout << get<2>(*edge) << " ";
            std::cout << get<3>(*edge) << endl;
        }
    }

    void printStack(){ //печать перевернутого стека
        std::stack<string> newS; //содаем новый стек
        while (!stackk.empty()) { //пока стек не пустой
            newS.push(stackk.top()); //добавляем вершины в новый
            stackk.pop();  //и убираем их из старого
        }
        while (!newS.empty()) { //пока нвый не пустой
            cout << newS.top(); //печатаем вершину
            newS.pop();//и убираем ее со стека
        }
    }

    void findWay(string startTop, string endTop) {
        std::cout << "Start find way in graph" << std::endl;
        stackk.push(startTop); //кладем стартовую вершину на стек
        bool finded = false; //флаг для остановки поиска
        while(!finded) {
        std::cout << "Start top is (" << startTop << ")" << std::endl;
            Edge bestEdge = {"", "", 1000.0, false}; // будем сравнивать все ребра с данным
            for (auto & edge : edges) { //проходимся по каждому ребру в графе
                if ((get<0>(*edge) == startTop) && (get<2>(*edge) < get<2>(bestEdge)) && get<3>(*edge) != true) { //если оно является не пройденым, инцидентным данному, а также его вес меньше текущего лучшего
                    bestEdge = *edge;
                }
            }
            //if(finded) break;
            if(get<0>(bestEdge).empty()){ //если не нашли инцидентную или они все пройдены
                std::cout << "There is no neighbour or there were passed for top (" << startTop <<")"<< std::endl;
                string popedTop = stackk.top(); //снимаем со стека с вершины и приравниваем к новой переменной
                stackk.pop();
                std::cout << "Return to top (" << stackk.top() << ")" << std::endl;
                for(int i = 0; i < edges.size(); i++){   //ищем инцидентное ребро между удаленной вершиной и вершиной стека
                    if(get<1>(*edges[i]) == popedTop && get<0>(*edges[i]) == stackk.top()) {
                        get<3>(*edges[i]) = true;
                        startTop = stackk.top(); //делаем верхушку стека стартовой вершиной
                    }
                }
            }
            else {
                std::cout << "Best top for (" << startTop <<") is (" << get<1>(bestEdge) << ")" << std::endl;
                stackk.push(get<1>(bestEdge)); //добавляем вершину на стек
                startTop = get<1>(bestEdge); //и делаем ее стартовой
            }
            if(get<1>(bestEdge) == endTop) { //если смежное является конечным
                    std::cout << "Find end top!" << std::endl;
                    finded = true; //значит нашли конечную
                    break;
            }

        }
        printStack();
    }
};


int main() {
    string vertex1, vertex2; // вершины для поиска пути
    cin >> vertex1 >> vertex2;
    std::string from;
    std::string to;
    double defaultEdgeLength = 1.0;
    EdgesList edgesList; //инициализируем граф
    while (cin >> from >> to >> defaultEdgeLength) { //счиываем пока cin не вернет false
        auto* currentEdge = new Edge{ from, to, defaultEdgeLength, false};  //создаем новое ребро и добавляем его в граф
        edgesList.addEdge(currentEdge);
    }
    //edgesList.printList();
    edgesList.findWay(vertex1, vertex2); //вызов функции поиска пути в графе
    std::cout << std::endl;
    return 0;
}