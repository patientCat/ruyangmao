//c++ program
//design by >^.^<
//@date    2019-03-24 16:19:49
//根据动态规划，来帮忙计算的一个褥羊毛程序中的核心算法



// 在input文件中输入商品数目，满减价格，还有商品名字还有价格，最终会得到一份清单在shoppinglist中
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <fcntl.h>

using namespace std;

struct Item
{
    int price;
    string name;
};

class Solution
{
public:

    void readInput(string inputfile)
    {
        ifstream ifs(inputfile.c_str(), ifstream::in);
        if(!ifs)
            cerr << inputfile << " open failed" << endl;
        string line;
        getline(ifs, line);
        istringstream iss(line);
        iss >> nitems_;
        iss >> prefer_price_;
        items_vec_.reserve(nitems_);
        ret_.reserve(nitems_);

        while(getline(ifs, line))
        {
            stringstream ss(line);
            Item tmp;
            ss >> tmp.name;
            ss >> tmp.price;
            items_vec_.push_back(tmp);
        }
        ifs.close();
    }

    bool getShoppingList()
    {
        if(items_vec_.empty())
            return false;
        const int max_price = 3 * prefer_price_;
        vector<vector<bool>> states(nitems_, vector<bool> (max_price + 1, false));

        states[0][0] = 1;
        states[0][items_vec_[0].price] = 1;
        for(int i = 1; i < nitems_; ++i)
        {
            for(int j = 0; j < max_price + 1; ++j)
            {
                if(states[i-1][j])
                {
                    states[i][j] = 1; // 不拿
                    if(j + items_vec_[i].price < max_price + 1) // 拿
                        states[i][j+items_vec_[i].price] = 1;
                }
            }
        }

        int j = 0;
        for(j = prefer_price_; j < max_price + 1; ++j)
        {
            if(states[nitems_-1][j])
                break;
        }
        if(j == max_price)
            return false;

        // 状态states[i][j]只与states[i-1][j]和states[i-1][j-items_vec_[i].price]有关
        addPath(states, items_vec_, nitems_-1, j, vector<Item>{});
        return true;
    }
    void addPath(const vector<vector<bool>> &states, const vector<Item>& items, int starti, int startj, vector<Item> path)
    {
        if(0 == starti && states[0][startj])
        {
            path.push_back(items[0]);
            ret_.push_back(path);
            return;
        }

        int j = startj;
        for(int i = starti; i > 0 && j > 0; --i)
        {
            if(states[i-1][j] && states[i-1][j-items[i].price])
            {
                addPath(states, items, i-1, j, path);
            }
            if(states[i-1][j-items[i].price])
            {
                path.push_back(items[i]);
                j -= items[i].price;
            }
        }
        ret_.push_back(path);
    }
    void output()
    {
        int i = 0;
        ofstream ofs("shoppinglist");
        for(auto &b : ret_)
        {
            ofs << "The shopping list "
                 << ++i 
                 << endl;
            for(auto &a : b)
                ofs << a.name << "  " << a.price << endl;
            ofs << endl;
        }
        
        ofs.close();
    }

private:
    vector<Item> items_vec_;
    vector<vector<Item>> ret_;
    int nitems_;
    int prefer_price_;
};

int main()
{
    // 2 2 4 6 3
    Solution tmp;
    tmp.readInput("input"); 
    tmp.getShoppingList();
    tmp.output();
    return 0;
}
