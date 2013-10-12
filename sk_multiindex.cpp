#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
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
  std::string name;
  int         age;

  employee(const std::string& name, int age):name(name), age(age){}

  bool operator<(const employee& e)const{  return name < e.name;};
  friend std::ostream& operator<<(std::ostream& os, const employee &);

};

std::ostream& operator<<(std::ostream& os, const employee &ee)
{
	os << ee.name << ":" << ee.age;
	return os;
}

typedef multi_index_container<
  employee,
  indexed_by<
    // sort by employee::operator<
    ordered_non_unique<member<employee, std::string, &employee::name> >,
    
    // sort by less<string> on name
    ordered_non_unique<member<employee, int, &employee::age> >
  > 
> employee_set;

void print_out_by_age(const employee_set& es)
{
  // get a view to index #1 (name)
  const employee_set::nth_index<1>::type& index=es.get<1>();
  // use index as a regular std::set
  std::copy(
    index.begin(),index.end(),
    std::ostream_iterator<employee>(std::cout, "\n"));
}

void print_out_by_name(const employee_set& es)
{
  // get a view to index #1 (name)
  const employee_set::nth_index<0>::type& name_index=es.get<0>();
  // use name_index as a regular std::set
  std::copy(
    name_index.begin(),name_index.end(),
    std::ostream_iterator<employee>(std::cout, "\n"));
}

int main(int argc, char** argv)
{
	employee_set es;
	es.insert(employee("omgea", 21));
	es.insert(employee("bbb", 28));
	es.insert(employee("zcc", 29));
	es.insert(employee("ddd", 22));
	es.insert(employee("fff", 30));
	employee_set::nth_index<0>::type &str_index = es.get<0>();
	employee_set::nth_index<0>::type::iterator itr = str_index.find("ddd");
	if (itr != str_index.end()){
		employee newe(*itr);
		newe.name = "spark";
		str_index.replace(itr, newe);
	}
	print_out_by_name(es);
	cout << "-----\n";
	print_out_by_age(es);
	return 0;
}

