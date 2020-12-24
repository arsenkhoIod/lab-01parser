// Copyright 2020 Your Name <your_email>
#ifndef INCLUDE_PARSING_HPP_
#define INCLUDE_PARSING_HPP_
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <any>
#include <fstream>
#include <vector>
#include <cstring>
#include <iterator>
#include <string>

using json = nlohmann::json;//заменяем название
struct Item
{
  std::string name;
  std::any group;
  std::any avg;
  std::any debt;
};
struct Meta
{
  int count;
};
struct Students
{
  struct Meta _meta;
  std::vector <struct Item*> Items;
};

class Parsing {
 public:
  Parsing();
  ~Parsing();

  void set_data(std::string& s);
  json get_data();
  Students* get_Studlist();

  void set_count();
  int get_count();

  static std::string get_name(const json& jit);
  static std::any get_group(const json& jit);
  static std::any get_avg(const json& jit);
  static std::any get_debt(const json& jit);

  void item_fjson(const json& jit);
  void from_json();

  static std::string convert_to_string(const std::any& any);

  std::string print();

 private:
  json data;
  Students* StudList;
};
#endif
