#include <vector>
#include <utility>

#ifndef slots
#define slots
struct arm {
    int id;
    int q_val;
    int n=0; //times called;

};

struct comp{
    bool operator()(arm& n1, arm& n2) {
        return n1.q_val < n2.q_val;
    }
};


class SlotMachine {
    private:
    int arms;
    std::vector<int> offsets; // Max and Min for uniform  dist of arm i
    int max;

    int clamp(int val) {
        if (val > max) {
            return max;
        } 
        else if (val < 0 ) {
            return  0;
        }
        else {return val;}
    }
    std::default_random_engine generator;
    std::normal_distribution<double> distribs;
    std::uniform_int_distribution<int> offset;

    public:
    SlotMachine(int arms, int max) { //Initialize array of random ranges for each arm 
        this->arms=arms;
        offsets.resize(arms);
        this->max=max;
        int temp = max%2;
        distribs = std::normal_distribution<double>(0,1);
        offset = std::uniform_int_distribution<int>(-max/2, (max/2) + temp);
    }
    SlotMachine() {arms=0;max=0;}
    int getArms() { return arms; }
    int getMax()  {return max; }
    void fillRange() {//Fills offset array with randomly generated offsets from the mean
        for(auto& v: offsets) {
            v = offset(generator);
        }
    }

    int pullArm(int arm) { //Pulls arm k 
    //get offset
        int arm_offset = offsets[arm];
        return clamp(arm_offset + (int)(max*distribs(generator)));
    }
};
#endif
