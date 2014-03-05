#ifndef SFC_FWK_NAMED_INTERFACE_H
#define SFC_FWK_NAMED_INTERFACE_H

#include "fwkPtrInterface.h"
#include "fwkName.h"

namespace fwk
{
/*
An object of public interface type T in namespace Ns is implemented by object module, 
a single C++ code file NsT.cpp.
The module implements a framework factory interface for instantiating an instance
of this module, which corresponds to instantiating some top-level type in this mod-
ule, such as a TDirectory. For example, Switch::PortDirIs(Fwk::Directory *, Fwk::String
name) might be the factory function for a parent object for Switch::Port instances.
This type implements the framework directory interface, supporting the lookup of
an instance of type T by name, returning a reference to the instance with that name.
The top-level parent type is a unit of aggregation of instances implemented by the
module, managing a shared pool of resources used to implement these instances.
*/

/*
Use the NamedInterface base class rather than the PtrInterface class template
when the class is a public interface and thus should be accessible through the direc-
tory system.
All publicly accessible interfaces should be named and thus derived from NamedInterface, 
only classes that are internal to a module typically derive directly from PtrInterface.
An interface that is publicly available by name, what you might call a published
interface class is derived from the framework base class:
*/
class NamedInterface : public PtrInterface<NamedInterface> 
{
public:
	typedef fwk::Ptr<NamedInterface const> PtrConst;
	typedef fwk::Ptr<NamedInterface> Ptr;
	Name name() const;

	/*
	Continuing our example, PortDir is derived from NamedInterface so overrides the
	interface member function to return the result of calling the �port� accessor. I.e.
	NamedInterface::Ptr
	PortDir::interface( Name n ) { return port(n); }
	interface call as as comparable to the open call in a file system in the sense that it
	converts a reference by name to a reference by pointer. In this sense, the interface
	instance is regarded as equivalent to a named open file.
	The const interface returns a logical const interface pointer. Thus, it is comparable
	to accessing a file for reading while the other call is comparable to accessing a file for
	reading and writing. You can read attributes of the object but you cannot modify
	it. It is logical const because it logically does not change although the instance may
	be actually modified in some ways, if only by the reference counting implemented
	by the smart pointers. (A file system is similar in that the file may change access
	time as a result of being opened for reading but its contents does not change.)
	In general, an interface instance is looked up by name and then cast to the expected
	type. For example, to locate a Window named �CadEditor� in a directory dir, you
	could write the code:
	Name n(��CadEditor��);
	NamedInterface::Ptr ni = dir->interface(n);
	if( !ni ) error( ��editor not found�� );
	Window::Ptr w = dynamic_cast<Window *>(ni.ptr());
	if(!w) error(��CadEditor is not a window��);
	The interface call returns 0 when the interface is not found rather than throwing
	an exception.
	*/
	virtual NamedInterface::Ptr interface(Name path) = 0;
	virtual NamedInterface::PtrConst interface(Name path) const = 0;

	/*
	The member template versions are provided as a convenience, allowing you to lookup
	an object and convert its pointer to the expected type in a single call, i.e.
	NamedInterface::Ptr dir = ...
	Port::Ptr p = dir->interface<Port>(��port0��);
	This shortened form is only applicable when you do not need to distinguish between
	the interface object not being found and the object being of a different type.
	*/
	template<typename T>
	typename T::Ptr interface(Name path) { return dynamic_cast<T *>(interface(path).ptr()); }

	template<typename T>
	typename T::PtrConst interface(Name path) const { return dynamic_cast<T const *>(interface(path).ptr()); }

	/*
	The system-wide name for the interface.
	The �interface� implementation is actually more complicated to support pathname lookup, i.e.
	NamedInterface::Ptr
	PortDir::interface( Name path ) 
	{
		if(isAbsolutePath(path)) return rootDir_->interface(path);
		Name compName(firstComponent(path));
		Port::Ptr p = port(compName);
		return (compName==path) ? p : (p ? p->interface(suffix(path) : 0);
	}
	That is, if the path is a single component name, it acts as before just returning the
	result of the lookup in the �port� collection attribute. If an absolute path name,
	it looks up starting at the root directory. Otherwise, it find the port named by
	the first component of the path name and delegates the rest of the lookup to the
	resulting port object.
	*/
	Name fullName() const;

	/*
	The Directory::interfaceDel operation supports deleting an interface by name.
	Subsequently, looking up name by which the interface was created returns a 0 until
	a new instance is created with the same name.
	You cannot actually destruct the instance in response to this operation
	because another entity may have a smart pointer to this instance. Thus, you are
	limited to just removing the instance from the directory.
	However, even making the instance have no name is problematic because a client with a 
	pointer to this instance can still call NameInterface::fullName as part of error reporting, etc. 
	Clearly, having this call fail at this stage is unacceptable, given it would then cause problems with
	logging and error reporting. However, the system should allow creation of a new
	instance with the original name to be created, because this is the expected behavior
	after a delete.
	Give an instance a system-generated suffix to its name when it is created. 
	I.e. a special delimiter such as �#� followed by a random number.
	A call to NameInterface::fullName returns the full name including this hidden
	suffix. A call to interface with this system-generated name continues to return
	this instance even after it has been �deleted� by a call to interfaceDel. However,
	if n is the original client-specified name name, calling interface(n) after the call
	to interfaceDel causes it to return 0. Moreover, you can instantiate a new object
	with the original name and it should succeed, or at least not fail for the reason
	of the name still being in use. In essence, an object is created with two names,
	the user-assigned name plus a related system-generated name. The former can be
	deleted from the directory but the second is only deleted from the directory when
	the instance is actually destructed.
	Thus, a interfaceDel effectively just removes the name by which the instance was
	created but retains the system-generated name corresponding to its original name
	plus the system-generated suffix. This can be implemented by storing the name plus
	suffix in the directory entry, tagging a directory entry as �deleted� or not, and only
	mapping names without the special suffix to non-delete-tagged directory entries.
	The implementation of interfaceDel marks the corresponding directory entry as
	deleted and decrements the reference count on the interface, relying on the call to
	onZeroReferences to actually remove the entry from the directory when all the
	references go to zero.
	*/
	NamedInterface::Ptr interfaceDel(Name path);

private:
	Name name_;
};
}

#endif
