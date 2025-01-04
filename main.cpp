#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct Passenger
{
    string name;
    int age;
    string passport_number;
    Passenger(string name,int age,string passport_number)
        :name(name),age(age),passport_number(passport_number){}
};

struct queue_node
{
    string name;
    int age;
    string passport_number;
    int seats_requested;
};

struct tree_node
{
    string name;
    vector<tree_node*> children;
    bool is_full;
    Passenger* passenger;
    tree_node(string value):name(value),is_full(false),passenger(nullptr) {}
};

tree_node *root=new tree_node("Train");
queue<queue_node>reservation_list;

void add_cabin(tree_node *&root,int val)
{
    char alp='A';
    for(int i=0;i<val;i++)
    {
        tree_node *new_node=new tree_node("Cabin "+string(1,alp));
        root->children.push_back(new_node);
        alp=alp+1;
    }
}

void add_seats(tree_node *&root,bool status,int cabin_num,int seat_num)
{
    cabin_num--;
    tree_node *new_node=new tree_node("Seat "+to_string(seat_num));
    new_node->is_full=status;
    root->children[cabin_num]->children.push_back(new_node);
}

void entire_display(tree_node *&root)
{
    cout<<root->name<<endl;
    int cabin_size=root->children.size(),seat_size;
    for(int i=0;i<cabin_size;i++)
    {
        cout<<" - "<<root->children[i]->name<<endl;
        seat_size=root->children[i]->children.size();
        for(int j=0;j<seat_size;j++)
        {
            cout<<"  "<<root->children[i]->children[j]->name;
            if(root->children[i]->children[j]->is_full)
            {
                cout<<" [FULL] - Passenger: "<<root->children[i]->children[j]->passenger->name<<endl;
            }
            else
            {
                cout<<" [Empty] "<<endl;
            }
        }
    }
}

void empty_display(tree_node *&root)
{
    cout<<root->name<<endl;
    int cabin_size=root->children.size(),seat_size;
    for(int i=0;i<cabin_size;i++)
    {
        cout<<" - "<<root->children[i]->name<<endl;
        seat_size=root->children[i]->children.size();
        for(int j=0;j<seat_size;j++)
        {
            if (!root->children[i]->children[j]->is_full)
            {
                cout<<"  "<<root->children[i]->children[j]->name<<" [Empty]"<<endl;
            }
        }
    }
}

bool allocate_seats(tree_node *&root,int seats_needed,const string &name,int age,const string &passport_number)
{
    int available_seats=0;
    for(auto &cabin:root->children)
    {
        for(auto &seat:cabin->children)
        {
            if (!seat->is_full)
            {
                available_seats++;
            }
        }
    }
    if(seats_needed>available_seats)
    {
        return false;
    }
    for(auto &cabin:root->children)
    {
        for(auto &seat:cabin->children)
        {
            if(!seat->is_full && seats_needed>0)
            {
                seat->is_full=true;
                seat->passenger=new Passenger(name,age,passport_number);
                seats_needed--;
            }
        }
    }
    return true;
}

void reservation_system(tree_node *&root, string name, int age, string passport_number, int seats_requested)
{
    if (allocate_seats(root, seats_requested, name, age, passport_number))
    {
        cout<<"Seats allocated to "<<name<<endl;
    }
    else
    {
        cout<<"Not enough seats available. Adding "<<name<<" to the reservation list."<<endl;
        reservation_list.push({name, age, passport_number, seats_requested});
    }
}

void release_seat(tree_node *&root)
{
    if(!reservation_list.empty())
    {
        queue_node res=reservation_list.front();
        if(allocate_seats(root, res.seats_requested,res.name,res.age,res.passport_number))
        {
            cout<<"Seats allocated to "<<res.name<<" from reservation list."<<endl;
            reservation_list.pop();
        }
    }
}

void seat_summary(tree_node *&root)
{
    int occupied_seats=0,total_seats=0;
    for(auto &cabin : root->children)
    {
        total_seats+=cabin->children.size();
        for(auto &seat:cabin->children)
        {
            if(seat->is_full)
            {
                occupied_seats++;
            }
        }
    }
    cout<<"Total Seats: "<<total_seats<<", Occupied Seats: "<<occupied_seats<<", Empty Seats: "<<total_seats-occupied_seats<<endl;
}

void print_reservation_list()
{
    if(reservation_list.empty())
    {
        cout<<"Reservation list is empty."<<endl;
    }
    else
    {
        cout<<"Reservation List==> "<<endl;
        queue<queue_node>temp=reservation_list;
        while(!temp.empty())
        {
            queue_node res=temp.front();
            temp.pop();
            cout<<"Name: "<<res.name<<", Age: "<<res.age<<", Passport: "<<res.passport_number
                 <<", Seats Requested: "<<res.seats_requested<<endl;
        }
    }
}
/**/
void cancel_booking(tree_node *&root,const string &name)
{
    bool found=false;
    for(auto &cabin:root->children)
    {
        for(auto &seat:cabin->children)
        {
            if(seat->is_full && seat->passenger && seat->passenger->name==name)
            {
                delete seat->passenger;
                seat->passenger=nullptr;
                seat->is_full=false;
                found=true;
            }
        }
    }
    if(found)
    {
        cout<<"Booking for "<<name<<" has been canceled."<<endl;
        release_seat(root);
    }
    else
    {
        cout<<"No booking found under the name "<<name<<endl;
    }
}
int main()
{
    int i,j,cabins=5,seats_per_cabin=4;
    add_cabin(root,cabins);
    for(i=0;i<cabins;i++)
    {
        for(j=1;j<=seats_per_cabin;j++)
        {
            add_seats(root,false,i+1,j);
        }
    }
    cout<<"Initial Train Structure==> "<<endl;
    entire_display(root);
    int seats_requested;
    string name;
    int age;
    string passport_number;
    int choice;
    while(true)
    {
        cout<<"Menu:"<<endl;
        cout<<"1. Reserve Seats"<<endl;
        cout<<"2. View Reservation List"<<endl;
        cout<<"3. Cancel Booking"<<endl;
        cout<<"4. Display Entire Train"<<endl;
        cout<<"5. Exit"<<endl;
        cout<<"Enter Choice==> ";
        cin>>choice;
        switch (choice)
        {
        case 1:
            {
                cout<<"Enter number of seats required==> ";
                cin>>seats_requested;
                cout<<"Enter your name==> ";
                cin>>name;
                cout<<"Enter your age==> ";
                cin>>age;
                cout<<"Enter your passport number==> ";
                cin>>passport_number;
                reservation_system(root,name,age,passport_number,seats_requested);
                seat_summary(root);
                cout<<endl;
                cout<<"Empty Seats after allocation==>\n";
                empty_display(root);
                release_seat(root);
                break;
            }
        case 2:
            {
                print_reservation_list();
                break;
            }
        case 3:
            {
                cout<<"Enter the name for the booking you want to cancel==> ";
                cin>>name;
                cancel_booking(root,name);
                seat_summary(root);
                break;
            }
        case 4:
            {
                entire_display(root);
                break;
            }
        case 5:
            {
                cout<<"TERMINATING THE CODE..."<<endl;
                cout<<endl;
                return 0;
            }
        default:
            {
                cout<<"Wrong Input, Please Try Again!"<<endl;
                cout<<endl;
                break;
            }
        }
    }
    return 0;
}
