#include<iostream>
#include<vector>
#include<string>

using namespace std;

// ================= VEHICLE =================
class Vehicle {
public:
    virtual void display() = 0;
    virtual string getType() = 0;
};

class Car : public Vehicle {
public:
    void display() { cout << "Car entered the parking.\n"; }
    string getType() { return "car"; }
};

class Bike : public Vehicle {
public:
    void display() { cout << "Bike entered the parking.\n"; }
    string getType() { return "bike"; }
};

// ================= FACTORY =================
class VehicleFactory {
public:
    static Vehicle* createVehicle(string type) {
        if (type == "car" || type == "Car" || type == "CAR")
            return new Car();
        else if (type == "bike" || type == "Bike" || type == "BIKE")
            return new Bike();
        return nullptr;
    }
};

// ================= STATE PATTERN =================

class SlotState {
public:
    virtual string getState() = 0;
};

class AvailableState : public SlotState {
public:
    string getState() { return "Available"; }
};

class OccupiedState : public SlotState {
public:
    string getState() { return "Occupied"; }
};

// ================= PARKING SLOT =================
class ParkingSlot {
public:
    int slotID;
    Vehicle* vehicle;
    SlotState* state;

    ParkingSlot(int id) {
        slotID = id;
        vehicle = nullptr;
        state = new AvailableState();
    }

    bool isAvailable() {
        return state->getState() == "Available";
    }

    void parkVehicle(Vehicle* v) {
        vehicle = v;
        state = new OccupiedState();
    }

    void removeVehicle() {
        vehicle = nullptr;
        state = new AvailableState();
    }
};

// ================= TICKET =================
class ParkingTicket {
public:
    int ticketID;
    int slotID;;
    Vehicle* vehicle;

    ParkingTicket(int id, Vehicle* v, int slot) {
        ticketID = id;
        vehicle = v;
        slotID = slot;
    }

    void printTicket() {
        cout << "Ticket ID: " << ticketID << ", Slot: " << slotID << endl;
    }

    void calculateFee(int hours) {
        double fee = (vehicle->getType() == "car") ? hours * 20 : hours * 10;
        cout << "Parking Fee: Rs " << fee << "/-\n";
    }
};

// ================= SINGLETON MANAGER =================
class ParkingManager {
private:
    static ParkingManager* object;

    vector<ParkingSlot> bikeSlots;
    vector<ParkingSlot> carSlots;
    vector<ParkingTicket*> tickets;

    int ticketCounter;

    ParkingManager() {
        ticketCounter = 1;

        for (int i = 1; i <= 5; i++)
            bikeSlots.push_back(ParkingSlot(i));

        for (int i = 1; i <= 5; i++)
            carSlots.push_back(ParkingSlot(i));
    }

public:
    static ParkingManager* getInstance() {
        if (object == nullptr)
            object = new ParkingManager();
        return object;
    }

    int assignSlot(Vehicle* v) {
        vector<ParkingSlot>& slots =
            (v->getType() == "car") ? carSlots : bikeSlots;

        for (auto& s : slots) {
            if (s.isAvailable()) {
                s.parkVehicle(v);
                cout << v->getType() << " parked at slot: " << s.slotID << endl;
                return s.slotID;
            }
        }

        cout << "No slots available!\n";
        return -1;
    }

    ParkingTicket* createTicket(Vehicle* v, int slot) {
        ParkingTicket* t = new ParkingTicket(ticketCounter++, v, slot);
        tickets.push_back(t);
        return t;
    }

    ParkingTicket* findTicket(int id) {
        for (auto t : tickets)
            if (t->ticketID == id)
                return t;
        return nullptr;
    }

    void releaseSlot(ParkingTicket* ticket) {
        vector<ParkingSlot>& slots =
            (ticket->vehicle->getType() == "car") ? carSlots : bikeSlots;

        for (auto& s : slots) {
            if (s.slotID == ticket->slotID) {
                s.removeVehicle();
            }
        }
    }

    void showAvailableSlots() {
        cout << "Bike Slots: ";
        for (auto& s : bikeSlots)
            if (s.isAvailable()) cout << s.slotID << " ";

        cout << "\nCar Slots: ";
        for (auto& s : carSlots)
            if (s.isAvailable()) cout << s.slotID << " ";

        cout << endl;
    }
};

ParkingManager* ParkingManager::object = nullptr;

// ================= COMMAND PATTERN =================
class Command {
public:
    virtual void execute() = 0;
};

class ParkCommand : public Command {
public:
    void execute() {
        string type;
        cout << "Enter vehicle type: ";
        cin >> type;

        Vehicle* v = VehicleFactory::createVehicle(type);
        if (!v) {
            cout << "Invalid type!\n";
            return;
        }

        v->display();

        ParkingManager* manager = ParkingManager::getInstance();
        int slot = manager->assignSlot(v);

        if (slot != -1) {
            ParkingTicket* t = manager->createTicket(v, slot);
            t->printTicket();
        }
    }
};

class ExitCommand : public Command {
public:
    void execute() {
        int id, hours;
        cout << "Enter Ticket ID: ";
        cin >> id;

        ParkingManager* manager = ParkingManager::getInstance();
        ParkingTicket* t = manager->findTicket(id);

        if (!t) {
            cout << "Invalid ID\n";
            return;
        }

        cout << "Enter hours: ";
        cin >> hours;

        t->calculateFee(hours);
        manager->releaseSlot(t);

        cout << "Vehicle exited.\n";
    }
};

class ShowSlotsCommand : public Command {
public:
    void execute() {
        ParkingManager::getInstance()->showAvailableSlots();
    }
};

// ================= MAIN =================
int main() {
    Command* commands[3];
    commands[0] = new ParkCommand();
    commands[1] = new ExitCommand();
    commands[2] = new ShowSlotsCommand();

    int choice;

    do {
        cout << "\n1. Park\n2. Exit\n3. Show Available Slots\n0. Quit\nChoice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 3)
            commands[choice - 1]->execute();

    } while (choice != 0);

    return 0;
}