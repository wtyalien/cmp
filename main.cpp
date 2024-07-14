#include <iostream>
#include <string>
#include <utility>
#include "functor_cmp.h"
#include "vector"
#include "algorithm"
class employee{
public:
    employee(std::string name,int age):emp_name(std::move(name)),emp_age(age){}
    bool operator<(const employee &item) const{//重载小于号，这样cmp在使用小于号时，也会使用重载版本<
        return this->emp_age < item.emp_age;//this可以省略
    }
//private:
    std::string emp_name;
    int emp_age;
};

class ResourceHolder {
public:
    ResourceHolder(const char* res) : resource_(res) {}//这是普通构造，不是拷贝构造
    // 移动构造函数
    ResourceHolder(ResourceHolder&& other) noexcept
            : resource_(std::move(other.resource_)) {
        // 资源已经被移动，清空原始对象的资源
        other.resource_.clear();
    }
    // 移动赋值运算符（本来不需要的，但是std::sort老是想用被delete的拷贝构造和赋值
    ResourceHolder& operator=(ResourceHolder&& other) noexcept {
        if (this != &other) {//避免移动自己本身，导致悬挂
            resource_ = std::move(other.resource_);//移动
            other.resource_.clear();//把被移动的资源clear
        }
        return *this;
    }

    ResourceHolder (const ResourceHolder & )=delete;//禁拷贝构造
    ResourceHolder& operator= (const ResourceHolder & )=delete;//禁赋值，且必须返回类型为引用，因为赋值会使得当前对象被修改


    /*cmp版本1
    bool operator<(const ResourceHolder & item) const {
        return resource_<item.resource_; //同1，也是重载比较符 来让cmp里的小于号 实现这种自定义的比大小
        //★特别注意，这里是重载的小于号，而cmp重载的是小括号
    }

     */

    // 成员变量，用于存储资源
    std::string resource_;


};
int main() {
    //0常规系统默认类型
    functor_cmp cmp;
    std::vector<double> arr={1,6234.58,53.4,743.5,5.2,6.47,6.3,52.3,35.0};
    std::sort(arr.begin(),arr.end(),cmp);
    //打印结果
    for(auto i:arr){
        std::cout<<i<<std::endl;
    }



    //1使用自定义类型
    functor_cmp cmp2;
    std::vector<employee> arr2{
            {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}
    };
    std::sort(arr2.begin(), arr2.end(), cmp2);
    for (const auto& emp : arr2) {
        std::cout << emp.emp_name << " : " << emp.emp_age << std::endl;
    }

    //2使用移动语义,假设ResourceHolder是资源密集类，string是资源
    functor_cmp cmp3;

    /*
    std::vector<ResourceHolder> resources = {//看似普通构造，实际上因为花括号的存在，变成了列表初始化的拷贝构造！！
            ResourceHolder("Resource A"),
            ResourceHolder("Resource C"),
            ResourceHolder("Resource B"),
    };
     */
    std::vector<ResourceHolder> resources;
    // 法一：使用 emplace_back 来添加新元素，避免使用拷贝构造函数

    resources.emplace_back("Resource A");
    resources.emplace_back("Resource D");
    resources.emplace_back("Resource C");

    resources.emplace_back("Resource E");//就地构造，既不是拷贝、也不是移动

    //法二：下面两行代码是绑定使用的，先创造临时对象，然后move进去， 这两行代码是完全等价于一行代码emplace的
    ResourceHolder res_B("Resource B");//普通构造
    resources.push_back(std::move(res_B));//移动构造

    // 使用仿函数对资源进行排序
    std::sort(resources.begin(), resources.end(),
              [](const ResourceHolder& a, const ResourceHolder& b) {
                  return a.resource_ < b.resource_;
              });

    // 打印排序后的结果
    for (const auto& res : resources) {
        std::cout << res.resource_ << std::endl;
    }


    return 0;
}
