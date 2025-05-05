// Рабочее место кассира. Реализовано в классе Rmk, использует 2 структуры - Order для хранения информации о заказе, 
// Shift_data для информации о сменах. Информация о заказах остаётся в памяти - для данного задания это не обязательно, 
// можно было бы не использовать vector<Shift_data>, а хранить 1 объект этого типа, очищать его при создании новой смены.
// Функция dto_string нужна для "округления" double, и вывода меньшего количества знаков после запятой. Предполагается, 
// что таблица на входе имеет 3 столбца - название, код, цена товаров - при это код > 0. Все ошибки с обращениями к элементам 
// данных о заказах проверяются внутри Rmk, ошибки типов (int << string) ловятся через try - catch.

#include <iostream>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

const string dto_string(double a) {
  string out = to_string(a);
  return out.substr(0, out.size() - 4);
}

//Номер заказа, пара<код товара, количество>
struct Order {
  int number;
  vector<pair<long, int>> goods;
  string type = "cash";
  bool error = false; 
  bool closed = false;
  
  Order(int number_) {
    number = number_;
    goods = vector<pair<long, int>> {};
  };
  
  const string bill_print(map<long, pair<string, double>> &table, double received, double change) {
    string result = "Bill number " + to_string(number) + ":\n";
    for (auto i : goods) {
      result += table[i.first].first + "\t" + to_string(i.second) + "\t" + dto_string(i.second * table[i.first].second) + "\n";
    }
    result += "Summary " + dto_string(get_order_summary(table)) + "\n";
    result += "Paid by " + type + "\n";
    result += "Received " + dto_string(received) + ", change " + dto_string(change) + "\n";
    return result;
  };
  
  const double get_order_summary(map<long, pair<string, double>> &table) { 
    double sum = 0;
    for (auto i : goods)
      sum += table[i.first].second * i.second;
    return sum;
  };
};

struct Shift_data {
  string who;
  vector<Order> orders;
  bool error = false;
  bool closed = false;
  
  Shift_data(const string who_) { 
    who = who_; 
    orders = vector<Order> {};
  };
};

class Rmk {
private:
  double cash_left = 0;
  int current_shift = -1;
  vector<Shift_data> shift; 
  map<long, pair<string, double>> table; //code, name, price
public:
  Rmk() {}
  Rmk(const double cash_, map<long, pair<string, double>> &table_) {
    cash_left = cash_;
    table = move(table_);
  }
  ~Rmk() {
    shift.clear();
    table.clear();
  }
  void add_cash_change(double add) {
    cash_left += add;
    cout << "Successfully added! Current amount is " << cash_left << "!\n";
  }
  void shift_open(const string who) {
    if (current_shift != -1 && !shift[current_shift].closed)
      cerr << "Close the previous shift before opening a new one!\n";
    else {
      shift.emplace_back(who);
      current_shift += 1;
      cout << "Shift number " << current_shift << " was opened by " << who << "!\nThere is " << cash_left << " in cash for change!\n";
    }
  }
  void shift_close() {
    if (current_shift == -1 && !shift[current_shift].closed)
      cerr << "Close the previous shift before opening a new one!\n";
    if (current_shift == -1 || shift[current_shift].closed) {
      cerr << "You are trying to close already closed shift!\n";
    }
    else {
      vector<int> opened;
      vector<int> errors;
      for (auto i : shift[current_shift].orders) {
        if (i.error) 
          errors.push_back(i.number);
        if (!i.closed)
          opened.push_back(i.number);
      }
      if (!opened.empty() || !errors.empty()){
        cerr << "Cannot close the shift!\n";
        shift[current_shift].error = false;
        for (auto i : opened) 
          cout << "Order number " << i << " wasn't closed!\n";
        for (auto i : errors)
          cout << "Error occured during order number " << i << "!\n";
      }
      else{
        shift[current_shift].closed = true;
        cout << "Shift number " << current_shift << " was successfully closed! The statistics for it:\n" << Rmk::get_summary(current_shift);
      }
    }
  }
  string get_summary(int shift_number) {
    if (shift_number > current_shift || shift_number <= -1) {
      cerr << "Shift number " << shift_number << " wasn't opened!\n";
      return "";
    }
    else {
      double card = 0;
      double cash = 0;
      for (auto i : shift[shift_number].orders) {
        if (i.error)
          cout << "During " << i.number << " order an error occured!\n";
          shift[current_shift].error = true;
        if (!i.closed) {
          cout << "Order number " << i.number << " wasn't closed!\n";
        }
        else if (i.type == "card")
          card += i.get_order_summary(table);
        else
          cash += i.get_order_summary(table);
      }
      return (string("The statistics for shift number ") + to_string(shift_number) + ":\nIt was opened by " + shift[shift_number].who + 
      ",\nTotal income in cash: " + dto_string(cash) + ",\nTotal income by card: " + dto_string(card) + ".\n");
    }
  }
  void bill_create() {
    if (current_shift != -1 && !shift[current_shift].closed) {
      shift[current_shift].orders.push_back(shift[current_shift].orders.size());
      cout << "Bill number " << shift[current_shift].orders.size() - 1 << " was successfully created!\n";
    }
    else 
      cerr << "Open a shift first!\n";
  }
  void bill_add(const int bill_num, const long code) {
    if (bill_num <= -1) {
      cerr << "Bill number can't be below 0!\n";
      return;
    }
    if (bill_num >= shift[current_shift].orders.size()) {
      cerr << "Invalid number of bill! Add bill number " << shift[current_shift].orders.size() << " first!\n";
      return;
    }
    if (shift[current_shift].orders[bill_num].closed){
      cerr << "This bill is closed!\n";
      return;
    }
    if (find_good(code) != -1) {
      cout << "The good was found! Enter the amount of " << table[code].first << ":\n";;
      int amount;
      cin >> amount;
      shift[current_shift].orders[bill_num].goods.emplace_back(pair(code, amount));
      cout << "Successfully added!\n";
    }
    else 
      cerr << "There isn't such code! Try using find command first!\n";
  }
  void bill_add(const int bill_num, const string name) {
    if (bill_num <= -1) {
      cerr << "Bill number can't be below 0!\n";
      return;
    }
    if (bill_num >= shift[current_shift].orders.size()) {
      cerr << "Invalid number of bill! The last number of bill is " << shift[current_shift].orders.size() - 1 << "!\n";
      return;
    }
    if (shift[current_shift].orders[bill_num].closed){
      cerr << "This bill is closed!\n";
      return;
    }
    long found = find_good(name);
    if (found != -1) {
      cout << "The good was found! Enter the amount of " << name << ":\n";;
      int amount;
      cin >> amount;
      shift[current_shift].orders[bill_num].goods.emplace_back(pair{found, amount});
      cout << "Successfully added!\n";
    }
    else 
      cerr << "There isn't such code! Try using find command first!\n";
  }
  void bill_close(const int bill_num, string payment_type) {
    if (current_shift == -1 || shift[current_shift].closed) {
      cerr << "Open a shift first!\n";
      return;
    }
    if (bill_num <= -1) {
      cerr << "Bill number can't be below 0!\n";
    }
    if (bill_num >= shift[current_shift].orders.size() || shift[current_shift].orders[bill_num].closed) {
      cerr << "The bill closed or doesn't exsist!\n";
      return;
    }
    if (payment_type == "cash") {
      shift[current_shift].orders[bill_num].type = "cash";
      double received;
      cout << "Enter the received payment (" << shift[current_shift].orders[bill_num].get_order_summary(table) << " required):\n";
      cin >> received;
      double change = received - shift[current_shift].orders[bill_num].get_order_summary(table);
      if (change < 0){
        cout << "The payment was rejected! The amount is not enough!\n";
        shift[current_shift].orders[bill_num].error = true;
        return;
      }
      if (change > cash_left) {
        cerr << "There is not enough money in cash box! The payment was rejected!\nNeeded amount is " << change << " but only " << cash_left << " was found!\n";
        shift[current_shift].orders[bill_num].error = true;
        return;
      }
      else {
        cout << "The payment was received successfully! The bill is closed!\nChange is " << change << "!\n";
        cash_left -= change;
        shift[current_shift].orders[bill_num].closed = true;
        shift[current_shift].orders[bill_num].error = false;
        cout << shift[current_shift].orders[bill_num].bill_print(table, received, change);
      }
    }
    else if (payment_type == "card") {
      shift[current_shift].orders[bill_num].type = "card";
      double received;
      cout << "Enter the received payment (" << shift[current_shift].orders[bill_num].get_order_summary(table) << " required):\n";
      cin >> received;
      if (received < shift[current_shift].orders[bill_num].get_order_summary(table)){ 
        cout << "The payment was rejected! The amount is not enough! Required " << shift[current_shift].orders[bill_num].get_order_summary(table) << "!\n";
        shift[current_shift].orders[bill_num].error = true;
        return;
      }
      else {
        cout << "The payment was received successfully! The bill is closed!\n";
        shift[current_shift].orders[bill_num].closed = true;
        shift[current_shift].orders[bill_num].error = false;
        cout << shift[current_shift].orders[bill_num].bill_print(table, received, 0);
      }
    }
    else {
      cout << "Unknown type of payment! Try again!\n";
    }
  }
  long find_good(const long code) {
    if (table.count(code) == 1)
      return code;
    else 
      return -1;
  }
  long find_good(const string name) {
    for (auto [key, i] : table) {
      if (i.first == name)
        return key;
    }
    return -1;
  }
};

int main() {
  cout << fixed << setprecision(2);
  cin.exceptions(ios::failbit);
  ifstream file("./table.csv");
  map<long, pair<string, double>> temp;
  string name;
  string code;
  string price;
  getline(file, name);
  assert(name == "name,code,price");
  while (getline(file, name, ',')) {
    getline(file, code, ',');
    getline(file, price);
    temp[stoi(code)] = make_pair(name, stof(price));
  }
  double cash;
  cout << "How much cash is there in the beginning of the day?\n";
  try {
    cin >> cash;
  }
  catch(exception &e) {
    cerr << "Bad input\n";
    return 1;
  }
  if (cash < 0)
    cash = 0;
  auto rmk = new Rmk(cash, temp);
  
  cout << "Enter commands one word per line or type \"help\"\n";
  string input = "Ok";
  try {
  while(input != "exit"){
    cin >> input;
    
    if (input == "shift") {
      cout << "open/close/summary?\n";
      cin >> input;
      if (input == "open"){
        cout << "Who opened the shift?\n";
        string name_;
        cin >> name_;
        rmk->shift_open(name_);
      }
      else if (input == "close")
        rmk->shift_close();
      else if (input == "summary") {
        cout << "Enter the number of shift:\n";
        int num_;
        cin >> num_;
        cout << rmk->get_summary(num_);
      }
      else 
        cout << "Wrong input!\n";
    }
    
    else if (input == "bill"){
      cout << "create/add/add_name/close?\n";
      cin >> input;
      if (input == "create")
        rmk->bill_create();
      else if (input == "add"){
        int num_;
        cout << "Enter number of bill:\n";
        cin >> num_;
        cout << "Enter the code of a good:\n";
        long code_;
        cin >> code_;
        rmk->bill_add(num_, code_);
      }
      else if (input == "add_name"){
        int num_;
        cout << "Enter number of bill:\n";
        cin >> num_;
        string name_;
        cout << "Enter the name of a good:\n";
        cin >> name_;
        rmk->bill_add(num_, name_);
      }
      else if (input == "close") {
        cout << "Enter the bill number:\n";
        int num_;
        cin >> num_;
        cout << "Enter the type of payment:\n";
        string type_; 
        cin >> type_;
        rmk->bill_close(num_, type_);
      }
      else 
        cout << "Wrong input!\n";
    }
    
    else if (input == "find") {
      cout << "code/name?\n";
      cin >> input;
      if (input == "code") {
        cout << "Enter the code:\n";
        long code_;
        cin >> code_;
        long a_ = rmk->find_good(code_);
        if (a_ == -1)
          cout << "Good doesn't exist!\n";
        else 
          cout << "This good was found!\n";
      }
      else if (input == "name") {
        cout << "Enter the name:\n";
        string name_;
        cin >> name_;
        long a_ = rmk->find_good(name_);
        if (a_ == -1)
          cout << "Good doesn't exist!\n";
        else 
          cout << "This good was found!\n";
      }
      else 
        cout << "Wrong input!\n";
    }
    
    else if (input == "add_cash"){
      double cash_;
      cout << "Enter how many cash should be added:\n";
      cin >> cash_;
      rmk->add_cash_change(cash_);
    }
    
    else if (input == "help")
      cout << "Available commands:\nshift\n\topen/close/summary\nbill\n\tcreate/add/add_name/close\nfind\n\tcode/name\nadd_cash\nhelp\nexit\nType one word per line!\n\n";
    else if (input == "exit")
      cout << "Goodbye!\n";
    else 
      cout << "Unknown operation! Try writing \"help\":\n";
  }
  }
  catch (exception &a) {
    cerr << "Bad input, type mismatch!\n";
  }
}

