"""
MQ relay module
"""

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
	qma 		= 'QM.1'
	channel 	= None
	qaddress 	= '192.168.1.121(1434)'

	qmgr = pymqi.connect(qma, channel, qaddress)

	putq = pymqi.Queue(qmgr, 'TESTQ.1')
	putq.put('Hello from Python!')