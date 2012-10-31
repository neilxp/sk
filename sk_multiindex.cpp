#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
using namespace std;
using namespace boost::multi_index;

struct employee
{
  int         id;
  std::string name;

  employee(int id,const std::string& name):id(id),name(name){}

  bool operator<(const employee& e)const{return id<e.id;}
  friend std::ostream& operator<<(std::ostream& os, const employee &);

};

std::ostream& operator<<(std::ostream& os, const employee &ee)
{
	os << ee.id << ":" << ee.name;
	return os;
}

typedef multi_index_container<
  employee,
  indexed_by<
    // sort by employee::operator<
    ordered_unique<identity<employee> >,
    
    // sort by less<string> on name
    ordered_non_unique<member<employee,std::string,&employee::name> >
  > 
> employee_set;

void print_out_by_name(const employee_set& es)
{
  // get a view to index #1 (name)
  const employee_set::nth_index<1>::type& name_index=es.get<1>();
  // use name_index as a regular std::set
  std::copy(
    name_index.begin(),name_index.end(),
    std::ostream_iterator<employee>(std::cout, "\n"));
}

int main(int argc, char** argv)
{
	employee_set es;
	es.insert(employee(1, "omgea"));
	es.insert(employee(2, "bbb"));
	es.insert(employee(3, "zcc"));
	es.insert(employee(4, "ddd"));
	es.insert(employee(5, "fff"));
	employee_set::nth_index<1>::type &str_index = es.get<1>();
	employee_set::nth_index<1>::type::iterator itr = str_index.find("ddd");
	if (itr != str_index.end()){
		employee newe(*itr);
		newe.name = "spark";
		str_index.replace(itr, newe);
	}
	print_out_by_name(es);
	return 0;
}

