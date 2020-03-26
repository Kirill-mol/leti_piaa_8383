#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <sstream>
#include <string>

using namespace std;

using Edge = tuple<string, string, double, bool>;

class EdgesList {
private:
    vector<Edge*> edges;
    stack<string> stackk;
public:
    EdgesList() = default;
    void addEdge(Edge* edge) {
        edges.push_back(edge);
    }
    void printList() {
        for (unsigned int i = 0; i < edges.size(); i++) {
            std::cout << get<0>(*edges[i]) << " ";
            std::cout << get<1>(*edges[i]) << " ";
            std::cout << get<2>(*edges[i]) << " ";
            std::cout << get<3>(*edges[i]) << endl;
        }
    }

    void printStack(){
        std::stack<string> newS;
        while (!stackk.empty()) {
            newS.push(stackk.top());
            stackk.pop();
        }
        while (!newS.empty()) {
            cout << newS.top();
            newS.pop();
        }
    }

    void findWay(string startTop, string endTop) {
        stackk.push(startTop);
        const string start = startTop;
        bool finded = false;
        while(!finded) {
            Edge bestEdge = {"", "", 1000.0, false};
            for (auto & edge : edges) {
                if ((get<0>(*edge) == startTop) && (get<2>(*edge) < get<2>(bestEdge)) && get<3>(*edge) != true) {
                    bestEdge = *edge;
                }
                if(get<1>(bestEdge) == endTop) {
                    finded = true;
                    stackk.push(endTop);
                    break;
                }
            }
            if(finded) break;
            if(get<0>(bestEdge).empty()){
                string popedTop = stackk.top();
                stackk.pop();
                for(int i = 0; i < edges.size(); i++){
                    if(get<1>(*edges[i]) == popedTop && get<0>(*edges[i]) == stackk.top()) {
                        get<3>(*edges[i]) = true;
                        startTop = stackk.top();
                    }
                }
            }
            else {
                get<3>(bestEdge) = true;
                stackk.push(get<1>(bestEdge));
                startTop = get<1>(bestEdge);
            }

        }
        printStack();
    }
};


int main() {
    string vertex1, vertex2, curEdge;
    cin >> vertex1 >> vertex2;
    std::string from;
    std::string to;
    double defaultEdgeLength = 1.0;
    EdgesList edgesList;
    while (true/*cin >> from >> to >> defaultEdgeLength*/) {
        cin >> from >> to >> defaultEdgeLength;
        if(defaultEdgeLength == -1.0) break;
        auto* currentEdge = new Edge{ from, to, defaultEdgeLength, false};
        edgesList.addEdge(currentEdge);
    }
    edgesList.printList();
    edgesList.findWay(vertex1, vertex2);
    return 0;
}