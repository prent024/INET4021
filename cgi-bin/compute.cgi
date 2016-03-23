#!/usr/bin/python
import cgi
import cgitb
import sys
import os
cgitb.enable()

print sys.argv

i = sys.argv[1].index("num1=")+5
j = sys.argv[1].index("&")
num1 = sys.argv[1][i:j]
num2 = sys.argv[1][sys.argv[1].index("num2=")+5:]
print "num1: "+num1
print "num2: "+num2

print "Content-Type:text/html\n\n"

HTML_TEMPLATE="""
  <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
  <html>
  <head>
    <title>Home Page</title>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    <style type="text/css">
    .text {
      font-family: Arial, Helvetica, sans-serif;
      font-size: 11px;
      color: #FFFFFF;
    }
    </style>
  </head>
  <body>
    <h1 align="center">Welcome To Our Webpage</h1>
    <p align="center">Wanna Have Some Math "Fun"?</p>
    <p align="center">Add Two Numbers!</p>
    <div align="center">
      <img src="pic_mountain.jpg" alt="Mountain View" height="200" width="300">
    </img><br/><br/>
    <h2 align="center"> Your Result: </h2>
    <p align="center" style="color: red">%(MESSAGE)d</p>
    <input type="button"  value="Do Again!" onClick="goBack()"/>
    </div>
    </body>
    <script type="text/javascript">
    function goBack() {
      window.history.href="../index.html";
    }
    </script>
  </html>
"""

def compute(num1, num2, sock):
  # form = cgi.FieldStorage()
  # if not form.has_key(num1) and not form.has_key(num2):
  #     return
  # num1 = int(form.getvalue("num1"))
  # num2 = int(form.getvalue("num2"))
  total = num1 + num2
  fd = os.open(sock, os.O_WRONLY)
  fd.write(HTML_TEMPLATE%{'MESSAGE': total})

compute(int(num1), int(num2), int(sys.argv[2]))
