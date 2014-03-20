#ifndef SFC_ODM_REL_VAR_H
#define SFC_ODM_REL_VAR_H

#include "../fwk/FwkNamedInterface.h"
#include "OdmRelation.h"

namespace Odm
{
class Relvar : public Fwk::NamedInterface
{
private:
	Relation::Ptr relation_;
};
}

#endif