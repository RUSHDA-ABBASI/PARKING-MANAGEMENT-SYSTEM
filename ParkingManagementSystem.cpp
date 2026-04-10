#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Vehicle{
public:
    virtual void display() = 0;
    virtual string getType() = 0;
    
};

class Car : public Vehicle{
public:
    void display(){
        cout<<"Car entered the parking.\n";
    }
    string getType(){ 
        return "car"; 
    }
};

class Bike : public Vehicle{
public:
    void display(){
        cout<<"Bike entered the parking.\n";
    }
    string getType(){
        return "bike"; 
    }
};

class VehicleFactory{  //factory method design pattern
public:
    static Vehicle* createVehicle(string type){
        if(type == "car" || type == "Car" || type == "CAR")
            return new Car();
        else if(type == "bike" || type == "Bike" || type == "BIKE")
            return new Bike();
        else
            return nullptr;
    }
};

class ParkingSlot {
public:
    int slotID;
    bool isOccupied;
    Vehicle* vehicle;  //pointer pointing to vehicle class

    ParkingSlot(int id){
        slotID = id;
        isOccupied = false;
        vehicle = nullptr;
    }

    bool isAvailable(){
        return isOccupied == false;   //returns true if not occupied
    }

    void parkVehicle(Vehicle* v){
        vehicle = v;  
        isOccupied = true;
    }

    void removeVehicle(){
        vehicle = nullptr;
        isOccupied = false;
    }
};

class ParkingTicket {
public:
    int ticketID;
    int slotID;
    Vehicle* vehicle;

    ParkingTicket(int id, Vehicle* v, int slot){
        ticketID = id;
        vehicle = v;
        slotID = slot;
    }

    void printTicket(){
        cout << "Ticket ID: " << ticketID << ", Slot: " << slotID << endl;
    }

    void calculateFee(int hours){
        double fee;
        if(vehicle->getType() == "car")
            fee = hours * 20;
        else
            fee = hours * 10;

        cout << "Parking Fee: Rs " << fee <<"/-"<< endl;
    }
};

class ParkingManager{  //singleton design pattern
private:
    static ParkingManager* object;

    vector<ParkingSlot> bikeSlots;
    vector<ParkingSlot> carSlots;
    vector<ParkingTicket*> tickets;

    int ticketCounter;

    ParkingManager(){  //private constructor
        ticketCounter = 1;

        for(int i=1; i<=15; i++)
            bikeSlots.push_back(ParkingSlot(i));

        for(int i=1; i<=10; i++)
            carSlots.push_back(ParkingSlot(i));
    }

public:
    static ParkingManager* getInstance(){
        if(object == nullptr)
            object = new ParkingManager();
        return object;
    }

    int assignSlot(Vehicle* v){
        if(v->getType() == "car"){
             for (int i = 0; i < carSlots.size(); i++) {
                if (carSlots[i].isAvailable()) {
                    carSlots[i].parkVehicle(v);
                    cout << "Car parked at slot: " << carSlots[i].slotID << endl;
                    return carSlots[i].slotID;
                }
            }
            cout<<"No car slots available\n";
        }
        else{
             for (int i = 0; i < bikeSlots.size(); i++) {
                if (bikeSlots[i].isAvailable()) {
                    bikeSlots[i].parkVehicle(v);
                    cout << "Bike parked at slot: " << bikeSlots[i].slotID << endl;
                    return bikeSlots[i].slotID;
                }
            }
            cout<<"No bike slots available\n";
        }
        return -1;  //returns invalid 
    }

	 ParkingTicket* createTicket(Vehicle* v, int slot) {
        ParkingTicket* t = new ParkingTicket(ticketCounter, v, slot);
        ticketCounter++;
        tickets.push_back(t);
        return t;
    }

    ParkingTicket* findTicket(int id) {
        for (int i = 0; i < tickets.size(); i++) {
            if (tickets[i]->ticketID == id) {
                return tickets[i];
            }
        }
        return nullptr;
    }

     void releaseSlot(ParkingTicket* ticket) {

        if (ticket->vehicle->getType() == "car") {
            for (int i = 0; i < carSlots.size(); i++) {
                if (carSlots[i].slotID == ticket->slotID) {
                    carSlots[i].removeVehicle();
                }
            }
        }
        else {
            for (int i = 0; i < bikeSlots.size(); i++) {
                if (bikeSlots[i].slotID == ticket->slotID) {
                    bikeSlots[i].removeVehicle();
                }
            }
        }
    }


    void showAvailableSlots(){
        cout << "Available Bike Slots: ";
        for (int i = 0; i < bikeSlots.size(); i++) {
            if (bikeSlots[i].isAvailable()) {
                cout << bikeSlots[i].slotID << " ";
            }
        }
        cout << endl;

        cout << "Available Car Slots: ";
        for (int i = 0; i < carSlots.size(); i++) {
            if (carSlots[i].isAvailable()) {
                cout << carSlots[i].slotID << " ";
            }
        }
        cout << endl;
    }
};

// static initialization
ParkingManager* ParkingManager::object = nullptr;

int main(){

    ParkingManager* manager = ParkingManager::getInstance();

    int choice;

    do{
        cout<<"\n=======================================\n";
        cout<<"   PARKING MANAGEMENT SYSTEM\n";
        cout<<"=======================================\n";
        cout<<"1. Park Vehicle\n";
        cout<<"2. Exit Vehicle\n";
        cout<<"3. Show Available Slots\n";
        cout<<"0. Exit Program\n";
        cout<<"Enter choice: ";
        cin>>choice;

        if(choice == 1){
            string type;
            cout<<"Enter vehicle type (car/bike): ";
            cin>>type;

            Vehicle* v = VehicleFactory::createVehicle(type);

            if(v == nullptr){
                cout<<"Invalid vehicle type!\n";
                continue;
            }

            v->display();

            int slot = manager->assignSlot(v);

            if(slot != -1){
                ParkingTicket* ticket = manager->createTicket(v, slot);
                ticket->printTicket();
            }
        }

        else if(choice == 2){
            int id, hours;
            cout<<"Enter Ticket ID: ";
            cin>>id;

            ParkingTicket* ticket = manager->findTicket(id);

            if(ticket == nullptr){
                cout<<"Invalid Ticket ID!\n";
                continue;
            }

            cout<<"Enter parking hours: ";
            cin>>hours;

            ticket->calculateFee(hours);
            manager->releaseSlot(ticket);

            cout<<"Vehicle exited successfully.\n";
        }

        else if(choice == 3){
            manager->showAvailableSlots();
        }

    } while(choice != 0);

    cout<<"\nProgram ended.\n";
    return 0;
}