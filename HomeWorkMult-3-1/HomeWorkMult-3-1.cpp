#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <Windows.h>

void min_i(int current_min_i, std::vector<int>& sorted, std::promise<int> prom) {
    for (int j{current_min_i}; j < sorted.size(); ++j) {
            if (sorted.at(j) <= sorted.at(current_min_i)) {
                current_min_i = j;
            }
        }
    prom.set_value(current_min_i);
}

void selection_sort (std::vector<int>& sorted) {
    int min{}, min_index{};
    for (int i{}; i < sorted.size(); ++i) {
        min = sorted.at(i);
        min_index = i;
        std::promise<int> prom;
        std::future<int> ft_res = prom.get_future();
        auto res = std::async(min_i, min_index, sorted, std::move(prom));
        min_index = ft_res.get();
        min = sorted.at(min_index);
        if (sorted.at(i) > sorted.at(min_index)) {
            int temp{sorted.at(i)};
            sorted.at(i) = sorted.at(min_index);
            sorted.at(min_index) = temp;
        }
    } 
}

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

int main() {
    int elem{100000};
    std::vector<int> sorted{};
    srand(time(NULL));
    for (int i{}; i  < elem; ++i) {
        sorted.push_back(rand());
    }
    std::packaged_task<void(std::vector<int>&)> task(selection_sort);
    std::future<void> f = task.get_future();
    auto start = std::chrono::steady_clock::now();
    std::thread th(std::move(task), std::ref(sorted));
    th.detach();
    int counter{};
    auto status = f.wait_for(std::chrono::milliseconds(0));
    system("cls");
    std::cout << "Execution";
    while (status != std::future_status::ready) {
        if (counter == 3) {
            setcur(9,0);
            std::cout << "   ";
            counter = 0;
        }
        else {
            setcur(counter + 9,0);
            std::cout << ".";
            counter++;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        status = f.wait_for(std::chrono::milliseconds(0));
    }
    f.get();
    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    system("cls");
    std::cout << "Time: " << double(diff.count()) / 1000 << " s\n";
    std::cout << "Begin: ";
    for (int i{}; i < 15; ++i) {
        std::cout << sorted.at(i) << " ";
    }
    std::cout << "\nEnd: ";
    for (unsigned _int64 i{sorted.size() - 15}; i < sorted.size(); ++i) {
        std::cout << sorted.at(i) << " ";
    }
}