#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <Windows.h>

void selection_sort (std::vector<int>& sorted) {
    int min{}, min_index{};
    for (int i{}; i < sorted.size(); ++i) {
        min = sorted.at(i);
        min_index = i;
        for (int j{i}; j < sorted.size(); ++j) {
            if (sorted.at(j) <= min) {
                min = sorted.at(j);
                min_index = j;
            }
        }
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