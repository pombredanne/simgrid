/* Copyright (c) 2013-2015. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <algorithm>

#include "simgrid/s4u/link.hpp"
#include "simgrid/sg_config.h"
#include "simgrid/simix.hpp"
#include "src/surf/network_interface.hpp"
#include "xbt/log.h"

XBT_LOG_NEW_DEFAULT_SUBCATEGORY(s4u_link, s4u, "Logging specific to the S4U links");

/*********
 * C API *
 *********/

extern "C" {

const char* sg_link_name(Link* link)
{
  return link->name();
}
Link* sg_link_by_name(const char* name)
{
  return Link::byName(name);
}

int sg_link_is_shared(Link* link)
{
  return link->sharingPolicy();
}
double sg_link_bandwidth(Link* link)
{
  return link->bandwidth();
}
double sg_link_latency(Link* link)
{
  return link->latency();
}
void* sg_link_data(Link* link)
{
  return link->getData();
}
void sg_link_data_set(Link* link, void* data)
{
  link->setData(data);
}
int sg_link_count()
{
  return simgrid::surf::LinkImpl::linksCount();
}
Link** sg_link_list()
{
  simgrid::surf::LinkImpl** list = simgrid::surf::LinkImpl::linksList();
  Link** res                     = (Link**)list; // Use the same memory area

  int size = sg_link_count();
  for (int i = 0; i < size; i++)
    res[i] = &(list[i]->piface_); // Convert each entry into its interface

  return res;
}
void sg_link_exit()
{
  simgrid::surf::LinkImpl::linksExit();
}
}

/***********
 * C++ API *
 ***********/

namespace simgrid {
namespace s4u {
Link* Link::byName(const char* name)
{
  surf::LinkImpl* res = surf::LinkImpl::byName(name);
  if (res == nullptr)
    return nullptr;
  return &res->piface_;
}
const char* Link::name()
{
  return pimpl_->getName();
}
bool Link::isUsed()
{
  return pimpl_->isUsed();
}

double Link::latency()
{
  return pimpl_->latency();
}

double Link::bandwidth()
{
  return pimpl_->bandwidth();
}

int Link::sharingPolicy()
{
  return pimpl_->sharingPolicy();
}

void Link::turnOn()
{
  simgrid::simix::kernelImmediate([this]() {
    this->pimpl_->turnOn();
  });
}
void Link::turnOff()
{
  simgrid::simix::kernelImmediate([this]() {
    this->pimpl_->turnOff();
  });
}

void* Link::getData()
{
  return pimpl_->getData();
}
void Link::setData(void* d)
{
  simgrid::simix::kernelImmediate([this, d]() {
    this->pimpl_->setData(d);
  });
}

void Link::setStateTrace(tmgr_trace_t trace)
{
  simgrid::simix::kernelImmediate([this, trace]() {
    this->pimpl_->setStateTrace(trace);
  });
}
void Link::setBandwidthTrace(tmgr_trace_t trace)
{
  simgrid::simix::kernelImmediate([this, trace]() {
    this->pimpl_->setBandwidthTrace(trace);
  });
}
void Link::setLatencyTrace(tmgr_trace_t trace)
{
  simgrid::simix::kernelImmediate([this, trace]() {
    this->pimpl_->setLatencyTrace(trace);
  });
}
}
}