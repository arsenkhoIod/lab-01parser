#include <Parsing.hpp>

int main(){
  Parsing List;
  std::string str = "../Students.json";
  List.set_data(str);
  List.from_json();
  std::cout << List.print();
}
