#pragma once
#include <string>
#include <utility>
#include <map>
#include <utility>
#include <functional>
#include <array>      /* array */
#include <assert.h>   /* assert */


template <class T_CommunicationPolicy>
class Communicator : public T_CommunicationPolicy {
private:
    typedef T_CommunicationPolicy                          CommunicationPolicy;

public:
    typedef typename CommunicationPolicy::BinaryOperation  BinaryOperation;
    typedef typename CommunicationPolicy::Event             Event;
    typedef typename CommunicationPolicy::BinaryOperations  BinaryOperations;
    typedef typename CommunicationPolicy::Context           Context;
    typedef typename CommunicationPolicy::ContextID         ContextID;
    typedef unsigned                                        CommID;

public:

    /***************************************************************************
     *
     * POINT TO POINT COMMUNICATION
     *
     ***************************************************************************/
    template <typename T>
    void send(const CommID destCommID, const unsigned tag, const Context context, const T& sendData) const {
	Event e = asyncSend(destCommID, tag, context, sendData);
	e.wait();
    }

    template <typename T>
    Event asyncSend(const CommID destCommID, const unsigned tag, const Context context, const T& sendData){
	return CommunicationPolicy::asyncSendData(sendData.data(), sendData.size(), destCommID, context, tag);
    }


    template <typename T>
    void recv(const CommID srcCommID, const unsigned tag, const Context context, const T& recvData){
	Event e =  asyncRecv(srcCommID, tag, context, recvData);
	e.wait();

    }

    template <typename T>
    Event asyncRecv(const CommID srcCommID, const unsigned tag, const Context context, const T& recvData){
	return CommunicationPolicy::asyncRecvData(recvData.data(), recvData.size(), srcCommID, context, tag);
    }


    /**************************************************************************
     *
     * COLLECTIVE OPERATIONS
     *
     **************************************************************************/ 
    // TODO 
    // Make collective interfaces more slim
    template <typename T>
    void gather(const CommID rootCommID, const Context context, const std::vector<T>& sendData, std::vector<T>& recvData){
    	CommunicationPolicy::gather(sendData.data(), sendData.size(), recvData.data(), sendData.size(), rootCommID, context);
    }

    template <typename T>
    void gather2(const CommID rootCommID, const Context context, const std::vector<T>& sendData, std::vector<T>& recvData){
    	CommunicationPolicy::gather2(sendData.data(), sendData.size(), recvData.data(), sendData.size(), rootCommID, context);
    }

    template <typename T>
    void allGather(const Context context, const std::vector<T>& sendData, std::vector<T>& recvData){
    	CommunicationPolicy::allGather(sendData.data(), sendData.size(), recvData.data(), context);
    }

    // TODO
    // give some nice name (allGatherV sounds crap)
    template <typename T>
    void allGather2(const Context context, const std::vector<T>& sendData, std::vector<T>& recvData){
    	CommunicationPolicy::allGather2(sendData.data(), sendData.size(), recvData, context);
    }


    template <typename T_Send, typename T_Recv>
    void scatter(const CommID rootCommID, const Context context, const T_Send& sendData, const T_Recv& recvData){
    	CommunicationPolicy::gather(sendData.data(), sendData.size(), recvData.data(), recvData.size(), rootCommID, context);
    }

    template <typename T_Send, typename T_Recv>
    void allToAll(const Context context, const T_Send& sendData, const T_Recv& recvData){
    	CommunicationPolicy::allToAll(sendData.data(), sendData.size(), recvData.data(), recvData.size(), context);
    }

    // TODO 
    // Fill with various binary functions
    template <typename T>
    void reduce(const CommID rootCommID, const Context context, const BinaryOperation op, const std::vector<T> sendData, const T& recvData){
     	CommunicationPolicy::reduce(sendData.data(), &recvData, sendData.size(), op, rootCommID, context);
    }

    template <typename T>
    void allReduce(const Context context, const BinaryOperation op, const T& sendData, T& recvData){
	CommunicationPolicy::allReduce(sendData.data(), recvData.data(), sendData.size(), op, context);
    }

    template <typename T>
    void broadcast(const CommID rootCommID, const Context context, const T& data){
	CommunicationPolicy::broadcast(data.data(), data.size(), rootCommID, context);
	
    }


     void synchronize(const Context context){
     	CommunicationPolicy::synchronize(context);
     }


    /***************************************************************************
     *
     * ORGANISATION
     *
     ***************************************************************************/
    Context createContext(const std::vector<CommID> ids, const Context oldContext){
	return CommunicationPolicy::createContext(ids, oldContext);
    }

    Context getGlobalContext(){
	return CommunicationPolicy::initialContext;
    }


};

