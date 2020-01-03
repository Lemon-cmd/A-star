#include <iostream>
#include <string> 
#include <vector> 
#include <map> 
#include <unordered_set>
#include <math.h>

using namespace std;

// create an edge structure
struct Connection 
{
    int cost; 
    vector<string> path;
};

// create node structure for heap
struct Node 
{
    Connection* current; 
    Node* next; 
    Node* prev; 
};

class MinHeap
{
    /* Heap Class */
    private:  
        // declare head and bottom
        Node* head;
        Node* bottom;
        int count = 0;  // integer for keeping track of size

        void display_path(vector<string> path)
        {
            //display path of the node
            for (int i = 0; i < path.size(); i ++)
            {
                cout << path[i] << " ";
            }
            cout << " " << endl;
        }

    public:    
        MinHeap() 
        {
            //construct head and bottom
            //initialize their values to null pointers
            head = (Node*)malloc(sizeof(Node));
            bottom = (Node*)malloc(sizeof(Node));

            head->current = nullptr;
            head->next = nullptr; 
            head->prev = nullptr; 

            bottom->current = nullptr;
            bottom->next = nullptr; 
            bottom->prev = nullptr;
        }
        
        bool isEmpty()
        {
            // A method for checking if queue is empty
            if (count == 0)
            {
                return true;
            }
            return false;
        }

        void push(Connection* item)
        {
            /* Push Method; higher to low == left to right */
            if (head->current == nullptr)   // if head is empty
            {
                head->current = item;
                head->next = bottom;
                head->prev = nullptr;
                bottom->prev = head;
                count += 1;
            }

            else if (bottom->current == nullptr && count == 1)    // if bottom is empty
            {
                if (head->current->cost < item->cost)
                {
                    bottom->current = head->current;
                    bottom->prev = head;
                    head->current = item;
                    head->next = bottom;
                    head->prev = nullptr;
                    count += 1;
                }

                else 
                {
                    bottom->current = item;
                    bottom->prev = head;
                    head->next = bottom;
                    count += 1;
                }
            }

            else    // if neither are empty
            {
                if (head->current->cost <= item->cost)  // check if head is lower than new item
                {
                    //set new item as the new head
                    Node* oldH = new Node();
                    oldH->current = head->current;
                    oldH->next = head->next;
                    head->next->prev = oldH;
                    head->current = item; 
                    head->next = oldH;
                    head->prev = nullptr;
                    oldH->prev = head;
                    count += 1;
                }

                else if (bottom->current->cost >= item->cost)   // check if bottom is greater than new item
                {
                    //set new item as the new bottom
                    Node* oldB = new Node();
                    oldB->current = bottom->current;
                    oldB->prev = bottom->prev;
                    bottom->prev->next = oldB;
                    bottom->current = item;
                    bottom->prev = oldB;
                    bottom->next = nullptr; 
                    oldB->next = bottom;
                    count += 1;
                }

                else 
                {  
                    //Look up a node that is less than item
                    Node* start = bottom;
                    while (start->prev != nullptr && start->current->cost < item->cost)
                    {
                        start = start->prev;
                    }
                    //insert the new entry in between start and start->next
                    Node* new_entry = new Node();
                    new_entry->current = item; 
                    new_entry->next = start->next; 
                    new_entry->prev = start;
                    start->next->prev = new_entry;
                    start->next = new_entry;

                    count += 1;
                }
            }

        }

        Node pop()
        {
            if (count == 1)     
            {
                //pop head
                Node oldH = {head->current, nullptr, nullptr};
                head->current = nullptr;
                count -= 1;

                return oldH;
            }

            else if (count == 2)
            {
                //pop bottom
                Node oldB = {bottom->current, nullptr, nullptr};
                bottom->current = nullptr;
                count -= 1;
                
                return oldB;
            }

            else if (count > 2)
            {
                //pop bottom and set bottom = its previous node
                Node oldB = {bottom->current, nullptr, nullptr};
                bottom = bottom->prev; 
                bottom->next = nullptr;
                count -= 1;

                return oldB;
            }
            
            else 
            {
                cout << "CANNOT POP AN EMPTY HEAP" << endl;
            }
        }

        void display()
        {
            // Method for displaying the Heap
            
            if (count > 0)
            {
                if (count == 1)
                {
                    display_path(head->current->path);
                }

                else if (count > 1)
                {
                    Node* start = bottom;
                    while (start->prev != nullptr)
                    {
                        display_path(start->current->path);
                        start = start->prev;
                    }

                    if (start->current != nullptr)
                    {
                        display_path(start->current->path);
                    }
                }
            }
             
        }
};

class AStar
{
    private: 
        // create a map for neighbors 
        map< string, vector<pair<string, int> > > neighbors; 
        map< string, pair<float,float> > coordinates;

        bool manhattan_dist;

        float euclidean(string origin, string destination)
        {
            pair<float, float> origin_p = coordinates[origin];
            pair<float, float> dest_p = coordinates[destination];

            float x1 = origin_p.first;
            float y1 = origin_p.second;
            float x2 = dest_p.first;
            float y2 = dest_p.second;
            float euclidean_score = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
            
            return euclidean_score;
        }

        float manhattan(string origin, string destination)
        {
            pair<float, float> origin_p = coordinates[origin];
            pair<float, float> dest_p = coordinates[destination];

            float x1 = origin_p.first;
            float y1 = origin_p.second;
            float x2 = dest_p.first;
            float y2 = dest_p.second; 

            float manhattan_score = (abs(x1 - x2) + abs(y1 - y2));

            return manhattan_score;
        }

        float heuristic(string origin, string destination)
        {
           if (manhattan_dist == true)
           {
               float heuristic = manhattan(origin, destination);
               cout << "Heuristic from " << origin << " to " << destination << " : " << heuristic << endl;
               return heuristic;
           }
           
           else 
           {
               float heuristic = euclidean(origin, destination);
               cout << "Heuristic from " << origin << " to " << destination << " : " << heuristic << endl;
               return heuristic;
           }
         
        }

    public:     
        AStar(string type)
        {
            if (type == "manhattan")
            {
                manhattan_dist = true;
            }

            else 
            {
                manhattan_dist = false;
            }

        }

        void make(string name, float x, float y)
        {
            //Create an entry in the map and set it equals to a vector
            vector <pair<string, int> > list; 
            pair<float, float> coord;
            coord.first = x;
            coord.second = y;

            neighbors[name] = list;
            coordinates[name] = coord;
        }

        void create(string origin, string destination, int cost)
        {
            //Create an Edge using a pair
            pair<string, int> origin_dest;
            origin_dest = make_pair(destination, cost); 
            
            pair<string, int> dest_origin;
            dest_origin = make_pair(origin, cost);
            
            //bi-directional edge
            neighbors[origin].push_back(origin_dest);           
            neighbors[destination].push_back(dest_origin);
            //cout << "Origin: " << origin << " # of edges: " < neighbors[origin].size() << endl;
        }

        void search(string origin, string destination)
        {
            /* Search Method */

            MinHeap route = MinHeap();
            float h_origin = heuristic(origin, destination);
            
            cout << h_origin << endl;

            //Grab all of origin's neighbors (edges)
            for (int i = 0; i < neighbors[origin].size(); i ++)
            {
                cout << "neighbor: " << neighbors[origin][i].first << endl;
                Connection* edge = new Connection();
                int cost = neighbors[origin][i].second;
                
                edge->cost = cost + heuristic(neighbors[origin][i].first, destination);
                edge->path.push_back(origin);
                edge->path.push_back(neighbors[origin][i].first);
                route.push(edge);
            }
            
            unordered_set<string> visited;          // create a set of strings
            visited.insert(origin);                 // insert origin
            
            int pass = 0;

            while (route.isEmpty() != true)
            {
                //route.display();
                cout << "PASS: " << pass << endl;

                Node current_entry = route.pop();                   // Grab the pop'd Node
                int cost = current_entry.current->cost;             // set cost
                vector<string> path = current_entry.current->path;  // set vector

                string current = path[path.size()-1];               // set current which is the last item in the path (vector)
                cout << "Current: " << current << endl;

                if (visited.find(current) != visited.end())         // if current is in visited, continue
                {
                    continue;
                }

                if (current == destination)                         // if current is destination, output and stop
                {
                    cout << "\n-----FOUND DESTINATION-----" << endl;
                    cout << "COST: " << cost << " ";
                    cout << "\tPATH: ";

                    for (int i = 0; i < path.size(); i  ++)
                    {
                        cout  << path[i] << "  " ;
                    }
                    cout << " " << endl;
                    break;
                }

                // Grab all of current's edges with its neighbors
                for (int i = 0; i < neighbors[current].size(); i ++ )
                {
                    if ( visited.find(neighbors[current][i].first) == visited.end() )
                    {
                        int new_cost = cost + neighbors[current][i].second;         // set neighbor cost + current cost
                        vector<string> new_path = path;                             // set a new path
                        new_path.push_back(neighbors[current][i].first);            // new path appends neighbor
                        Connection* new_edge = new Connection();                    // create a new connection
                        new_edge->cost = new_cost + heuristic(neighbors[current][i].first, destination);  // assign new values to struct
                        new_edge->path = new_path;

                        route.push(new_edge);                                       //push in the new edge
                    }
                }
                
                visited.insert(current);                // insert current to visited set
                pass += 1;
            }

        }
        
};

int main()
{
    AStar search = AStar("default");
    
    search.make("A", 2, 5);
    search.make("B", 4, 4);
    search.make("C", 3, 2);
    search.make("D", 3, 0);
    search.make("E", 2, 1);
    search.make("F", 5, -1);

    search.create("A", "B", 2);
    search.create("B", "D", 2);
    search.create("A", "E", 2);
    search.create("E", "D", 1);
    search.create("A", "C", 2);
    search.create("C", "D", 2);
    search.create("B", "F", 1);
    search.create("D", "F", 1);

    search.search("A", "F");
     
}