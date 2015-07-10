/*
 * MQ relay module
 * Compatible with Websphere MQ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cmqc.h>

using namespace std;

#define OUT 
#define REF



/**
 * Create a connection to MQ
 * param {const char*} queue manager name
 */
bool create_conn(const MQCHAR* qmname, const MQCHAR* qname, OUT MQHCONN* p_hconn, OUT MQHOBJ* p_hobj){

	MQLONG result;
	MQLONG reason;

	cout << "Destination Queue manager: " << qmname << endl;
	cout << "Destination Queue: " << qname << endl;

	// Create a new connection object
	MQCONN( (MQCHAR*)qmname, p_hconn, &result, &reason );

	// Success?
	if (result == MQCC_FAILED){
		cerr << "FAILED to initialize: " << reason << endl;
		exit((int)reason);
	}

	// Open a connection
	MQOD objectDesc = {MQOD_DEFAULT};
	strncpy( objectDesc.ObjectName, qname, (size_t)MQ_Q_NAME_LENGTH);
	strncpy( objectDesc.ObjectQMgrName, qmname, (size_t)MQ_Q_MGR_NAME_LENGTH);
	
	MQLONG options = MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING;

	MQOPEN( *p_hconn, &objectDesc, options, p_hobj, &result, &reason ); 

	if (reason != MQRC_NONE || result == MQCC_FAILED){
		cerr << "FAILED to create a connection to MQ server: " << reason << endl;
		return false;
	}

	return true;
}

/**
 * Drop a connection 
 */
bool drop_conn(MQHCONN *p_hconn, MQHOBJ *p_hobj){
	if (!p_hconn)
		return false;

	MQLONG result;
	MQLONG reason;

	// Close the connection
	MQLONG options = MQCO_NONE;
	MQCLOSE( *p_hconn, p_hobj, options, &result, &reason );

	if (reason != MQRC_NONE){
		cerr << "FAILED to close the connection: " << reason << endl;
		return false; 
	}

	// Disconnect from the server
	MQDISC( p_hconn, &result, &reason );

	// Success?
	if (reason != MQRC_NONE){
		cerr << "FAILED to disconnect from MQ server: " << reason << endl;
		exit((int)reason);
	}

	return true;
}


/**
 * Put a message to the specified queue
 */
int put_message(MQHCONN *p_hconn, MQHOBJ *p_hobj, const char* msg, int message_length){
	if (!p_hconn)
		return false;

	MQMD msgdesc = {MQMD_DEFAULT};
	memcpy( msgdesc.Format, MQFMT_STRING, (size_t)MQ_FORMAT_LENGTH);
	memcpy( msgdesc.MsgId, MQMI_NONE, sizeof(msgdesc.MsgId));

	MQPMO datagram = {MQPMO_DEFAULT};
	datagram.Options = MQPMO_NO_SYNCPOINT | MQPMO_FAIL_IF_QUIESCING;
	
	MQLONG result;
	MQLONG reason;
	MQPUT( 
		*p_hconn, 
		*p_hobj, 
		&msgdesc, 
		&datagram, 
		message_length, 
		(char*)msg, 
		&result, 
		&reason 
	);

	// Success?
	if (reason != MQRC_NONE){
		cerr << "FAILED to transmit a message: " << reason << endl;
	}

	return reason;
}


/**
 * Get a message out of the queue
 */
char* get_message(MQHCONN *hconn, const char* qname, int message_capacity=1<<24){
	// TAOTODO:
}



int main(int argc, char** argv){
	cout << "Establishing connection..." << endl;


	MQCHAR *qmname = new MQCHAR[MQ_Q_MGR_NAME_LENGTH];
	MQCHAR *qname = new MQCHAR[MQ_Q_NAME_LENGTH];
	strcpy( qmname, "QMA" );
	strcpy( qname, "Q1" );

	// Test creating a new connection
	MQHCONN conn;
	MQHOBJ hobj;
	if (!create_conn( (const MQCHAR*)qmname, (const MQCHAR*)qname, &conn, &hobj )){
		exit(1);
	}
	else{
		cout << "Connected..." << endl;
		// Test sending a message
		cout << "Sending a message..." << endl;
		char msg[1024];
		strcpy( msg, "This message is relayed from c++ code." );
		put_message( &conn, &hobj, msg, 1024 );

		// Drop the connection afterwards
		drop_conn( &conn, &hobj );
	}

}