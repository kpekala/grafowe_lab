#include <iostream>
#include "vector"
using namespace std;

struct Match{
    int first,second,winner,bribe;
};

void runTournament();

void createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches);



int main() {
    int T;
    cin>>T;
    for(int i=0; i<T; i++)
        runTournament();

    return 0;
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
    createD(D, matches, n, king_matches);
    t = (n-1) - (int)king_matches.size();
    sortMatches(king_matches);
    cout<<t<<endl;
}

void createD(vector<vector<int>> &D, Match matches[], int n, vector<Match> &king_matches) {
    D.assign(n, vector<int>());
    for(int i=0; i<n*(n-1)/2; i++){
        Match m = matches[i];
        if(m.first == m.winner)
            D[m.first].push_back(m.second);
        else
            D[m.second].push_back(m.first);
        if((m.first == 0 or m.second == 0) and m.winner != 0)
            king_matches.push_back(m);
    }
}
