#ifndef SFC_ODM_UTEST_RELVAR_TEST
#define SFC_ODM_UTEST_RELVAR_TEST

#include "../OdmRelvar.h"

namespace odm {
class Person : Fwk::NamedInterface {
public:
	unsigned int id() const { return 1; }
	bool flag() const { return false; }
};
}

using namespace odm;

int main(char **argv, int argc) {
	DataDriver dd;
	DataSource<Person> ds(dd);
	Perspn p;
	ds->objectAdd(p);
	
	Query<Person> q;
	ResultSet<Person> rs;
	rs = ds->query(q);


	Person p = db->object(id);

	ds->objectIs(is, p)


	Relvar::Ptr table;
	return 0;
}

#endif