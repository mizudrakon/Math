#ifndef QUICK_H
#define QUICK_H

#include <concepts>
#include <format>
#include <queue>
#include <memory>
#include <vector>

//convenient print function using format
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
            //print("range struct made\n");
        }
        size_t size() {return end - start; }
        ~Range(){
            //print("destr\n");
        }
    };

    template <Iterator It>
    It three_med(It s, It e){
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
                continue;
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

    //SELECT - UNTESTED
    template<Iterator>
    using vector = std::vector<Iterator>;
    
    template <Iterator It, Container C>
    vector<It> copy_to_vec(const It& start, const It& end, size_t size){
        vector<It> vec;
        for (auto c_it = start; c_it < end; ++c_it)
            vec.push_back(c_it);

        while (vec.size() < size)
            vec.push_back(nullptr);
        return vec;
    } 

    //sorts and returns k-th element
    template <Iterator It, typename F>
    It trivial_select(const size_t k, const It& start, const It& end, F f){
        vector<It> vec = copy_to_vec(start, end, end-start);
        if (k > end-start) k = end - start; //just to be sure
        sort(vec.start(), vec.end(), f);
        return *(vec.start() + k - 1);
    }

    //gets median of five elements, if we give it fewer elements, it adds nullpointers
    template <Iterator It, typename F>
    It five_median(const It& start, const It& end, F f){
        const size = 5;
        vector<It> vec = copy_to_vec(start, end, size);
        sort(vec.begin(),vec.end(),f);
        return *(vec.begin() + size / 2);
    }

    //quicksort algorithm
    template <Iterator It, typename F>
    It Select(const size_t k, const It& start, const It& end, F f){
        if (end - start <= 5) 
            return trivial();
        vector<It> m;
        for (auto& it = start; it+5 < end; it += 5){
            m.push_back(five_median(it, it+5, f));
        }
        auto p = Select(m.size() / 2, m.begin(), m.end(), f);
        vector<It> L_p, G_p, E_p;
        for (auto& it = start; it < end; ++it){
            if (f(*it,*p))
                L_p.push_back(it);
            else if (*it == *p){
                E_p.push_back(it);
                continue;
            }
            G_p.push_back(it);
        }
        if (k <= L_p.size()) return Select(k,L_p.begin(),L_p.end(),f);
        else if (k <= L_p.size() + G_p.size()) return p;
        return Select(k - L_p.size() - G_p.size(), G_p, f);
    }
}
#endif