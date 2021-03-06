/*
 * Copyright (C) 2010-2011  George Parisis and Dirk Trossen
 * Copyright (C) 2015-2017  Mays AL-Naday
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See LICENSE and COPYING for more details.
 */

#ifndef CLICK_INFORMATIONITEM_HH
#define CLICK_INFORMATIONITEM_HH

#include "common.hh"
#include "scope.hh"
#include "globalconf.hh"

#include <click/straccum.hh>

CLICK_DECLS

class RemoteHost;
class Scope;
class InformationItem;

/**
 * @brief (Blackadder Core) An InformationItem represents an item that exists somewhere in the information graph managed by a rendezvous element.
 * 
 * It is not the same as an ActivePublication. InformationItem and Scope are managed by the rendezvous element, which knows the whole graph structure.
 * So an InformationItem keeps track of its place in the graph by having pointers to its father scopes. Note that only RemoteHost objects are used here. 
 * The rendezvous element does not know about LocalHost objects (applications and click elements).
 * @param _strategy
 * @param father_scope
 */
class InformationItem {
public:
    /**
     * @brief Constructor: It creates an information item and links it under a single father scope. Other father scopes are added later as a result of pub/sub requests.
     * 
     * The father scope is also updated to store a pointer to its child item.
     * The publisher or subscriber initiated the construction is later added using the updatePublishers method (the same method actually stores the information identifier for this item).
     * @param _strategy The assigned dissemination strategy
     * @param father_scope A pointer to the father Scope
     */
    InformationItem(unsigned char _strategy, Scope *father_scope);
    /**
     * @brief Destructor: deletes the pairs of sets of publishers and subscribers for each full information identifier (ids).
     */
    ~InformationItem();
    /**
     * @brief All ids must be updated because of the publication or the republication. For instance the item may have two father scopes, each one being identified by multiple identifiers. 
     * 
     * For ALL these unique identifiers this method adds respective identifiers using the provided suffix. The index provided (which is the InformationItem index of the rendezvous element) is also updated.
     * @param pubIndex a reference to a HashTable where ids are mapped to InformationItem pointers (the rendezvous index for all information item identifiers).
     * @param suffixID the last fragment of the identifier of the added InformationItem.
     */
    void updateIDs(IIHashMap &pubIndex, String suffixID);
    /**
     * @brief Update the set of publishers by adding the provided RemoteHost (publisher) for the provided identifier. Note that we keep a pair of such sets for each different information identifier.
     * 
     * @param ID the information identifier for which the set of publishers will be updated.
     * @param _publisher A pointer to a RemoteHost representing a publisher (a Blackadder node identified by a statistically unique label).
     * @return False if the publisher was already in the set for the provided ID. True if it wasn't or if the identifier wasn't there (a republication under a new scope). 
     */
    bool updatePublishers(String ID, RemoteHost *_publisher);
    /**
     * @brief Update the set of subscribers by adding the provided RemoteHost (subscriber) for the provided identifier. Note that we keep a pair of such sets for each different information identifier.
     * 
     * @param ID the information identifier for which the set of subscribers will be updated.
     * @param _subscriber A pointer to a RemoteHost representing a subscriber (a Blackadder node identified by a statistically unique label).
     * @return False if the subscriber was already in the set for the provided ID. True if it wasn't or if the identifier wasn't there. 
     */
    bool updateSubscribers(String ID, RemoteHost *_subscriber);
    /**
     * @brief Checks if publishers or subscribers for this information item exist ONLY under the provided fatherScope.
     * 
     * @param fatherScope the father Scope under which the method will look for publishers or subscribers. Note that we keep a pair of such sets for each different information identifier.
     * So the fatherScope will give us the clue about what pair to look.
     * @return True if publishers or subscribers exist. False if not.
     */
    bool checkForOtherPubSub(Scope *fatherScope);
    /**
     * @brief Updates the provided set of publishers with the publishers (for all ids) for this item. The method does NOT look into father scopes.
     * 
     * @param publishers a set of publishers passed by reference.
     */
    void getPublishers(RemoteHostSet &publishers);
    /**
     * @brief Updates the provided set of subscribers with the subscribers (for all ids) for this item. The method does NOT look into father scopes.
     * 
     * @param subscribers a set of subscribers passed by reference.
     */
    void getSubscribers(RemoteHostSet &subscribers);
    /**
     *@brief Returns the set of Root Scopes of the identifiers of this InformationItem
     
     *@param Scopes a set of scopes passed by reference
     */
    void getRootScopes(StringSet &_ids);
    /**
     * @brief Returns the first identifier of this InformationItem in a binary format. It has to be quoted_hex() for printing in the usual hex format.
     * 
     * @return a string of the binary representation of the first InformationItem's identifier.
     */
    String printID();
    /**
     * @brief Returns the ith identifier of this InformationItem in a binary format. It has to be quoted_hex() for printing in the usual hex format.
     *
     * @param i index of indentifier
     * @return a string of the binary representation of the ith InformationItem's identifier.
     */
    String printID(unsigned int i);
    /**
     * @brief Returns all identifiers of this InformationItem in a binary (?) format. It has to be quoted_hex() for printing in the usual hex format (?).
     * 
     * @param delim Delimiter character to be added after each ID
     * @return a string of the binary (?) representations of all InformationItems' identifiers.
     */
    String printIDs(char delim);
    /** @brief The information identifiers of this InformationItem. 
     * 
     *  They are represented as a HashTable of strings (binary representation of an identifier) mapped to a Pair of set of RemoteHosts, containing the publishers and subscribers, respectively.
     */
    IdsHashMap ids;
    /**
     * @brief A set containing all subscribers for this item (regardless of the potentially multiple ids)
     */
    RemoteHostSet fullSubscribersList;
    /** 
     * @brief A set of scopes which are the father of this information item
     */
    ScopeSet fatherScopes;
    /** @brief The assigned dissemination strategy
     */
    unsigned char strategy;
};

CLICK_ENDDECLS
#endif

