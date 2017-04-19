/**
 * This class implements Kunth's DLX algorithm that solves exact cover problem.
 * @file    DancingLink.h
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-04-18 23:32:21
 *
 * Created on Tue 18 Apr 2017 07:25:04 PM EDT.
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <limits>

class Data;
class Column;

typedef std::shared_ptr<Data>   DataPtr;
typedef std::shared_ptr<Column> ColumnPtr;

struct Data 
{
    DataPtr left, right, up, down;
    ColumnPtr col;

    int row_id;

    Data (int r) : row_id(r) {}
    virtual ~Data() = default;
};

struct Column : Data
{
    int size;
    int col_id;

    Column(int c) : Data(-1), col_id(c) {}
    ~Column() {}
};

inline std::ostream& operator<< (std::ostream& os, const Data& d)
{
    os << "Data(row_id=" << d.row_id 
       << ", col_id=" << d.col->col_id << ")";
    return os;
}

inline std::ostream& operator<< (std::ostream& os, const Column& c)
{
    os << "Column(col_id=" << c.col_id
       << ", size=" << c.size << ")";
    return os;
}


struct DancingLink
{
    ColumnPtr head; 
    std::vector<ColumnPtr> cols;
    std::unordered_map<int, std::vector<DataPtr>> rows;

    std::vector<std::vector<int>> all_solutions;
    std::vector<DataPtr> cur_solution;

    void add_columns (int num_cols);
    void add_element (int row, int col);
    void print_columns ();

    void search (int k);
    ColumnPtr choose_column ();
    void cover (ColumnPtr& c);
    void uncover (ColumnPtr& c);

    DancingLink();
    ~DancingLink();
};

