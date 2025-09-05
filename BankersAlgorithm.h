#pragma once
#include "Request.h"
#include <fstream>
#include <sstream>
#include <algorithm>

class BankersAlgorithm{
    public:
        int processes_number;
        std::vector<std::string> processes_names;
        int resources_number;
        std::vector<std::string> resources_names;
        std::vector<int> resources_instances;
        std::vector<int> available;
        std::vector<std::vector<int>> allocation;
        std::vector<std::vector<int>> max;
        std::vector<std::vector<int>> need;
        std::vector<Request> requests_list;

    public:
        BankersAlgorithm() {}
        BankersAlgorithm(int processes_num,std::vector<std::string>& processes_names,int resources_num,std::vector<int>& resources_instances,std::vector<int>& available,std::vector<std::vector<int>>& allocation,std::vector<std::vector<int>>& max,std::vector<Request>& requests):processes_number(processes_num),processes_names(processes_names),resources_number(resources_num),resources_instances(resources_instances),available(available),max(max),allocation(allocation),requests_list(requests){}


        void calculate_need(){
            int row = processes_number;
            int col = resources_number;
            need.resize(row, std::vector<int>(col, 0));
            for (size_t i = 0; i < row; ++i) {
                for (size_t j = 0; j < col; ++j) {
                    need[i][j] = max[i][j] - allocation[i][j];
                }
            }
        }

        void printMatrix(const std::vector<std::vector<int>>& matrix)const{
            int row = matrix.size();
            int col = matrix[0].size(); 
            for (size_t i = 0; i < row; ++i) {
                for (size_t j = 0; j < col; ++j) {
                    std::cout << matrix[i][j] << " "; 
                }
                std::cout << std::endl;
            }
        }

        void printNeedMatrix()const{
            std::cout<<"Need matrix:"<<std::endl;
            printMatrix(this->need); 
        }

        //-------------------------------------------------------------
        //Helper functions used for addition, subtraction, and comparison of elements of two vectors 
        std::vector<int> addVectors(const std::vector<int>& x, const std::vector<int>& y){
            std::vector<int> result(x.size());
            for(size_t i = 0; i < x.size(); ++i){
                result[i] = x[i] + y[i];
            }
            return result;
        }

        std::vector<int> subtractVectors(const std::vector<int>& x, const std::vector<int>& y){
            std::vector<int> result(x.size());
            for(size_t i = 0; i < x.size(); ++i){
                result[i] = x[i] - y[i];
            }
            return result;
        }

        bool isSmaller(const std::vector<int>& x, const std::vector<int>& y) {
            if (x.size() != y.size()) {
                return false;
            }
            for (int i = 0; i < x.size(); ++i) {
                if (x[i] > y[i]) {
                    return false;
                }
            }
            return true;
        }
        //-------------------------------------------------------------

        int findElementPosition(const std::vector<std::string>& x, std::string str) {
            for (size_t i = 0; i < x.size(); ++i) {
                if (x[i] == str) {
                    return i; 
                }
            }
            return -1; 
        }



        std::pair<bool, std::vector<std::string>> isStable(){
            std::pair<bool, std::vector<std::string>> result;
            std::vector<int> end(processes_number, 0);
            std::vector<int> current_available=available;
            int row = processes_number;
            while(!std::all_of(end.begin(), end.end(), [](int elem) { return elem == 1; })){
                bool foundValidProcess = false;
                for (size_t i = 0; i < row; ++i) {
                    if(isSmaller(need[i],current_available) && end[i]!=1){
                        current_available=addVectors(subtractVectors(current_available,need[i]),max[i]);
                        end[i]=1;
                        result.second.push_back(processes_names[i]);
                        foundValidProcess = true;
                    }
                }
                if (!foundValidProcess) {
                    result.first=false;
                    return result;
                }
            }
            result.first=true;
            return result;
        }
        
        void printProcessSequence(std::pair<bool, std::vector<std::string>> isStableResult){
            if(isStableResult.first==false){
                std::cout<<"System is not in a stable state."<<std::endl;
            }
            else if(isStableResult.first==true){
                std::cout<<"System is in a stable state (";
                bool first=true;
                for(const auto& str : isStableResult.second){
                    if(!first){
                        std::cout<<"->";
                    }
                    std::cout<<str;
                    first=false;
                }
                std::cout<<")."<<std::endl;
            }
        }

        void isAccepted(){
            for(size_t i=0;i<(this->requests_list).size();++i){
                if(isSmaller(requests_list[i].resources_request,this->available)){
                    BankersAlgorithm b=*this;
                    int position=findElementPosition(b.processes_names,requests_list[i].name);
                    b.allocation[position]=addVectors(requests_list[i].resources_request,b.allocation[position]);
                    b.available=subtractVectors(b.available,requests_list[i].resources_request);
                    b.calculate_need();
                    std::pair<bool, std::vector<std::string>> isStableResult;
                    isStableResult=b.isStable();
                    if(isStableResult.first==true){
                        std::cout<<"Request "<<requests_list[i].name<<" will be approved"<<std::endl;
                    }
                    else if(isStableResult.first==false){
                        std::cout<<"Request "<<requests_list[i].name<<" will not be approved"<<std::endl;
                    }
                }
                else std::cout<<"Request "<<requests_list[i].name<<" will not be approved"<<std::endl;
            }
        }

};

BankersAlgorithm read_from_file(std::istream& in){
    std::string line;
    getline(in,line);
    std::stringstream ss(line);
    std::string token;
    int resources_number,processes_number;
    std::vector<std::string> processes_names;
    std::vector<std::string> resources_names;
    std::vector<int> resources_instances;
    std::vector<int> available;
    std::vector<std::vector<int>> allocation;
    std::vector<std::vector<int>> max;
    std::vector<std::vector<int>> need;
    std::vector<Request> requests_list;
    getline(ss, token, ',');
    processes_number = std::stoi(token);
    while (getline(ss, token, ',')){
        processes_names.push_back(token);
    }
    getline(in,line);
    std::stringstream ss1(line);
    getline(ss1, token, ',');
    resources_number = std::stoi(token);
    while (getline(ss1, token, ',')){
        resources_names.push_back(token);
    }
    getline(in,line);
    std::stringstream ss2(line);
    while (getline(ss2, token, ',')){
        resources_instances.push_back(std::stoi(token));
    }
    for (size_t i = 0; i < processes_number; ++i) {
        std::vector<int> row;
        if (getline(in, line)) {
            std::stringstream ss(line);
            for (size_t j = 0; j < resources_number; ++j) {
                std::string token;
                getline(ss,token,' ');
                row.push_back(std::stoi(token));
            }
            allocation.push_back(row);
        }
    }
    getline(in,line);
    std::stringstream ss3(line);
    while (getline(ss3, token, ',')){
        available.push_back(std::stoi(token));
    }
    for (size_t i = 0; i < processes_number; ++i) {
        std::vector<int> row;
        if (getline(in, line)) {
            std::stringstream ss(line);
            for (size_t j = 0; j < resources_number; ++j) {
                std::string token;
                getline(ss,token,' ');
                row.push_back(std::stoi(token));
            }
            max.push_back(row);
        }
    }
    while(getline(in,line)){
        Request r;
        std::stringstream ss(line);
        getline(ss, token, ',');
        r.name = token;
        while (getline(ss, token, ',')){
            r.resources_request.push_back(std::stoi(token));
        }
        requests_list.push_back(r);
    }
    BankersAlgorithm ba(processes_number,processes_names,resources_number,resources_instances,available,allocation,max,requests_list);
    return ba;
}

