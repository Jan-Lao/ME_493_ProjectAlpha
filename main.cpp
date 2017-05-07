//
//  main.cpp
//  Project Alpha Resend Draft 1
//
//  Created by Jan Lao on 4/27/17.
//  Copyright © 2017 University Nevada, Reno. All rights reserved.
//

#include <iostream>
#include <random>
#include <assert.h>
#include <numeric>

#define JLRAND (double)rand()/RAND_MAX

using namespace std;

int num_arms = 3;

class Agent{
public:
    int action;
    double alpha = 0.1;
    double epsilon = 0.25;
    double reward;
    double Q;
    
    void Qvectorinit();
    int decide();
    int act();
    void react(double rr);
    
    vector<double> values;
};

class Arm{
public:
    double mu;
    double sigma;
    
    void arminit();
};

class Bandit{
public:
    int choice;
    int result;
    
    double box_muller(double mu, double sigma);
    vector<Arm> start();
    void run();
    void TestB(double rr);
    
    vector<Arm> MAB;
    
    Agent*p1;
};

/////====================================AGENT====================================
////-------------------------Qvectorinit()-----------------------------
void Agent::Qvectorinit(){
    for(int i=0; i<num_arms; i++){
        values.push_back(0);
    }
}

////-------------------------box_muller()-----------------------------
double Bandit::box_muller(double mu, double sigma){
    //BOX MULLER TRANSFORMATION RAND NUM GENERATIOR.......
    //........Found online from the Class Wiki and other sites combined
    //mean = mu; sigma = variance = stddev^2
    static int n = 0;
    double x1, x2, y1, r;
    static double  y2;
    
    if (n){
        y1 = y2;
        n = 0;
    }
    else{
        do{
            x1 = 2.0*rand()/RAND_MAX -1;
            x2 = 2.0*rand()/RAND_MAX -1;
            r = x1 * x1 + x2 * x2;
        }
        while(r >= 1.0);
        r = sqrt((-2.0 * log(r))/r);
        y1 = x1 * r;
        y2 = x2 * r;
        n = 1;
    }
    return (mu + y1 * sqrt(sigma)); //Eq: mean+y1*stddev
}

////-------------------------Decide()-----------------------------
int Agent::decide(){
    double placeholder = JLRAND;
    //action = 0;
    cout << "The Epsilon Value is: " << placeholder << endl; //debugging
    if (placeholder <= epsilon){
        //Choose the random action (epsilon)% of the time
        cout << "Random action taken..." << endl;
        action = rand()%num_arms;
    }
    else if(placeholder > epsilon){
        cout << "Greedy action taken..." << endl;
        int maxindex = 0;
        for(int j = 0; j< num_arms; j++){
            if (values.at(maxindex)<values.at(j)){
                maxindex = j;
            }
        }
        action = maxindex;
        //cout << maxindex << endl; //For debugging purposes prior reward structure
    }
    else {
        action = 0;
        cout << "First arm pulld because it failed:" << placeholder << endl;
    }
    cout << "The current action is: " << action << endl;
    return action;
}

////-------------------------React()-----------------------------
void Agent::react(double rr){
    reward = rr;
    
    Q = values.at(action);
    Q = reward * alpha + Q*(1-alpha);
    //cout << Q << endl;
    values.at(action) = Q;
}


/////====================================BANDIT====================================
////-------------------------start()-----------------------------
void Arm::arminit(){
    mu = abs((((double)rand() / RAND_MAX) - 0.5) * 100);
    sigma = abs((((double)rand() / RAND_MAX) - 0.5) * 2);
}

////-------------------------start()-----------------------------
vector<Arm> Bandit::start(){
    
    for(int k=0; k < num_arms; k++){
        Arm A;
        A.arminit();
        MAB.push_back(A);
    }
    return MAB;
}

////-------------------------run()-----------------------------
void Bandit::run(){
    start();
    p1->Qvectorinit();
    int moves = 0;
    
    vector<double> TestBArm0;
    vector<double> TestBArm1;
    vector<double> TestBArm2;
    
    for(int w=0; w<100; w++){
        cout << "============================================================" << endl;
        cout << endl << "Q-Learner: New Iteration" << endl;
        cout << "-------------------------------------------------" << endl;
            
        choice = p1->decide(); //choice = action
            
        double rr;
        double a;
        double b;
        a = MAB.at(choice).mu; //mu @ choice
        b = MAB.at(choice).sigma; //sigma @ choice
        rr = box_muller(a, b); //pull with choice and corresponding mu and sigma
        cout << "Pulling choice " << choice << "'s result: " << rr << endl; //output the result of the pull
        p1->react(rr);
        
        //TestB(rr) is hard coded for 3 distinct levers
        
        if(choice == 0){
            TestBArm0.push_back(rr);
        }
        else if(choice == 1){
            TestBArm1.push_back(rr);
        }
        else if(choice == 2){
            TestBArm2.push_back(rr);
        }
        
        moves++;
    }
    cout << endl << endl << "EPISODES ENDED. THE MOVES TAKEN ARE: " << moves << endl;
    cout << "============================================================" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    
    cout << "The pull frequency of the 0TH hard coded arm is: " << TestBArm0.size() << endl;
    cout << "The pull frequency of the 1ST hard coded arm is: " << TestBArm1.size() << endl;
    cout << "The pull frequency of the 2ND hard coded arm is: " << TestBArm2.size() << endl << endl;
    
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    start();
}

/////====================================MISC.====================================

void TestA(){
    vector <double> TestAresults;
    int big_number = 10000; //some arbitrarily big number
    int TestArr;
    int totalresultsA;
    int averageA;
    
    Arm TestAArm; //Testing and initializing one arm
    TestAArm.arminit();
    
    Bandit TestABandit;
    
    //Pulling said arm "big_number" of times
    for(int i=0; i< big_number; i++){
        TestArr = TestABandit.box_muller(50, 1); //Testing on mu =50.
        TestAresults.push_back(TestArr);
    }
    
    totalresultsA = accumulate(TestAresults.begin(), TestAresults.end(), 0);
    averageA = totalresultsA / TestAresults.size();
    
    //Asserting that the average calculated is within a margin of error
    assert(averageA > 48 && averageA < 52);
    cout << endl << "Test A succeeded. Test arm coverges to mu after " << big_number << " runs." << endl;
}

void Bandit::TestB(double rr){
    vector<double> TestBArm0;
    vector<double> TestBArm1;
    vector<double> TestBArm2;
    
    //TestB is hardcoded for 3 distinct levers
    if(choice == 0){
        TestBArm0.push_back(rr);
    }
    else if(choice == 1){
        TestBArm1.push_back(rr);
    }
    else if(choice == 2){
        TestBArm2.push_back(rr);
    }
}

/////====================================MAIN====================================
int main() {
    srand(time(NULL));
    Agent agent;
    Bandit bandit;
    
    cout << "Welcome to the Multi-Armed Bandit, Agent!" << endl;
    
    Agent*p1=&agent;
    bandit.p1 = p1;
    
    for(int u = 0; u<30; u++){ //for 30 statistical runs
        bandit.run();
        cout << "STATISTICAL RUN ENDED" << endl;
    }
    
    TestA();
    
    return 0;
}
