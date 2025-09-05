#include "BankersAlgorithm.h"


int main(){

    //NOTES:
    //Data can be read from the terminal (user inputs all required information) or from a file
    //File format:
        //number of processes, process names (separated by commas)
        //number of resources, resource names (separated by commas)
        //number of instances per resource (separated by commas)
        //allocation matrix (each row represents a process and numbers within the row are separated by spaces)
        //availability vector (numbers separated by commas)
        //maximum demand matrix (each row represents a process and numbers within the row are separated by spaces)
        //requests (process name, resources; one request per line)

    int choice = 0;

    while (choice != 1 && choice != 2) {
        std::cout << "\nSelect the data input method:" << std::endl;
        std::cout << "   [1] From file" << std::endl;
        std::cout << "   [2] From terminal" << std::endl;
        std::cin >> choice;
        std::cin.ignore();

        if (choice != 1 && choice != 2) {
            std::cout << "Invalid option! Please try again.\n" << std::endl;
        }
    }

    if(choice == 1) {
        BankersAlgorithm b;
        std::string filename;
        std::cout << "Enter the file name: ";
        std::cin >> filename;

        std::ifstream file1(filename); 
        if(file1.is_open()) {
            b = read_from_file(file1); 
            file1.close(); 
        } else {
            std::cerr << "Error: file \"" << filename << "\" not found!" << std::endl;
            return 1;
        }

        b.calculate_need();
        b.printNeedMatrix();
        std::pair<bool, std::vector<std::string>> isStableResult = b.isStable();
        b.printProcessSequence(isStableResult);
        b.isAccepted();
    }

    else if(choice == 2) {
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
    
        std::cout<<"Enter the number of processes "<<std::endl;
        std::cin>>processes_number;
        std::cin.ignore();
        std::cout<<"Enter process names (in the same line separated by spaces): "<<std::endl;
        std::string p_names_string;
        getline(std::cin,p_names_string);
        std::stringstream ss(p_names_string);
        std::string token;
        while (getline(ss, token, ' ')) {
            processes_names.push_back(token);
        }
        std::cout<<"Enter the number of resources "<<std::endl;
        std::cin>>resources_number;
        std::cin.ignore();
        std::cout<<"Enter resource names (in the same line separated by spaces)"<<std::endl;
        std::string r_names_string;
        getline(std::cin,r_names_string);
        std::stringstream ss2(r_names_string);
        std::string token2;
        while (getline(ss2, token2, ' ')) {
            resources_names.push_back(token2);
        }
        std::cout<<"Enter the number of instances per resource (in the same line separated by spaces)"<<std::endl;
        std::string instances_num_string;
        getline(std::cin,instances_num_string);
        std::stringstream ss3(instances_num_string);
        std::string token3;
        while (getline(ss3, token3, ' ')) {
            resources_instances.push_back(std::stoi(token3));
        }
        std::cout<<"Enter the availability vector (in the same line separated by spaces)"<<std::endl;
        std::string available_string;
        getline(std::cin,available_string);
        std::stringstream ss4(available_string);
        std::string token4;
        while (getline(ss4, token4, ' ')) {
            available.push_back(std::stoi(token4));
        }
        std::cout<<"Enter the allocation matrix (numbers separated by spaces; press Enter to move to the next row)"<<std::endl;
        for(int i = 0; i < processes_number; ++i) {
            std::vector<int> row;
            std::string line;
            getline(std::cin, line);
            std::stringstream ss(line);
            for(int j = 0; j < resources_number; ++j) {
                std::string token;
                getline(ss,token,' ');
                row.push_back(std::stoi(token));
            }
            allocation.push_back(row);
        }
        std::cout<<"Enter the maximum demand matrix (numbers separated by spaces; press Enter to move to the next row)"<<std::endl;
        for(int i = 0; i < processes_number; ++i) {
            std::vector<int> row;
            std::string line;
            getline(std::cin, line);
            std::stringstream ss(line);
            for(int j = 0; j < resources_number; ++j) {
                std::string token;
                getline(ss,token,' ');
                row.push_back(std::stoi(token));
            }
            max.push_back(row);
        }
        std::cout<<"Enter a request (format: ProcessName Resource1 Resource2 Resource3 ...)"<<std::endl;
        std::string requests_string;
        getline(std::cin,requests_string);
        std::stringstream ss5(requests_string);
        std::string token5;
        Request r;
        getline(ss5,token5, ' ');
        r.name=token5;
        while(getline(ss5,token5,' ')) {
            r.resources_request.push_back(std::stoi(token5));
        }
        requests_list.push_back(r);
        std::string select;
        do {
            std::cout << std::endl;
            std::cout<<"Do you want to add another request?" <<std::endl;
            std::cout<< "   [1] Yes         [2] No     " <<std::endl;
            std::cin>>select;
            std::cin.ignore();

            if(select == "1"){
                std::cout<<"Enter a request "<<std::endl;
                std::string requests_string2;
                getline(std::cin,requests_string2);
                std::stringstream ss6(requests_string2);
                std::string token6;
                Request r2;
                getline(ss6, token6, ' ');
                r2.name=token6;
                while(getline(ss6,token6,' ')) {
                    r2.resources_request.push_back(std::stoi(token6));
                }
                requests_list.push_back(r2);
            }
        } while(select != "2");


        BankersAlgorithm bank_algorithm(processes_number,processes_names,resources_number,resources_instances,available,allocation,max,requests_list);
        bank_algorithm.calculate_need();
        bank_algorithm.printNeedMatrix();
        std::pair<bool, std::vector<std::string>> is_stable_result=bank_algorithm.isStable();
        bank_algorithm.printProcessSequence(is_stable_result);
        bank_algorithm.isAccepted();
    }

    return 0;

}