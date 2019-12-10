// Copyright 2019 Kirill <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

class Experiment
{
public:
    Experiment(const int &l1, const int &l2, const int &l3);
    Experiment(Experiment &prev_ex);
    ~Experiment();
    string get_travel_variant();
    void count_number_of_experiments();
    void create_test_buffer(const int _quantity);
    void warm_up_cache(const int _size);
    void run(int _size);
    void stop(unsigned start);
    void print_results();
    void just_do_it();

public:
    string travel_variant;
    map <char, int> cache_sizes;

private:
    int _number_of_experiments;
    vector <int> _buffer_sizes;
    unsigned char *_buffer;
    vector <clock_t> _duration;
    ofstream _ofile;
};

void Pushnoy(int l1, int l2, int l3);

#endif // INCLUDE_HEADER_HPP_

