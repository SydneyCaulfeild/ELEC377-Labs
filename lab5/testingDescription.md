Sydney Caulfeild / 20102648  
Emma Paczkowski / 20095427  

**Selfcomp test (testSelfcomp.txt)**  
We showed that the selfcomp.c file is able to retrieve the /etc/passwd/ file by using the script command (saved to testSelfcomp.txt) to run selfcomp and save the output. We also ran "cat /etc/passwd/" to show that the output given by selfcomp is identical to the /etc/passwd/ file.

**Client test (testClientOutput.txt and testServerOutput.txt)**  
We also showed that the client is able to retreive the /etc/passwd/ file and crash the server. We did this by running the script command in two separate consoles. In one console, we made a script that saves to testServerOutput.txt and ran the server on port 10,000. In the other console we ran the client on the same port, and as you can see in testClientOutput.txt, the contents of /etc/passwd/ were printed. You can see in testServerOutput.txt that in the server console this caused the server to crash. You can see that these two scripts were executed together because the port numbers and the times in testClientOutput.txt and testServerOutput.txt match.
