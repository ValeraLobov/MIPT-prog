#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <queue>
#include <array>

struct bohr_node {
  std::array<int, 26> next;  // массив для обозначения переходов к вершинам
  std::array<int, 26> move;  // массив для обозначения переходов автомата
  bool leaf;                 // флаг для обозначения "листа" - вершина является концом
                             // некоторой строки
  int suff_link;             // суффиксная ссылка
  int suff_glink;  // suff_glink - "good" суф. ссылка (сжатая)
  int parent;
  std::vector<int> pattern_number;  //номер строки-паттерна, которая
                                    //соответствует этой вершине
  char symb;  //символ на ребре от parent к этой вершине
};

class Bohr {
 private:
  bohr_node node;
  std::vector<bohr_node> bohr;
  std::vector<std::pair<std::vector<char>, int>> small_patterns;

 public:
  Bohr() {}
  void make_bohr_node(const int parent, const char c);
  int get_move(int v, char ch);
  int get_suff_link(int v);
  int get_suff_glink(int v);
  void add_string(const std::vector<char> &s, const int pattern_num);
  void check(
      const std::vector<std::pair<std::vector<char>, int>> &small_patterns,
      std::vector<int> &counts, const int v, const int i);
  void find_all_pos(
      const std::vector<std::pair<std::vector<char>, int>> &small_patterns,
      std::vector<int> &counts, const std::string &s);
  void BFS(int s);
  void inizialization(const std::string text, const std::string main_pattern,
                      std::vector<int> &counts);
};

void Bohr::make_bohr_node(const int parent, const char c) {
  bohr_node vertex;
  memset(vertex.next.data(), 255, sizeof(vertex.next));
  memset(vertex.move.data(), 255, sizeof(vertex.move));
  vertex.parent = parent;
  vertex.symb = c;
  vertex.leaf = false;
  vertex.suff_link = -1;
  vertex.suff_glink = -1;
  bohr.push_back(vertex);
}

int Bohr::get_suff_link(const int v) {
  if (bohr[v].suff_link == -1)  //если суффиксная ссылка еще не посчитана
  {
    if (v == 0 || bohr[v].parent == 0) {
      bohr[v].suff_link = 0;
    } else
      bohr[v].suff_link = get_move(get_suff_link(bohr[v].parent), bohr[v].symb);
  }
  return bohr[v].suff_link;
}

int Bohr::get_move(const int v, const char ch) {
  if (bohr[v].move[ch] == -1) {
    if (bohr[v].next[ch] != -1) {
      bohr[v].move[ch] = bohr[v].next[ch];
    } else {
      if (v == 0) {
        bohr[v].move[ch] = 0;
      } else {
        bohr[v].move[ch] = get_move(bohr[v].suff_link, ch);
      }
    }
  }
  return bohr[v].move[ch];
}

int Bohr::get_suff_glink(const int v) {
  if (bohr[v].suff_glink == -1) {
    const int u = get_suff_link(v);
    if (bohr[u].leaf == true) {
      bohr[v].suff_glink = u;
    } else {
      if (u == 0) {
        bohr[v].suff_glink = 0;
      } else {
        bohr[v].suff_glink = get_suff_glink(u);
      }
    }
  }
  return bohr[v].suff_glink;
}

void Bohr::BFS(const int s) {
  std::queue<int> q;
  const int n = bohr.size();
  q.push(s);
  get_suff_glink(s);
  std::vector<bool> used(n);
  used[s] = true;
  while (!q.empty()) {
    const int v = q.front();
    q.pop();
    for (size_t i = 0; i < 26; ++i) {
      if (bohr[v].next[i] == -1) {
        continue;
      }
      int to = bohr[v].next[i];
      if (!used[to]) {
        get_suff_glink(to);
        used[to] = true;
        q.push(to);
      }
    }
  }
}

void Bohr::add_string(const std::vector<char> &s, const int pattern_num) {
  int v = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    char ch = s[i] - 'a';
    if (bohr[v].next[ch] == -1) {
      make_bohr_node(v, ch);
      bohr[v].next[ch] = bohr.size() - 1;
    }
    v = bohr[v].next[ch];
  }
  bohr[v].leaf = true;
  bohr[v].pattern_number.push_back(pattern_num);
}

void Bohr::check(
    const std::vector<std::pair<std::vector<char>, int>> &small_patterns,
    std::vector<int> &counts, const int v, const int i) {
  for (int u = v; u != 0; u = bohr[u].suff_glink) {
    bool flag = false;
    if (bohr[u].leaf) {
      for (int j = 0; j < bohr[u].pattern_number.size(); j++) {
        int tmp_ptrn_number = bohr[u].pattern_number[j];
        int tmp = (i - small_patterns[tmp_ptrn_number].first.size()) -
                  small_patterns[tmp_ptrn_number].second;
        if (tmp < 0 || tmp >= counts.size()) {
          continue;
        }
        counts[tmp]++;
      }
    }
  }
}

void Bohr::find_all_pos(
    const std::vector<std::pair<std::vector<char>, int>> &small_patterns,
    std::vector<int> &counts, const std::string &s) {
  int u = 0;
  for (int i = 0; i < s.length(); i++) {
    u = get_move(u, s[i] - 'a');
    check(small_patterns, counts, u, i + 1);
  }
}

void Bohr::inizialization(const std::string text,
                          const std::string main_pattern,
                          std::vector<int> &counts) {
  std::vector<char> s;
  int j = 0;
  for (size_t i = 0; i < main_pattern.length();
       ++i)  //поиск маленьких паттернов
  {
    if (main_pattern[i] != '?') {
      if (s.size() == 0 && j == 0) j = i;
      s.push_back(main_pattern[i]);
    } else {
      if (s.size() == 0) continue;
      add_string(s, small_patterns.size());
      small_patterns.push_back(make_pair(s, j));
      s.clear();
      j = 0;
    }
  }
  if (s.size() != 0) {
    add_string(s, small_patterns.size());
    small_patterns.push_back(make_pair(s, j));
  }
  BFS(0);
  find_all_pos(small_patterns, counts, text);

  for (int i = 0; i < text.size() - main_pattern.size() + 1; ++i) {
    if (counts[i] == small_patterns.size()) std::cout << i << " ";
  }
}
int main() {
  std::cin.tie(NULL);
  std::ios_base::sync_with_stdio(false);
  Bohr main_tree;
  main_tree.make_bohr_node(0, '$');  // инициализация бора
  std::string main_pattern;  //главный паттерн со знаками вопросов
  std::string text;  //текст
  std::cin >> main_pattern;
  std::cin >> text;
  std::vector<std::pair<std::vector<char>, int>>
      small_patterns;  // куски главного паттерна без масок
  std::vector<int> counts(text.length(), 0);
  if (main_pattern.length() > text.length()) {
    return 0;
  }
  main_tree.inizialization(text, main_pattern, counts);
  return 0;
}