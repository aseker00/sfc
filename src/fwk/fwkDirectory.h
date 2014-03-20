#ifndef SFC_FWK_DIRECTORY_H
#define SFC_FWK_DIRECTORY_H

#include "FwkNamedInterface.h"

namespace Fwk
{
/*
Framework Directory interface, supporting the lookup of an instance of type T by name, 
returning a reference to the instance with that name.
A framework type Fwk::Directory is provided as the root Directory and for instantiating 
intermediate Dir structure that does not require application-specific attributes.
Each module instance is mounted into a system-wide name space supported by a
root Directory, thereby instantiated or linked into the name space.
Providing hierarchical (character-string) naming support, pointer/reference support and 
the ability to map from names to pointers and vice versa. 
Referencing an object by name is really a matter of mapping the name to a pointer to an interface and
then performing operations on that object through the pointer to the interface.
*/
class Directory : public NamedInterface 
{
public:
	/*
	An optimized case arises when the child entities are strictly internal to the manager
	class. For example, the entry elements managed by a collection class can be regarded
	as being managed by the collection class. Because of the tight coupling of collection
	class to entry class, it is feasible to make the entry class an implementation class
	that is directly used by the collection class, avoiding the overhead of virtual function
	calls.
	Module-based naming means objects of a given type are named relative to a directory 
	corresponding to the containing module.
	*/
	class Entry : public PtrInterface<Entry> 
	{
	public:
		typedef fwk::Ptr<const Entry> PtrConst;
		typedef fwk::Ptr<Entry> Ptr;
		Name name() const { return name_; }
		String typeName() const { return typeName_; }
		NamedInterface::PtrConst ptr() const { return ptr_; }
		NamedInterface::Ptr ptr() { return ptr_; }

	private:
		Name name_;
		String typeName_;
		Ptr ptr_;
	};
	/*
	The Directory provides an object to store type-specific information about the aggregate of Foo instances.
	the foo collection attribute provides notification to reactors when a new Foo instance is instantiated.
	A type that is instantiated as a child of the module top-level object can also have an
	instantiating attribute and thus create its own child objects, and similar for these
	child objects.
	The instantiating collection attributes provide the membership information called for in the Dir
	for this portion of the hierarchy.
	A typical interface module consists of one or more entity types corresponding to the visible 
	interfaces provided to clients of this module. 
	A �Port� module might contain the types Switch::Port and Switch::PortDirectory, with the latter being
	the top-level parent object that instantiates Port objects as its children.
	In this structure, the top-level object contains an instantiating attribute that allows
	instantiation of new child objects as part of adding it to the associated collection. For
	example, Switch::PortDirectory::port could be an instantiating attribute that creates
	new port objects and adds them to the port collection attribute, allowing them to
	be accessed by name relative to the PortDirectory object. I.e.
	class PortDirectory : Fwk::NamedInterface {
	public:
		Port::Ptr portIs( String name, PortId );
	. . .
	};
	The �portIs� mutator adds the new Port instance to its collection attribute �port�
	and also initializes a backpointer attribute in this instance to its parent, namely
	this PortDir instance. The collection attribute keeps track of the instances instantiated
	by the PortDir in type-specific form. It also acts as a virtual constructor, concealing
	the specific constructor being called and any additional actions, such as setting the
	backpointer, etc. from the client. In contrast, for example, the client directly calling
	Port::Ptr p = new PortImpl( . . . );
	compromises the separation of interface from implementation because it directly ties
	the client code to a particular implementation, namely the PortImpl implementation
	class.
	The PortDirectory provides an object to store type-specific information about the aggregate of Port instances. 
	For example, there may be a PortDirectory::defaultPortSpeed attribute that provides the default data rate for each port. 
	Finally, the port collection attribute provides notification to reactors when a new Port instance is instantiated.
	*/

	/*
	The entry attribute, as specified above, allows you to specify the name and typename
	for an entry and rely on the Fwk::Directory mechanism to instantiate an instance of
	the specified type, and set the ptr attribute of this entry to this new instance.
	This relies on a dynamic instantiation mechanism mapping typename to factory
	function for this type. The alternative is to have the entry mutator take a pointer
	to the associated object, which requires the client code to invoke the factory function
	separately.
	The mount facility thus must instantiate local proxies if the module instance is a remote object 
	and set the entry with the associated name with a pointer to the local (proxy) instance.
	The specification of what to mount can use a URI format, specifying protocol, host
	as well as path name. The URI format has also been extended to specify lots of additional parameters. For
	instance, the URI can be extended to restrict the access through a name to read-only or const.
	*/
	Entry::Ptr entryIs(Name _name, String _typeName);
	// default collection.
	// ...
};
}

#endif
