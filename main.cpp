#include <iostream>
#include <vector>
#include <math.h>

#define MAX_NUMBER_OF_LEAS 500006
#define MAX_NUMBER_OF_QUERIES 500006
#define MAX_NUMBER_OF_FATHERS 22
#define NO_EDGE -1

using namespace std;

int visited[MAX_NUMBER_OF_LEAS], height[MAX_NUMBER_OF_LEAS],
        furthest_down[MAX_NUMBER_OF_LEAS], furthest_up[MAX_NUMBER_OF_LEAS],
        furthest_up_val[MAX_NUMBER_OF_LEAS], left_son[MAX_NUMBER_OF_LEAS],
        right_son[MAX_NUMBER_OF_LEAS], brother[MAX_NUMBER_OF_LEAS],
        furthest[MAX_NUMBER_OF_LEAS], furthest_val[MAX_NUMBER_OF_LEAS],
        fathers[MAX_NUMBER_OF_LEAS][MAX_NUMBER_OF_FATHERS];
int N, M;
vector<int> neighbours[MAX_NUMBER_OF_LEAS];
bool DEBUG = false;

void push_edge(int i, int second) {
    if (second != NO_EDGE) {
        if (DEBUG) {
            cout << "Pushing edge " << i << ", " << second << endl;
        }
        neighbours[i].push_back(second);
        neighbours[second].push_back(i);
    }
}

void build_edges() {
    scanf("%d", &N);
    int l, r;
    for (int i = 1; i <= N; i++) {
        scanf("%d%d", &l, &r);
        push_edge(i, l);
        push_edge(i, r);
    }
}

void build_tree(int current_lea, int current_height) {
    if (DEBUG) {
        cout << "I'm in " << current_lea << " and height is " <<
                current_height << endl;
    }
    visited[current_lea] = 1;
    height[current_lea] = current_height;
    bool was_left = false;
    for (int lea: neighbours[current_lea]) {
        if (visited[lea] == 0) {
            if (!was_left) {
                if (DEBUG) {
                    cout << lea << " <- " << current_lea << endl;
                }
                was_left = true;
                left_son[current_lea] = lea;
            } else {
                if (DEBUG) {
                    cout << current_lea << " -> " << lea << endl;
                }
                right_son[current_lea] = lea;
            }
            fathers[lea][0] = current_lea;
            build_tree(lea, current_height + 1);
        }
    }
    if (DEBUG) {
        cout << "Ended " << current_lea << endl;
    }
}

void update_furthest_down (int current_lea) {
    if (current_lea != 0) {
        update_furthest_down(left_son[current_lea]);
        update_furthest_down(right_son[current_lea]);
        int lfd = furthest_down[left_son[current_lea]], rfd =
                furthest_down[right_son[current_lea]];
        if (lfd == rfd) { //current_lea is a leaf in our tree
            furthest_down[current_lea] = current_lea;
        } else {
            furthest_down[current_lea] = height[lfd] >= height[rfd] ? lfd : rfd;
        }
        if (DEBUG) {
            cout << "Furthest down for " << current_lea << " is " <<
                    furthest_down[current_lea] << endl;
        }
    }
}

void pair_brothers(int left, int right) {
    if (left != 0 && right != 0) {
        if (DEBUG) {
            cout << left << " and " << right << " are brothers" << endl;
        }
        brother[left] = right;
        brother[right] = left;
    }
}

void find_brothers (int current_lea) {
    if (current_lea != 0) {
        pair_brothers(left_son[current_lea], right_son[current_lea]);
        find_brothers(left_son[current_lea]);
        find_brothers(right_son[current_lea]);
    }
}

void update_furthest_up (int current_lea, int my_father) {
    if (current_lea != 0) {
        if (my_father == 0) { //we are in a root
            furthest_up[current_lea] = current_lea;
            furthest_up_val[current_lea] = 0;
        } else {
            int fathers_fu = furthest_up[my_father],
                    brothers_fd = furthest_down[brother[current_lea]];
            int ffu_value = 1 + furthest_up_val[my_father],
                    bfd_value = 2 + height[brothers_fd] -
                                height[brother[current_lea]];
            if (brother[current_lea] != 0) {
                if (ffu_value > bfd_value) {
                    furthest_up[current_lea] = fathers_fu;
                    furthest_up_val[current_lea] = ffu_value;
                } else {
                    furthest_up[current_lea] = brothers_fd;
                    furthest_up_val[current_lea] = bfd_value;
                }
            } else {
                furthest_up[current_lea] = fathers_fu;
                furthest_up_val[current_lea] = ffu_value;
            }
        }
        if (DEBUG) {
            cout << "Furthest up for " << current_lea << " is " <<
                    furthest_up[current_lea] << " and its distance is " <<
                    furthest_up_val[current_lea] << endl;
        }
        update_furthest_up(left_son[current_lea], current_lea);
        update_furthest_up(right_son[current_lea], current_lea);
    }
}

void update_furthest (int current_lea) {
    if (current_lea != 0) {
        if (furthest_up_val[current_lea] >= height[furthest_down[current_lea]]
                                           - height[current_lea]) {
            furthest[current_lea] = furthest_up[current_lea];
            furthest_val[current_lea] = furthest_up_val[current_lea];
        } else {
            furthest[current_lea] = furthest_down[current_lea];
            furthest_val[current_lea] = height[furthest_down[current_lea]]
                           - height[current_lea];
        }
        if (DEBUG) {
            cout << "The furthest lea from " << current_lea << " is " <<
                    furthest[current_lea] << " and its distance is " <<
                    furthest_val[current_lea] << endl;
        }
        update_furthest(left_son[current_lea]);
        update_furthest(right_son[current_lea]);
    }
}

void find_distant_fathers(int current_lea, int height) {
    if (current_lea != 0) {
        int how_far_is_our_father = 2, i = 0;
        while (height >= how_far_is_our_father) {
            if (DEBUG) {
                cout << fathers[fathers[current_lea][i]][i] << " is father of " <<
                        current_lea << " distant by 2^" << i+1 << endl;
            }
            fathers[current_lea][i + 1] = fathers[fathers[current_lea][i]][i];
            i++;
            how_far_is_our_father *= 2;
        }
        find_distant_fathers(left_son[current_lea], height + 1);
        find_distant_fathers(right_son[current_lea], height + 1);
    }
}

int find_ancestor(int current_lea, int height) {
    if (DEBUG) {
        cout << "Ancestor of " << current_lea << " higher by " << height <<
                " is ";
    }
    int i = log2(N);
    while (height > 0) {
        if (pow(2, i) > height) {
            i--;
        } else {
            current_lea = fathers[current_lea][i];
            height -= pow(2, i);
        }
    }
    if (DEBUG) {
        cout << current_lea << endl;
    }
    return current_lea;
}

int get_closest_common_father(int lea1, int lea2) {
    int l1 = lea1, l2 = lea2;
    int h1 = height[lea1], h2 = height[lea2];
    if (h1 < h2) {
        lea2 = find_ancestor(lea2, h2 - h1);
        h2 = height[lea2];
    } else {
        lea1 = find_ancestor(lea1, h1 - h2);
        h1 = height[lea1];
    }
    if (DEBUG) {
        cout << "The closest common father of " << l1 << " and " << l2 <<
             " is ";
    }
    if (lea1 == lea2) {
        if (DEBUG) {
            cout << lea1 << endl;
        }
        return lea1;
    } else {
        int i = log2(N);
        while (i >= 0) {
            if (fathers[lea1][i] != fathers[lea2][i]) {
                lea1 = fathers[lea1][i];
                lea2 = fathers[lea2][i];
            }
            i--;
        }

        if (DEBUG) {
            cout << fathers[lea1][0] << endl;
        }
        return fathers[lea1][0];
    }
}

int query(int lea, int distance) {
    if (distance > furthest_val[lea]) {
        if (DEBUG) {
            cout << "The answer for the query: " << lea << ", " << distance
            << " is -1" << endl;
        }
        return -1;
    } else {
        int common_ancestor = get_closest_common_father(lea, furthest[lea]);
        int dist1 = height[lea] - height[common_ancestor], dist2 =
                height[furthest[lea]] - height[common_ancestor];
        int answer = (distance <= dist1 ? find_ancestor(lea, distance) :
                      find_ancestor(furthest[lea], furthest_val[lea] - distance));
        if (DEBUG) {
            cout << "The answer for the query: " << lea << ", " << distance
            << " is " << answer << endl;
        }
        return answer;
    }
}

void answer_queries() {
    scanf("%d", &M);
    int q1, q2;
    for (int i = 0; i < M; i++) {
        scanf("%d%d", &q1, &q2);
        printf("%d\n", query(q1, q2));
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    build_edges();
    build_tree(1, 0);
    update_furthest_down(1);
    find_brothers(1);
    update_furthest_up(1, 0);
    update_furthest(1);
    find_distant_fathers(1, 0);
    answer_queries();
    return 0;
}

