#include <bits/stdc++.h>
#include <fstream>
using namespace std;

const int UNDEF = 99999;
const int MAX_RAND = 13;
const int MAX_CARD_AMT = 4;

vector<string> numbers(4);
vector<string> permutations;
vector<bool> chosen(4);
set<string> results;

bool cmpf(float a, float b = 24) { return fabs(a - b) < 0.000005f; }

float calculate_basic_arithmetic(float a, float b, string s) {
  if (s == "+") {
    return a + b;
  }
  if (s == "-") {
    return a - b;
  }
  if (s == "*") {
    return a * b;
  }
  if (s == "/" && !cmpf(b, 0)) {
    return a / (b + 0.0);
  }
  return UNDEF;
}

float stack_calculation(stack<float> &oprd, stack<string> &optr) {
  float second_number = oprd.top();
  oprd.pop();
  float first_number = oprd.top();
  oprd.pop();

  float result =
      calculate_basic_arithmetic(first_number, second_number, optr.top());
  if (result == UNDEF)
    return UNDEF;

  oprd.push(result);

  optr.pop();

  return result;
}

float calculate_operation(vector<string> s) {
  stack<float> oprd;
  stack<string> optr;

  for (auto x : s) {
    if (x == "")
      continue;
    if (x != "(" && x != ")" && x != "*" && x != "/" && x != "-" && x != "+") {
      oprd.push(stoi(x));
    } else {
      if (optr.empty() || x == "*" || x == "/" || optr.top() == "(" ||
          x == "(") {
        optr.push(x);
      } else if (x == ")" || ((x == "-" || x == "+") && oprd.size() >= 2)) {
        if (stack_calculation(oprd, optr) == UNDEF)
          return UNDEF;
        if (x == ")")
          optr.pop();
      }
    }
  }

  while (!optr.empty()) {
    if (stack_calculation(oprd, optr) == UNDEF)
      return UNDEF;
  }

  return oprd.top();
}

bool validate_input() {
  for (auto number : numbers) {
    bool is_valid = false;
    vector<string> valid_input = {"A", "2", "3",  "4", "5", "6", "7",
                                  "8", "9", "10", "J", "Q", "K"};
    for (auto input : valid_input) {
      if (input == number) {
        is_valid = true;
        break;
      }
    }
    if (!is_valid) {
      cout << "Invalid input. Please try again!\n\n";
      return false;
    }
  }
  return true;
}

string convert_to_s_number_ar(string data) {
  if (data == "A")
    data = "1";
  else if (data == "1")
    data = "A";
  else if (data == "J")
    data = "11";
  else if (data == "11")
    data = "J";
  else if (data == "Q")
    data = "12";
  else if (data == "12")
    data = "Q";
  else if (data == "K")
    data = "13";
  else if (data == "13")
    data = "K";
  return data;
}

string generate_string(vector<string> data) {
  string result = "";
  for (auto s : data) {
    result += convert_to_s_number_ar(s) + " ";
  }
  return result;
}

void write_to_file(string file_name) {
  ofstream new_file("output/" + file_name + ".txt");
  for (auto result : results) {
    new_file << result << '\n';
  }
  new_file.close();
}

bool validate_file_name(string file_name) {
  if (file_name.size() > 100) {
    cout << "File name maximum 100 characters!\n";
    return false;
  }
  return true;
}

void search() {
  if (permutations.size() == MAX_CARD_AMT) {
    vector<string> optr = {"*", "+", "/", "-"};

    vector<vector<pair<pair<int, int>, pair<int, int>>>> brackets = {
        // (a + b) + (c + d)
        {{{0, 1}, {0, 4}}, {{2, 3}, {6, 10}}},
        // ((a + b) + c) + d
        {{{0, 1}, {1, 5}}, {{2, -7}, {0, 8}}, {{3, 3}, {10, 10}}},
        // a + ((b + c) + d)
        {{{1, 2}, {3, 7}}, {{3, -9}, {2, 10}}, {{0, 0}, {0, 0}}},
        // (a + (b + c)) + d
        {{{1, 2}, {3, 7}}, {{0, -1}, {0, 8}}, {{3, 3}, {10, 10}}},
        // a + (b + (c + d))
        {{{2, 3}, {5, 9}}, {{1, -3}, {2, 10}}, {{0, 0}, {0, 0}}},
    };

    int solutions_found = 0;
    for (auto x : brackets) {
      vector<string> s(11);
      for (auto y : x) {
        if (y.first.first == y.first.second) {
          s[y.second.second] = permutations[y.first.first];
          continue;
        }
        s[y.second.first] = "(";
        s[y.second.second] = ")";
        if (y.first.second < 0) {
          s[-y.first.second] = permutations[y.first.first];
          continue;
        }
        s[y.second.first + 1] = permutations[y.first.first];
        s[y.second.second - 1] = permutations[y.first.second];
      }

      int cnt_numbers = 0, i = 0;
      vector<int> optr_indexs;
      for (auto y : s) {
        if (y != "" && y != "(" && y != ")") {
          cnt_numbers++;
        } else if (y == "") {
          optr_indexs.push_back(i);
        }
        i++;
      }

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          for (int k = 0; k < 4; k++) {
            s[optr_indexs[0]] = optr[i];
            s[optr_indexs[1]] = optr[j];
            s[optr_indexs[2]] = optr[k];
            float res = calculate_operation(s);
            if (cmpf(res)) {
              results.insert(generate_string(s));
              solutions_found++;
            }
          }
        }
      }
    }
  } else {
    for (int i = 0; i < 4; i++) {
      if (chosen[i])
        continue;
      chosen[i] = true;
      permutations.push_back(convert_to_s_number_ar(numbers[i]));
      search();
      chosen[i] = false;
      permutations.pop_back();
    }
  }
}

void print_results() {
  cout << '\n';
  for (auto result : results) {
    cout << result << '\n';
  }
}

int main() {
  clock_t start, end;
  string choice;

  cout << "===\n";
  cout << "WELCOME TO 24 CARD GAME SOLVER!\n";
  cout << "===\n\n";

  while (choice != "1" && choice != "2") {
    cout << "Options:\n";
    cout << "1. Manual Input\n";
    cout << "2. Randomize Input\n";
    cout << "Your choice (1/2): ";
    getline(cin, choice);
    if (choice != "1" && choice != "2") {
      cout << "Invalid input. Please try again!\n";
    }
    cout << '\n';
  }

  if (choice == "1") {
    do {
      cout << "Available Cards: (A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K)\n";
      cout << "Card 1: ";
      getline(cin, numbers[0]);
      cout << "Card 2: ";
      getline(cin, numbers[1]);
      cout << "Card 3: ";
      getline(cin, numbers[2]);
      cout << "Card 4: ";
      getline(cin, numbers[3]);
    } while (!validate_input());
  } else {
    // Seeding with current time
    srand(time(0));
    for (int i = 0; i < 4; i++) {
      numbers[i] = convert_to_s_number_ar(to_string((rand() % MAX_RAND) + 1));
      cout << "Card " << i + 1 << ": " << numbers[i] << '\n';
    }
  }
  cout << '\n';

  start = clock();
  search();
  end = clock();

  cout << "===\n\n";
  cout << results.size() << " solution(s) found.";
  print_results();
  cout << "\n===\n";

  while (results.size() > 0 && choice != "y" && choice != "Y" &&
         choice != "n" && choice != "N") {
    cout << "Save to file? (y/n): ";
    getline(cin, choice);
  }

  if (choice == "y" || choice == "Y") {
    string file_name;
    do {
      cout << "File name: ";
      getline(cin, file_name);
    } while (!validate_file_name(file_name));
    write_to_file(file_name);
    cout << "Result saved to output/" << file_name << ".txt\n";
  }

  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "\nTotal time: " << fixed << time_taken << setprecision(5)
       << " sec.\n";

  return 0;
}
