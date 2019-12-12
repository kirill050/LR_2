// Copyright 2019 Kirill <your_email>

#include <header.hpp>

Experiment::Experiment(const int &l1, const int &l2, const int &l3)
    {
        this->travel_variant = "Direct";
        _ofile.open("direct.txt");
        cache_sizes['1'] = l1;
        cache_sizes['2'] = l2;
        cache_sizes['3'] = l3;
    }

Experiment::Experiment(Experiment &prev_ex)
    {
        if (prev_ex.travel_variant == string("Direct"))
        {
            travel_variant = "Return";
            _ofile.open("return.txt");
        } else {
            _ofile.open("random.txt");
            travel_variant = "Random";
        }
        cache_sizes['1'] = prev_ex.cache_sizes['1'];
        cache_sizes['2'] = prev_ex.cache_sizes['2'];
        cache_sizes['3'] = prev_ex.cache_sizes['3'];
    }

Experiment::~Experiment()
    {
        _ofile.close();
        delete _buffer;
    }

string Experiment::get_travel_variant()
    {
        return travel_variant;
    }

void Experiment::count_number_of_experiments()
    {
        int buf_size =  cache_sizes['1'] * 0.5;
        _buffer_sizes.push_back(buf_size);
        float osnovaniye = 0;
        if ((cache_sizes['3']))
        {
            int max_buf = 1.5 * (cache_sizes['3']);
                        while (pow(2.0, osnovaniye) < max_buf)
                        {
                                buf_size = pow(2.0, osnovaniye);
                                if (buf_size <= _buffer_sizes[0])
                                {
                    if ((cache_sizes['1']) < (cache_sizes['2']))
                    {
                                            osnovaniye++;
                                            continue;
                    }
                                }
                                _buffer_sizes.push_back(buf_size);
                                osnovaniye++;
                        }
                        _buffer_sizes.push_back(max_buf);
        } else {
            int max_buf = 1.5 * (cache_sizes['2']);
            while (pow(2.0, osnovaniye) < max_buf)
            {
                buf_size = pow(2.0, osnovaniye);
                if (buf_size == _buffer_sizes[0])
                {
                    if ((cache_sizes['1']) < (cache_sizes['2']))
                    {
                                                osnovaniye++;
                                                continue;
                                        }
                }
                _buffer_sizes.push_back(buf_size);
                osnovaniye++;
            }
            _buffer_sizes.push_back(max_buf);
        }
        _number_of_experiments = _buffer_sizes.size();
    }

void Experiment::create_test_buffer(const size_t _quantity)
    {
	unsigned now = time(0);
        _buffer = new unsigned char[_quantity];
        for (size_t i = 0; i < _quantity; ++i){
            //_buffer[i] = rand()%256;
            _buffer[i] = static_cast<char>(rand_r(&now) % 256 - 128);
        }
    }

void Experiment::warm_up_cache(const size_t _size)
    {
	unsigned now = time(0);
        for (size_t j = 0; j < 100; ++j)
        {
            for (size_t k = 0; k < _size; ++k)
            {
                //_buffer[k] = rand()%255;
                _buffer[k] = static_cast<char>(rand_r(&now) % 256 - 128);
            }
        }
    }

void Experiment::run(size_t _size)
    {
        _ofile << _size << "  ";
        clock_t start = 0;
	unsigned now = time(0);
        if (travel_variant == "Direct")
        {
            start = clock();
            for (size_t ii = 0; ii < 1000; ++ii)
            {
                srand(time(NULL));
                for (size_t jj = 0; jj < _size; ++jj)
                {
                    //_buffer[jj] = rand()%255;
                    _buffer[jj] = static_cast<char>(rand_r(&now) % 256 - 128);
                }
            }
        } else if (travel_variant == "Return") {
            start = clock();
            for (size_t ii = 0; ii < 1000; ++ii)
            {
                srand(time(NULL));
                for (size_t jj = (_size -1); jj >= 0 ; --jj)
                {
                    //_buffer[jj] = rand()%255;
                    _buffer[jj] = static_cast<char>(rand_r(&now) % 256 - 128);
                }
             }
        } else {
            vector <int> random_indexes;
            for (size_t cx = 0; cx < _size; ++cx)
            {
                random_indexes.push_back(cx);
            }
            random_shuffle(random_indexes.begin(), random_indexes.end());
            start = clock();
            now = time(0);
            for (size_t ii = 0; ii < 1000; ++ii)
            {
                 for (size_t jj = 0; jj < _size; ++jj)
                 {
                     //_buffer[random_indexes[jj]] = rand()%255;
                     _buffer[jj] = static_cast<char>(rand_r(&now) % 256 - 128);
                  }
             }
        }

        stop(start);
    }

void Experiment::stop(clock_t start)
    {
        clock_t stop_t = clock();
        clock_t duration = stop_t - start;
        cout << duration << "ms" << endl;
        _duration.push_back(duration);
        _ofile << duration << endl;
    }

void Experiment::print_results()
    {
        cout << "investigation:" << endl;
        cout << "\ttravel_variant: " << travel_variant << endl;
        cout << "\texperiments:" << endl;
        for (size_t i = 0; i < _number_of_experiments; ++i)
        {
            cout << "\t--experiment:" << endl;
            cout << "\t\tnumber:" << i << endl;
            cout << "\t\tinput data: "  << endl;
            cout << "\t\t buffer_size:";
            if ((_buffer_sizes[0] >= _buffer_sizes[1]) && (!i))
                cout << _buffer_sizes[i] << "kb ";
            else
                cout << _buffer_sizes[i] << "mb ";
            cout << endl << "\t\tresults:" << endl;
            cout << "\t\t duration: ";
            cout << _duration[i] << "ms";
            cout << endl;
        }
    }

void Experiment::just_do_it()
    {
        count_number_of_experiments();
        for (size_t j = 0; j < _number_of_experiments; ++j)
        {
            int factor = pow(2.0, 20.0);
            if ((_buffer_sizes[0] >= _buffer_sizes[1]) && (!j))
            {
                 factor = pow(2.0, 10.0);
            }
            size_t size = _buffer_sizes[j]*factor;
            create_test_buffer(size);
            warm_up_cache(size);
            run(size);
        }
        print_results();
    }

void Pushnoy(int l1, int l2, int l3)
{
    Experiment ex_1(l1, l2, l3);
    ex_1.just_do_it();

    Experiment ex_2 = ex_1;
    ex_2.just_do_it();

    Experiment ex_3 = ex_2;
    ex_3.just_do_it();
}

int main()
{
    cout << "It`s alive!" << endl;
    Pushnoy(32, 3, 0);
    return 0;
}
