
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <queue>
#include <functional>
#include <unordered_set>
#define FIN 1147797409030816545

using namespace std;

const unsigned long long AntiMasks[] = {
    0xFFFFFFFFFFFFFFF0, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFF0FF,
    0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFF0FFFF, 0xFFFFFFFFFF0FFFFF,
    0xFFFFFFFFF0FFFFFF, 0xFFFFFFFF0FFFFFFF, 0xFFFFFFF0FFFFFFFF,
    0xFFFFFF0FFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFF0FFFFFFFFFFF,
    0xFFF0FFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF,
    0x0FFFFFFFFFFFFFFF};

const unsigned long long Masks[] = {
    0x000000000000000F, 0x00000000000000F0, 0x0000000000000F00,
    0x000000000000F000, 0x00000000000F0000, 0x0000000000F00000,
    0x000000000F000000, 0x00000000F0000000, 0x0000000F00000000,
    0x000000F000000000, 0x00000F0000000000, 0x0000F00000000000,
    0x000F000000000000, 0x00F0000000000000, 0x0F00000000000000,
    0xF000000000000000,
};

struct Puzzle {
  unsigned long long data;
  string previousSteps;
  unsigned long long previousState;
  int distance;
  Puzzle(string prev) : distance(0), previousState(0), previousSteps(prev) {}
  void setAt(int place, unsigned char value) {
    data = (data & AntiMasks[place]) |
           (static_cast<long long>(value) << (place << 2));
  }

  unsigned char getAt(int place) const {
    return static_cast<unsigned char>((data & Masks[place]) >> (place << 2));
  }
};

class Compare {
 public:
  bool operator()(Puzzle p1, Puzzle p2) {
    if (p2.distance != p1.distance) {
      return p2.distance < p1.distance;
    } else if (p2.distance == p1.distance) {
      return p2.previousSteps.length() > p1.previousSteps.length();
    }
  }
};

int dist(Puzzle p) {
  char c;
  int d = 0;
  int lc = 0;
  for (int i = 0; i < 16; i++) {
    c = p.getAt(i);
    if (c != 0) {
      int y_finish = (c - 1) / 4;
      int x_finish = (c - 1) % 4;
      int dx = x_finish - i % 4;
      int dy = y_finish - i / 4;
      if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) lc++;

      d += abs(dx) + abs(dy);
    }
  }
  return p.previousSteps.length() + d + lc * 2;
}

Puzzle moveUp(Puzzle current, int pos) {
  current.previousState = current.data;
  current.setAt(pos, current.getAt(pos - 4));
  current.setAt(pos - 4, 0);
  current.previousSteps = current.previousSteps + "D";
  current.distance = current.previousSteps.length() + dist(current);
  return current;
}

Puzzle moveDown(Puzzle current, int pos) {
  current.setAt(pos, current.getAt(pos + 4));
  current.previousState = current.data;
  current.setAt(pos + 4, 0);

  current.previousSteps = current.previousSteps + "U";
  current.distance = current.previousSteps.length() + dist(current);
  return current;
}

Puzzle moveLeft(Puzzle current, int pos) {
  current.previousState = current.data;
  current.setAt(pos, current.getAt(pos - 1));
  current.setAt(pos - 1, 0);
  current.previousSteps = current.previousSteps + "R";
  current.distance = current.previousSteps.length() + dist(current);
  return current;
}

Puzzle moveRight(Puzzle current, int pos) {
  current.previousState = current.data;
  current.setAt(pos, current.getAt(pos + 1));
  current.setAt(pos + 1, 0);

  current.previousSteps = current.previousSteps + "L";
  current.distance = current.previousSteps.length() + dist(current);
  return current;
}

vector<Puzzle> getNextSteps(Puzzle temp) {
  vector<Puzzle> q;
  if (temp.getAt(0) == 0) {
    Puzzle current = moveRight(temp, 0);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 0);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(1) == 0) {
    Puzzle current = moveRight(temp, 1);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 1);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 1);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(2) == 0) {
    Puzzle current = moveRight(temp, 2);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 2);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 2);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(3) == 0) {
    Puzzle current = moveLeft(temp, 3);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 3);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(4) == 0) {
    Puzzle current = moveRight(temp, 4);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 4);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 4);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(5) == 0) {
    Puzzle current = moveRight(temp, 5);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 5);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 5);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 5);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(6) == 0) {
    Puzzle current = moveRight(temp, 6);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 6);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 6);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 6);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(7) == 0) {
    Puzzle current = moveLeft(temp, 7);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 7);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 7);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(8) == 0) {
    Puzzle current = moveRight(temp, 8);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 8);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 8);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(9) == 0) {
    Puzzle current = moveRight(temp, 9);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 9);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 9);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 9);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(10) == 0) {
    Puzzle current = moveRight(temp, 10);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 10);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 10);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 10);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(11) == 0) {
    Puzzle current = moveLeft(temp, 11);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveDown(temp, 11);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 11);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(12) == 0) {
    Puzzle current = moveRight(temp, 12);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 12);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(13) == 0) {
    Puzzle current = moveRight(temp, 13);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 13);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 13);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(14) == 0) {
    Puzzle current = moveRight(temp, 14);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveLeft(temp, 14);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 14);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  } else if (temp.getAt(15) == 0) {
    Puzzle current = moveLeft(temp, 15);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }

    current = moveUp(temp, 15);
    if (temp.previousState != current.data) {
      q.push_back(current);
    }
  }
  return q;
}

unordered_set<unsigned long long> processed;
priority_queue<Puzzle, vector<Puzzle>, Compare> q;
Puzzle final_state("");
Puzzle search(Puzzle node, int bound) {
  if (node.distance > bound) return node;

  if (node.data == FIN) {
    return node;
  }

  Puzzle min("");
  min.distance = 100000000;
  vector<Puzzle> nextSteps = getNextSteps(node);
  for (int i = 0; i < nextSteps.size(); i++) {
    Puzzle temp = search(nextSteps[i], bound);
    if (temp.data == FIN) {
      return temp;
    }
    if (temp.distance < min.distance) {
      min = temp;
    }
  }
  return min;
}

Puzzle ida_star(Puzzle root) {
  int bound = dist(root);
  root.distance = bound;
  while (true) {
    Puzzle t = search(root, bound);
    if (t.data == FIN) {
      return t;
    }
    bound = t.distance;  // f*
  }
}

void read_puzzle(Puzzle &start, int &zero_pos) {
  int c = 0;
  for (int i = 0; i < 16; i++) {
    cin >> c;
    start.setAt(i, c);
    if (c == 0) {
      zero_pos = i / 4;
    }
  }
  start.distance = dist(start);
}

Puzzle solve_puzzle(Puzzle &start, int &zero_pos) {
  int inv = zero_pos + 1;
  for (int i = 0; i < 16; i++) {
    char tmp = start.getAt(i);
    for (int j = i + 1; j < 16; j++) {
      char tmp2 = start.getAt(j);
      if (tmp2 < tmp & tmp != 0 & tmp2 != 0) inv++;
    }
  }
  if (inv % 2 != 0) {
    Puzzle t2("");
    t2.distance = -1;
    return t2;
  }
  return ida_star(start);
}
int main() {
  Puzzle start("");
  int zero_pos = 0;
  read_puzzle(start, zero_pos);
  Puzzle temp = solve_puzzle(start, zero_pos);
  if (temp.distance != -1) {
    printf("%d", temp.previousSteps.size());
    printf("%c", '\n');
    for (int i = 0; i < temp.previousSteps.length(); i++)
      putchar(temp.previousSteps[i]);
  } else
    printf("%d", -1);
  return 0;
}