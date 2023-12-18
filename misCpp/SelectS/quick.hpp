#ifndef QUICK_H
#define QUICK_H

//#include <vector>
#include <concepts>
#include <format>
#include <iostream>
#include <queue>
#include <memory>

constexpr void print(const std::string_view text, auto&&... args){
    fputs(std::vformat(text,std::make_format_args(args...)).c_str(),stdout);
}

template <class C>
concept Container = requires(C lst){
    lst.begin();
    lst.end();
    lst[0];
};

template <typename T>
concept Iterator = requires(T a, T b){
    //need to be comparable and point to comparable values
    a + 1;
    a - b;
    a == b;
    a < b;
    *a < *b;
    *a == *b;
};

namespace Quick
{
    template <Iterator It>
    struct Range {
        It start;
        It end;
        Range(const Iterator auto& s, const Iterator auto& e):start(s),end(e){
            print("range struct made\n");
        }
        size_t size() {return end - start; }
        ~Range(){print("destr\n");}
    };

    template <Iterator It>
    It three_med(const It s, const It e){
        It m = (s + (e - s)/2);
        //Iterator f = s, l = e;
        if (*s > *m) std::swap(s,m);
        if (*m > *e) std::swap(m,e);
        if (*s > *m) std::swap(s,m);
        return m;
    }

    template <Iterator It, typename F>
    void procedure_sort(const It& start,const It& end, F f){
        //we queue start and end iterators instead of calling the procedure recursively
        std::queue<std::unique_ptr<Range<It>>> R;    
        auto first = std::make_unique<Range<It>>(start,end);
        R.push(std::move(first));
        while (!R.empty())
        {   
            //copy fixed range pointers:
            const auto s = R.front()->start;
            const auto e = R.front()->end;
            //init moving pointers:
            auto i = s, j = e;
            //remove the range element from queue:
            R.pop();
            //check if we even need to bother
            if (j <= i){ 
                break;
            }
            //using the median of three procedure to select the pivot
            auto p = *three_med(i, j);
            //main sorting loop for the given range [s,e]
            while (i <= j){
                while (f(*i,p)) ++i;
                while (f(p,*j)) --j;
                if (i < j) std::swap(*i,*j);
                if (i <= j) ++i,--j;
            }
            //queuing the smaller ranges (s,e are constant borders and i,j the movin ones)
            auto a = std::make_unique<Range<It>>(s, j);
            auto b = std::make_unique<Range<It>>(i, e);
            R.push(std::move(a));
            R.push(std::move(b));
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
}
#endif