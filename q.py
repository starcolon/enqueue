"""
MQ relay module

	Prereq:		python2.6
						pymqi
						Running QMA and queue

	Usage:		To put a message to queue:
						python q.py + {message}
				To retrieve a message from a queue:
						python q.py
"""

import sys
import pymqi

"""
Read the recent message from a queue
"""
def get_queue(qmgr,queue):
	getq = pymqi.Queue(qmgr,queue)
	return getq.get()


"""
Push a message to a queue
"""
def push_queue(qmgr,queue,message):
	putq = pymqi.Queue(qmgr,queue)
	putq.put(message)


if __name__ == "__main__":
	# Read queue configurations from config file
	# TAOTODO:
	qma 		= 'QMA'
	channel 	= None
	qaddress 	= 'localhost(1434)'
	qname 		= 'Q1';

	qmgr = pymqi.connect(qma, channel, qaddress)

	# Now do something with the queue manager
	if len(sys.argv)>1 and sys.argv[1]=='+':
		# push a message to the queue
		push_queue( qmgr, qname, ' '.join(sys.argv[2:]))
	else:
		print get_queue( qmgr, qname )
