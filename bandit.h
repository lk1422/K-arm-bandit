#include <queue> 
#include <random>
#include <vector> 
#include <unordered_map>
#include "Slots.h"
#include "maxheap.h"
//Global Random Variable information for later reuse
#ifndef band
#define  band
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0,1.0);


bool RNG(double divide) {
    return dis(gen) > divide;
}


enum init { ZERO, OPTIMISTIC }; // OPTIMISTIC has the same init as upper confidence

class Bandit {
    private:
    cpriority_queue<arm, comp> q_vals; //stores the arms in max heap fashion
    int iterations; // How many total iterations have passed
    SlotMachine m;
    int arms;
    double epsilon;
    int max;
    int totalReward;

    bool explore() {
        return RNG(1-epsilon);
    }
    public:
    Bandit(SlotMachine& machine, double epsilon, init METHOD) { m=machine;this->max=m.getMax(); this->arms=m.getArms(); this->epsilon=epsilon,initialize_q(METHOD), iterations=0;totalReward=0; } 

    void initialize_q(init METHOD) { // if METHOD is optim expect max to be set with setMax();
        for(int i=0; i<arms; i++) {
                arm temp;
                temp.id=i;
                if (METHOD == ZERO) { temp.q_val=0; }
                else { temp.q_val = max; }
                q_vals.push(temp);
        }
    }

    void takeAction() {
        //Generate next action
        arm arm2pull;
        if (explore()) { // get random node and delete it from queue
            int index;
            std::uniform_int_distribution<> d (0, q_vals.size() -1);
            index = d(gen);
            arm2pull = q_vals.get(index);
            q_vals.remove(index);
        }
        else {
            arm2pull = q_vals.top();
            q_vals.pop();
        }

        //Increment iteration (becuase w init to 0)
        arm2pull.n++;
        iterations++;

        int reward = m.pullArm(arm2pull.id);
        totalReward+=reward;
        std::cout << "Pulled arm: " << arm2pull.id << ", Recieved: " << reward << std::endl;

        sampleAverageUpdate(arm2pull,reward);
        
        q_vals.push(arm2pull);
    }
    
    void sampleAverageUpdate(arm & pulled, int reward) {
        pulled.q_val += (1/pulled.n) * (reward-pulled.q_val);
    }
    void weightedUpdate(double decayRate, arm& pulled, int reward) {
        pulled.q_val = (pulled.q_val * (1-decayRate) ) + ((1-decayRate) * reward);
    }

    void runBandit(int samples) {
        for (int i=0; i<samples; i++) {
            takeAction();
        }
        std::cout << "Total reward: " << totalReward << std::endl<<"Average reward: " << ((double)totalReward/samples) << std::endl;
    }
};



#endif
