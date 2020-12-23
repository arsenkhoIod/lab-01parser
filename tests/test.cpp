// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>

#include <fstream>

#include "Parsing.hpp"

TEST(Print, All_is_Correct) {
  std:: string test_string = \
 R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})";
  std::ofstream testFile;
  testFile.open("json_file.json");
  testFile << test_string;
  testFile.close();
  Parsing List;
  std::string str = "json_file.json";
  List.set_data(str);
  List.from_json();

  std::string res_string = \
 R"(| name          | group  | avg  | debt    |
|---------------|--------|------|---------|
| Ivanov Petr   | 1      | 4.25 | Null    |
|---------------|--------|------|---------|
| Sidorov Ivan  | 31     | 4    | C++     |
|---------------|--------|------|---------|
| Pertov Nikita | IU8-31 | 3.33 | 3 items |
|---------------|--------|------|---------|
)";
  EXPECT_EQ(List.print(), res_string);
}
TEST(AddData, Correctfile){
  std:: string test_string = \
 R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})";
  std::ofstream testFile;
  testFile.open("json_file.json");
  testFile << test_string;
  testFile.close();
  Parsing List;
  std::string str = "json_file.json";
  List.set_data(str);
  std::ifstream file("json_file.json");
  EXPECT_EQ(json::parse(file), List.get_data());
}
TEST(AddData, UncorrectFile){
  std::string res_str = "Unable to open file: json_fil.json";
  Parsing List;
  std::string str = "json_fil.json";
  try{
    List.set_data(str);
  }
  catch (std::runtime_error& err) {
    EXPECT_EQ(err.what(), res_str);
  }
}
TEST(AddData, EmptyItems){
  std::string res_str = "No items in file";

  std:: string test_string = \
 R"({
  "items":[],
  "_meta": {
    "count": 3
  }
})";
  std::ofstream testFile;
  testFile.open("json_file.json");
  testFile << test_string;
  testFile.close();

  try{
    Parsing List;
    std::string str = "json_file.json";
    List.set_data(str);
  }
  catch (std::runtime_error& err) {
    EXPECT_EQ(err.what(), res_str);
  }
}
TEST(AddData, EmptyMeta){
  std:: string test_string = \
 R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta":[]
})";
  std::ofstream testFile;
  testFile.open("json_file.json");
  testFile << test_string;
  testFile.close();
  Parsing List;
  std::string str = "json_file.json";
  std::string res_str = "No _meta data in file";
  try{
    List.set_data(str);
  }
  catch (std::runtime_error& err) {
    EXPECT_EQ(err.what(), res_str);
  }
}
TEST(SetCount, AllCorrect){
  std:: string test_string = \
 R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})";
  std::ofstream testFile;
  testFile.open("json_file.json");
  testFile << test_string;
  testFile.close();
  Parsing List;
  std::string str = "json_file.json";
  List.set_data(str);
  List.set_count();
  EXPECT_EQ(List.get_count(), 3);
}
TEST(GetName, AllCorrect){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": "1",
  "avg": "4.25",
  "debt": null
})");

  std::string res_string = "Ivanov Petr";
  EXPECT_EQ(res_string, Parsing::get_name(js.at("name")));
}
TEST(GetName, NameNoString){
  json js = json::parse(R"({
  "name": 12,
  "group": "1",
  "avg": "4.25",
  "debt": null
})");
  std::string res_string = "Invalid expression in name";
  try {
    Parsing::get_name(js.at("name"));
  }
  catch (std::runtime_error& err) {
    EXPECT_EQ(err.what(), res_string);
  }
}
TEST(GetAvg, StringAvg){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": "1",
  "avg": "4.25",
  "debt": null
})");
  std::string res_string = "4.25";
  EXPECT_EQ(res_string,
            std::any_cast<std::string>(Parsing::get_avg(js.at("avg"))));
}
TEST(GetAvg, NullAvg){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": "1",
  "avg": null,
  "debt": null
})");
  EXPECT_EQ(nullptr,
            std::any_cast<std::nullptr_t>(Parsing::get_avg(js.at("avg"))));
}
TEST(GetAvg, DoubleAvg){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": "1",
  "avg": 4.25,
  "debt": null
})");
  EXPECT_EQ(4.25,
            std::any_cast<double>(Parsing::get_avg(js.at("avg"))));
}
TEST(GetAvg, Size_tAvg){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": "1",
  "avg": 4,
  "debt": null
})");
  EXPECT_EQ(4,
            std::any_cast<size_t>(Parsing::get_avg(js.at("avg"))));
}
TEST(GetGroup, Size_tGroup){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": 1,
  "avg": 4,
  "debt": null
})");
  EXPECT_EQ(1,
            std::any_cast<size_t>(Parsing::get_group(js.at("group"))));
}
TEST(GetGroup, NullGroup){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": null,
  "avg": 4,
  "debt": null
})");
  std::any res = Parsing::get_group(js.at("group"));
  EXPECT_EQ(nullptr, std::any_cast<std::nullptr_t>(res));
}
TEST(GetGroup, StringGroup){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": "10",
  "avg": 4,
  "debt": null
})");
  std::string res_str = "10";
  std::any res = Parsing::get_group(js.at("group"));
  EXPECT_EQ(res_str, std::any_cast<std::string>(res));
}
TEST(GetDebt, NullDebt){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": null,
  "avg": 4,
  "debt": null
})");
  std::any res = Parsing::get_debt(js.at("debt"));
  EXPECT_EQ(nullptr, std::any_cast<std::nullptr_t>(res));
}
TEST(GetDebt, StringDebt){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": null,
  "avg": 4,
  "debt": "C++"
})");
  std::any res = Parsing::get_debt(js.at("debt"));
  std::string res_str = "C++";
  EXPECT_EQ(res_str, std::any_cast<std::string>(res));
}
TEST(GetDebt, VectorDebt){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": null,
  "avg": 4,
  "debt": [
        "C++",
        "Linux",
        "Network"
      ]
})");
  std::any res = Parsing::get_debt(js.at("debt"));
  std::vector<std::string> res_vec{"C++", "Linux", "Network"};
  EXPECT_EQ(res_vec, std::any_cast<std::vector<std::string>>(res));
}
TEST(ItemFjson, AllCorrect){
  json js = json::parse(R"({
  "name": "Ivanov Petr",
  "group": null,
  "avg": 4,
  "debt": "C++"
})");
  Parsing List;
  List.item_fjson(js);
  std::string res_name = "Ivanov Petr";
  std::string res_debt = "C++";
  EXPECT_EQ(res_name, List.get_Studlist()->Items.back()->name);
  std::any group = List.get_Studlist()->Items.back()->group;
  EXPECT_EQ(nullptr, std::any_cast<std::nullptr_t>(group));
  std::any avg = List.get_Studlist()->Items.back()->avg;
  EXPECT_EQ(4, std::any_cast<size_t>(avg));
  std::any debt = List.get_Studlist()->Items.back()->debt;
  EXPECT_EQ(res_debt, std::any_cast<std::string>(debt));
}
TEST(FromJson, ArrSizeNotMeta) {
  std::string test_string =
      R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 5
  }
})";
  std::ofstream testFile;
  testFile.open("json_file.json");
  testFile << test_string;
  testFile.close();
  Parsing List;
  std::string str = "json_file.json";
  List.set_data(str);
  std::string res_str = "content of _meta != real count of items";
  try {
    List.from_json();
  } catch (std::runtime_error& err) {
    EXPECT_EQ(err.what(), res_str);
  }
}
TEST(Convertation, Fromstring){
  std::string str = "Hey";
  std::any a = str;
  std::string res_str = "Hey";
  EXPECT_EQ(res_str, Parsing::convert_to_string(a));
}
TEST(Convertation, FromInt){
  std::size_t integer = 12;
  std::any a = integer;
  std::string res_str = "12";
  EXPECT_EQ(res_str, Parsing::convert_to_string(a));
}
TEST(Convertation, FromDouble){
  std::any a = 4.24;
  std::string res_str = "4.24";
  EXPECT_EQ(res_str, Parsing::convert_to_string(a));
}
TEST(Convertation, FromVector){
  std::vector<std::string> vec{"C++", "Linux"};
  std::any a = vec;
  std::string res_str = "2 items";
  EXPECT_EQ(res_str, Parsing::convert_to_string(a));
}
TEST(Convertation, FromNull){
  std::any a = nullptr;
  std::string res_str = "Null";
  EXPECT_EQ(res_str, Parsing::convert_to_string(a));
}
TEST(Convertation, BadCast){
  Item structure;
  std::any a = structure;
  std::string res_str = "bad_any_cast";
  EXPECT_EQ(res_str, Parsing::convert_to_string(a));
}