#!/usr/bin/python

import cgi
import cgitb; cgitb.enable()
import os


form = cgi.FieldStorage()
def update_file_name (form_field):
    if form.has_key(form_field):
       if len(form['title'].value) == 0:
           
           print "<!DOCTYPE html>"
           print "<html>"
           print "<head>"
           print "<title> Edit Picture Title </title>"
           print "<meta http-equiv='Content-type' content='text/html; charset=iso-8859-1'>"
           print "<link rel='stylesheet' type='text/css' href='style.css'>"
           print "<script type='text/javascript'></script>"
           print "</head>"
           print "<body>"
           print "<h2>Edit Picture Title</h2>"
           print "<div style='margin-left:auto;margin-right:auto;text-align:center;width:500px;height:180px;border:1px solid black;'>"
           print "<br/>"
           print "<form action='' name='editForm' id='editForm' method='POST' enctype ='multipart/form-data'>"
           print "<br/>"
           print "<p id='error' name='error' style='color:red'> Please enter valid title.</p>"
           print "<br/>"
           print "<input type='hidden'  name ='picname' value='' ></input>"
           print "Title: <input name='title' id='title' type='text' value=''></input>"
           print "<br/>"
           print "<br/>"
           print "<input type='submit'  value='Update'/>"
           print "<input type='button'  value='Cancel' onClick='goBack()'/>"
           print "</form>"
           print "</div>"
           print "</body>"
           print "<script>" 
           print "function goBack() {"
           print "window.history.back();"
           print "}"
           print "</script>"
           print "</html>"
           return
       else:
           os.rename("Pictures/"+form['picname'].value, "Pictures/"+form['title'].value)
           os.rename(form['picname'].value,form['title'].value)
           redirect = """<!DOCTYPE html>
           <html>
           <head>
           <meta http-equiv="refresh" content="0;gallery.cgi">
           </head>
           </html>
           """
           print "Content-Type: text/html\n\n"
           print redirect
   
if form.has_key('Name'):
    picname = form['Name'].value
    print "Content-Type: text/html\n\n"

    print "<!DOCTYPE html>"
    print "<html>"
    print "<head>"
    print "<title> Edit Picture Title </title>"
    print "<meta http-equiv='Content-type' content='text/html; charset=iso-8859-1'>"
    print "<link rel='stylesheet' type='text/css' href='style.css'>"
    print "<script type='text/javascript'></script>"
    print "</head>"
    print "<body>"
    print "<h2>Edit Picture Title</h2>"
    print "<div style='margin-left:auto;margin-right:auto;text-align:center;width:500px;height:180px;border:1px solid black;'>"
    print "<br/>"
    print "<form action='' name='editForm' id='editForm' method='POST' enctype ='multipart/form-data'>"
    print "<br/>"
    print "<p id='error' name='error'></p>"
    print "<br/>"
    print "<input type='hidden'  name ='picname' value='" +  picname + "'></input>"
    print "Title <input name='title' id='title' type='text' value='" +  picname + "'></input>"
    print "<br/>"
    print "<br/>"
    print "<input type='submit'  value='Update'></input>"
    print "<input type='button'  value='Cancel' onClick='goBack()'></input>"
    print "</form>"
    print "</div>"
    print "</body>"
    print "<script>" 
    print "function goBack() {"
    print "window.history.back();"
    print "}"
    print "</script>"
    print "</html>"
    
else:
    
    update_file_name("picname")

	
