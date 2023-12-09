#include <iostream>
#include <vector>

#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>

int partition(std::vector<int> &a, int l, int r) {
    std::srand((unsigned) time(NULL));
    int pivot = a[std::rand() % (r-l) + l];
    int i = l;
    int j = r;
    while(i<=j) {
        while(a[i] < pivot) ++i;
        while(a[j] > pivot) --j;
        if (i >= j) break;

        std::swap(a[i++], a[j--]);
    }
    return j;
}

void sequential(std::vector<int> &a, int l, int r) {
    if (r - l > 0) {
        int idx = partition(a, l, r);
        sequential(a, l, idx);
        sequential(a, idx + 1, r);
    }
}

void parallel(std::vector<int> &a, int l, int r) {
    if (r - l < 1000) {
        sequential(a, l, r);
        return;
    }
    int i = partition(a, l, r);

    parlay::par_do(
            [&]() { parallel(a, l, i); },
            [&]() { parallel(a, i + 1, r); }
    );
}

std::vector<int> generate_random_vector(int n) {
    std::vector<int> a = std::vector<int>(n, 0);
    std::srand((unsigned) time(NULL));
    for (int i = 0; i < n; i++) {
        a[i] = std::rand() % 20;
    }
    return a;
}


long long int benchmark(const std::function<void(std::vector<int>&, int, int)>& quicksort) {
    std::vector<int> a = generate_random_vector(100'000'000);
    auto start = std::chrono::high_resolution_clock::now();
    quicksort(a, 0, a.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    std::destroy(a.begin(), a.end());
    std::cout << (end - start).count() << '\n';
    return (end - start).count();
}


int main() {
    int repeat = 5;
    long long int time_seq = 0;
    std::cout << "SEQUENTIAL\n";
    for (int i = 0; i < repeat; i++) {
        time_seq += benchmark(sequential);
    }
    std::cout <<"AVERAGE " << time_seq / repeat << '\n';

    long long int time_par = 0;
    std::cout << "PARALLEL\n";
    for (int i = 0; i < repeat; i++) {
        time_par += benchmark(parallel);
    }
    std::cout <<"AVERAGE " << time_par / repeat << '\n';
    std::cout << "BOOST " << (double) time_seq / time_par;

    return 0;
}
