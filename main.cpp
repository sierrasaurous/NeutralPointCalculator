//
//  main.cpp
//  ZeroMomentCalculator
//
//  Created by Sierra on 7/12/16.
//  Copyright (c) 2016 Sierra Adibi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

class Point{        // Individual Neutral Point Location
public:
    double x, z;
};

class Case{         // Individual Case Data
public:
    double alpha, Fx, Fz, My;
    int caseNumber;
    vector<Point> NeutralPoints;
    
    void getCaseData(int c);
    void getNeutralPoints(double L);
};


void Case::getCaseData(int count){
    
    // Get data from RotCFD from user
    
    caseNumber = count+1;
    
    cout << "\nCase " << caseNumber << " pitch angle:\t";
    cin >> alpha;
    cout << "Case " << caseNumber << " Fx:\t";
    cin >> Fx;
    cout << "Case " << caseNumber << " Fz:\t";
    cin >> Fz;
    cout << "Case " << caseNumber << " My:\t";
    cin >> My;
    
}

void Case::getNeutralPoints(double L){
    double x,z;             //x and z locations on fuselage
    double dx = 1;          //resolution of fuselage chord for calculations
    double xmax = 0.75*L;   //only perform calculations along middle 50% of fuselage
    
    
    //calculate z locations, based on different fuselage chord locations
    x=0.25*L;
    double AOA = alpha*atan(1)/45; //convert pitching angle to radians for calculations
    double cosAlpha = cos(AOA);
    
    while(x <= xmax){
        Point p;
        
        z = (-My + Fz*x*cosAlpha)/(Fx*cosAlpha);
        
        p.x = x;
        p.z = z;
        
        NeutralPoints.push_back(p);
        x = x+dx;
    }
}

void writeData(vector<Case> data, ofstream & file, int c){
    
    file << "Case: \t Pitch: \t Fx: \t Fz: \t My: \t X-Location: \t Z-Location: \n";
    //file << "Case: \t Pitch: \t X-Location: \t Z-Location: \n";
    file << "-----------------------------------------------------------\n";
    
    cout << "\n\nCase: \t Pitch: \t Fx: \t Fz: \t My: \t X-Location: \t Z-Location: \n";
    //cout << "\n\nCase: \t Pitch: \t X-Location: \t Z-Location: \n";
    cout << "-----------------------------------------------------------\n";
    
    for(int i = 0; i < c; i++){
        
        for(int j=0; j< data.at(i).NeutralPoints.size();j++){
            
            // Data set without Force and Moments
            //file << data.at(i).caseNumber << "\t\t\t" << data.at(i).alpha << "\t\t\t" << data.at(i).NeutralPoints.at(j).x << "\t\t\t" << data.at(i).NeutralPoints.at(j).z << endl;
            //cout << data.at(i).caseNumber << "\t\t\t" << data.at(i).alpha << "\t\t\t" << data.at(i).NeutralPoints.at(j).x << "\t\t\t" << data.at(i).NeutralPoints.at(j).z << endl;
            
            
            // Data set with Force and Moments
            file << data.at(i).caseNumber << "\t\t\t" << data.at(i).alpha << "\t\t\t" << data.at(i).Fx << "\t\t\t" << data.at(i).Fz << "\t\t\t" << data.at(i).My << "\t\t\t" << data.at(i).NeutralPoints.at(j).x << "\t\t\t" << data.at(i).NeutralPoints.at(j).z << endl;
            cout << data.at(i).caseNumber << "\t\t\t" << data.at(i).alpha << "\t\t\t" << data.at(i).Fx << "\t\t\t" << data.at(i).Fz << "\t\t\t" << data.at(i).My << "\t\t\t" << data.at(i).NeutralPoints.at(j).x << "\t\t\t" << data.at(i).NeutralPoints.at(j).z << endl;
            
        }
    }
    
}



int main() {
    
    int CaseCount;
    double FuselageLength;
    vector<Case> data;
    ofstream myfile;
    
    // Get the number of cases to compare and length of fuselage from user
    cout << "How long is the craft fuselage to be examined?\t";
    cin >> FuselageLength;
    cout << "How many cases would you like to compare?\t";
    cin >> CaseCount;
    
    data.reserve(CaseCount);
    
    
    // For each case, get Force and Moment Data, and calculate neutral points
    for(int i=0;i<CaseCount;i++){
        Case c;
        c.getCaseData(i);
        c.getNeutralPoints(FuselageLength);
        data.push_back(c);
    }
    
    // Open Output File, Write Data, and Close Output File
    myfile.open("NeutralPointData.txt");
    writeData(data, myfile, CaseCount);
    myfile.close();
    
    return 0;
}
