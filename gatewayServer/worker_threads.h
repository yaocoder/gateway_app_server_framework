/*
 * CThread.h
 *
 *  Created on: Mar 4, 2013
 *      Author: yaowei
 */

#ifndef WORK_THREAD_H_
#define WORK_THREAD_H_

#include <vector>
#include "defines.h"
#include <boost/thread.hpp>

class CWorkerThread
{
public:
	CWorkerThread();
	virtual ~CWorkerThread();

public:

	bool InitThread(struct event_base* main_base);

	static void DispatchSfdToWorker(const int sfd, const int id, const struct sockaddr_in& sin);

private:

	bool SetupThread(LIBEVENT_THREAD* me);

	static void ThreadLibeventProcess(int fd, short event, void* arg);

	static conn *conn_new(const CQ_ITEM* item, LIBEVENT_THREAD* libevent_thread_ptr) ;


	static void CreateWorker(void *(*func)(void *), void *arg);

	static void *WorkerLibevent(void *arg);


	static void RegisterThreadInitialized(void);

	static void WaitForThreadRegistration(int nthreads);


	static void ConnQueueInit(CQ *cq);

	static CQ_ITEM * ConnQueueItemNew(void);

	static void ConnQueuePush(CQ *cq, CQ_ITEM *item);

	static CQ_ITEM* ConnQueuePop(CQ *cq);

	static void ConnQueueItemFree(CQ_ITEM *item);



	static void ClientTcpReadCb(struct bufferevent *bev, void *arg);

	static void ClientTcpErrorCb(struct bufferevent *bev, short event, void *arg);

	static void conn_init(void);

	static conn *conn_from_freelist();

	static bool conn_add_to_freelist(conn *c);

	static void conn_free(conn *c);

	static void conn_close(conn *c, struct bufferevent *bev);


	static bool ClientInMessageOpt(const conn* c);

private:

	static std::vector<LIBEVENT_THREAD*> vec_libevent_thread_;

	static int init_count_;
	static pthread_mutex_t 	init_lock_;
	static pthread_cond_t 	init_cond_;

	/* Free list of CQ_ITEM structs */
	static CQ_ITEM *cqi_freelist_;
	static pthread_mutex_t 	cqi_freelist_lock_;

	static int last_thread_;

	/*
	 * Free list management for connections.
	 */

	static boost::mutex mutex_;
	static std::vector<conn*> vec_freeconn_;
	static int freetotal_;
	static int freecurr_;
};

#endif /* CTHREAD_H_ */
