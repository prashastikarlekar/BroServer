#include<iostream>
#include<forward_list>
using namespace std;
int main()
{
	forward_list<int> list;
	forward_list<int>::iterator i;
	i=list.before_begin();
	i=list.insert_after(i,100);
	i=list.insert_after(i,200);
	i= list.insert_after(i,400);
	i= list.insert_after(i,500);
	i=list.begin();
	while(i!=list.end())
	{
		cout<<*i<<endl;
		++i;
	}
	return 0;
}