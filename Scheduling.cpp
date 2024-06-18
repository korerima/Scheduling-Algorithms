#include <bits/stdc++.h>

using namespace std;

const int QUANTUM_TIME = 3;
enum SCHEDULS{FIRST_COME_FIRST_SERVE = 1, SHORTEST_JOB_FIRST_NON_PREEMPTIVE, SHORTEST_JOB_FIRST_PREEMPTIVE, ROUND_ROBIN, PRIORITY_NON_PREEMPTIVE};

void getProcess(queue<array<int, 4>> &);
void getProcess(queue<array<int, 3>> &);
void getProcess(set<array<int, 3>> &);
int getSchedule();
void firstComeFirstServer();
void shortestJobFirst(bool);
void roundRobin();
void priorityScheduling(bool);

int main(){
    char quit;
    do{
        int choice = getSchedule();

        switch(choice){
        case FIRST_COME_FIRST_SERVE:
            firstComeFirstServer();
            break;
        case SHORTEST_JOB_FIRST_NON_PREEMPTIVE:
            shortestJobFirst(0);
            break;
        case SHORTEST_JOB_FIRST_PREEMPTIVE:
            shortestJobFirst(true);
            break;
        case ROUND_ROBIN:
            roundRobin();
            break;
        case PRIORITY_NON_PREEMPTIVE:
            priorityScheduling(0);
            break;
        default:
            priorityScheduling(1);
            ;
        }
        cout << "Enter Q or q to quit : ";
        cin >> quit;
        if(quit != 'q' && quit != 'Q')
            system("cls");

    }while(quit != 'q' && quit != 'Q');
}
void display(vector<vector<int>> processTable){

    system("cls");
    cout << "\t\t\t\tSCHEDULING TABLE HERE" << endl;
    cout << setfill('-') << setw(91) << "" << setfill(' ') << endl;
    cout << "| Process | Arrival Time | Brust Time | Waiting Time | Completion Time | Turn Around Time |\n";
    cout << setfill('-') << setw(91) << "" << setfill(' ') << endl;
    for(int i = 0; i < processTable.size(); i++){
        cout << '|' << setw(9) << processTable[i][0] << '|' << setw(14) << processTable[i][1] << '|' << setw(12) << processTable[i][2]
             << '|' << setw(14) << processTable[i][3] <<'|' << setw(17) << processTable[i][4] << '|' << setw(18)
             << processTable[i][3] + processTable[i][2] << '|' << endl;
        cout << setfill('-') << setw(91) << "" << setfill(' ') << endl;
    }
}
void firstComeFirstServer(){
    queue<array<int, 3>> jobQueue;
    getProcess(jobQueue);
    int time = 0, n = jobQueue.size();
    vector<vector<int>> processTable(n, vector<int>(5, 0));
    while(!jobQueue.empty()){
        if(time >= jobQueue.front()[0]){
            array<int, 3> process = jobQueue.front();
            jobQueue.pop();
            processTable[process[2] - 1] = {process[2], process[0], process[1], time - process[0], time + process[1]};
            time += process[1];
            continue;
        }
        time++;
    }
    display(processTable);
}
void shortestJobFirst(bool isPreemptive){
    queue<array<int, 3>> jobQueue;
    set<array<int, 3>> readyQueue;
    getProcess(jobQueue);
    int time = 0, n = jobQueue.size();
    vector<vector<int>> processTable(n, vector<int>(5, 0));

    while(!jobQueue.empty()){
        bool hasExcuted = 0;
        while(!jobQueue.empty() && time == jobQueue.front()[0]){
            array<int, 3> nextProcess = {jobQueue.front()[1], jobQueue.front()[0], jobQueue.front()[2]};
            readyQueue.insert(nextProcess);
            jobQueue.pop();
        }
        while(!readyQueue.empty()){
            array<int, 3> process = *readyQueue.begin();
            readyQueue.erase(readyQueue.begin());
            int idx = process[2] - 1;
            if(!processTable[idx][0]){
                processTable[idx][0] = idx + 1;
                processTable[idx][1] = process[1];
                processTable[idx][2] = process[0];
            }
            processTable[idx][3] += time - process[1];
            while(process[0] != 0){
                if(!jobQueue.empty() && time == jobQueue.front()[0]){
                    while(!jobQueue.empty() && time == jobQueue.front()[0]){
                        array<int, 3> nextProcess = {jobQueue.front()[1], jobQueue.front()[0], jobQueue.front()[2]};
                        readyQueue.insert(nextProcess);
                        jobQueue.pop();
                    }
                    array<int, 3> nextProcess = *readyQueue.begin();
                    if(isPreemptive && nextProcess[0] < process[0]){
                        process[1] = time;
                        readyQueue.insert(process);
                        break;
                    }
                }
                process[0]--;
                time++;
            }
            processTable[idx][4] = time;
            hasExcuted = 1;
        }
        if(!hasExcuted)
            time++;
    }
    display(processTable);
}
void roundRobin(){
    set<array<int, 3>> jobQueue;
    getProcess(jobQueue);

    int time = 0, n = jobQueue.size();
    vector<vector<int>> processTable(n, vector<int>(5, 0));
    while(!jobQueue.empty()){
        array<int, 3> process = *jobQueue.begin();
        if(time >= process[0]){
            jobQueue.erase(jobQueue.begin());
            int idx = process[2] - 1;
            if(processTable[idx][0] == 0){
                processTable[idx][0] = idx + 1;
                processTable[idx][1] = process[0];
                processTable[idx][2] = process[1];
            }
            // waiting time
            processTable[idx][3] += time - process[0];
            // completion time
            time += (process[1] >= QUANTUM_TIME ? QUANTUM_TIME : process[1]);
            processTable[idx][4] = time;

            process[1] -=  QUANTUM_TIME ;
            process[0] = time;
            if(process[1] > 0)
                jobQueue.insert(process);
            continue;
        }
        time++;
    }
    display(processTable);
}
void priorityScheduling(bool isPreemptive){
    queue<array<int, 4>> jobQueue;
    set<array<int, 4>> readyQueue;
    getProcess(jobQueue);
    int time = 0, n = jobQueue.size();
    vector<vector<int>> processTable(n, vector<int>(5, 0));
    while(!jobQueue.empty()){
        bool hasExcuted = 0;
        while(!jobQueue.empty() && time == jobQueue.front()[0]){
            array<int, 4> nextProcess = {jobQueue.front()[2], jobQueue.front()[0], jobQueue.front()[1], jobQueue.front()[3]};
            readyQueue.insert(nextProcess);
            jobQueue.pop();
        }
        array<int, 4> process = *readyQueue.begin();
        while(!readyQueue.empty()){
            array<int, 4> process = *readyQueue.begin();
            readyQueue.erase(readyQueue.begin());
            int idx = process[3] - 1;
            if(!processTable[idx][0]){
                processTable[idx][0] = idx + 1;
                processTable[idx][1] = process[1];
                processTable[idx][2] = process[2];
            }
            processTable[idx][3] += time - process[1];
            while(process[2] != 0){
                if(!jobQueue.empty() && time == jobQueue.front()[0]){
                    while(!jobQueue.empty() && time == jobQueue.front()[0]){
                        array<int, 4> nextProcess = {jobQueue.front()[2], jobQueue.front()[0], jobQueue.front()[1], jobQueue.front()[3]};
                        readyQueue.insert(nextProcess);
                        jobQueue.pop();
                    }
                    array<int, 4> nextProcess = *readyQueue.begin();
                    if(isPreemptive && nextProcess[0] < process[0]){
                        process[1] = time;
                        readyQueue.insert(process);
                        break;
                    }
                }
                process[2]--;
                time++;
            }
            processTable[idx][4] = time;
            hasExcuted = 1;
        }
        if(!hasExcuted)
            time++;
    }
    display(processTable);
}
int getNumberofProcess(){
    int numberOfProcess;
    do{
        cout << "Enter number of process : ";
        cin >> numberOfProcess;
        if(numberOfProcess < 1)
            cout << "Invalid process count!!!\n";
    }while(numberOfProcess < 1);
    return numberOfProcess;
}
void getProcess(set<array<int, 3>> & jobQueue){
    vector<array<int, 3>> tempQueue;
    int numberOfProcess = getNumberofProcess();
    for(int i = 0; i < numberOfProcess; i++){
        cout << "Enter the process Arrival and brust time respectively of process " << i + 1 << " : ";
        array<int, 3> input;
        cin >> input[0] >> input[1];
        input[2] = i + 1;
        tempQueue.push_back(input);
    }
    for(int i = 0; i < numberOfProcess; i++)
        jobQueue.insert(tempQueue[i]);
}
void getProcess(queue<array<int, 3>>& jobQueue){
    vector<array<int, 3>> tempQueue;
    int numberOfProcess = getNumberofProcess();
    for(int i = 0; i < numberOfProcess; i++){
        cout << "Enter the process Arrival and brust time respectively of process " << i + 1 << " : ";
        array<int, 3> input;
        cin >> input[0] >> input[1];
        input[2] = i + 1;
        tempQueue.push_back(input);
    }
    sort(tempQueue.begin(), tempQueue.end());
    for(int i = 0; i < numberOfProcess; i++)
        jobQueue.push(tempQueue[i]);
}
void getProcess(queue<array<int, 4>>& jobQueue){
    vector<array<int, 4>> tempQueue;
    int numberOfProcess = getNumberofProcess();
    for(int i = 0; i < numberOfProcess; i++){
        cout << "Enter the process Arrival, brust time and priority respectively of process " << i + 1 << " : ";
        array<int, 4> input;
        cin >> input[0] >> input[1] >> input[2];
        input[3] = i + 1;
        tempQueue.push_back(input);
    }
    sort(tempQueue.begin(), tempQueue.end());
    for(int i = 0; i < numberOfProcess; i++)
        jobQueue.push(tempQueue[i]);
}
int getSchedule(){
    int choice;
    do{
        cout <<"\tChoose Scheduling Algorithm \n"
             << "1. First come Fist Serve \n"
             << "2. Shortest Job first non-preemptive\n"
             << "3. Shortest Job first preemptive\n"
             << "4. Round Robin\n"
             << "5. Priority Scheduling non-preemptive\n"
             << "6. Priority Scheduling preemptive\n"
             << "Enter choice : ";
        cin >> choice;
        if(choice > 6 || choice < 1){
            cout << "Invalid choice!!!\n";
            system("pause");
            system("cls");
        }
    }while(choice > 6 || choice < 1);

    return choice;
}
