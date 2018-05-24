import socket
import sys

#Evan Matoske 02/12/2018 
#Project1
#Sources: 
#	https://www.youtube.com/watch?v=DIPZoZheMTo&t=46s 
#	https://docs.python.org/release/2.6.5/library/socketserver.html
#

#Start the server on the provided port from the comand line
def setupSocket():

	portNum = int(sys.argv[1])
	
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)	
		
	sock.bind(('', portNum))

	sock.listen(1)

	return sock

#Grabs the message the user wants to send and prepends handle
def sendServerMsg(conn):
	serverMsg = raw_input("serverO> ")
	if serverMsg == "\quit":
		return serverMsg
	
	serverMsg = "serverO> " + serverMsg
	
	conn.send(serverMsg) 
	
	return serverMsg

#Recieves messages once it is connected to a client
def recvClientMsg(conn):
	
	msg = conn.recv(500).decode()
	
	return msg



#Makes sure there is an appropriate number of command line arguments
if(len(sys.argv) !=2):
	print "Insufficient number of arguments."
	sys.exit(0)

server = setupSocket()


#This while loop keeps the program running until a connection is made or ^c command is given
while True:
	print "<-- Server started on port:" +sys.argv[1]+". Waiting for client -->"

	conn, addr = server.accept()
	
	print "<-- Connection started -->"
	
	#Once a connection is made this loop is run until it recieves \quit
	while True:
			
		msg = recvClientMsg(conn)
		
		
		if msg == "\quit":
			break
		
		print msg
		
		serverMsg = sendServerMsg(conn)
		
		if serverMsg == "\quit": 
			break

	conn.close() 
