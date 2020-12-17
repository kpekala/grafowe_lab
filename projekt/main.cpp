#include <iostream>
#include "vector"
using namespace std;

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

void runTournament();
void createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches_to_bribe, int B);
void makeFlowGraph(vector<vector<Edge>> G, Match matches[], vector<Match> &bribedKingMatches, int n, int B, int x);
void sortMatches(vector<Match> &matches);

int main() {
    int T;
    cin>>T;
    for(int i=0; i<T; i++)
        runTournament();

    return 0;
}


void runTournament(){
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
    createD(D, matches, n, king_matches,B);
    t = (n-1) - (int)king_matches.size();
    sortMatches(king_matches);
    vector<Match> bribedKingMatches;
    for(int i=0; i< king_matches.size(); i++){
        vector<vector<Edge>> G;
        bribedKingMatches.push_back(king_matches[i]);
        makeFlowGraph(G,matches, bribedKingMatches, n, B, t+i);
    }
}

void makeFlowGraph(vector<vector<Edge>> G, Match matches[], vector<Match> &bribedKingMatches, int n, int B, int x) {
    int m = n * (n-1)/2;
    int g_n = 2 + n + (n-1) * n/2;
    G.assign(g_n, vector<Edge>());
    //źródło
    for(int i=0; i<m; i++){
        Match match = matches[i];
        if(match.bribe > B){
            Edge edge(1 + i, 1, 0);
            G[0].push_back(edge)
            //G[1+ i].
        }
    }
}

void createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches_to_bribe, int B) {
    D.assign(n, vector<int>());
    for(int i=0; i<n*(n-1)/2; i++){
        Match m = matches[i];
        if(m.first == m.winner)
            D[m.first].push_back(m.second);
        else
            D[m.second].push_back(m.first);
        if((m.first == 0 or m.second == 0) and m.winner != 0 and m.bribe <= B)
            king_matches_to_bribe.push_back(m);
    }
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

