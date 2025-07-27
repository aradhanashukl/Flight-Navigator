#include <bits/stdc++.h>
using namespace std;
class Airport;
class Flights
{
public:
    Airport *from;
    Airport *to;
    double distance, time, price;
    Flights(Airport *f, Airport *t, double d, double ti, double p) : from(f), to(t), distance(d), time(ti), price(p) {}
    double getWeight(string Critera)
    {
        if (Critera == "distance")
            return distance;
        if (Critera == "time")
            return time;
        if (Critera == "price")
            return price;
        if (Critera == "best")
            return distance * 0.4 + time * 0.3 + price * 0.3;
        return distance;
    }
};
class Airport
{
    string code;

public:
    string Name;
    vector<Flights *> flights;
    Airport(string c, string n) : code(c), Name(n) {}
    string getCode() { return code; }
};

class FlightNetwork
{
    unordered_map<string, Airport *> airport;

public:
    ~FlightNetwork()
    {
        // Cleanup memory
        for (auto &entry : airport)
        {
            for (Flights *f : entry.second->flights)
                delete f;
            delete entry.second;
        }
    }
    void addAirport(string code, string name)
    {
        if (airport.find(code) != airport.end())
            return;
        Airport *newport = new Airport(code, name);
        airport[code] = newport;
    }
    void addFlight(string from, string to, double distance, double time, double price)
    {
        if (airport.find(from) == airport.end() || airport.find(to) == airport.end())
        {
            cout << "Error: No Flight in this Route" << endl;
            return;
        }
        Airport *src = airport[from];
        Airport *dest = airport[to];
        Flights *fli = new Flights(src, dest, distance, time, price);
        src->flights.push_back(fli);
    }
    void removeAirport(string code)
    {
        if (airport.find(code) == airport.end())
            return;
        Airport *target = airport[code];
        // we need to delete all flights going out of it
        for (Flights *f : target->flights)
            delete f;
        target->flights.clear();
        // remove all flights from other airports pointing to this airport
        for (auto &entry : airport)
        {
            Airport *a = entry.second;
            a->flights.erase(
                remove_if(a->flights.begin(), a->flights.end(),
                          [code](Flights *f)
                          {
                              if (f->to->getCode() == code)
                              {
                                  delete f; // clear  memory
                                  return true;
                              }
                              return false;
                          }),
                a->flights.end());
        }

        // Finally, remove airport
        delete target;
        airport.erase(code);
    }
    void removeflight(string from, string to)
    {
        if (airport.find(from) == airport.end())
            return;
        Airport *src = airport[from];
        src->flights.erase(
            remove_if(src->flights.begin(), src->flights.end(),
                      [to](Flights *f)
                      {
                          if (f->to->getCode() == to)
                          {
                              delete f; // Free memory
                              return true;
                          }
                          return false;
                      }),
            src->flights.end());
    }
    vector<string> shortestpath(string start, string end, string critera)
    {
        if (airport.find(start) == airport.end() || airport.find(end) == airport.end())
        {
            cout << "NO PATH IS THERE" << endl;
            return {};
        }
        unordered_map<string, double> dist;
        unordered_map<string, string> parents;
        for (auto &entry : airport)
        {
            dist[entry.first] = DBL_MAX; // initally put the infinity
        }
        dist[start] = 0;
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        pq.push({0, start});
        while (!pq.empty())
        {
            double cost = pq.top().first;
            string curr = pq.top().second;
            pq.pop();
            if (curr == end)
                break;
            for (Flights *f : airport[curr]->flights)
            {
                string next = f->to->getCode();
                double weight = f->getWeight(critera);
                double nextcst = cost + weight;
                if (nextcst < dist[next])
                {
                    dist[next] = nextcst;
                    parents[next] = curr;
                    pq.push({nextcst, next});
                }
            }
        }
        vector<string> path;
        if (dist[end] == DBL_MAX)
        {
            cout << "NO PATH FOUND BETWEEN" << start << "TO" << end;
            return path;
        }
        string node = end;
        while (node != start)
        {
            path.push_back(node);
            node = parents[node];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
        return path;
    }
    void displayAllFlights()
    {
        cout << "\nAvailable Flights:\n";
        for (auto &entry : airport)
        {
            Airport *a = entry.second;
            for (Flights *f : a->flights)
            {
                cout << f->from->getCode() << " -> " << f->to->getCode()
                     << " (Distance: " << f->distance << " km, "
                     << "Time: " << f->time << " hr, "
                     << "Price: " << f->price << ")\n";
            }
        }
        cout << endl;
    }
    Airport *getAirport(const string &code)
    {
        if (airport.find(code) != airport.end())
            return airport[code];
        return nullptr;
    }
};
class RouterPlan
{
    FlightNetwork *network;

public:
    RouterPlan(FlightNetwork *net) : network(net) {};
    void flightfastroute(string start, string end)
    {
        vector<string> path = network->shortestpath(start, end, "time");
        cout << "\n fastest route by time is ";
        displayroute(path);
    }
    void flightcheaproute(string start, string end)
    {
        vector<string> path = network->shortestpath(start, end, "price");
        cout << "\n Cheapest route by price is";
        displayroute(path);
    }
    void flightshortroute(string start, string end)
    {
        vector<string> path = network->shortestpath(start, end, "distance");
        cout << "\n Shortest route by distance is";
        displayroute(path);
    }
    void flightbestrout(string start, string end)
    {
        vector<string> path = network->shortestpath(start, end, "best");
        cout << "\n best route to choose is";
        displayroute(path);
    }
    void displayroute(const vector<string> &path)
    {
        if (path.empty())
        {
            cout << "NO PATH FOUND";
            return;
        }
        double totalDistance = 0, totalTime = 0, totalPrice = 0;

        for (size_t i = 0; i < path.size(); ++i)
        {
            cout << path[i];
            if (i < path.size() - 1)
                cout << " -> ";
        }
        cout << endl;
        for (size_t i = 0; i < path.size() - 1; ++i)
        {
            Airport *src = network->getAirport(path[i]);
            for (Flights *f : src->flights)
            {
                if (f->to->getCode() == path[i + 1])
                {
                    totalDistance += f->distance;
                    totalTime += f->time;
                    totalPrice += f->price;
                    break;
                }
            }
        }

        cout << "\nTotal Distance: " << totalDistance << " km"
             << ", Total Time: " << totalTime << " hr"
             << ", Total Price: " << totalPrice << " INR" << endl;
    }
};

int main()
{
    FlightNetwork network;

    // Add airports
    network.addAirport("DEL", "Delhi");
    network.addAirport("MUM", "Mumbai");
    network.addAirport("BLR", "Bangalore");
    network.addAirport("HYD", "Hyderabad");
    network.addAirport("CHE", "Chennai");
    network.addAirport("KOL", "Kolkata");
    network.addAirport("PNQ", "Pune");
    network.addAirport("GOI", "Goa");
    network.addAirport("LKO", "Lucknow");
    network.addAirport("AMD", "Ahmedabad");
    network.addAirport("CCU", "Coimbatore");
    network.addAirport("BHU", "Bhubaneswar");
    network.addAirport("PAT", "Patna");
    network.addAirport("JAI", "Jaipur");
    network.addAirport("TRV", "Trivandrum");
    network.addAirport("IXC", "Chandigarh");
    network.addAirport("RPR", "Raipur");
    network.addAirport("VNS", "Varanasi");
    network.addAirport("NAG", "Nagpur");
    network.addAirport("IMF", "Imphal");

    // Add initial flights
    network.addFlight("DEL", "MUM", 1400, 2, 5000);
    network.addFlight("DEL", "BLR", 1700, 2.5, 6000);
    network.addFlight("DEL", "KOL", 1500, 2, 5500);
    network.addFlight("DEL", "LKO", 500, 1, 2500);
    network.addFlight("DEL", "AMD", 950, 1.5, 4000);
    network.addFlight("MUM", "GOI", 600, 1, 3000);
    network.addFlight("MUM", "PNQ", 150, 0.5, 1200);
    network.addFlight("MUM", "HYD", 900, 1.5, 3500);
    network.addFlight("MUM", "CHE", 1300, 2, 4500);
    network.addFlight("BLR", "HYD", 600, 1, 3000);
    network.addFlight("BLR", "CHE", 350, 0.8, 2000);
    network.addFlight("BLR", "TRV", 730, 1.2, 2700);
    network.addFlight("KOL", "PAT", 550, 1, 2300);
    network.addFlight("KOL", "BHU", 430, 0.9, 2100);
    network.addFlight("CHE", "TRV", 750, 1.2, 2900);
    network.addFlight("PNQ", "GOI", 450, 0.9, 1800);
    network.addFlight("AMD", "JAI", 670, 1.3, 2600);
    network.addFlight("JAI", "IXC", 540, 1.1, 2300);
    network.addFlight("IXC", "DEL", 250, 0.7, 1800);
    network.addFlight("PAT", "VNS", 300, 0.6, 1500);
    network.addFlight("RPR", "NAG", 400, 0.8, 1700);
    network.addFlight("NAG", "HYD", 500, 1, 2200);
    network.addFlight("IMF", "KOL", 1200, 2.5, 5000);
    network.addFlight("TRV", "BLR", 730, 1.2, 2700);
    network.addFlight("GOI", "BLR", 600, 1, 2500);

    RouterPlan router(&network);

    string role;
    cout << "Enter role (user/faculty): ";
    cin >> role;

    if (role == "faculty")
    {
        int option;
        do
        {
            cout << "\n--- Faculty Menu ---\n";
            cout << "1. Add Airport\n2. Add Flight\n3. Remove Airport\n4. Remove Flight\n";
            cout << "5. Show Flights\n6. Plan Route\n7. Exit\n";
            cout << "Enter option: ";
            cin >> option;

            if (option == 1)
            {
                string code, name;
                cout << "Enter airport code and name: ";
                cin >> code >> name;
                network.addAirport(code, name);
            }
            else if (option == 2)
            {
                string from, to;
                double dist, time, price;
                cout << "Enter from, to, distance, time, price: ";
                cin >> from >> to >> dist >> time >> price;
                network.addFlight(from, to, dist, time, price);
            }
            else if (option == 3)
            {
                string code;
                cout << "Enter airport code to remove: ";
                cin >> code;
                network.removeAirport(code);
            }
            else if (option == 4)
            {
                string from, to;
                cout << "Enter from and to airport code: ";
                cin >> from >> to;
                network.removeflight(from, to);
            }
            else if (option == 5)
            {
                network.displayAllFlights();
            }
            else if (option == 6)
            {
                string start, end, choice;
                network.displayAllFlights();
                cout << "Enter source airport code: ";
                cin >> start;
                cout << "Enter destination airport code: ";
                cin >> end;
                cout << "Choose route preference (fast/cheap/short/best): ";
                cin >> choice;

                if (choice == "fast")
                    router.flightfastroute(start, end);
                else if (choice == "cheap")
                    router.flightcheaproute(start, end);
                else if (choice == "short")
                    router.flightshortroute(start, end);
                else if (choice == "best")
                    router.flightbestrout(start, end);
                else
                    cout << "Invalid choice!" << endl;
            }
        } while (option < 7 && option > 0);
    }
    else if (role == "user")
    {
        network.displayAllFlights();

        string start, end, choice;
        cout << "Enter source airport code: ";
        cin >> start;
        cout << "Enter destination airport code: ";
        cin >> end;
        cout << "Choose route preference (fast/cheap/short/best): ";
        cin >> choice;

        if (choice == "fast")
            router.flightfastroute(start, end);
        else if (choice == "cheap")
            router.flightcheaproute(start, end);
        else if (choice == "short")
            router.flightshortroute(start, end);
        else if (choice == "best")
            router.flightbestrout(start, end);
        else
            cout << "Invalid choice!" << endl;
    }
    else
    {
        cout << "Invalid role!" << endl;
    }
    return 0;
}