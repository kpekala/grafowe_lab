#include <iostream>
#include "vector"
#include <algorithm>
#include <queue>
using namespace std;

const int INF = 1e9;

struct Match{
    int first,second,winner,bribe;
};
struct Edge{
    int to, capacity, cost;
    Edge(int to, int capacity, int cost){
        this->to = to;
        this->capacity = capacity;
        this->cost = cost;
    }
};

bool runTournament();
bool createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches_to_bribe, int B, int &t);
void makeFlowGraph(vector<vector<Edge>>& G, Match matches[], vector<Match> &bribedKingMatches, int n, int B, int x);
void sortMatches(vector<Match> &matches);
bool inMatches(const vector<Match>& matches, Match match);
void testFlowGraph(vector<vector<Edge>> graph, int n, vector<Match> matches);
int minCostFlow(vector<vector<Edge>> &graph, int initialFlow, int neededFlow);
void shortest_paths(vector<vector<Edge>> &graph, int s, vector<int>& d, vector<int>& p);

int main() {
    int T;
    cin>>T;
    for(int i=0; i<T; i++)
        runTournament();

    return 0;
}


bool runTournament(){
    int n, B, t;
    cin>>B>>n;
    int m = n*(n-1)/2;
    vector<vector<int>> D;
    vector<Match> king_matches;
    Match matches[m];
    for(int i=0; i<m; i++){
        Match match{};
        cin>>match.first>>match.second>>match.winner>>match.bribe;
        matches[i] = match;
    }
    //Graf zwyciezców
    if(createD(D, matches, n, king_matches,B, t))
        return true;
    sortMatches(king_matches);
    vector<Match> bribedKingMatches;
    int initialBribe = 0;
    if(king_matches.empty())
        return false;
    for(int i=0; i < king_matches.size(); i++){
        vector<vector<Edge>> G;
        bribedKingMatches.push_back(king_matches[i]);
        initialBribe += king_matches[i].bribe;
        makeFlowGraph(G,matches, bribedKingMatches, n, B, t+i);
        testFlowGraph(G, n, bribedKingMatches);
    }
    return false;
}

void makeFlowGraph(vector<vector<Edge>> &G, Match matches[], vector<Match> &bribedKingMatches, int n, int B, int x) {
    int m = n * (n-1)/2;
    int g_n = 2 + n + (n-1) * n/2;
    int initialFlow = 0;
    vector<int> winMatches;
    winMatches.assign(n, 0);
    G.assign(g_n, vector<Edge>());
    //źródło
    for(int i=0; i<m; i++){
        int i_left = i + 1;
        int i_right = m+1;
        Match match = matches[i];
        int loser = match.first == match.winner ? match.second: match.first;
        Edge edge_left(i_left, 1, 0);
        G[0].push_back(edge_left);
        if(inMatches(bribedKingMatches, match)){
            //Ten mecz musimy "zamarkować" jako przekupiony
            //Edge edge_right(i_right,0,0);
            initialFlow++;
            //G[i_left].push_back(edge_right);
        }else{
            //Dodajemy krawędzie miedzy meczem a graczami
            //wygrany:
            if (match.bribe > B) {
                //Nie ma opcji żeby przekupić tego gracza :/ Więc niech flow tedy poplynie
                winMatches[match.winner]++;
                initialFlow++;
                Edge edge_right(i_right + match.winner,0,0);
                G[i_left].push_back(edge_right);
            }else if(match.winner != 0){
                Edge edge_right1(i_right + match.winner,1,0);
                G[i_left].push_back(edge_right1);
                Edge edge_right2(i_right + loser,1,match.bribe);
                G[i_left].push_back(edge_right2);
            }else{
                initialFlow++;
            }
        }
    }
    //Dodamy krawędzie miedzy graczami a końcem
    for(int i=0; i<n; i++){
        int i_right = m+1+i;
        Edge edge(g_n - 1,x - winMatches[i],0);
        if(i==0)
            edge.capacity = 0;
        if(edge.capacity < 0)
            cout<<"Mamy problem!"<<endl;
        G[i_right].push_back(edge);
    }

}


bool createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches_to_bribe, int B, int &t) {
    int maxOut = 0;
    D.assign(n, vector<int>());
    for(int i=0; i<n*(n-1)/2; i++){
        Match m = matches[i];
        if(m.first == m.winner)
            D[m.first].push_back(m.second);
        else
            D[m.second].push_back(m.first);
        if((m.first == 0 or m.second == 0) and m.winner != 0 and m.bribe <= B)
            king_matches_to_bribe.push_back(m);
        if (D[m.winner].size() > maxOut)
            maxOut = D[m.winner].size();
    }
    t = D[0].size();
    return t == maxOut;
}


void sortMatches(vector<Match> &matches){
    int n = matches.size();
    int i, j;
    for (i = 0; i < n-1; i++)
        for (j = 0; j < n-i-1; j++)
            if (matches[j].bribe > matches[j+1].bribe){
                Match tmp = matches[j];
                matches[j] = matches[j+1];
                matches[j+1] = tmp;
            }

}

bool inMatches(const vector<Match>& matches, Match match){
    for(Match m: matches){
        if (m.first == match.first and m.second == match.second)
            return true;
    }
    return false;
}

void printMatch(const Match match){
    cout<<match.first<<" "<<match.second<<" "<<match.winner<<" "<<match.bribe<<endl;
}

void testFlowGraph(vector<vector<Edge>> graph, int n, vector<Match> matches) {
    cout<<"Przekupstwa:"<<endl;
    for(Match match: matches){
        printMatch(match);
    }
    int m = (n)*(n-1)/2;
    cout<<endl<<"Zrodlo ma: "<<endl;
    cout<<"- "<<graph[0].size()<<"Krawedzi"<<endl;
    for(int i=1; i<=n*(n-1)/2; i++){
        cout<<"Mecz "<<i-1<<endl;
        for(Edge edge: graph[i]){
            cout<<"Krawedz do gracza "<<edge.to - (1 + m)<<" o koszcie: "<<edge.cost<<" i o c: "<<edge.capacity<<endl;
        }
    }

    for(int i=0; i<n; i++){
        int i_right = m+1+i;
        Edge e = graph[i_right][0];
        cout<<"Gracz "<<i<<endl;
        cout<<e.to<<" "<<e.capacity<<" "<<e.cost<<endl;
    }
}

Edge* getEdge(vector<vector<Edge>> &graph, int u, int v){
    for(Edge edge: graph[u]){
        if (edge.to == v)
            return &edge;
    }
    cout<<"Error!"<<endl;
    return NULL;
}

int minCostFlow(vector<vector<Edge>> &graph, int initialFlow, int neededFlow) {
    int n = graph.size();
    int flow = initialFlow;
    int cost = 0;
    vector<int> d, p;
    while (flow < neededFlow) {
        shortest_paths(graph, 0, d, p);
        if (d[n-1] == INF)
            break;

        // find max flow on that path
        int f = neededFlow - flow;
        int iter = n - 1;
        while (iter != 0) {
            f = min(f, getEdge(graph, p[iter], iter).capacity);
            iter = p[iter];
        }

        // apply flow
        flow += f;
        cost += f * d[n-1];
        iter = n-1;
        while (iter != 0) {
            Edge *e1 = getEdge(graph, p[iter], iter);
            e1->capacity -= f;
            Edge *e2 = getEdge(graph, iter, p[iter]);
            e2->capacity += f;
            iter = p[iter];
        }
    }

    if (flow < neededFlow)
        return -1;
    else
        return cost;
}

void shortest_paths(vector<vector<Edge>> &graph, int s, vector<int>& d, vector<int>& p) {
    int n = graph.size();
    d.assign(n, INF);
    d[s] = 0;
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    p.assign(n, -1);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visited[u] = false;
        for (Edge e : graph[u]) {
            if (e.capacity > 0 && d[e.to] > d[u] + e.cost) {
                d[e.to] = d[u] + e.cost;
                p[e.to] = u;
                if (!visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    }
}

