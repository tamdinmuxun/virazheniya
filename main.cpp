#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

string s;
string oper = "+-*/";
vector<char> ss;
set<char> opers{'+', '-', '*', '/'};
struct tree{
    char n;
    tree* left, *right;
    tree(){
        n = ' ';
        left = nullptr;
        right = nullptr;
    }
    tree (char t){
        n = t;
        left = new tree();
        right = new tree();
    }

    void buildpref(int &i);
    void printpre();
    void printpost();
    void printin();
    void buildpost(int &i);
    void buildin(int l, int r);
};

void tree::buildpref(int &i) {
    if (oper.find(ss[i]) != -1){
        n = ss[i];
        left = new tree();
        right = new tree();
        ++i;
        left->buildpref(i);
        right->buildpref(i);
    }else{
        n = ss[i];
        ++i;
        return;
    }
}

void tree::printpre() {
    cout << n << ' ';
    if (left != nullptr){
        left->printpre();
    }if (right != nullptr){
        right->printpre();
    }
}

void tree::printpost() {
    if (left != nullptr){
        left->printpost();
    }if (right != nullptr){
        right->printpost();
    }
    cout << n << ' ';
}

void tree::printin() {
    if (left != nullptr){
        if (n == '+' || n == '-') cout << '(';
        left->printin();
    }
    cout << n << ' ';
    if (right != nullptr){
        right->printin();
        if (n == '+' || n == '-') cout << ") ";
    }

}

void tree::buildpost(int &i) {
    if (oper.find(ss[i]) != -1){
        n = ss[i];
        left = new tree();
        right = new tree();
        --i;
        right->buildpost(i);
        left->buildpost(i);
    }else{
        n = ss[i];
        --i;
        return;
    }
}

void tree::buildin(int l, int r) {
    if (l == r) return;
    int balance = 0, minb = 1e9;
    int i = l;
    for (int j = l; j < r; ++j){
        char c = ss[j];
        if (c == '(') ++ balance;
        else if (c == ')') --balance;
        else if (oper.find(c) != -1){
            if ((c == '*' || c == '/') && balance < minb){
                minb = balance;
                i = j;
            }else if ((c == '+' || c == '-') && balance <= minb){
                minb = balance;
                i = j;
            }
        }
    }
    if (ss[i] != '(' && ss[i] != ')') n = ss[i];
    if (l != i && !(i - l == 1 && (ss[l] == '(' || ss[l] == ')'))){
        left = new tree();
        left->buildin(l, i);
    }
    if (i + 1 != r && !(r - i - 1 == 1 && (ss[i + 1] == '(' || ss[i + 1] == ')'))){
        right = new tree();
        right->buildin(i + 1, r);
    }
}


int main() {
    string t;
    cin >> t;
    getline(cin, s);
    getline(cin, s);
    for (char i : s){
        if (i != ' '){
            ss.push_back(i);
        }
    }
    if (t == "prefix"){
        tree* root = new tree(ss[0]);
        int i = 1;
        root->left->buildpref(i);
        root->right->buildpref(i);
        root->printpost();
        cout << '\n';
        root->printin();
    }else if (t == "postfix"){
        tree* root = new tree(ss.back());
        int i = (int)ss.size() - 2;
        root->right->buildpost(i);
        root->left->buildpost(i);
        root->printpre();
        cout << '\n';
        root->printin();
    }
    else if (t == "infix"){
        int balance = 0, minb = 1e9;
        int i = 0;
        for (int j = 0; j < ss.size(); ++j){
            char c = ss[j];
            if (c == '(') ++ balance;
            else if (c == ')') --balance;
            else if (oper.find(c) != -1){
                if ((c == '*' || c == '/') && balance < minb){
                    minb = balance;
                    i = j;
                }else if ((c == '+' || c == '-') && balance <= minb){
                    minb = balance;
                    i = j;
                }
            }
        }
        tree* root = new tree(ss[i]);
        root->left->buildin(0, i);
        root->right->buildin(i + 1, ss.size());
        root->printpre();
        cout << '\n';
        root->printpost();
    }
//    5 + 5 + 5 + 3 * 4 * 5 + 5 + (5 + 5) * 5 = + + + + + 5 5 5 * * 3 4 5 5 * + 5 5 5
////////////////////////////////////////////////5 5 5 3 4 5 * * 5 + + + + 5 5 + 5 * +
// + + + + 5 5 * + 5 3 * 4 5 5 * + 5 5 5
    return 0;
}
