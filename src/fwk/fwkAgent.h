#ifndef SFC_FWK_AGENT_H
#define SFC_FWK_AGENT_H

namespace fwk
{
/*
An agent is an operating system process that performs some specified processing
actions, normally reacting to one or more inputs and updating one or more outputs.
Thus, an agent mounts interface module instances corresponding to its inputs, nor-
mally read-only or const. It also may mount interface module instances as outputs in
read-write mode, Here, we assume support for mounting remote objects. Its process-
ing entails reacting to time and changes to its inputs, performing some processing
and updating the outputs.
Each agent instantiates and mounts a configuration interface and a status interface in a 
designated directory as part of its initialization.
It monitors the configuration interface for changes and reports changes in its status
by writing to the status interface. By having these interfaces is a specified directory
for agent configuration and another for agent status, the state of each agent in the
system is readily available.
With this structure, the basic agent structure is to instantiate one or more interface
modules, mounting them as appropriate in the system-wide name space and then
proceed to perform processing.
An agent is normally instantiated to manage a pool of resources, shared among the
instances that it manages. In this sense, it is a form of aggregation, aggregating the
resource demands of many instances of a common type under its control.
*/
class Agent
{
};
}

#endif
