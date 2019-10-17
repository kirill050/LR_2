#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

class Experiment{
public:
	Experiment(const int &l1, const int &l2, const int &l3){
		travel_variant = "Direct";
		_ofile.open("direct2.txt");
		cache_sizes['1'] = l1;
		cache_sizes['2'] = l2;
		cache_sizes['3'] = l3;
	}
	Experiment(Experiment &prev_ex){
		if (prev_ex.travel_variant == string("Direct")){
			travel_variant = "Return";
			_ofile.open("return2.txt");
		}
		else{
			_ofile.open("random2.txt");
			travel_variant = "Random";
		}
		//cache_sizes = prev_ex.cache_sizes;
		cache_sizes['1'] = prev_ex.cache_sizes['1'];
                cache_sizes['2'] = prev_ex.cache_sizes['2'];
                cache_sizes['3'] = prev_ex.cache_sizes['3'];
	}
	~Experiment(){
		_ofile.close();
		delete _buffer;
	}
	string get_travel_variant(){return travel_variant;}
	void run(int _size){
		_ofile << _size << "  ";
		unsigned start = 0;
		if (travel_variant == "Direct"){
			start = clock();
			for (int ii = 0; ii < 1000; ii++){
				srand(time(NULL));
				for (int jj = 0; jj < _size; jj++){
					_buffer[jj] = rand()%255;
				}
			}
		}
		else if (travel_variant == "Return"){
			start = clock();
			for (int ii = 0; ii < 1000; ii++){
                                srand(time(NULL));
                                for (int jj = (_size -1); jj >= 0 ; jj--){
                                        _buffer[jj] = rand()%255;
                                }
                        }
		}
		else{
			vector <int> random_indexes;
			for (int cx = 0; cx < _size; cx++){
				random_indexes.push_back(cx);
			}
			start = clock();
			for (int ii = 0; ii < 1000; ii++){
                                srand(time(NULL));
				random_shuffle(random_indexes.begin(), random_indexes.end());
                                for (int jj = 0; jj < _size; jj++){
                                        _buffer[random_indexes[jj]] = rand()%255;
                                }
                        }
		}

		stop(start);
	}
	void stop(unsigned start){
		unsigned stop_t = clock();
		unsigned duration = stop_t - start;
		cout << ((float) duration) / CLOCKS_PER_SEC << "sec" << endl;
		_duration.push_back(duration);
		_ofile << ((float) duration) / CLOCKS_PER_SEC << endl;
	}
	void count_number_of_experiments(){
		int buf_size =  cache_sizes['1'] * 0.5;
		_buffer_sizes.push_back(buf_size);
		float osnovaniye = 0;
		if ( (cache_sizes['3']) ){
			int max_buf = 1.5 * (cache_sizes['3']);
                        while (pow(2.0, osnovaniye) < max_buf ){
                                buf_size = pow(2.0, osnovaniye);
                                if (buf_size <= _buffer_sizes[0]){
					if ( (cache_sizes['1']) < (cache_sizes['2']) ){
                                        	osnovaniye++;
                                        	continue;
					}
                                }
                                _buffer_sizes.push_back(buf_size);
                                osnovaniye++;
                        }
                        _buffer_sizes.push_back(max_buf);
		}
		else{
			int max_buf = 1.5 * (cache_sizes['2']);
			while (pow(2.0, osnovaniye) < max_buf ){
				buf_size = pow(2.0, osnovaniye);
				if (buf_size == _buffer_sizes[0]){
					if ( (cache_sizes['1']) < (cache_sizes['2']) ){
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
	void create_test_buffer(const int _quantity){
		_buffer = new unsigned char[_quantity];
		for (int i = 0; i < _quantity; i++){
			_buffer[i] = rand()%256;
		}
	}
	void warm_up_cache(const int _size){
		for (int j = 0; j < 100; j++){
			for (int k = 0; k < _size; k++){
				_buffer[k] = rand()%255;
			}
		}
	}
	void print_results(){
		cout << "investigation:" << endl;
		cout << "\ttravel_variant: " << travel_variant << endl;
		cout << "\texperiments:" << endl;
		for (int i = 0; i < _number_of_experiments; i++){
			cout << "\t--experiment:" << endl;
			cout << "\t\tnumber:" << i << endl;
			cout << "\t\tinput data: "  << endl;
			cout << "\t\t buffer_size:";
			if ( (_buffer_sizes[0] >= _buffer_sizes[1]) && (!i)){
                        	cout << _buffer_sizes[i] << "kb ";
                	}
                	else cout << _buffer_sizes[i] << "mb ";
			cout << endl << "\t\tresults:" << endl;
			cout << "\t\t duration: " << ((float) _duration[i]) / CLOCKS_PER_SEC << "sec";
			cout << endl;
		}
	}
	void just_do_it(){
		count_number_of_experiments();
		for (int j = 0; j < _number_of_experiments; j++){
			int factor = pow(2.0, 20.0);
			if ( (_buffer_sizes[0] >= _buffer_sizes[1]) && (!j) ){
				 factor = pow(2.0, 10.0);
			}
			int size = _buffer_sizes[j]*factor;
			create_test_buffer(size);
			warm_up_cache(size);
			run(size);
		}
		print_results();
	}
public:// DO NOT FORGET TO MAKE THEM PRIVATE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	string travel_variant;
	map <char, int> cache_sizes;
	int _number_of_experiments;
	vector <int> _buffer_sizes;
	unsigned char *_buffer;
	vector <int> _duration;
	ofstream _ofile;
};
void pushnoy(int l1, int l2, int l3){
	Experiment ex_1(l1, l2, l3);
	ex_1.just_do_it();

	Experiment ex_2 = ex_1;
	ex_2.just_do_it();

	Experiment ex_3 = ex_2;
        ex_3.just_do_it();
}
int main(){
	cout << "It`s alive!" << endl;
        pushnoy(32, 3, 0);
	return 0;
}
