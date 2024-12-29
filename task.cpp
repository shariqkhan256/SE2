#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

using namespace std;

// Singleton Pattern: Meadow Class
class Meadow {
private:
    static Meadow* instance;
    Meadow() {}

public:
    static Meadow* getInstance() {
        if (!instance)
            instance = new Meadow();
        return instance;
    }

    void createAntFarm() {
        cout << "AntFarm created in Meadow." << endl;
    }
};
Meadow* Meadow::instance = nullptr;

// Colony Class to store information about each colony
class Colony {
public:
    string species;
    int workers;
    int warriors;
    int antKills;
    int colonyKills;
    int ticksAlive;
    bool isAlive;

    Colony(string sp) : species(sp), workers(0), warriors(0), antKills(0), colonyKills(0), ticksAlive(0), isAlive(true) {}

    void addResource(const string& resource, int amount) {
        if (resource == "food") workers += amount;
        else if (resource == "warrior") warriors += amount;
    }

    void killAnt() { antKills++; }
    void killColony() { colonyKills++; isAlive = false; }

    void tick() { ticksAlive++; }
};

// Command Handling
class CommandHandler {
private:
    unordered_map<int, unique_ptr<Colony>> colonies;
    int nextColonyId;

public:
    CommandHandler() : nextColonyId(1) {}

    void spawn(int x, int y, const string& species) {
        colonies[nextColonyId] = make_unique<Colony>(species);
        cout << "Colony " << nextColonyId << " of species " << species << " created at position (" << x << ", " << y << ")." << endl;
        nextColonyId++;
    }

    void give(int id, const string& resource, int amount) {
        if (colonies.find(id) != colonies.end()) {
            colonies[id]->addResource(resource, amount);
            cout << "Colony " << id << " receives " << amount << " " << resource << "." << endl;
        } else {
            cout << "Colony " << id << " not found!" << endl;
        }
    }

    void tick(int t = 1) {
        for (auto& colony : colonies) {
            for (int i = 0; i < t; ++i) {
                colony.second->tick();
            }
        }
        cout << "Performed " << t << " tick(s)." << endl;
    }

    void summary(int id) {
        if (colonies.find(id) != colonies.end()) {
            Colony* colony = colonies[id].get();
            cout << "Summary of Colony " << id << " (" << colony->species << "):" << endl;
            cout << "Workers: " << colony->workers << endl;
            cout << "Warriors: " << colony->warriors << endl;
            cout << "Ant Kills: " << colony->antKills << endl;
            cout << "Colony Kills: " << colony->colonyKills << endl;
            cout << "Ticks Alive: " << colony->ticksAlive << endl;
            cout << "Status: " << (colony->isAlive ? "Alive" : "Killed") << endl;
        } else {
            cout << "Colony " << id << " not found!" << endl;
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Singleton Meadow
    Meadow* meadow = Meadow::getInstance();
    meadow->createAntFarm();
    CommandHandler commandHandler;
    commandHandler.spawn(14, -32, "Killer");
    commandHandler.spawn(5, 10, "Pansy");
    commandHandler.give(1, "food", 50);
    commandHandler.give(2, "warrior", 10);
    commandHandler.tick(10);
    commandHandler.summary(1);
    commandHandler.summary(2);

    return 0;
}
