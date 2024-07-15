//
// Created by wty on 2024/7/12.
//

#ifndef FUNCTOR_CMP_FUNCTOR_CMP_H
#define FUNCTOR_CMP_FUNCTOR_CMP_H

#include<utility>
class functor_cmp {
    public:
    // 默认情况下，使用小于号比较
        template<class T>
        bool operator()(T &&a,T &&b){
            return std::forward<T>(a) < std::forward<T>(b);
        }
        //更进一步，可以接受一个函数（或lambda），来决定cmp逻辑
        // 使用函数对象作为比较逻辑的参数
        template<class T, class Predicate>
        bool operator()(const T& a, const T& b, Predicate pred) {
            return pred(a, b);
        }
        /*predicate的使用例子见main.cpp    （predicate不是关键字，只是一个和T一样的标识符，反正传入的参数类型会被编译器自己识别的
         * //这相当于 重载小于号 的平替版本，这样就不会污染employee类里的小于号
        特别注意，pred最好用lambda，而不是在类里面定义，因为在main里面 是不好去用类里面定义的pred的
         */
};
456
123
我全都要


#endif //FUNCTOR_CMP_FUNCTOR_CMP_H
