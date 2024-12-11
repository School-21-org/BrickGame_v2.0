#ifndef __OBJECTS_HPP__
#define __OBJECTS_HPP__
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <set>
using namespace std;

#define S_WIDTH 10U
#define S_HEIGHT 20U
#define SNAKE_LEN 4U
#define S_DEFAULT_SPEED 600U
#define MAX_POINTS 200U
#define MAX_LEVEL 10U

typedef enum { LEFT = 3, RIGHT, UP, DOWN } t_dir;

namespace s21 {

class Snake {
 public:
  static void create_snake() {
    clear();
    length = 4U;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_w(0, S_WIDTH - 1);
    uniform_int_distribution<> dis_h(12, S_HEIGHT - 1);
    size_t x = dis_w(gen);
    size_t y = dis_h(gen);

    s_coordinates.insert(make_pair(y, x));
    q_coordinates.push(make_pair(y, x));
    s_coordinates.insert(make_pair(--y, x));
    q_coordinates.push(make_pair(y, x));
    s_coordinates.insert(make_pair(--y, x));
    q_coordinates.push(make_pair(y, x));
    s_coordinates.insert(make_pair(--y, x));
    q_coordinates.push(make_pair(y, x));
  }

  static bool move_snake(t_dir action) {
    pair<size_t, size_t> head = q_coordinates.back();
    if ((move_dir == UP || move_dir == DOWN) && action == LEFT) {
      --head.second;
      move_dir = LEFT;
    } else if ((move_dir == UP || move_dir == DOWN) && action == RIGHT) {
      ++head.second;
      move_dir = RIGHT;
    } else if ((move_dir == LEFT || move_dir == RIGHT) && action == UP) {
      --head.first;
      move_dir = UP;
    } else if ((move_dir == LEFT || move_dir == RIGHT) && action == DOWN) {
      ++head.first;
      move_dir = DOWN;
    } else {
      if (move_dir == UP) --head.first;
      if (move_dir == DOWN) ++head.first;
      if (move_dir == RIGHT) ++head.second;
      if (move_dir == LEFT) --head.second;
    }

    bool result = false;
    if (head.first > S_HEIGHT - 1 || head.second > S_WIDTH - 1)
      ;
    else {
      result = s_coordinates.insert(head).second;
      if (result) q_coordinates.push(head);
    }

    return result;
  }

  static t_dir get_move_dir() { return move_dir; }

  static void clear() {
    move_dir = UP;
    s_coordinates.clear();
    while (!q_coordinates.empty()) q_coordinates.pop();
    length = 0U;
  }

  static void pop_tail() {
    if (!q_coordinates.empty()) {
      s_coordinates.erase(q_coordinates.front());
      q_coordinates.pop();
    }
  }

  static void add_length() { ++length; }

  static size_t get_lenght() { return length; }

  static const set<pair<size_t, size_t>>& get_snake_coordinates() {
    return s_coordinates;
  }

  static bool contains(pair<size_t, size_t> apple) {
    return s_coordinates.find(apple) != s_coordinates.end();
  }

 private:
  static t_dir move_dir;
  static size_t length;
  static set<pair<size_t, size_t>> s_coordinates;
  static queue<pair<size_t, size_t>> q_coordinates;
};

class Field {
 public:
  static pair<size_t, size_t> get_apple_coordinates() {
    return apple_coordinates;
  }

  static void set_random_apple_coordinates(
      set<pair<size_t, size_t>> coordinates) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_w(0, width - 1);
    uniform_int_distribution<> dis_h(0, height - 1);
    apple_coordinates.first = dis_h(gen);
    apple_coordinates.second = dis_w(gen);
    while (coordinates.find(apple_coordinates) != coordinates.end()) {
      apple_coordinates.first = dis_h(gen);
      apple_coordinates.second = dis_w(gen);
    }
  }

  static size_t get_width() { return width; }

  static size_t get_heigh() { return height; }

 private:
  static size_t width;
  static size_t height;
  static pair<size_t, size_t> apple_coordinates;
};

class User {
 public:
  static size_t get_score() { return score; }
  static size_t get_max_score() {
    if (!max_score) read_max_score();
    return max_score;
  }
  static size_t get_level() { return level; }
  static size_t get_speed() { return speed; }

  static void add_score(size_t bonus) { score += bonus; }
  static void set_score_to_zero() { score = 0U; }

  static void read_max_score() {
    ifstream infile(PATH);
    if (!infile.good()) std::ofstream outfile(PATH);
    ifstream inputFile(PATH);
    if (!inputFile.is_open()) {
      throw runtime_error("Error opening file for reading");
    }
    inputFile >> max_score;
  }

  static void set_max_score() {
    ofstream outputFile(PATH, ios::trunc);
    if (!outputFile) {
      throw runtime_error("Error opening file for writing");
    }
    outputFile << score << " ";
    max_score = score;
  }

  static void up_level() {
    if (level < MAX_LEVEL) {
      ++level;
      speed -= 50U;
    }
  }

  static void set_level_to_zero() {
    level = 1U;
    speed = S_DEFAULT_SPEED;
  }

 private:
  static size_t score;
  static size_t max_score;
  static size_t level;
  static size_t speed;
};

}  // namespace s21

#endif