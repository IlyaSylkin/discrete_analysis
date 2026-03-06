#include <iostream>
#include <vector>
#include <string>

struct Date {
    int day;
    int month;
    int year;
};

void parse_date(const std::string& s, Date& d) {
    size_t p1 = s.find('.');
    size_t p2 = s.find('.', p1 + 1);

    d.day = std::stoi(s.substr(0, p1));
    d.month = std::stoi(s.substr(p1 + 1, p2 - p1 - 1));
    d.year = std::stoi(s.substr(p2 + 1));
}

void counting_sort_digit(std::vector<int>& order,
                         std::vector<int>& buffer,
                         const std::vector<Date>& dates,
                         int exp,
                         int part)
{
    int count[10] = {0};

    for (int idx : order) {
        int value;

        if (part == 0) value = dates[idx].day;
        else if (part == 1) value = dates[idx].month;
        else value = dates[idx].year;

        int digit = (value / exp) % 10;
        count[digit]++;
    }

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = order.size() - 1; i >= 0; i--) {
        int idx = order[i];

        int value;
        if (part == 0) value = dates[idx].day;
        else if (part == 1) value = dates[idx].month;
        else value = dates[idx].year;

        int digit = (value / exp) % 10;

        buffer[--count[digit]] = idx;
    }

    order.swap(buffer);
}

void radix_sort(std::vector<int>& order,
                std::vector<int>& buffer,
                const std::vector<Date>& dates,
                int part)
{
    int max_val = 0;

    for (int idx : order) {
        int value;
        if (part == 0) value = dates[idx].day;
        else if (part == 1) value = dates[idx].month;
        else value = dates[idx].year;

        if (value > max_val)
            max_val = value;
    }

    for (int exp = 1; max_val / exp > 0; exp *= 10)
        counting_sort_digit(order, buffer, dates, exp, part);
}

int main() {
    std::vector<std::string> lines;
    std::vector<Date> dates;

    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;

        Date d;
        parse_date(line.substr(0, tab), d);

        lines.push_back(line);
        dates.push_back(d);
    }

    int n = lines.size();
    if (n == 0) return 0;

    std::vector<int> order(n);
    std::vector<int> buffer(n);

    for (int i = 0; i < n; i++)
        order[i] = i;

    radix_sort(order, buffer, dates, 0); 
    radix_sort(order, buffer, dates, 1); 
    radix_sort(order, buffer, dates, 2); 

    for (int idx : order)
        std::cout << lines[idx] << '\n';
}