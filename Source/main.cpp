#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

#define SHOW_DEBUG

class GraphAModify
{
    struct edge
    {
        char first; //первая вершина
        char second; //вторая вершина
        float weight; //вес ребра
    };

    struct steps
    {
        string path; //строка, содержащая путь
        double length; //длина пути
    };
private:
    vector <edge> graph; //контейнер для ребер
    vector <steps> solutions[2]; //контейнер для текущего пути
    vector <char> checkedpoint[2]; //контейнер для просмотренных вершин
    char start; //начальная вершина
    char destination[2]; //вершина назначения
    int found[2]; //массив, позволящий отслеживать кол-ко уже найденных вершин

public:
    GraphAModify()
    {
        //ifstream fin("C:\\Users\\Evgeniy\\Desktop\\input.txt");
        //cin >> start >> destination[0] >> destination[1];
        string str;
        getline(cin, str);

        istringstream iss(str);
        iss >> start;
        iss >> destination[0];
        iss>>destination[1];

        if(iss.fail()) { //если  нет 2 пункта назначения
            destination[1] = destination[0];
        }

        char symb;
        while(cin >> symb)
        {
            if (symb == '!') break;
            edge cell;
            cell.first = symb;
            cin >> cell.second;
            cin >> cell.weight;
            graph.push_back(cell);
        }
        string buf = "";
        buf += start;
        for(int  i = 0; i < graph.size(); i++)
        {
            if(graph[i].first == start)
            {
                buf += graph[i].second;

                for(int k=0;k<2;k++)
                    solutions[k].push_back( { buf, graph[i].weight} );

                buf.resize(1);
            }
        }

        for(int k=0;k<2;k++)
            checkedpoint[k].push_back(start); //сохраняем вершины, которые уже обошли
    }



    int minInd(int k) //возвращает индекс минимального элемента из непросмотренных
    {
        float min = 111;
        int ind;
        for(int i = 0; i < solutions[k].size(); i++)
        {
            if(solutions[k][i].length + abs(destination[k] - solutions[k][i].path.back()) < min)
            {
                if(isCheck(k, solutions[k].at(i).path.back()))
                {
                    solutions[k].erase(solutions[k].begin() + i);
                }
                else
                {
                    min = solutions[k][i].length + abs(destination[k] - solutions[k][i].path.back());
                    ind = i;
                }
            }
        }
        return ind;
    }

    bool isCheck(int k, char value)
    {
        for(int i = 0; i < checkedpoint[k].size(); i++)
            if(checkedpoint[k][i] == value)
                return true;
        return false;
    }

    void search()
    {
        found[0] = -1; // путь до 1-ой цели еще не найден
        found[1] = -1; // путь до 2-ой цели еще не найден
        while(found[0]<0 || found[1]<0) //поиск продолжается до тех пор, пока существует хоть одна найденная вершина
        {
            for(int k=0;k<2;k++) {

                if(found[k]!=-1)
                    continue;
#ifdef SHOW_DEBUG
                // debug
                cout << "checkedpoint " << k << ": ";
                for(auto c : checkedpoint[k]) {
                    cout << c;
                }
                cout << endl;

                cout << "solutions " << k << ": ";
                for(auto r : solutions[k]) {
                    cout << r.path << "|" << r.length << ", ";
                }
                cout << endl;
#endif
                int min = minInd(k);
                if(solutions[k].at(min).path.back() == destination[k])
                {
                    //return;
                    //                  cout << "found "<<k<< " = " << min << endl;
                    //                  cout << solutions[k].at(min).path;
                    found[k] = min;
                    continue;
                }
                //for(int i = 0; i < graph.size(); i++)
                for(auto v : graph)
                {
                    if(v.first == solutions[k].at(min).path.back())
                    {
                        string buf = solutions[k].at(min).path;
                        buf += v.second;
                        solutions[k].push_back({buf, v.weight + solutions[k].at(min).length});
                    }
                }
                checkedpoint[k].push_back(solutions[k][min].path.back());
                solutions[k].erase(solutions[k].begin() + min);
              }
        }
#ifdef SHOW_DEBUG
        cout << "Results:\n";

        for(int k=0;k<2;k++) {
            cout <<"way "<< k << ") " << solutions[k].at(found[k]).path << ", " << solutions[k].at(found[k]).length << endl;
        }

#endif
     //cout<<solutions[0].at(found[0]).path;
    }
};
/*

a e c
a b 3.0
b c 1.0
c d 1.0
a d 5.0
d e 1.0
!

a e
a b 3.0
c d 1.0
a d 5.0
d e 1.0
!
 */
int main()
{
    GraphAModify element;
    element.search();
    return 0;
}
