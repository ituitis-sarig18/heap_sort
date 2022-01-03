/*
Gökay Sarı
150180727
Analysis of Algorithm HW2
*/
#include<iostream>
#include<fstream> // file operations
#include<string>
#include<vector> // for vector
#include<sstream>
#include<cstdlib>
#include<chrono>



using std::vector;
//#include <ctime> //random variables
using namespace std;

//using the datas from vehicle file
class Vehicles{
public:
    string vehicleID;
    string location;
    double distance;
    int speed;
    double time;
};

//using the datas from request file
class Requests{
public:
    string location;
    double distance;
    int luckyNumber;
};

void readFromFile(vector<Vehicles> &vehicleVector)
{
    Vehicles temp;   // object temp and assign read value to temp. and then add the properties of the objects to vector.
    ifstream file;                                    

    file.open("vehicles.txt" , ios::in);

    if (!file.is_open())
    {
        cerr << "File cannot be opened!";
        exit(1);
    }

    else 
    {
        //read the datas from vehicle file
        string header, speed, vehicleID, distance, location, dummy;
        double time;    //obtain time value from given formulae in hw

        getline(file, header); //this is the header line
        int i = 0;
        while (file.good() && i < 1642) //row num = 1642 of given file
        {
            getline(file, vehicleID, '\t'); //read the id
            getline(file, location, '\t'); // read the location
            file >> distance; // read the distance
            file >> speed; // read the speed
            getline(file, dummy, '\n'); //this is for reading the \n character into dummy variable.

            Vehicles temp;      
            temp.vehicleID = vehicleID;
            temp.location = location;
            temp.distance = stod(distance); //string to double
            temp.speed = stoi(speed);       //string to int

            time = temp.distance / temp.speed;    //we get the time value with distance/speed
            temp.time = time;                     

            vehicleVector.push_back(temp); // adding to the vector
            i++;
            
        }
    }
    file.close();
}

//read the datas from requests.txt 
void readFromFileRequests(vector<Requests> &requestvector)
{
    ifstream RequestFile;
    RequestFile.open("requests.txt" , ios::in);
    
    if (!RequestFile.is_open())
    {
        cerr << "File cannot be opened!";
        exit(1);
    }
    
    string header;
    getline(RequestFile, header);   //header line
    int i = 0;
    while (!RequestFile.eof() && i < 43877) //row num is 43877 of given request file
    {
    string distance, location, luckyNumber;
    getline(RequestFile, location, '\t');   //read the location
    getline(RequestFile, distance, '\t');   //read the distance
    getline(RequestFile, luckyNumber);      //read the lucky num

    Requests temp;
    temp.location = location;
    temp.distance = stod(distance);         //string to double
    temp.luckyNumber = stoi(luckyNumber);   //string to int

    requestvector.push_back(temp);          //adding to the vector
    i++;
    }
}

void saveFile(vector<string> &output,int num)
{
    //open file to save datas
    ofstream CallHistoryFile;
    string outputFileName = "call_history_" + to_string(num) + ".txt";  //file name is call_history_N.txt

    CallHistoryFile.open(outputFileName , ios::out);                    
    for(int i = 0; i < output.size(); i++)
    {
        CallHistoryFile<<output[i]<<endl;
    }
    
}

int leftChild(int i)    //finding left child
{return (2*i+1);}   
                    
int rightChild(int i)      //finding right child
{return (2*i+2);}

int Parent(int i)       //finding parent
{return (i-1)/2;}

//heapify up for adding a new vehicle to the heap
//compare the value of its parent 
//if smaller, change occurs
void heapifyUp(vector<Vehicles> &vehicleVectorHf, int i)
{
    while(vehicleVectorHf[i].time < vehicleVectorHf[Parent(i)].time && i != 0)
    {
        //swap function 
        swap(vehicleVectorHf[i],vehicleVectorHf[Parent(i)]);
        i = Parent(i);
    }
}

//used for deleting a vehicle in the heap 
//compare the value of its children 
//and swap with smallest value 
void heapifyDown(vector<Vehicles> &vehicleVectorHf, int i)
{
    int leftC = leftChild(i);
    int rightC = rightChild(i);
    int smallest = i;

    //find smallest
    //compare the vector[i] and right child
    if(rightC < vehicleVectorHf.size() && vehicleVectorHf[rightC].time < vehicleVectorHf[smallest].time)
    {
        smallest = rightC;
    }
    //compare the vector[i] and left child
    if(leftC < vehicleVectorHf.size() && vehicleVectorHf[leftC].time < vehicleVectorHf[smallest].time)
    {
        smallest = leftC;
    }

    // swap smallest child and apply heapifydown on the child
    if(smallest != i)
    {
        swap(vehicleVectorHf[i],vehicleVectorHf[smallest]);
        heapifyDown(vehicleVectorHf,smallest);
    }
}

//building the heap 
void heapBuild(vector<Vehicles> &vehicleVectorHf)
{
    int i = vehicleVectorHf.size() / 2;
    for(; i >= 0; i--)
    {
        heapifyDown(vehicleVectorHf, i);
    }
}

//decrease key operation for heap 
//we use heapifyUp function 
void decreaseKey(vector<Vehicles> &vehicleVectorHf, int luckyNumber)
{
    vehicleVectorHf[luckyNumber].time = 0;
    heapifyUp(vehicleVectorHf, luckyNumber);
}

//insert operation for heap to add a new item
//push the temp to vector
//we use the heapifyUp function
void insertHeap(vector<Vehicles> &vehicleVectorHf, Vehicles temp)
{
    vehicleVectorHf.push_back(temp);
    heapifyUp(vehicleVectorHf, vehicleVectorHf.size() - 1);
}

//extract operation for heap
//we use heapifyDown func
Vehicles extractMin(vector<Vehicles> &vehicleVectorHf)
{
    Vehicles temp = vehicleVectorHf[0];
    vehicleVectorHf[0] = vehicleVectorHf[vehicleVectorHf.size() - 1];
    vehicleVectorHf.pop_back();
    heapifyDown(vehicleVectorHf, 0);
    return temp;
}

int main(/* int argc, char* argv[] */)
{
    //int num = atoi(argv[1]);
    int num = 10000;       //num is the N
    int counter = 0;        //counter 
    vector<Vehicles> vehiclesVectorTemp;
    readFromFile(vehiclesVectorTemp);   //vehicles file read func.
    heapBuild(vehiclesVectorTemp);      //build the heap
    vector<Requests> requestVector;     
    readFromFileRequests(requestVector);//request file read func
    vector<string> outputs;
    int i = 0;
    chrono::steady_clock::time_point _start(std::chrono::steady_clock::now()); // clock starts
    //apply the operations as much as the N numbers
    while(counter <= num)
    {
        Requests currentRequest = requestVector[i++];
        Vehicles currentVehicle;
        if(currentRequest.luckyNumber != 0) //comparison by lucky num
        {
            decreaseKey(vehiclesVectorTemp, currentRequest.luckyNumber);
            counter++;
        }
        if(counter == num)
        {
            break;
        }
        currentVehicle = extractMin(vehiclesVectorTemp);
        counter++;
        currentVehicle.location = currentRequest.location;
        currentVehicle.distance = currentRequest.distance;
        currentVehicle.time = currentVehicle.distance / currentVehicle.speed;
        if(counter== num)
        {
            break;
        }
        insertHeap(vehiclesVectorTemp, currentVehicle);
        counter++;
        outputs.push_back(currentVehicle.vehicleID);
    }
    chrono::steady_clock::time_point _end(std::chrono::steady_clock::now()); // clock ends
    cout << "Elapsed time of execution: "<<std::chrono::duration_cast<std::chrono::duration<double>>(_end - _start).count()<<"\n";
    cout<<"Number of N: "<<counter << endl;
    saveFile(outputs, num);
    return 0;
}