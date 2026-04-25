#include <bits/stdc++.h>
using namespace std;

struct StateHash {
    size_t operator()(const string &s) const noexcept { return std::hash<string>()(s); }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 10;
    vector<vector<int>> g(N, vector<int>(N));
    int sx = -1, sy = -1;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(!(cin>>g[i][j])) return 0; // no input
            if(g[i][j]==4){ sx=i; sy=j; }
        }
    }
    if(sx==-1){ return 0; }

    // directions: 0: up, 1: right, 2: down, 3: left
    const int dx[4] = {-1,0,1,0};
    const int dy[4] = {0,1,0,-1};

    // Map pellets to bitset indices
    int id[10][10];
    memset(id, -1, sizeof(id));
    int pid = 0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(g[i][j]==2){ id[i][j]=pid++; }
        }
    }
    bitset<100> pellets; // supports up to 100 pellets
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) if(id[i][j]!=-1) pellets.set(id[i][j]);

    auto inb = [&](int x,int y){ return x>=0 && x<N && y>=0 && y<N; };
    auto passable = [&](int x,int y){ return inb(x,y) && g[x][y]!=0; };

    int x=sx, y=sy;
    int dir=0; // initial facing up
    long long score = 0;

    unordered_set<string, StateHash> vis;
    auto stateKey = [&](int x,int y,int dir,const bitset<100>& bs){
        string s;
        s.reserve(32 + (100+7)/8);
        s.push_back(char(x)); s.push_back(char(y)); s.push_back(char(dir));
        // serialize bitset to bytes
        unsigned char byte = 0; int cnt=0;
        for(int i=0;i<100;i++){
            byte |= (bs.test(i)?1:0) << (i%8);
            cnt++;
            if(cnt%8==0){ s.push_back((char)byte); byte=0; }
        }
        if(cnt%8!=0) s.push_back((char)byte);
        return s;
    };

    // record initial state
    vis.insert(stateKey(x,y,dir,pellets));

    // simulate steps
    for(;;){
        // determine next move by right-hand rule: right > straight > left > back
        int order[4] = {(dir+1)&3, dir, (dir+3)&3, (dir+2)&3};
        bool moved=false;
        int ndir=-1, nx=-1, ny=-1;
        for(int k=0;k<4;k++){
            int d = order[k];
            int tx = x + dx[d];
            int ty = y + dy[d];
            if(passable(tx,ty)){
                ndir=d; nx=tx; ny=ty; moved=true; break;
            }
        }
        if(!moved){
            // no available moves, stop
            cout << score << '\n';
            return 0;
        }

        x=nx; y=ny; dir=ndir;
        if(g[x][y]==3){
            score -= 500;
            cout << score << '\n';
            return 0;
        }
        if(g[x][y]==2){
            score += 2;
            // consume pellet
            if(id[x][y]!=-1) pellets.reset(id[x][y]);
            g[x][y]=1; // turn into empty
        }

        string key = stateKey(x,y,dir,pellets);
        if(vis.find(key)!=vis.end()){
            cout << "Silly Pacman\n";
            return 0;
        }
        vis.insert(std::move(key));
    }
}
