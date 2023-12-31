#ifndef QUICK_H
#define QUICK_H

#include <concepts>
#include <format>
#include <queue>
#include <memory>
#include <vector>
#include <utility>
#include "heap.hpp"
/*  Just to be clear, I'm pretending <algorithm> and <iterator> doesn't exist.
    It may seem dumb, since it's generally vary wise to use them, but this is more of an exercise.
    It may be even more funny, since I'm going for C++20 here and some stuff here doesn't work otherwise,
    like <format>, or <concepts>, I'm afraid.
*/

//convenient print function using format C++20
constexpr void print(const std::string_view text, auto&&... args){
    fputs(std::vformat(text,std::make_format_args(args...)).c_str(),stdout);
}


namespace quick
{
    using std::vector, std::swap;
    
    template <Iterator It>
    It three_med(It s, It e){
        It m = (s + (e - s)/2);
        if (*s > *m) swap(s,m);
        if (*m > *e) swap(m,e);
        if (*s > *m) swap(s,m);
        return m;
    }

    template <Iterator It, typename F>
    void procedure_sort(const It& start,const It& end, F f){
        using std::pair, std::make_unique, std::move;
        //we queue start and end iterators instead of calling the procedure recursively
        std::queue<std::unique_ptr<pair<It,It>>> R;    
        auto first = make_unique<pair<It,It>>(start,end);
        R.push(move(first));
        while (!R.empty())
        {   
            //copy fixed range pointers:
            const auto s = R.front()->first;
            const auto e = R.front()->second;
            //init moving pointers:
            auto i = s, j = e;
            //remove the range element from queue:
            R.pop();
            //check if we even need to bother
            if (j <= i){ 
                continue;
            }
            //using the median of three procedure to select the pivot
            auto p = *three_med(i, j);
            //main sorting loop for the given range [s,e]
            while (i <= j){
                while (f(*i,p)) ++i;
                while (f(p,*j)) --j;
                if (i < j) swap(*i,*j);
                if (i <= j) ++i,--j;
            }
            //queuing the smaller ranges (s,e are constant borders and i,j the movin ones)
            auto a = make_unique<pair<It,It>>(s, j);
            auto b = make_unique<pair<It,It>>(i, e);
            R.push(move(a));
            R.push(move(b));
        } 
    }

    template <typename F>
    bool sort(const Iterator auto& start,const Iterator auto& end, F f){
        procedure_sort(start,end-1,f);
        return true;
    }

    void print_vec(const Container auto& vec){
        for (auto i = vec.begin(); i < vec.end(); ++i){
            print("{},",*i);
        }
        print("\n");
    }

    void print_vec(const Iterator auto& start, const Iterator auto& end){
        for (auto i = start; i < end; ++i){
            print("{},",*i);
        }
        print("\n");
    }

    //SELECT - UNTESTED
    
    template <typename T, Iterator It>
    vector<T> copy_to_vec(const It& start, const It& end, size_t size){
        vector<T> vec{};
        for (auto c_it = start; c_it < end && c_it - start < size; ++c_it)
            vec.push_back(*c_it);
        return vec;
    } 

    //sorts and returns k-th element
    template <typename T, typename F>
    T trivial_select(const size_t k, const Iterator auto& start, const Iterator auto& end, F f){
        auto vec = copy_to_vec<T>(start, end, end-start);
        sort(vec.begin(), vec.end(), f);
        print_vec(vec);
        if (k > end-start) return *(vec.end()-1); //just to be sure
        return *(vec.begin() + k - 1);
    }

    //gets median of five elements, if we give it fewer elements, it adds nullpointers
    //I'm making this as an exercise without <algorithm>, so I will need my own heap
    template <typename T, typename F>
    T five_median(const Iterator auto& start, const Iterator auto& end, F f){
        const int size = 5;
        auto vec = copy_to_vec<T>(start, end, size);
        sort(vec.begin(),vec.end(),f);
        return *(vec.begin() + vec.size() / 2);
    }

    //quicksort algorithm divides the group into 3 smaller group and figures out where to look for the answer
    template <typename T, typename F>
    T select(const size_t k, const Iterator auto& start, const Iterator auto& end, F f){
        if (end - start <= 5) 
            return trivial_select<T>(k, start, end, f);
        //getting median of five, I'll change it to heap later
        vector<T> m;
        for (auto it = start; it < end; it += 5){
            int chunk_size = (it + 5 < end)? 5 : end - it;
            m.push_back(five_median<T>(it, it+chunk_size, f));
        }
        print_vec(m);
        auto p = trivial_select<T>(m.size() / 2, m.begin(), m.end(), f);
        print("the pivot is: {}\n", p);
        //3 smaller groups 
        vector<T> L_p, G_p, E_p;//less_than_p, greater, equal
        for (auto it = start; it < end; ++it){
            if (f(*it,p)){
                L_p.push_back(*it);
                continue;
            }
            else if (*it == p){
                E_p.push_back(*it);
                continue;
            }
            G_p.push_back(*it);
        }
        //figure out where to look
        if (k <= L_p.size()) {
            return select<T>(k,L_p.begin(),L_p.end(),f);
        }
        else if (k <= L_p.size() + E_p.size()) return p;
        return select<T>(k - L_p.size() - E_p.size(), G_p.begin(), G_p.end(), f);
    }
}
#endif