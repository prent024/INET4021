Work in Group of Two
Name: Gavin Prentice
Student ID:
x500: prent024

Name: Hang Li
Student ID: 4631724
x500: lixx2264

This is our Lab 1 submission.
(Need to set the permission to 755 to execute the file)

1. We use C socket to build up the server, (with the ability to read the conf file and output access/error record in the log files) <- not yet
2. The index.html will create a single form with two form field, and it will return the two integers entered in the form field
3. The server will pass the two returned parameters (num1 and num2) to the compute.cgi file, and the cgi file will compute the sum of the two integers then produce the sum
4. Then the resulting HTML will resend to the existing socket and produce the webpage in the web browser
5. The problem is that we have everything working right now, but after the cgi file produces the resulting HTML, it will not send the HTML to the web browser, instead, the resulting HTML will be returned to the terminal and shown as plain HTML script
6. Except No.5 other parts are working perfectly
