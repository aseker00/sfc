#ifndef SFC_ODM_RELATION_H
#define SFC_ODM_RELATION_H

#include "../fwk/FwkPtrInterface.h"
#include "../clc/ClcLinkedBinarySearchTree.h"
#include "../clc/ClcLinkedHashTable.h"
#include "OdmAttribute.h"
#include "OdmValue.h"
#include "OdmQuery.h"

namespace Odm
{
class Relation : public Fwk::PtrInterface<Relation>
{
public:
	typedef Fwk::Ptr<const Relation> PtrConst;
	typedef Fwk::Ptr<Relation> Ptr;

	class Heading
	{
	public:
		typedef Clc::LinkedBinarySearchTree<Attribute> Attributes;

		void attributeAdd(Attribute::Ptr a) { attributes_.newMember(a); }
		Attributes::IteratorConst iteratorConst() const { return attributes_.iteratorConst(); }

	private:
		Attributes attributes_;
	};
	class Tuple : public Fwk::PtrInterface<Tuple>
	{
	public:
		typedef Fwk::Ptr<const Tuple> PtrConst;
		typedef Fwk::Ptr<Tuple> Ptr;

		typedef Clc::LinkedHashTable<Value> AttributeValues;

		void linkedBinaryTreeLeftIs(Tuple *t) { left_ = t; }
		Tuple *linkedBinaryTreeLeft() const { return left_; }
		void linkedBinaryTreeRightIs(Tuple *t) { right_ = t; }
		Tuple *linkedBinaryTreeRight() const { return right_; }
		void valueIs(Attribute, Value);
		Value value(Attribute) const;
		AttributeValues::IteratorConst iteratorConst() const { return attrValues_.iteratorConst(); }

	private:
		AttributeValues attrValues_;
		Tuple *left_, *right_;
	};
	class Body
	{
	public:
		typedef Clc::LinkedBinarySearchTree<Tuple> Tuples;

		void tupleAdd(Tuple::Ptr t) {
			tuples_.newMember(t);
		}
		Tuples::IteratorConst iteratorConst() const { return tuples_.iteratorConst(); }

	private:
		Tuples tuples_;
	};
	void tupleAdd(Tuple::Ptr t) {
		Tuple::AttributeValues::IteratorConst iter = t->iteratorConst();
		while (iter) {
			Value v = (*iter);

		}
		body_.tupleAdd(t);
	}
	void queryIs(Query);

private:
	Heading heading_;
	Body body_;
};
}

#endif