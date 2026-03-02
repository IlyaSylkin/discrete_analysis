#include <iostream>
#include <cstdio>
#include <ostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>


class Date {
    private:
        std::string date;
        std::string key;
        int days_since_christ_birth;
        
    public:
        Date(std::string &date_from_ctdin, std::string &key_from_ctdin){
            date = date_from_ctdin;
            key = key_from_ctdin;
            days_since_christ_birth = get_days_since_c_b_from_ddmmyyyy(date_from_ctdin);
        }

        Date(){
            date = "";
            key = "";
            days_since_christ_birth = 0;
        }

        int get_days_since_c_b_from_ddmmyyyy(std::string &date_from_ctdin){
            size_t p1 = date_from_ctdin.find('.');
            size_t p2 = date_from_ctdin.find('.', p1 + 1);

            int days = static_cast<unsigned>(std::stoi(date_from_ctdin.substr(0, p1)));
            int months = static_cast<unsigned>(std::stoi(date_from_ctdin.substr(p1 + 1, p2 - p1 - 1)));
            int years = std::stoi(date_from_ctdin.substr(p2 + 1));

            std::chrono::year_month_day chrono_date = std::chrono::year_month_day(
                std::chrono::year(years),
                std::chrono::month(months),
                std::chrono::day(days)
            );

            std::chrono::sys_days chrono_days = chrono_date;

            std::chrono::sys_days chrono_start = std::chrono::year_month_day(
                std::chrono::year(1),
                std::chrono::month(1),
                std::chrono::day(1)
            ); 
            
            return (chrono_days - chrono_start).count();

        }

        int get_days_since_c_b(){
            return days_since_christ_birth;
        }

        std::string get_date(){
            return date;
        }

        std::string get_key(){
            return  key;
        }

    };

std::vector<Date> radix_sort(std::vector<Date> &data, int exp){

    int counts[10] {};
    std::vector<Date> result_vector(data.size());
    int index;
    
    for (int i = 0; i < data.size(); ++i){
        index = (data[i].get_days_since_c_b() / exp) % 10;
        counts[index]++;
    }

    for (int i = 1; i < 10; ++i){
        counts[i] += counts[i-1];

    }

    for (int i = data.size() - 1; i >= 0; --i){
        index = (data[i].get_days_since_c_b() / exp) % 10; //цифра для элемента из иисходного
        result_vector[counts[index] - 1] = data[i];
        counts[index] -= 1;
    }

    return result_vector;
}

int main() {
    std::vector<Date> data;
    std::string line;
	std::ifstream file("test_input.txt");

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t tab = line.find('\t');
        // std::cout << "tab" << tab << std::endl;
        if (tab == std::string::npos) continue;

        std::string date = line.substr(0, tab);
        std::string key = line.substr(tab + 1);

        //обрезаем только дату слева
        while (!date.empty() && date.front() == ' ')
            date.erase(date.begin());
        if (date.empty()) continue;
        data.emplace_back(date, key);
        // std::cout << date << "|" << key << std::endl;
    }

	if (data.empty())
    	return 0;

    int max_val = data[0].get_days_since_c_b();

    for (size_t i = 1; i < data.size(); ++i) {
		
        if (data[i].get_days_since_c_b() > max_val) {
            max_val = data[i].get_days_since_c_b();
			
        }
    }
    
    int exp = 1;
    while (max_val / exp > 0) {
        data = radix_sort(data, exp);
        exp *= 10;
    }
	std::ofstream fout("res1.txt");
	for (int i = 0; i < data.size(); ++i){
    fout << data[i].get_date() << "\t" << data[i].get_key() << '\n';
	}
	fout.close();
    // for (int i = 0; i < data.size(); ++i){
    //     std::cout << data[i].get_date() << "\t" << data[i].get_key() << '\n';
    // }

    return 0;
}