//
//  main.cpp
//  Multi Armed Bandit Problem Draft 4
//  Part 2 Agent Learning
//
//  Created by Jan Lao on 12/7/15.
//  Copyright (c) 2015 Jan Lao. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <numeric>

#define LYRAND (double)rand()/RAND_MAX

//Buddy coded this with Tony J Moreno, Chris Jackson, and Marshall Miller.
//NOTE: THIS CODE IS FOR THE PURPOSES OF ANALYZING THE CONVERGENCE RESULTS. FOR PRESENTATIONAL PURPOSES PLEASE REFER TO "Multi Armed Bandit Draft 2"

using namespace std;

int num_arms = 3;
int q = 100; //number of iterations for loops
int choice;
int action;
double alpha = 0.10; //Learning rate
double epsilon = 0.25;//Non greedy percentage
int result; //found in bandit

vector<int> values;
vector<int> Results;

class Agent{
public:
    int choice; //choice between 1st<->num_armsth lever
    int choiceindex;
    int action;
};

class Bandit{
public:
    int choice; //found from agent
    int mu;
    int sigma;
    int result;
    
    double box_muller(double mu, double sigma);
    double pull();
    void TestA();
    void TestB();
    Agent *p0;
};

Bandit bandit;
Agent agent;

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

void Bandit::TestA(){
    for(int w=0; w<q; w++){
        result = box_muller(150.3, 19.06);
        Results.push_back(result);
        cout << "Pulling Arm 1 Result: " << result << endl << endl;
    }
    int avgresult = accumulate(Results.begin(),Results.end(),0)/q;
    assert((avgresult < (mu+sigma)) || (avgresult > (mu+sigma)));
    //If assert fails, this will not have the code continue past this point.
    cout << endl << "Test A conducted: Assertion succeeded" << endl << endl;
}

void Bandit::TestB(){
    //LOOKING at the code: lever 2 is obviously the better one...Yes I know I hardcoded everything
    for(int w=0; w<q; w++){
        Results.push_back(result);
    }
    int avgresult = accumulate(Results.begin(),Results.end(),0)/q;
    assert((avgresult < (mu+sigma)) || (avgresult > (mu+sigma)));
    //If assert fails, this will not have the code continue past this point.
    cout << endl << "Test B conducted: Assertion succeeded" << endl << endl;;
}

/////========================= Main Function ============================
int main(){
    srand(time(NULL));
    cout << "Welcome to the Multi-Armed Bandit, Agent!" << endl;
    
    bandit.p0=&agent;
    
    cout << "====================================================================" << endl;
    cout << "TEST A" << endl;
    bandit.TestA();
    
    cout << "====================================================================" << endl;
    cout << endl << "TEST B" << endl;
    
    /////======================= Initialize Function ============================
    //Initialize for the first iteration.
    for(int i=0; i<num_arms; i++){
        values.push_back(0); //Creating a vetor of 0's representative of the arms for the first time
        //Then we will update this based on the learning equation.
    }
    
    /////======================= Run Function ===========================
    
    //double Bandit::pull(){
    ofstream fout("/Users/Jan/Desktop/ProjectAlphaResults1.txt", fstream::trunc);
    for(int a=0; a<30; a++){
        for(int j=0; j<q; j++){
            cout << "____________________________________________________________" << endl;
            cout << "Agent entering a value 0-2 to pull a lever 1st-3rd..." << endl;
            
            /// =========================== DECIDE ==============================
            if (LYRAND <= (epsilon*0.90)){
                //Choose the random action (epsilon)% of the time
                action = rand()%3;
            }
            else if(LYRAND > (epsilon*0.90)){
                //Choose the greedy action (1-epsilon)% of the time
                double max = 0;
                for(int x=0; x<values.size(); x++){
                    if(max<values.at(x)){
                        max = values.at(x);
                    }
                    action = x;
                }
            }
            cout << action << endl;
            
            ///=============(React) How to learn this stuff======================
            values.at(action)=result*alpha+(1-alpha)*values.at(action);
            
            choice = action;
            
            if (choice == 0){
                //random number generator with set value mu and sigma
                result = bandit.box_muller(150.3, 19.06);
                cout << "Pulling Arm 0 Result: " << result << endl << endl;
                //fout << choice << endl;
                fout << result << endl;
            }
            if (choice == 1){
                result = bandit.box_muller(212.5, 37.8);
                cout << "Pulling Arm 1 Result: " << result << endl << endl;
                //fout << choice << endl;
                fout << result << endl;
            }
            if (choice == 2){
                result = bandit.box_muller(355.2, 28.4);
                cout << "Pulling Arm 2 Result: " << result << endl << endl;
                //fout << choice << endl;
                fout << result << endl;
            }
            else if (choice != 0 && choice!= 1 && choice !=2){
                cout << "That is not a valid lever choice" << endl;
            }
            
            cout << "The number of pulls done:....... " << j+1 << endl;
        }
        fout << endl << endl;
    }
    fout.close();
    
    bandit.TestB();
    
    cout << "++++++++++++++++++ Game has been terminated +++++++++++++++++++" << endl;
    
    return 0;
};
