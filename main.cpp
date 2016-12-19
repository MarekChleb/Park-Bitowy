//
// Created by Marek on 2016-12-19.
//

#include <iostream>
#include <vector>

#define MAX_NUMBER_OF_LEAS 500006
#define MAX_NUMBER_OF_QUERIES 500006
#define NO_EDGE -1

using namespace std;

/**
 * TODO
 * dla danego wierzchołka potrzebujemy:
 *  ? tablica ojców w odległości 2^i
 *  + odległości najdalej oddalonego wierzchołka w górę i wskaźnieka do niego
 *  + odległości najdalej oddalonego wierzchołka w dół i wskaźnieka do niego
 *  + jego wysokości
 *  + brat
 *  + drzewo
 *  - test
 *  - the rest
 */

int visited[MAX_NUMBER_OF_LEAS], height[MAX_NUMBER_OF_LEAS];
int N, M;
vector<int> neighbours[MAX_NUMBER_OF_LEAS], binary_tree[MAX_NUMBER_OF_LEAS];
bool DEBUG = true;

void push_edge(int i, int second) {
    if (second != NO_EDGE) {
        neighbours[i].push_back(second);
        neighbours[second].push_back(i);
    }
}

void build_edges() {
    scanf("%d", &N);
    int l, r;
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &l, &r);
        push_edge(i, l);
        push_edge(i, r);
    }
}

void build_tree(int actual_lea) {
    if (DEBUG) {
        cout << "I'm in " << actual_lea << endl;
    }
    visited[actual_lea] = 1;
    for (int lea: neighbours[actual_lea]) {
        if (!visited[lea]) {
            binary_tree[actual_lea].push_back(lea);
        }
    }
    if (DEBUG) {
        cout << "Ended " << actual_lea << endl;
    }
}

void build_height(int actual_lea, int actual_height) {
    if (!visited[actual_lea]) {
        height[actual_lea] = actual_height;
        visited[actual_lea] = 1;
    }
}




int main() {
    build_edges();
    build_tree(1);
    return 0;
}

