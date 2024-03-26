#include <bits/stdc++.h>
#include <unistd.h>
#include <time.h>
using namespace std;

random_device rd;
mt19937 gen(rd());

struct Passenger
{
    int initial_floor;
    int target;
    bool up = true;
    bool operator<(const Passenger &other) const
    {
        return initial_floor < other.initial_floor;
    }
};

struct Floor
{
    bool active = false;
    bool elevator = false;
    Passenger passenger;
};

void show(map<int, Floor> building, int maximal_floor)
{
    vector<string> result;
    for (auto [floor, value] : building)
    {
        string f;
        if (floor == 1)
        {
            if (!value.elevator)
            {
                if (value.active)
                {
                    f += "______________________________ \n";
                    f += "|         |      O            |\n";
                    f += "|         |    /|_|\\          |\n";
                    f += "|         |     / \\           |\n";
                    f += "|_________|___________________|\n";
                }
                else
                {
                    f += "______________________________\n";
                    f += "|         |                   |\n";
                    f += "|         |                   |\n";
                    f += "|         |                   |\n";
                    f += "|_________|___________________|\n";
                }
            }
            else
            {
                if (value.active)
                {
                    f += "______________________________ \n";
                    f += "|  _____  |      O            |\n";
                    f += "|  |    | |    /|_|\\          |\n";
                    f += "|  |____| |     / \\           |\n";
                    f += "|_________|___________________|\n";
                }
                else
                {
                    f += "______________________________ \n";
                    f += "|  _____  |                   |\n";
                    f += "|  |    | |                   |\n";
                    f += "|  |____| |                   |\n";
                    f += "|_________|___________________|\n";
                }
            }
        }
        else
        {
            if (!value.elevator)
            {
                if (value.active)
                {
                    f += "______________________________ \n";
                    f += "|         |      O            |\n";
                    f += "|         |    /|_|\\          |\n";
                    f += "|         |     / \\           |\n";
                    f += "|         |                   |\n";
                }
                else
                {
                    f += "______________________________\n";
                    f += "|         |                   |\n";
                    f += "|         |                   |\n";
                    f += "|         |                   |\n";
                    f += "|         |                   |\n";
                }
            }
            else
            {
                if (value.active)
                {
                    f += "______________________________ \n";
                    f += "|  _____  |      O            |\n";
                    f += "|  |    | |    /|_|\\          |\n";
                    f += "|  |____| |     / \\           |\n";
                    f += "|         |                   |\n";
                }
                else
                {
                    f += "______________________________ \n";
                    f += "|  _____  |                   |\n";
                    f += "|  |    | |                   |\n";
                    f += "|  |____| |                   |\n";
                    f += "|         |                   |\n";
                }
            }
        }
        result.push_back(f);
    }
    reverse(result.begin(), result.end());
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i];
    }
}

int get_target(int floor, int max_floor)
{
    if (floor != 1)
    {
        uniform_int_distribution<int> first(1, 2);
        int to_first_floor = first(gen);
        if (to_first_floor == 1)
        {
            return 1;
        }
        else
        {
            int target = 2 + (rand() % (max_floor - 1));
            if (target == floor)
            {
                while (target == floor)
                {
                    target = 2 + (rand() % (max_floor - 1));
                }
            }
            return target;
        }
    }
    else
    {
        int target = 2 + (rand() % (max_floor - 1));
        return target;
    }
}

int maximal_target(multiset<Passenger> passengers, int current_floor, bool up, int maximal_floor)
{
    if (up)
    {
        int res = 1;
        for (const auto &passenger : passengers)
        {
            res = max(passenger.target, res);
            res = max(passenger.initial_floor, res);
        }
        return res;
    }
    else
    {
        int res = 100;
        for (const auto &passenger : passengers)
        {
            res = min(passenger.target, res);
            res = min(passenger.initial_floor, res);
        }
        return res;
    }
}

int main()
{
    int initial_time = 100, floor_amount = 6, i = 1, elevator = 0, got_down = 0;
    bool up = true;
    multimap<int, int> happy_passengers;
    multiset<Passenger> elevator_count;
    map<int, Floor> building;
    multiset<Passenger> passengers;
    Floor first_floor; // turning on an elevator on the first floor
    first_floor.elevator = true;
    building[1] = first_floor;
    for (int i = 2; i <= floor_amount; i++) // initializing floors according to floor amount
    {
        Floor floor;
        building[i] = floor;
    }
    while (initial_time > 0)
    {
        system("cls");
        srand(time(NULL));
        for (int j = 1; j <= floor_amount; j++) // people generator
        {
            int chanse = rand() % 50;
            if (chanse == 1)
            {
                building[j].active = true;
                building[j].passenger.target = get_target(j, floor_amount);
                building[j].passenger.initial_floor = j;
                if (building[j].passenger.target < j)
                {
                    building[j].passenger.up = false;
                }
                passengers.insert(building[j].passenger);
            }
        }
        show(building, floor_amount);
        bool noPassengers = passengers.empty();
        if (!noPassengers)
        {
            int current_target = maximal_target(passengers, i, up, floor_amount);
            for (auto passenger : passengers)
            {
                if (building[i].active and building[i].passenger.up == up)
                {
                    building[i].active = false;
                    elevator_count.insert(passenger);
                    elevator++;
                }
                if (passenger.target == i)
                {
                    auto it = elevator_count.find(passenger);
                    if (it != elevator_count.end())
                    {
                        passengers.erase(passenger);
                        elevator_count.erase(it);
                        happy_passengers.insert({passenger.initial_floor, passenger.target});
                        elevator--;
                        got_down++;
                    }
                }
            }
            if (up && i < floor_amount && current_target > i)
            {
                building[i].elevator = false;
                i++;
                building[i].elevator = true;
            }
            else if (!up && i > 1 && current_target < i)
            {
                building[i].elevator = false;
                i--;
                building[i].elevator = true;
            }
            else
            {
                up = !up;
            }
            cout << current_target << " - Current target!" << endl;
        }
        for (auto hello : passengers)
        {
            cout << hello.initial_floor << " " << hello.target << " " << hello.up << endl;
        }
        cout << "Satisfied passengers: " << got_down;
        sleep(1);
        initial_time--;
    }
}
