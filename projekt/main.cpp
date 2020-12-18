#include <iostream>
#include "vector"
#include <algorithm>
#include <queue>
using namespace std;

vector<vector<int>> adj, cost, capacity;

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

bool runTournament(int ind);
bool createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches_to_bribe, int B, int &t);
bool makeFlowGraph(Match matches[], int t, int n, int B, int x, int &initialFlow);
void testFlowGraph(vector<vector<Edge>> graph, int n, const vector<Match>& matches);
int minCostFlow(int initialFlow, int neededFlow, int n);
void printGraph(const vector<vector<Edge>> &graph);
void shortest_paths(int n, int v0, vector<int>& d, vector<int>& p);
int main() {
    int T;
    cin>>T;
    for(int i=0; i<T; i++)
        if(runTournament(i)){
            cout<<"TAK"<<endl;
        }else{
            cout<<"NIE"<<endl;
        }

    return 0;
}


bool runTournament(int ind){
    int n, B, t,initialFlow = 0;
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
    if(createD(D, matches, n, king_matches,B, t))
        return true;

    int min_cost = INF;
    for(int i=0; i<=(n-1) - t;i++){
        cout<<"siema"<<endl;
        if(!makeFlowGraph(matches, t, n, B, t+i, initialFlow))
            continue;
        cout<<"siema"<<endl;
        //printGraph(G);
        int flowCost = minCostFlow(0,m,n);
        if(flowCost == -1){
            continue;
        }
        min_cost = min(flowCost, min_cost);
    }
    //cout<<min_cost<<", "<<B<<endl;
    return min_cost <= B;
}

bool makeFlowGraph(Match matches[], int t, int n, int B, int x, int &initialFlow) {
    int m = n * (n-1)/2;
    int g_n = 3 + n + (n-1) * n/2;
    vector<int> winMatches(n, 0);
    adj.assign(g_n, vector<int>());
    cost.assign(g_n, vector<int>(g_n, 0));
    capacity.assign(g_n, vector<int>(g_n, 0));

    //źródło
    for(int i=0; i<m; i++){
        int i_left = i + 1;
        int i_right = m+1;
        Match match = matches[i];
        int loser = match.first == match.winner ? match.second: match.first;
        //Edge edge_left(i_left, 1, 0);
        adj[0].push_back(i_left);
        capacity[0][i_left] = 1;
        //G[0].push_back(edge_left);

        //Dodajemy krawędzie miedzy meczem a graczami
        //Edge edge_right(i_right + match.winner,1,0);
        adj[i_left].push_back(i_right + match.winner);
        capacity[i_left][i_right + match.winner] = 1;
        //G[i_left].push_back(edge_right);
        if (match.bribe <= B) {
            Edge edge_right2(i_right + loser,1,match.bribe);
            if(match.winner == 0)
                edge_right2.cost = INF;
            adj[i_left].push_back(i_right + loser);
            capacity[i_left][i_right + loser] = 1;
            cost[i_left][i_right + loser] = edge_right2.cost;
            cost[i_right + loser][i_left] = -edge_right2.cost;
            //G[i_left].push_back(edge_right2);
        }
    }
    //Dodamy krawędzie miedzy graczami a końcem
    for(int i=0; i<n; i++){
        int i_right = m+1+i;
        Edge edge(g_n - 2,x,0);
        if(i==0)
            edge.to = g_n-1;
        //G[i_right].push_back(edge);
        adj[i_right].push_back(g_n - 2);
        capacity[i_right][g_n - 2] = x;
    }
    //G[g_n-2].push_back(Edge(g_n-1,m-x,0));
    adj[g_n-2].push_back(g_n - 1);
    capacity[g_n-2][g_n - 1] = m-x;
    return true;
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

void printMatch(const Match match){
    cout<<match.first<<" "<<match.second<<" "<<match.winner<<" "<<match.bribe<<endl;
}

void printGraph(const vector<vector<Edge>> &graph){
    cout<<"Prinintg graph: "<<endl;
    int n = graph.size();
    for(int i=0; i<n; i++){
        cout<<i<<": "<<endl;
        for(Edge edge: graph[i]){
            cout<<edge.to<<" "<<edge.capacity<<" "<<edge.cost<<endl;
        }
    }
}

int minCostFlow(int initialFlow, int neededFlow, int n) {

    int flow = 0;
    int min_cost = 0;
    vector<int> d, p;
    while (flow < neededFlow) {
        shortest_paths(n, 0,d,p);
        if (d[n-1] == INF)
            break;

        int f = neededFlow - flow;
        int cur = n-1;
        while (cur != 0) {
            f = min(f, capacity[p[cur]][cur]);
            cur = p[cur];
        }

        flow += f;
        min_cost += f * d[n-1];
        cur = n-1;
        while (cur != 0) {
            capacity[p[cur]][cur] -= f;
            capacity[cur][p[cur]] += f;
            cur = p[cur];
        }
    }

    if (flow < neededFlow)
        return -1;
    else
        return min_cost;
}

void shortest_paths(int n, int v0, vector<int>& d, vector<int>& p) {
    d.assign(n, INF);
    d[v0] = 0;
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(v0);
    p.assign(n, -1);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visited[u] = false;
        for (int v : adj[u]) {
            if (capacity[u][v] > 0 && d[v] > d[u] + cost[u][v]) {
                d[v] = d[u] + cost[u][v];
                p[v] = u;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
}