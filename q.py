"""
MQ relay module

----------------------------------------------------------

	Prereq:		python2.6
				pymqi
				Running QM and queue
				Environment variable specifying QM 

	Usage:		To put a message to queue:

					$ python q.py {message}

				To retrieve a message from a queue:

					$ python q.py

----------------------------------------------------------
"""

import sys
import pymqi
from pymqi 		import *
from termcolor 	import colored

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

	# TAOTODO: Read these settings from the conf
	qchannel		= 'SVRCONN.1' #'SYSTEM.DEF.SVRCONN'
	qmanager_name	= 'QMA'
	qname 			= 'Q1'
	qserver			= 'localhost(1414)'

	# Set up connection
	print colored('Establishing connection','green')
	print colored('   server     : ','yellow') + qserver
	print colored('   queue mgr  : ','yellow') + qmanager_name
	print colored('   queue name : ','yellow') + qname
	qmgr = pymqi.connect(qmanager_name, qchannel, qserver)
	###qmgr = QueueManager(qmanager_name)
	
	# Now do something with the queue manager
	if len(sys.argv)>1:
		# push a message to the queue
		print colored("Pushing...","cyan")
		push_queue( qmgr, qname, ' '.join(sys.argv[2:]))
	else:
		print colored("Recieving...","cyan")
		print get_queue( qmgr, qname )

	# disconnect from the queue server
	pymqi.disconnect(qmgr)
