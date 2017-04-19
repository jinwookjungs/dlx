/**
 * @file    DancingLink.cpp
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-04-18 23:32:23
 *
 * Created on Tue 18 Apr 2017 07:32:56 PM EDT.
 */

#include "DancingLink.h"

using namespace std;

DancingLink::DancingLink() : head(make_shared<Column>(-1))
{
    //
}

DancingLink::~DancingLink()
{
    //
}

// num_cols should be larger than 1
void DancingLink::add_columns (int num_cols)
{
    cols.reserve(num_cols);

    // Create first column
    cols.emplace_back(make_shared<Column>(0));

    // Create column objects
    for (int i = 1; i < num_cols; i++) {
        auto col = make_shared<Column>(i);

        // Append it
        cols.back()->right = col;
        col->left = cols.back();

        cols.push_back(col);
    }

    // Make links of the head 
    head->right = cols[0];
    cols[0]->left = head;

    head->left = cols.back();
    cols.back()->right = head;

    // Set the up/down pointer to itself
    for (auto& c : cols) {
        c->up = c; 
        c->down = c; 
    }
}


void DancingLink::add_element (int r, int c)
{
    // Current column;
    auto& col = cols[c];

    // Create data object x
    auto x = make_shared<Data>(r);

    // Connect within a column
    x->down = col;
    x->col = col;

    if (col->size == 0) {
        x->up = col; 
        col->down = x;
    }
    else {
        x->up = col->up;
        col->up->down = x;
    }

    col->up = x;
    col->size++;

    // Connect within a row
    auto it = rows.find(r);

    if (it == rows.end()) {
        // Create a new row
        rows[r] = vector<DataPtr>();
        x->left = x;
        x->right = x;
    }
    else {
        // Just connect it
        x->left = it->second.back();
        it->second.back()->right = x;
        
        x->right = (it->second)[0];
        (it->second)[0]->left = x;
    }

    rows[r].push_back(x);
}

void DancingLink::print_columns ()
{
    ColumnPtr c = dynamic_pointer_cast<Column>(head->right);

    while (c->col_id != -1) {
        cout << *c << endl;
        ColumnPtr c_new = dynamic_pointer_cast<Column>(c->right);
        c = c_new;
    }
}


/**
 * Choose a column with the smallest number of 1s.
 */
ColumnPtr DancingLink::choose_column ()
{
    ColumnPtr chosen = dynamic_pointer_cast<Column>(head->right);
    int min_size = std::numeric_limits<int>::max();

    ColumnPtr c = dynamic_pointer_cast<Column>(head->right);

    while (c->col_id != -1) {
        if (c->size < min_size) {
            chosen = c;
            min_size = c->size;
        }

        ColumnPtr c_new = dynamic_pointer_cast<Column>(c->right);
        c = c_new;
    }

    return chosen;
}


/**
 * Cover a given column @a c.
 */
void DancingLink::cover (ColumnPtr& c)
{
    // Removes c from the header list: Set L[R[c]] <- L[c] and R[L[c]] <- R[c]
    c->right->left = c->left;
    c->left->right = c->right;

    // Removes all rows in c's own list from the other columns they are in
    DataPtr i = c->down;

    while (i.get() != c.get()) {
        i = i->down;
    }

    i = c->down;

    while (i.get() != c.get()) {
        DataPtr j = i->right; 

        while (j.get() != i.get()) {
            j->down->up = j->up;
            j->up->down = j->down;
            j->col->size--;

            j = j->right;
        }
        i = i->down;
    }
}


/**
 * We get to the point of this whole algorithm, the operation of uncovering 
 * a given column @a c.
 */
void DancingLink::uncover (ColumnPtr& c)
{
    DataPtr i = c->up;
    while (i.get() != c.get()) {
        DataPtr j = i->left;

        while (j.get() != i.get()) {
            j->down->up = j;
            j->up->down = j;

            j->col->size++;

            j = j->left;
        }

        i = i->up;
    }

    c->right->left = c;
    c->left->right = c;
}

/**
 * Solve exact cover.
 */
void DancingLink::search (int k)
{
    // If R[h] = h, print the current solution and return
    if (head->right.get() == head.get()) {
        all_solutions.emplace_back();
        for (auto& r : cur_solution) {
            all_solutions.back().emplace_back(r->row_id);
        }
        cur_solution.pop_back();
        return;
    }

    // Otherwise, choose a column object c
    ColumnPtr c = choose_column();

    // Cover column c
    cover(c);

    DataPtr r = c->down;
    while (r.get() != c.get()) {
        DataPtr o_k = r; 
        cur_solution.push_back(o_k);

        DataPtr j = r->right;
        while (j.get() != r.get()) {
            cover(j->col);      // cover column j
            j = j->right;
        }

        search(k+1);
        r = o_k;
        c = r->col;

        j = r->left;
        while (j.get() != r.get()) {
            uncover(j->col);
            j = j->left;
        }

        r = r->down;
    }

    uncover(c);

    if (cur_solution.size() > 0) {
        cur_solution.pop_back();
    }

    return;
}


int main (void)
{
    DancingLink dlx;

    dlx.add_columns(3);

//    // First row
//    dlx.add_element(0,2);
//    dlx.add_element(0,4);
//    dlx.add_element(0,5);
//
//    dlx.add_element(1,0);
//    dlx.add_element(1,3);
//    dlx.add_element(1,6);
//
//    dlx.add_element(2,1);
//    dlx.add_element(2,2);
//    dlx.add_element(2,5);
//
//    dlx.add_element(3,0);
//    dlx.add_element(3,3);
//
//    dlx.add_element(4,1);
//    dlx.add_element(4,6);
//
//    dlx.add_element(5,3);
//    dlx.add_element(5,4);
//    dlx.add_element(5,6);

    dlx.add_element(0,0);
    dlx.add_element(0,2);
    dlx.add_element(1,1);
    dlx.add_element(1,2);
    dlx.add_element(2,1);
    dlx.add_element(3,0);
    dlx.add_element(3,1);

    dlx.print_columns();

    dlx.search(0);

    for (auto& sol : dlx.all_solutions) {
        for (int& row_id : sol) {
            cout << row_id << " ";
        }
        cout << endl;
    }
}

