#include <queue>
template<typename T, typename comp>
class cpriority_queue : public std::priority_queue<T, std::vector<T>, comp >
{
  public:

      bool remove(int index) {
          auto it = this->c.begin() + index;
       
          if (it == this->c.end()) {
              return false;
          }
          if (it == this->c.begin()) {
              // deque the top element
              this->pop();
          }    
          else {
              // remove element and re-heap
              this->c.erase(it);
              std::make_heap(this->c.begin(), this->c.end(), this->comp);
         }
         return true;
     }
     T get(int index) {
          auto it = this->c.begin() + index;
          return (*it);
     }
};

