// Copyright 2020 Your Name <your_email>
#include <Parsing.hpp>

Parsing::Parsing()//конструктор
{
  this->StudList = new struct Students;
  this->StudList->_meta.count = 0;
}
Parsing::~Parsing()//деструктор
{
  if (!this->StudList->Items.empty()){
    for (auto & Item : this->StudList->Items){
      delete Item;
    }
  }
  delete this->StudList;
}
void Parsing::set_data(std::string& s)//берём строку,в которой находится полный
// путь до файла
{
  std::ifstream file;
  file.open(s);

  if (!file)
    throw std::runtime_error("Unable to open file: " + s);
  this->data = json::parse(file);//data - поле класса типа json,туда заносим все
  //данные со всего файла
  if (data.at("items").empty())
    throw std::runtime_error("No items in file");
  if (data.at("_meta").empty())
    throw std::runtime_error("No _meta data in file");
}
json Parsing::get_data() {
  return data;
}
void Parsing::set_count() {
  this->StudList->_meta.count = this->data["_meta"]["count"].get<int>();
}
int Parsing::get_count()
{
  return this->StudList->_meta.count;
}
std::string Parsing::get_name(const json& jit)
{
  if (!jit.is_string()) {
    throw std::runtime_error{"Invalid expression in name"};
  }
  return jit.get<std::string>();
}
std::any Parsing::get_group(const json& jit)
{
  if (jit.is_null())
    return nullptr;
  else if (jit.is_string())
    return jit.get<std::string>();
  else
    return jit.get<std::size_t>();
}
std::any Parsing::get_avg(const json& jit)
{
  if (jit.is_null())
    return nullptr;
  else if (jit.is_string())
    return jit.get<std::string>();
  else if (jit.is_number_float())
    return jit.get<double>();
  else
    return jit.get<std::size_t>();
}
std::any Parsing::get_debt(const json& jit)
{
  if (jit.is_null())
    return nullptr;
  else if (jit.is_string())
    return jit.get<std::string>();
  else
    return jit.get<std::vector<std::string> >();
}
void Parsing::item_fjson(const json& jit)//прибавление студентов, добав. итемы
{
  this->StudList->Items.push_back(new Item);
  Item* tmp = this->StudList->Items.back();
  tmp->name = get_name(jit.at("name"));
  tmp->group = get_group(jit.at("group"));
  tmp->avg = get_avg(jit.at("avg"));
  tmp->debt = get_debt(jit.at("debt"));
}
void Parsing::from_json() {//проходим по всем объектам файла
  set_count();
  //      Создание вектора объектов json
  std::vector<json> ItemsVec;
  //      Перенос "Items" в вектор ItemVec
  data["items"].get_to(ItemsVec);
  if (ItemsVec.size() != (size_t)get_count())
    throw std::runtime_error{"content of _meta != real count of items"};
  if (this->StudList->Items.empty() && get_count() != 0) {
    for (int i = 0; i < get_count(); i++) {
      item_fjson(ItemsVec[i]);//используем метод для каждого студента
    }
  }
}
std::string Parsing::print()
{
  const int spaces_w = 2;
  size_t nMax = 4;
  size_t gMax = 5;
  size_t aMax = 3;
  size_t dMax = 4;

  size_t arr_size = this->StudList->Items.size();
  for (size_t i = 0; i < arr_size; i++)
  {
    Item* tmp = this->StudList->Items[i];
    if (tmp->name.length() > nMax) {
      nMax = tmp->name.length();
    }

    if (convert_to_string(tmp->group) == "bad_any_cast") {
      throw std::runtime_error{"Incorrect type group"};
    }
    if (convert_to_string(tmp->group).length() > gMax) {
      gMax = convert_to_string(tmp->group).length();
    }

    if (convert_to_string(tmp->avg) == "bad_any_cast") {
      throw std::runtime_error{"Incorrect type avg"};
    }
    if (convert_to_string(tmp->avg).length() > aMax) {
      aMax = convert_to_string(tmp->avg).length();
    }

    if (convert_to_string(tmp->debt) == "bad_any_cast")
      throw std::runtime_error{"Incorrect type debt"};
    if (convert_to_string(tmp->debt).length() > dMax)
      dMax = convert_to_string(tmp->debt).length();
  }
  std::string sep;
  sep += "|";
  for (size_t i = 0; i < nMax + spaces_w; i++)
  {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i <   gMax + spaces_w; i++)
  {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < aMax + spaces_w; i++)
  {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < dMax + spaces_w; i++)
  {
    sep += "-";
  }
  sep += "|";

  std::stringstream out;
  out << "| name ";
  for (size_t i = 0; i < nMax -4; i++)
    out << " ";
  out << "| group ";
  for (size_t i = 0; i < gMax -5; i++)
    out << " ";
  out << "| avg ";
  for (size_t i = 0; i < aMax -3; i++)
    out << " ";
  out << "| debt ";
  for (size_t i = 0; i < dMax -4; i++)
    out << " ";
  out << "|" << std::endl << sep << std::endl;

  for (size_t i = 0; i < arr_size; i++)
  {
    Item* temp = this->StudList->Items[i];
    out << "| ";
    out << temp->name << " ";
    for (size_t j = 0; j < nMax - temp->name.length(); j++)
      out << " ";
    out <<"| " << convert_to_string(temp->group) << " ";
    for (size_t j = 0; j < gMax - convert_to_string(temp->group).length(); j++)
      out << " ";
    out <<"| " << convert_to_string(temp->avg) << " ";
    for (size_t j = 0; j < aMax - convert_to_string(temp->avg).length(); j++)
      out << " ";
    out <<"| " << convert_to_string(temp->debt) << " ";
    for (size_t j = 0; j < dMax - convert_to_string(temp->debt).length(); j++)
      out << " ";
    out << "|" << std::endl << sep << std::endl;
  }
  return out.str();
}
std::string Parsing::convert_to_string(const std::any& any) {//получаем any
  // и в зависимости от типа данных в ней, конвертируем в строку.
  if (!strcmp(any.type().name(),
              "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"))
  {
    return std::any_cast<std::string>(any);
  }
  if (!strcmp(any.type().name(), "m"))
  {
    std::stringstream ret;
    ret << std::any_cast<size_t>(any);
    return ret.str();
  }
  if (!strcmp(any.type().name(), "d"))
  {
    std::stringstream ret;
    ret << std::any_cast<double>(any);
    return ret.str();
  }
  if (!strcmp(any.type().name(),
              "St6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EE"))
  {
    size_t ret = std::any_cast<std::vector<std::string> >(any).size();
    std::stringstream my_stream;
    my_stream << ret;
    return my_stream.str() + " items";
  }
  if (!strcmp(any.type().name(), "Dn"))
  {
    return "Null";
  }
  return "bad_any_cast";
}
Students* Parsing::get_Studlist() {
  return StudList;
}
