#include <iostream>
#include <vector>
#include <algorithm>
#include "math.h"
#include <limits>
#include <cstdint>

double cos_polar(double x, double y) { return x / sqrt(x * x + y * y); }

class Point {
 public:
  Point(const Point &obj) {
    x = obj.x;
    y = obj.y;
  }
  Point(double x1, double y1) {
    x = x1;
    y = y1;
  }
  double x;
  double y;
};

void arrayRotateLeft(std::vector<Point> &array, int index) {
  int i;
  int size = array.size();
  for (int i = 0; i < size - index; i++) {
    Point temp = array[i];
    array[i] = array[i + index];
    array[i + index] = temp;
  }
}

void findLowestPoints(Point &point1, Point &point2,
                      const std::vector<Point> &figure1,
                      const std::vector<Point> &figure2, int &point1_index,
                      int &point2_index) {
  int size1 = figure1.size();
  int size2 = figure2.size();

  for (int i = 0; i < size1; ++i) {
    if (figure1[i].y < point1.y) {
      if (figure1[i].x <= point1.x) {
        point1.x = figure1[i].x;
        point1.y = figure1[i].y;
        point1_index = i;
      }
    }
  }

  for (int i = 0; i < size2; ++i) {
    if (figure2[i].y <= point2.y) {
      if (figure2[i].x <= point2.x) {
        point2.x = figure2[i].x;
        point2.y = figure2[i].y;
        point2_index = i;
      }
    }
  }
}

void answer_initialization(std::vector<Point> &answer,
                           const std::vector<Point> &figure1,
                           const std::vector<Point> &figure2) {
  int i = 0;
  int j = 0;
  int size1 = figure1.size();
  int size2 = figure2.size();
  while (i < size1 || j < size2) {
    Point temp(0, 0);
    temp.x = figure1[i % size1].x + figure2[j % size2].x;
    temp.y = figure1[i % size1].y + figure2[j % size2].y;
    answer.push_back(temp);

    double angle1 = (cos_polar(
        figure1[(i + 1) % size1].x - figure1[i % size1].x,
        figure1[(i + 1) % size1].y -
            figure1[i % size1].y));  // пол€рный угол вектора (i,(i+1))
    double angle2 =
        cos_polar((figure2[(j + 1) % size2].x - figure2[j % size2].x),
                  (figure2[(j + 1) % size2].y -
                   figure2[j % size2].y));  // пол€рный угол вектора (j,(j+1))
    if (angle1 > angle2) {
      if (i != size1) {
        ++i;
      } else
        ++j;

    } else if (angle1 < angle2) {
      if (j != size2) {
        ++j;
      } else
        ++i;
    } else {
      ++i;
      ++j;
    }
  }
}

bool minkowski_calculate(std::vector<Point> &figure1,
                         std::vector<Point> &figure2) {
  Point point1(100000, 100000);
  int point1_index = 0;
  Point point2(100000, 100000);
  int point2_index = 0;

  findLowestPoints(point1, point2, figure1, figure2, point1_index,
                   point2_index);

  if (point2_index != 0) arrayRotateLeft(figure2, point2_index);
  if (point1_index != 0) arrayRotateLeft(figure1, point1_index);

  std::vector<Point> answer;
  answer_initialization(answer, figure1, figure2);

  int size = answer.size();
  bool result = false;
  int j = size - 1;
  //ѕровер€ем, лежит ли точка (0,0) в нашем многоугольнике. ѕерва€ строка
  //провер€ет попадание y-координаты между answer[i].y и answer[j].y
  //¬тора€ строка провер€ет нахождение стороны answer[i]answer[j] слева от точки
  //(0,0).
  //“реть€ строка формирует отрицательный ответ при чЄтном количестве сторон
  //слева и положительный Ч при нечЄтном.
  for (int i = 0; i < size; i++) {
    if ((answer[i].y < 0 && answer[j].y >= 0 ||
         answer[j].y < 0 && answer[i].y >= 0) &&
        (answer[i].x +
             (0 - answer[i].y) / (answer[j].y - answer[i].y) *
                 (answer[j].x - answer[i].x) <
         0))
      result = !result;
    j = i;
  }
  return result;
}

int main() {
  int n = 0;
  double a, b;
  std::vector<Point> figure1;
  std::vector<Point> figure2;
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    std::cin >> a >> b;
    figure1.push_back(Point(a, b));
  }

  std::cin >> n;
  for (int i = 0; i < n; i++) {
    std::cin >> a >> b;
    figure2.push_back(Point(-a, -b));
  }
  bool result = minkowski_calculate(figure1, figure2);
  std::cout << ((result == true) ? "YES" : "NO");
  return 0;
}